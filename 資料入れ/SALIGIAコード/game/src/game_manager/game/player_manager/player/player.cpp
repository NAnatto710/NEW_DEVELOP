
/*!
 *  @file		player.cpp
 *  @brief		プレイヤー
 *  @author     Ryusei Shimizu
 *  @date       2026/04/13
 */

#include "player.h"
#include "../../stage_manager/stage_manager.h"
#include "../../stage_manager/stage_object/stage_object.h"
#include "../../camera_manager/camera_manager.h"
#include "../../scene_manager/scene_manager.h"
#include "../../../../utility/utility.h"
#include "../../../../utility/sound_manager/sound_manager.h"
#include "../../effect_manager/effect_manager.h"

// 定数の定義
const float	CPlayer::m_default_width				= 70.0f;	//!< デフォルトの横幅
const float	CPlayer::m_default_height				= 140.0f;	//!< デフォルトの高さ
const int	CPlayer::m_max_jump_count				= 1;		//!< 空中での最大ジャンプ回数
const int   CPlayer::m_max_invincible_time			= 180;		//!< 無敵時間
const int   CPlayer::m_invincible_visible_interval	= 6;		//!< 無敵時間中の点滅間隔
const int   CPlayer::m_plummet_input_time			= 15;		//!< 急落下入力受付時間
const int   CPlayer::m_respawn_delay				= 120;		//!< リスポーンまでの待機時間
const int   CPlayer::m_land_stiffness_time			= 6;		//!< 硬直時間

const PathData CPlayer::m_pathdata =
{
	"data\\player\\player\\texture\\head.png",			//!< 頭部のテクスチャパス
	"data\\player\\player\\texture\\body.png",			//!< 体のテクスチャパス
	"data\\player\\player\\texture\\head_light.png",	//!< 頭部のライトテクスチャパス
	"data\\player\\player\\texture\\body_light.png",	//!< 体のライトテクスチャパス
	"data\\player\\player\\texture\\shadow.png",		//!< 影のテクスチャパス
	"data\\player\\player\\resource.csv",				//!< リソースCSVファイルのパス名
	"data\\player\\player\\attribute.csv",				//!< 属性CSVファイルのパス名
	"data\\player\\player\\attack.csv",					//!< 攻撃CSVファイルのパス名
	"data\\player\\player\\passive.csv"					//!< パッシブCSVファイルのパス名
};

/*
 *	コンストラクタ
 */
CPlayer::
CPlayer(CATEGORY_ID category, int width, int height)
	: m_PlayerData
	({  width,
		height,
		vivid::Rect{ 0,0,width,height },
		vivid::Vector2::ZERO,
		vivid::Vector2(1.0f, 1.0f), 
		0xffffffff,
		0.0f,
		true,
		PLAYER_ID::NONE,
		category,
		vivid::controller::DEVICE_ID::MAX })
{
}

/*
 *	デストラクタ
 */
CPlayer::
~CPlayer(void)
{
}

/*
 *	初期化
 */
void
CPlayer::
Initialize(const PLAYER_ID player_id, vivid::controller::DEVICE_ID device_id, const BuildData& build, const vivid::Vector2& position)
{
	// プレイヤー識別子
	m_PlayerData.PlayerID = player_id;
	// デバイス識別子
	m_PlayerData.DeviceID = device_id;

	// デフォルトの色
	m_PlayerData.Direction = true;
	m_PlayerData.Color = 0xffffffff;

	// プレイヤー識別子によって向きを設定
	if (player_id == PLAYER_ID::PLAYER1)
	{
		m_PlayerData.Direction = true;
		m_PlayerData.Color = Utility::GetColorById(COLOR_ID::BLUE);		// プレイヤー1は青	
	}
	else if (player_id == PLAYER_ID::PLAYER2)
	{
		m_PlayerData.Direction = false;
		m_PlayerData.Color = Utility::GetColorById(COLOR_ID::RED);		// プレイヤー2は赤
	}
	else if (player_id == PLAYER_ID::PLAYER3)
	{
		m_PlayerData.Direction = true;
		m_PlayerData.Color = Utility::GetColorById(COLOR_ID::GREEN);		// プレイヤー3は緑
	}
	else if (player_id == PLAYER_ID::PLAYER4)
	{
		m_PlayerData.Direction = false;
		m_PlayerData.Color = Utility::GetColorById(COLOR_ID::YELLOW);		// プレイヤー4は黄
	}

	// ステータスの初期化
	m_AttributeComponent.Initialize(m_pathdata.AttributeCSVPath);
	m_ResourceComponent.Initialize(m_pathdata.ResourceCSVPath);

	// 状態管理の初期化
	m_StateController.Initialize();
	// ビルドの初期化
	m_BuildComponent.Initialize(build, m_pathdata.PassiveCSVPath, &m_StateController);
	m_BuildComponent.PassiveUpdate(m_AttributeComponent);

	// 物理演算の初期化
	m_PhysicsComponent.Initialize(m_PlayerData.Width, m_PlayerData.Height, position);
	// 攻撃の初期化
	m_AttackComponent.Initialize(m_PhysicsComponent.GetPosition(),m_PlayerData.Direction, build, m_pathdata.AttackCSVPath);
	// 入力の初期化
	m_PlayerInput.Initialize(m_PlayerData.DeviceID);
	// ガードの初期化
	m_GuardComponent.Initialize();
	// 投げの初期化
	m_ThrowComponent.Initialize(m_PlayerData.Width, m_PlayerData.Height);

	m_JumpCount = 0;
	this->Invincible(0);
	m_StiffnessTime = 0;
	m_PlummetInputTimer = m_plummet_input_time;
	m_RespawnTime = 0;
	m_HitStopTime = 0;
	m_IdleTimer = 0.0f;
	m_LandingSquash = 1.0f;
	m_PlummetTimerFlg = false;
	m_PlummetFlg = false;
	m_MoveFlg = false;
	this->SetActive(true);
	m_JumpFlg = false;
	m_InputJumpFlg = false;
	m_InputPlummetFlg = false;
}

/*
 *  更新
 */
void
CPlayer::
Update(void)
{
	// キャラクターの生存状態によって処理を分ける
	switch (m_StateController.GetAliveState())
	{
	case ALIVE_STATE::ALIVE:	this->Alive();	break;
	case ALIVE_STATE::DEAD:		this->Dead();	break;
	}
}

/*
 *	左腕の描画
 */
void
CPlayer::
DrawLeftArm(void)
{
	if (!IsDrawVisible()) return;

	m_AttackComponent.LeftArmDraw();
}

/*
 *	体の描画
 */
void
CPlayer::
DrawBody(void)
{
	if (!IsDrawVisible()) return;

	vivid::Vector2 scroll = CCameraManager::GetInstance().GetScroll();
	float camera_scale = CCameraManager::GetInstance().GetCameraScale();
	vivid::Vector2 draw_position = m_PhysicsComponent.GetPosition();
	unsigned int color = m_PlayerData.Color;

	float head_idle_offset = 0.0f;

	if (m_StateController.GetActionState() == ACTION_STATE::IDLE)
	{
		m_IdleTimer += 1.0f;
		head_idle_offset = std::sin(m_IdleTimer * 0.145f) * 1.5f;
	}
	else
	{
		m_IdleTimer = 0.0f;
	}

	// 着地時の沈み込み
	float squash = m_LandingSquash;

	// 横幅はそのまま、縦方向だけ縮める
	m_PlayerData.Scale = vivid::Vector2(camera_scale, camera_scale * squash);

	draw_position *= camera_scale;

	// 沈み込んだ分だけ描画位置を下げ足元の位置をなるべく維持するための補正
	draw_position.y += m_PlayerData.Height * camera_scale * (1.0f - squash);

	draw_position -= scroll;

	vivid::Vector2 head_draw_position = draw_position;
	head_draw_position.y += head_idle_offset * camera_scale;

	if (CCameraManager::GetInstance().IsPlayerShaking(m_PlayerData.PlayerID))
	{
		vivid::Vector2 shake_offset = CCameraManager::GetInstance().GetPlayerShakeOffset(m_PlayerData.PlayerID);

		draw_position -= shake_offset;
	}

	if (!m_PlayerData.Direction)
	{
		m_PlayerData.Scale.x *= -1;

		draw_position.x += m_PlayerData.Width * camera_scale;
		head_draw_position.x += m_PlayerData.Width * camera_scale;
	}

	vivid::DrawTexture(m_pathdata.HeadPath, head_draw_position, 0xffffffff, m_PlayerData.Rect, m_PlayerData.Anchor, m_PlayerData.Scale, m_PlayerData.Rotation);
	vivid::DrawTexture(m_pathdata.BodyPath, draw_position, 0xffffffff, m_PlayerData.Rect, m_PlayerData.Anchor, m_PlayerData.Scale, m_PlayerData.Rotation);
	vivid::DrawTexture(m_pathdata.HeadLightPath, head_draw_position, color, m_PlayerData.Rect, m_PlayerData.Anchor, m_PlayerData.Scale, m_PlayerData.Rotation);
	vivid::DrawTexture(m_pathdata.BodyLightPath, draw_position, color, m_PlayerData.Rect, m_PlayerData.Anchor, m_PlayerData.Scale, m_PlayerData.Rotation);

	// 影の描画
	if (m_PhysicsComponent.IsLanding())
	{
		vivid::DrawTexture(m_pathdata.ShadowPath, draw_position + vivid::Vector2(0, 75 * camera_scale), 0xffffffff, vivid::Rect{ 0, 0, 64, 80 }, vivid::Vector2::ZERO, m_PlayerData.Scale);
	}
}

/*
 *	右腕の描画
 */
void
CPlayer::
DrawRightArm(void)
{
	if (!IsDrawVisible()) return;

	m_AttackComponent.RightArmDraw();
}

/*
 *  解放
 */
void
CPlayer::
Finalize(void)
{
}

/*
 *	プレイヤーが操作状態にあるかどうか
 */
bool
CPlayer::
IsOperating(void) const
{
	return m_PlayerInput.IsOperating();;
}

/*
 *  投げられるかどうか
 */
bool
CPlayer::
CanBeThrown(void) const
{
	if (!m_ActiveFlg) return false;
	if (m_InvincibleFlg) return false;
	if (!m_PhysicsComponent.IsLanding()) return false;

	if (m_StateController.IsActionState(ACTION_STATE::RESPAWN)) return false;
	if (m_StateController.IsActionState(ACTION_STATE::STIFFNESS)) return false;
	if (m_StateController.IsActionState(ACTION_STATE::THROWN)) return false;
	if (m_ThrowComponent.IsSuccess()) return false;

	return true;
}

/*
 *  ダメージ
 */
bool
CPlayer::
Damage(const DamageInfo& damage_info)
{
	// 無敵状態で無敵を無視しないダメージは受けない
	if (m_InvincibleFlg) return false;

	// 投げ中の場合は投げをキャンセル
	m_ThrowComponent.Cancel();

	// HPを減少させる
	m_ResourceComponent.AddCurrent(RESOURCE_ID::HP, -damage_info.HpDamage);

	// ノックバックの処理
	m_PhysicsComponent.AddVelocity(VelocityID::KnockBack, damage_info.KnockBack);

	// ヒットストップの処理
	this->SetHitStop(damage_info.HitStop);

	// 硬直の処理
	this->Stiffness(damage_info.HitStun);

	return true;
}

/*
 *  硬直
 */
void
CPlayer::
Stiffness(const int time)
{
	m_StateController.ChangeActionState(ACTION_STATE::STIFFNESS);
	m_StiffnessTime = time;
	// プレイヤーの微振動を有効にする
	CCameraManager::GetInstance().SetPlayerMicroShake(m_PlayerData.PlayerID, true, vivid::Vector2(1.3f, 0.4f));
}

/*
 *  ヒットストップ
 */
void
CPlayer::
SetHitStop(const int time)
{
	m_HitStopTime = time;
}

/*
 *  無敵
 */
void
CPlayer::
Invincible(const int time)
{
	m_InvincibleFlg = true;
	m_InvincibleTime = time;
}

/*
 *  操作
 */
void
CPlayer::
Move(vivid::Vector2 dir)
{
	m_MoveFlg = true;

	m_PlayerData.Direction = dir.x > 0.0f;

	m_PhysicsComponent.Move(dir, m_AttributeComponent);
}

/*
 *  攻撃
 */
void
CPlayer::
Attack()
{
	// 攻撃の種類を決定
	if(m_MoveFlg)					
	{
		// 横方向の攻撃
		m_StateController.ChangeActionState(ACTION_STATE::ATTACK_SIDE);
	}
	else if (m_InputJumpFlg)		
	{
		// 上方向の攻撃
		m_StateController.ChangeActionState(ACTION_STATE::ATTACK_UP);
	}
	else if (m_InputPlummetFlg)		
	{
		// 下方向の攻撃
		m_StateController.ChangeActionState(ACTION_STATE::ATTACK_DOWN);
	}
	else							
	{
		// 通常攻撃
		m_StateController.ChangeActionState(ACTION_STATE::ATTACK_NEUTRAL);
	}
}

/*
 *  生存
 */
void
CPlayer::
Alive(void)
{
	// ヒットストップの処理
	if(m_HitStopTime > 0)
	{
		m_HitStopTime--;
		return;
	}

	// 硬直の処理
	if (m_PhysicsComponent.IsLandingStiffness())
	{
		m_LandingSquash = 0.82f;
		this->Stiffness(m_land_stiffness_time);
		m_PhysicsComponent.SetLandingStiffnessFlg(false);
		return;
	}

	// 着地時の沈み込みの回復
	if (m_LandingSquash < 1.0f)
	{
		m_LandingSquash += 0.03f;
		if (m_LandingSquash > 1.0f)
			m_LandingSquash = 1.0f;
	}

	// 投げの更新
	m_ThrowComponent.Update(*this);

	// アクション
	this->Action();

	// 状態の更新
	this->StateUpdate();

	// 入力の更新
	m_PlayerInput.Update();

	// 操作
	this->Control();

	// 物理演算の更新
	if (!m_StateController.IsActionState(ACTION_STATE::THROWN))
	{
		m_PhysicsComponent.Update();
	}
	else
	{
		m_PhysicsComponent.ResetVelocity(VelocityID::Final);
		m_PhysicsComponent.HitCapsuleUpdate();
	}

	// ビルドの更新
	m_BuildComponent.PassiveUpdate(m_AttributeComponent);

	// ステータスの更新
	m_ResourceComponent.Update();
	m_AttributeComponent.Update();

	vivid::Vector2 input_dir = vivid::Vector2(m_PlayerInput.GetInput().MoveX, m_PlayerInput.GetInput().MoveY);

	// 攻撃の更新
	m_AttackComponent.Update(m_PhysicsComponent.GetPosition(), m_PlayerData.Direction, input_dir, m_StateController.GetActionState() == ACTION_STATE::GUARD);

	// ガードの更新
	m_GuardComponent.Update(m_ResourceComponent);

	// 投げ中の腕ポーズ更新
	m_ThrowComponent.ApplyArmPose(*this);

	// 無敵時間の更新
	this->InvincibleUpdate();
}

/*
 *  死亡
 */
void
CPlayer::
Dead(void)
{
	this->SetActive(false);
}

/*
 *  無敵更新
 */
void
CPlayer::
InvincibleUpdate(void)
{
	// 無敵状態でない場合は処理を行わない
	if (!m_InvincibleFlg) return;

	// 無敵時間の減少
	if (m_InvincibleTime > 0)
		m_InvincibleTime--;

	// 無敵時間処理
	if (m_InvincibleTime <= 0)
	{
		m_InvincibleTime = 0;
		m_InvincibleFlg = false;
	}
}

/*
 *  状態更新
 */
void
CPlayer::
StateUpdate(void)
{
	// 生存状態の更新
	this->CharacterAliveStateUpdate();

	// 欲望状態の更新
	m_StateController.DesireStateUpdate(this);

	// キャラクター移動状態の更新
	m_StateController.MoveStateUpdate(&m_PhysicsComponent);
}

/*
 *  生存状態更新
 */
void
CPlayer::
CharacterAliveStateUpdate(void)
{
	// 画面外へ落下した場合はHPを0にする
	if (m_PhysicsComponent.GetPosition().y >= CStageManager::GetInstance().GetMapChipHeight() * CStageManager::GetInstance().GetBlockSize())
	{
		m_ResourceComponent.ClearCurrent(RESOURCE_ID::HP);
		CSoundManager::GetInstance().PlaySE(SOUND_ID::GUARD_BREAK);
	}

	 // HPが0以下になった場合
	if (m_ResourceComponent.IsZero(RESOURCE_ID::HP))
	{
		// 残機を減らす
		m_ResourceComponent.AddCurrent(RESOURCE_ID::LIFE, -1);

		// 残機がある場合はリスポーン
		if (!m_ResourceComponent.IsZero(RESOURCE_ID::LIFE))
		{
			// HPを最大値に戻す
			m_ResourceComponent.ResetCurrent(RESOURCE_ID::HP);

			// 無敵状態にする
			this->Invincible(m_max_invincible_time);

			// 落下硬直が有効になっていたら解除
			if (m_PhysicsComponent.IsLandingStiffness())
				m_PhysicsComponent.SetLandingStiffnessFlg(false);

			// 硬直を解除
			m_StiffnessTime = 0;

			CCameraManager::GetInstance().SetPlayerMicroShake(m_PlayerData.PlayerID, false, vivid::Vector2::ZERO);
			
			// リスポーン時間を設定
			m_RespawnTime = m_respawn_delay;

			// キャラクター動作状態をリスポーンにする
			m_StateController.ChangeActionState(ACTION_STATE::RESPAWN);
		}
		else
		{
			// 残機がない場合は死亡
			m_StateController.ChangeAliveState(ALIVE_STATE::DEAD);
		}
	}
}


/*
 *  操作
 */
void
CPlayer::
Control(void)
{
	Input input = m_PlayerInput.GetInput();

	m_MoveFlg = false;
	m_JumpFlg = false;
	m_PlummetFlg = false;
	m_InputJumpFlg = false;
	m_InputPlummetFlg = false;

	// ガード可能な状態かどうかの判定
	if (this->CanGuard())
	{
		// ガードの入力がある場合はガード状態にする
		if (input.Guard && !m_GuardComponent.IsBreak())
		{
			m_GuardComponent.Start();
			m_StateController.ChangeActionState(ACTION_STATE::GUARD);
		}
		else
		{
			m_GuardComponent.End();
		}
	}

	// 操作可能かどうかの判定
	if (!this->CanControl())	return;

	// 投げ入力
	if (input.Throw && this->CanThrow())
	{
		m_PhysicsComponent.ResetVelocity(VelocityID::Move);
		m_ThrowComponent.Start();
		m_StateController.ChangeActionState(ACTION_STATE::THROW);
		return;
	}

	// 移動入力がある場合
	if (input.MoveX != 0.0f)
		this->Move(vivid::Vector2(input.MoveX, 0.0f));

	// ジャンプの入力がある場合
	if (input.Jump && !input.Plummet)
	{
		if (m_JumpCount < m_max_jump_count)
		{
			// ジャンプフラグを立ててジャンプ回数を増加
			m_JumpFlg = true;
			m_JumpCount++;
		}
	}

	// 着地している場合はジャンプ回数をリセット
	if (m_PhysicsComponent.IsLanding())
		m_JumpCount = 0;

	// ジャンプの入力がある場合はジャンプ処理を行う
	if (m_JumpFlg)
	{
		vivid::Vector2 effect_pos = m_PhysicsComponent.GetPosition();

		CEffectManager::GetInstance().Create(EFFECT_ID::JAMP, m_PlayerData.PlayerID, effect_pos, DIRECTION::RIGHT
			, vivid::Vector2(1.0f,1.0f), Utility::GetColorById(COLOR_ID::WHITE), 0.0f);

		m_PhysicsComponent.Jump(input.MoveX, m_AttributeComponent.GetValue(ATTRIBUTE_ID::JUMP_POWER));
	}

	// 垂直方向の入力がある場合
	if (input.MoveY)
	{
		if (input.MoveY < 0.0f)
			m_InputJumpFlg = true;
		else
			m_InputPlummetFlg = true;
	}

	// 落下状態の場合は急落下の処理を行う
	// 急落下処理
	if (input.PlummetReleased)
	{
		m_PlummetTimerFlg = true;
		m_PlummetInputTimer = m_plummet_input_time;
	}

	if (m_PlummetTimerFlg)
	{
		// 急落下入力時間の減少
		m_PlummetInputTimer--;
		// 急落下入力時間のチェック
		if (m_PlummetInputTimer <= 0)
		{
			m_PlummetInputTimer = 0;
			m_PlummetTimerFlg = false;
		}
	}

	// 急落下の入力がある場合
	if (input.Plummet)
	{
		if (m_PlummetInputTimer > 0)
			m_PlummetFlg = true;
	}

	// 急落下フラグの設定
	if (m_PlummetFlg)
		m_PhysicsComponent.SetPlummetFlg(true);
	else
		m_PhysicsComponent.SetPlummetFlg(false);

	// 攻撃可能かどうかの判定
	if (this->CanAttack())
	{
		if (input.Attack)		this->Attack();

		// 無欲状態でない場合はスキル処理を行う
		if (m_StateController.GetDesireState() != DESIRE_STATE::MUYOKU)
		{
			if (input.SkillA)		m_StateController.ChangeActionState(ACTION_STATE::SKILLA);
			if (input.SkillX)		m_StateController.ChangeActionState(ACTION_STATE::SKILLX);
		}

		const ATTACK_ID attack_id =	this->GetCurrentAttackID();

		if (attack_id != ATTACK_ID::NONE)
		{
			ARM_ID arm_id = ARM_ID::BOTH;

			switch (m_StateController.GetActionState())
			{
			case ACTION_STATE::SKILLX:	arm_id = ARM_ID::RIGHT;		break;
			case ACTION_STATE::SKILLA:
				
				if (!m_BuildComponent.IsSameBuild())
				{
					arm_id = ARM_ID::LEFT;
				}
				else
				{
					arm_id = ARM_ID::BOTH;
				}

				break;

			default:					arm_id = ARM_ID::BOTH;		break;
			}


			m_AttackComponent.Attack(attack_id, m_ResourceComponent, m_AttributeComponent, m_PlayerData.Direction, arm_id);
		}
	}
}

/*
 *  キャラクターのアクション
 */
void
CPlayer::
Action(void)
{
	/*
	 *		キャラクターのアクションの更新
	 */

	// キャラクター動作状態によって処理を分ける
	if (m_StateController.GetActionState() == ACTION_STATE::IDLE)
	{
		// 待機状態の処理
		return;
	}
	else if (m_StateController.GetActionState() == ACTION_STATE::RESPAWN)
	{
		// リスポーン状態の処理
		this->Respawn();
		return;
	}
	else if (m_StateController.GetActionState() == ACTION_STATE::STIFFNESS)
	{
		// 硬直状態の処理
		this->Stiffness();
		return;
	}
	else if(m_StateController.GetActionState() == ACTION_STATE::GUARD)
	{
		// ガード状態の処理
		this->Guard();
		return;
	}
	else if (m_StateController.GetActionState() == ACTION_STATE::THROW)
	{
		// 投げ状態の処理
		this->ThrowUpdate();
		return;
	}
	else if (m_StateController.GetActionState() == ACTION_STATE::THROWN)
	{
		// 投げられ状態の処理
		this->Thrown();
		return;
	}
	else
	{
		// 攻撃状態の処理
		this->AttackUpdate();
		return;
	}

}

/*
 *  攻撃
 */
void
CPlayer::
AttackUpdate(void)
{
	// 攻撃が終了している場合は待機状態にする
	if (!m_AttackComponent.IsAttacking())
	{
		m_StateController.ChangeActionState(ACTION_STATE::IDLE);
	}
}

/*
 *  投げ
 */
void
CPlayer::
ThrowUpdate(void)
{
	if (!m_ThrowComponent.IsThrowing())
	{
		m_StateController.ChangeActionState(ACTION_STATE::IDLE);
	}
}

/*
 *  投げられ
 */
void
CPlayer::
Thrown(void)
{
	m_PhysicsComponent.ResetVelocity(VelocityID::Final);
}

/*
 *  ガード
 */
void
CPlayer::
Guard(void)
{
	// ガードブレイク時の処理
	if (m_GuardComponent.IsBreak())
	{
		const int break_time = m_GuardComponent.GetBreakStiffnessTime();

		// 両腕のガードブレイクモーション開始
		m_AttackComponent.StartGuardBreak(break_time);

		// ガードブレイク硬直
		this->Stiffness(break_time);

		return;
	}

	// ジャストガードの処理
	if(m_GuardComponent.IsJustGuard())
	{
		m_GuardComponent.End();
		// ジャストガード時の処理を追加する場合はここに記述;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	}

	// ガード状態でない場合は待機状態にする
	if (!m_GuardComponent.IsGuard())
		m_StateController.ChangeActionState(ACTION_STATE::IDLE);
}

/*
 *  硬直
 */
void
CPlayer::
Stiffness(void)
{
	// 硬直時間の減少
	if (m_StiffnessTime > 0)
	{
		m_StiffnessTime--;
		return;
	}

	// 硬直時間が0以下になった場合は待機状態にする
	if (m_StiffnessTime <= 0)
	{
		// ガードブレイク状態の場合はガードブレイクをリセット
		if (m_GuardComponent.IsBreak())
			m_GuardComponent.ResetBreak(m_ResourceComponent);

		// プレイヤーの微振動を無効にする
		CCameraManager::GetInstance().SetPlayerMicroShake(m_PlayerData.PlayerID, false, vivid::Vector2::ZERO);

		m_StiffnessTime = 0;
		m_StateController.ChangeActionState(ACTION_STATE::IDLE);
	}
}

/*
 *  リスポーン
 */
void
CPlayer::
Respawn(void)
{
	// キャラクターの位置を固定
	m_PhysicsComponent.SetPosition(m_PhysicsComponent.GetInitialPosition());
	m_PhysicsComponent.ResetVelocity(VelocityID::Final);
	
	// リスポーン待機
	if (m_RespawnTime > 0)
	{
		CEffectManager::GetInstance().Create(EFFECT_ID::RESPAWN_PARTICLE, m_PlayerData.PlayerID, m_PhysicsComponent.GetPosition(), DIRECTION::RIGHT, vivid::Vector2(0.5f, 0.5f), m_PlayerData.Color, 0.0f);

		m_RespawnTime--;
		return;
	}

	vivid::Vector2 effect_pos = m_PhysicsComponent.GetPosition();

	CEffectManager::GetInstance().Create(EFFECT_ID::RESPAWN_PARTICLE, m_PlayerData.PlayerID, m_PhysicsComponent.GetPosition(), DIRECTION::RIGHT, vivid::Vector2(0.5f, 0.5f), m_PlayerData.Color, 0.0f);

	// キャラクターの位置を固定
	m_PhysicsComponent.SetPosition(m_PhysicsComponent.GetInitialPosition());
	m_PhysicsComponent.ResetVelocity(VelocityID::Final);

	// ジャンプ回数をリセット
	m_JumpCount = 0;

	// 操作が入るとキャラクターの状態を待機にする
	if (m_MoveFlg || m_JumpFlg)
		m_StateController.ChangeActionState(ACTION_STATE::IDLE);
}

/*
 *  操作可能かどうか
 */
bool
CPlayer::
CanControl(void)
{
	// 操作可能かどうかの判定
	return m_StateController.GetActionState() == ACTION_STATE::IDLE ||
		m_StateController.GetActionState() == ACTION_STATE::RESPAWN;
}

/*
 *  攻撃可能かどうか
 */
bool
CPlayer::
CanAttack(void)
{
	return m_StateController.GetActionState() == ACTION_STATE::IDLE;
}

/*
 *  ガード可能かどうか
 */
bool
CPlayer::
CanGuard(void)
{
	return m_StateController.GetActionState() == ACTION_STATE::IDLE ||
		m_StateController.GetActionState() == ACTION_STATE::GUARD;
}

/*
 *  投げ可能かどうか
 */
bool
CPlayer::
CanThrow(void)
{
	return m_StateController.GetActionState() == ACTION_STATE::IDLE &&
		m_PhysicsComponent.IsLanding();
}


/*
 *  現在の攻撃IDを取得
 */
ATTACK_ID
CPlayer::
GetCurrentAttackID() const
{
	switch (m_StateController.GetActionState())
	{
	case ACTION_STATE::ATTACK_NEUTRAL:
		return ATTACK_ID::ATTACK_NEUTRAL;

	case ACTION_STATE::ATTACK_SIDE:
		return ATTACK_ID::ATTACK_SIDE;

	case ACTION_STATE::ATTACK_UP:
		return ATTACK_ID::ATTACK_UP;

	case ACTION_STATE::ATTACK_DOWN:
		return ATTACK_ID::ATTACK_DOWN;

	case ACTION_STATE::SKILLX:
		return SkillResolver::GetSkillX(m_BuildComponent.GetBuild());

	case ACTION_STATE::SKILLA:
		return SkillResolver::GetSkillA(m_BuildComponent.GetBuild());

	default:
		return ATTACK_ID::NONE;
	}
}

/*
 *  描画可能かどうか
 */
bool
CPlayer::
IsDrawVisible(void) const
{
	if (!m_ActiveFlg || m_RespawnTime > 0)
		return false;

	// 無敵時間中の点滅
	if (m_InvincibleTime > 0 &&
		(int)(m_InvincibleTime / m_invincible_visible_interval) % 2 != 0)
	{
		return false;
	}

	return true;
}
