
/*
 *  @file       player.cpp
 *  @brief      プレイヤー
 *  @author     Ryusei Shimizu
 *  @date       2025/10/15
 */

#include "player.h"
#include "../../../stage_manager/stage_manager.h"
#include "../../../camera_manager/camera_manager.h"
#include "../../../bullet_manager/bullet_manager.h"
#include "../../../upgrade_manager/upgrade_manager.h"
#include "../../../attack_manager/attack_manager.h"
#include "../../../effect_manager/effect_manager.h"
#include "../../../sound_manager/sound_manager.h"
#include "../../../../../utility/utility.h"


/*
 *	ステータス定義
 */

//!< ステータス配列
const float CPlayer::m_status[(int)STATUS_ID::MAX][(int)STATUS_DEFINITION::MAX] =
{
	//  初期値		最大値		最小値
	{   100.0f,		200.0f,		 0.0f },	//!< HP
	{     0.0f,		100.0f,		 0.0f },	//!< 満腹度
	{    10.0f,		 30.0f,		10.0f },	//!< 攻撃力
	{     2.7f,	      3.4f,	     2.0f },	//!< 速度
	{	  0.0f,		  0.0f,		 0.0f },	//!< 触れたときのダメージ
};

//!< アップグレードステータス配列
const float CPlayer::m_upgrade_status[(int)UPGRADE_STATUS_ID::MAX] =
{
	   0.7f,	//!< 弾のダメージ倍率
	   0.5f,	//!< 弾のクールタイム
	   8.0f,	//!< 弾の持続時間
	  14.0f,	//!< 弾のスピード
	   1.4f,	//!< 突進のダメージ倍率
	   4.0f, 	//!< 突進のクールタイム
       1.0f, 	//!< 突進の持続時間
	   6.0f, 	//!< 突進のスピード
};

const int				CPlayer::m_size							= 90;								//!< サイズ
const std::string       CPlayer::m_head_path_name				= "data\\character\\head.png";		//!< 画像名
const std::string       CPlayer::m_body_path_name				= "data\\character\\body.png";		//!< 画像名

const int               CPlayer::m_default_body_quantity		= 5;					//!< 体の数の初期値
const int               CPlayer::m_max_body_quantity			= 20;					//!< 体の数の最大値
const int               CPlayer::m_min_body_quantity			= 3;					//!< 体の数の最小値

const float				CPlayer::m_fullness_increase			= 10.0f;				//!< 満腹度増加量
const float				CPlayer::m_fullness_decrease			= 10.0f;				//!< 満腹度減少量
const float             CPlayer::m_fullness_decrease_interval	= 8.0f;					//!< 満腹度減少間隔

const float				CPlayer::m_rush_interval				= 1.0f;					//!< 突進のチャージ間隔

/*
 *  コンストラクタ
 */
CPlayer::
CPlayer(void)
	: ICharacter(m_size, m_size, m_head_path_name, CHARACTER_CATEGORY::PLAYER, CHARACTER_ID::PLAYER)
	, m_BodyQuantity(0)
	, m_TargetBodyQuantity(0)
	, m_ForrowMoveFlg(false)
	, m_UpdirectionFlg(false)
	, m_FullnessDecreaseFlg(false)
	, m_FullnessDecreaseTimer(0.0f)
	, m_BulletFireTimer(0)
{
}

/*
 *  コンストラクタ
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
Initialize(const vivid::Vector2& position)
{
	m_BodyPartsList.clear();

	// 基底クラスのInitializeを呼ぶ
	ICharacter::Initialize(position);

	m_Rotation = DEG_TO_RAD(90);

	// 体の数初期化
	m_BodyQuantity = 0;
	m_TargetBodyQuantity = m_default_body_quantity;

	// 追従フラグ、上方向フラグ、操作フラグ初期化
	m_ForrowMoveFlg = false;
	m_UpdirectionFlg = false;
	m_ControlFlg = false;

	// 突進初期化
	m_RushFlg = false;
	m_RushChargeFlg = false;
	m_RushIntervalTimer = m_rush_interval;

	// 満腹度減少処理初期化
	m_FullnessDecreaseFlg = false;
	m_FullnessDecreaseTimer = m_fullness_decrease_interval;

	// 弾発射タイマー初期化
	m_BulletFireTimer = 0;
	
	// 強化値の初期化
	m_RushAddDurationValue = 0;

	// 体のパーツを目標の数まで生成
	while (m_BodyQuantity < m_TargetBodyQuantity)
	{
		this->CreateBodyParts();
	}

	// ステータスの初期化
	for (int i = 0; i < (int)STATUS_ID::MAX; i++)
	{
		m_Status[i] = m_status[i][(int)STATUS_DEFINITION::DEFAULT];
		m_MaxStatus[i] = m_status[i][(int)STATUS_DEFINITION::DEFAULT];
	}

	// 強化ステータスの初期化
	for (int i = 0; i < (int)UPGRADE_STATUS_ID::MAX; i++)
	{
		m_UpgradeStatus[i] = m_upgrade_status[i];
	}

	m_UpgradeStatus[(int)UPGRADE_STATUS_ID::RUSH_COOLTIME] = 0.0f;

	m_MaxStatus[(int)STATUS_ID::FULLNESS] = m_status[(int)STATUS_ID::FULLNESS][(int)STATUS_DEFINITION::MAXIMUM];
};

/*
 *	更新
 */
void
CPlayer::
Update(void)
{
	CEffectManager& em = CEffectManager::GetInstance();

	ICharacter::Update();

	// 追従フラグが立っている場合、体のパーツを更新
	if (m_ForrowMoveFlg)
	{
		BodyPartsList::iterator it = m_BodyPartsList.begin();
		while (it != m_BodyPartsList.end())
		{
			CParts* parts = (*it);

			// 突進チャージ中は、体のパーツが縮んでいく処理が行われる
			if (m_RushChargeFlg)
			{
				float rate = (m_rush_interval - m_RushIntervalTimer) / m_rush_interval;
				float distance = (1.0f - rate) * 50 + 10;

				em.Create(EFFECT_ID::RUSH_CHARGE, m_CenterPosition, m_Color,0.0f);

				parts->Update(distance);
			}
			else
			{
				// 更新では、追従処理が行われる
				parts->Update();
			}
			
			++it;
		}
	}
}

/*
 *	描画
 */
void
CPlayer::
Draw(void)
{
	CUpgraeStatusManager& um = CUpgraeStatusManager::GetInstance();
	m_Color = um.GetUpgradeStatusNameColor();

	// 無敵時間中は点滅し、平常時は常に表示される
	if (m_InvincibleTime <= 0 || (int)(m_InvincibleTime / m_invincible_visible_interval) % 2 == 0)
	{
		vivid::Vector2 pos = m_Position;
		pos -= CCameraManager::GetInstance().GetPosition();

		// プレイヤー本体を描画
		vivid::DrawTexture(m_PathName, pos, m_Color, m_Rect, m_Anchor, m_Scale, m_Rotation);

		// 体のパーツを描画
		BodyPartsList::iterator it = m_BodyPartsList.begin();
		while (it != m_BodyPartsList.end())
		{
			CParts* parts = (*it);
			parts->Draw(m_Color);
			++it;
		}
	}
}

/*
 *	解放
 */
void
CPlayer::
Finalize(void)
{
	ICharacter::Finalize();

	// 体のパーツを全て削除
	BodyPartsList::iterator it = m_BodyPartsList.begin();
	while (it != m_BodyPartsList.end())
	{
		CParts* parts = (*it);
		parts->Finalize();
		parts->SetActive(false);
		delete parts;
		++it;
	}
	m_BodyPartsList.clear();
}

/*
 *	自キャラの操作フラグ
 */
bool
CPlayer::
IsControlFlg(void) const
{
	return m_ControlFlg;
}

/*
 *	満腹度上昇
 */
void
CPlayer::
IncreaseFullness(void)
{
	// 満腹度を増加
	this->IncreaseStatus(STATUS_ID::FULLNESS, m_fullness_increase);

	// 満腹度減少タイマーリセット
	m_FullnessDecreaseTimer = m_fullness_decrease_interval;
}

/*
 *  弾との判定
 */
bool
CPlayer::
CheckHitBullet(IBullet* bullet)
{
	// 弾が存在しない、もしくは同じカテゴリ、無敵状態、死亡状態なら処理を行わない
	if (!bullet || m_CharacterCategory == bullet->GetBulletCategory() || m_InvincibleFlg || m_CharacterAliveState == CHARACTER_ALIVE_STATE::DEAD || m_ControlFlg)
		return false;

	BodyPartsList::iterator it = m_BodyPartsList.begin();

	while (it != m_BodyPartsList.end())
	{
		CParts* parts = (*it);

		// 当たり判定チェック
		// 体のパーツ、もしくはプレイヤー本体に当たったら処理
		if (u_CheckHitObject(bullet->GetPosition(), bullet->GetWidth(), bullet->GetHeight(),
			parts->GetPosition() + vivid::Vector2(15.0f, 15.0f), parts->GetWidth() - 15, parts->GetHeight() - 15, parts->GetRotation()) ||
			u_CheckHitObject(bullet->GetPosition(), bullet->GetWidth(), bullet->GetHeight(),
				this->GetPosition() + vivid::Vector2(15.0f, 15.0f), this->GetWidth() - 15, this->GetHeight() - 15, this->GetRotation()))
		{
			bullet->SetActive(false);

			m_Status[(int)STATUS_ID::HP] -= bullet->GetStatus(BULLET_STATUS_ID::DAMAGE);

			//プレイヤーダメージ音
			CSoundManager::GetInstance().Play(SOUND_ID::PLAYER_DAMAGE, false);

			this->Invincible();

			return true;
		}
		++it;
	}
	return false;
}

/*
 *	自キャラと敵との当たり判定
 */
bool
CPlayer::
Hit(ICharacter* enemy)
{
	// 敵が存在しない、もしくは同じカテゴリ、無敵状態、死亡状態なら処理を行わない
	if (!enemy || m_CharacterCategory == enemy->GetCharacterCategory() || m_InvincibleFlg || m_CharacterAliveState == CHARACTER_ALIVE_STATE::DEAD || m_ControlFlg)
		return false;

	BodyPartsList::iterator it = m_BodyPartsList.begin();

	while (it != m_BodyPartsList.end())
	{
		CParts* parts = (*it);

		// 当たり判定チェック
		// 体のパーツ、もしくはプレイヤー本体に当たったら処理
		if (u_CheckHitObject(enemy->GetPosition(), enemy->GetWidth(), enemy->GetHeight(),
			parts->GetPosition() + vivid::Vector2(15.0f, 15.0f), parts->GetWidth() - 15, parts->GetHeight() - 15, parts->GetRotation()) ||
			u_CheckHitObject(enemy->GetPosition(), enemy->GetWidth(), enemy->GetHeight(),
				this->GetPosition() + vivid::Vector2(15.0f, 15.0f), this->GetWidth() - 15, this->GetHeight() - 15, this->GetRotation()))
		{
			m_Status[(int)STATUS_ID::HP] -= enemy->GetStatus(STATUS_ID::TOUTHDAMAGE);

			//プレイヤーダメージ音
			CSoundManager::GetInstance().Play(SOUND_ID::PLAYER_DAMAGE, false);

			this->Invincible();

			return true;
		}
		++it;
	}
	return false;
}

/*
 *  強化ステータスの取得
 */
float
CPlayer::
GetPlayerUpgradeStatus(UPGRADE_STATUS_ID id) const
{
	return m_upgrade_status[(int)id];
}

/*
 *  無敵更新
 */
void
CPlayer::
Invincible(void)
{
	m_InvincibleTime = m_max_invincible_time;
	m_InvincibleFlg = true;
}

/*
 *  強化ステータス更新
 */
void
CPlayer::
UpgradeStatusUpdate(void)
{
	CUpgraeStatusManager& um = CUpgraeStatusManager::GetInstance();

	um.SetFullness(this->GetStatus(STATUS_ID::FULLNESS));

	// 強化ステータスの更新
	for (int i = 0; i < (int)UPGRADE_STATUS_ID::MAX; i++)
	{
		// 強化ステータスIDと倍率を取得
		UPGRADE_STATUS_ID	upgrade_status_id = um.GetUpgradeStatusID((UPGRADE_STATUS_ID)i);
		float				upgrade = um.GetUpgrade((UPGRADE_STATUS_ID)i);

		// 弾のダメージ強化
		if (upgrade_status_id == UPGRADE_STATUS_ID::BULLET_DAMAGE_MAG)
		{
			// 強化ステータスに倍率をかける
			m_UpgradeStatus[(int)upgrade_status_id] = m_upgrade_status[(int)upgrade_status_id] + upgrade;
		}
		// 突進のダメージ強化
		else if (upgrade_status_id == UPGRADE_STATUS_ID::RUSH_DAMAGE_MAG)
		{
			// 強化ステータスに倍率をかける
			m_UpgradeStatus[(int)upgrade_status_id] = m_upgrade_status[(int)upgrade_status_id] + upgrade;
		}
		else if (upgrade_status_id == UPGRADE_STATUS_ID::RUSH_DURATION)
		{
			// 強化ステータスに加算する
			m_RushAddDurationValue = upgrade;
		}
		else if (upgrade_status_id == UPGRADE_STATUS_ID::RUSH_SPEED)
		{
			// 強化ステータスに加算する
			m_UpgradeStatus[(int)upgrade_status_id] = m_upgrade_status[(int)upgrade_status_id] + upgrade;
		}
	}
}

/*
 *	生存
 */
void
CPlayer::
Alive(void)
{
#if 0

	m_InvincibleFlg = true;

#endif

	// 死亡判定
	if (m_Status[(int)STATUS_ID::HP] <= m_status[(int)STATUS_ID::HP][(int)STATUS_DEFINITION::MINIMUM])
	{
		m_CharacterAliveState = CHARACTER_ALIVE_STATE::DEAD;
		return;
	}

	// 冬のとき、時間が10の倍数のとき、HPを減らす
	CGameParameterManager& gpm = CGameParameterManager::GetInstance();
	float time = gpm.GetDayCycleNumber();
	if ((int)time % 10 == 0 && gpm.GetSeasonId() == SEASON_ID::WINTER)
	{
		m_Status[(int)STATUS_ID::HP] -= 0.05f;
	}

	// 操作可能なら
	if (!m_ControlFlg)
	{
		// 操作
		this->Control();
	}

	// 攻撃
	this->Attack();

	// 発射
	this->Fire();

	// 位置反映
	this->Move();

	// ブロックとの当たり判定
	this->CheckHitBlock();

	// 無敵更新
	this->invincibleUpdate();

	// 冬の間は満腹度が減少しないため、冬のときは満腹度減少更新を行わない
	if (gpm.GetSeasonId() != SEASON_ID::WINTER)
	{
		// 満腹度減少更新
		this->FullnessDecreaseUpdate();
	}

	// 強化ステータス更新
	this->UpgradeStatusUpdate();

	// 体の数更新
	this->BodyQuantityUpdate();
}

/*
 *	ブロックとの当たり判定
 */
void
CPlayer::
CheckHitBlock(void)
{
	CStageManager& sm = CStageManager::GetInstance();
	int blockSize = sm.GetBlockSize();

	vivid::Vector2& pos = m_Position;

	// プレイヤーの四隅のブロック座標を取得
	int left = static_cast<int>(pos.x);
	int top = static_cast<int>(pos.y);
	int right = static_cast<int>(pos.x + m_Width);
	int bottom = static_cast<int>(pos.y + m_Height);

	// 四隅のブロック座標
	int blockLeft = left / blockSize;
	int blockTop = top / blockSize;
	int blockRight = (right - 1) / blockSize;
	int blockBottom = (bottom - 1) / blockSize;

	bool block_hit = false;

	// ブロックとの当たり判定を行う
	if (sm.IsWall(blockLeft, blockTop))    block_hit = true;
	if (sm.IsWall(blockRight, blockTop))   block_hit = true;
	if (sm.IsWall(blockLeft, blockBottom)) block_hit = true;
	if (sm.IsWall(blockRight, blockBottom))block_hit = true;

	// 追従フラグの設定
	m_ForrowMoveFlg = !block_hit;

	// 当たっていたら
	if (block_hit)
	{
		// 当たっていたら前フレームの位置に戻す
		m_Position = m_PreviousPosition;
		// 速度も止める
		m_Accelerator = vivid::Vector2::ZERO;
		m_Velocity = vivid::Vector2::ZERO;

		// 突進中なら突進も止める
		m_RushFlg = false;
		m_ControlFlg = false;

		// 体のパーツの移動も止める
		this->StopBodyParts();
	}
}

/*
 *	攻撃
 */
void
CPlayer::
Attack(void)
{
	CAttackManager& am = CAttackManager::GetInstance();

	std::vector<vivid::Vector2>previouspos;

	// 頭の座標を追加
	previouspos.push_back(this->GetPosition());

	BodyPartsList::iterator it = m_BodyPartsList.begin();
	// 体の座標を追加
	while (it != m_BodyPartsList.end())
	{
		CParts* parts = (*it);
		previouspos.push_back(parts->GetPosition());
		++it;
	}

	// 突進のクールタイムを減らす
	if (m_UpgradeStatus[(int)UPGRADE_STATUS_ID::RUSH_COOLTIME] > 0)
		m_UpgradeStatus[(int)UPGRADE_STATUS_ID::RUSH_COOLTIME] -= vivid::GetDeltaTime();

	namespace keyboard = vivid::keyboard;
	namespace controller = vivid::controller;

	m_RushChargeFlg = false;

	bool c = keyboard::Button(keyboard::KEY_ID::C) ||
		controller::Button(controller::DEVICE_ID::PLAYER1,controller::BUTTON_ID::B);

	if (!m_RushFlg)
	{
		if (c && m_UpgradeStatus[(int)UPGRADE_STATUS_ID::RUSH_COOLTIME] <= 0)
		{
			// チャージ中
			m_RushIntervalTimer -= vivid::GetDeltaTime();

			// 速度を止める
			m_Accelerator = vivid::Vector2::ZERO;

			if (!m_RushChargeFlg)
				//突進のチャージサウンド再生
				CSoundManager::GetInstance().Play(SOUND_ID::CHARGE, false);

			// チャージフラグを立てる
			m_RushChargeFlg = true;

			// チャージ完了
			if (m_RushIntervalTimer <= 0.0f)
			{
				//突進のチャージサウンド停止
				CSoundManager::GetInstance().Stop(SOUND_ID::CHARGE);
				m_RushFlg = true;
				m_RushIntervalTimer = m_rush_interval;
			}
		}
		else
		{
			m_RushIntervalTimer = m_rush_interval;

			//突進のチャージサウンド停止
			CSoundManager::GetInstance().Stop(SOUND_ID::CHARGE);

			// 突進が途中で終了したとき、持続時間をリセットする
			if (m_UpgradeStatus[(int)UPGRADE_STATUS_ID::RUSH_DURATION] != m_upgrade_status[(int)UPGRADE_STATUS_ID::RUSH_DURATION] + m_RushAddDurationValue)
			{
				m_UpgradeStatus[(int)UPGRADE_STATUS_ID::RUSH_DURATION] = m_upgrade_status[(int)UPGRADE_STATUS_ID::RUSH_DURATION] + m_RushAddDurationValue;
				m_UpgradeStatus[(int)UPGRADE_STATUS_ID::RUSH_COOLTIME] = m_upgrade_status[(int)UPGRADE_STATUS_ID::RUSH_COOLTIME];
				m_RushFlg = false;
				m_ControlFlg = false;
			}
		}
	}

	if (m_RushFlg)
	{
		int count = 0;

		// 操作不可
		m_ControlFlg = true;

		if (m_UpgradeStatus[(int)UPGRADE_STATUS_ID::RUSH_DURATION] == m_upgrade_status[(int)UPGRADE_STATUS_ID::RUSH_DURATION] + m_RushAddDurationValue)
			//突進開始サウンド再生
			CSoundManager::GetInstance().Play(SOUND_ID::RUSH, false);

		// 突進の持続時間を減らす
		m_UpgradeStatus[(int)UPGRADE_STATUS_ID::RUSH_DURATION] -= vivid::GetDeltaTime();

		// 速度を増やす
		m_Velocity = am.Rush(m_Velocity, this->GetStatus(STATUS_ID::SPEED), m_Rotation, m_UpgradeStatus[(int)UPGRADE_STATUS_ID::RUSH_SPEED]);

		BodyPartsList::iterator it = m_BodyPartsList.begin();
		while (it != m_BodyPartsList.end())
		{
			// previousの0番目は頭それ以降は体、0番目からやることによって前のあたまの座標が一番最初の体の位置になる
			(*it)->SetPosition(previouspos[count] - m_Velocity.Normalize() * 25);
			++count;
			++it;
		}

		//持続タイマーが0になると
		if (m_UpgradeStatus[(int)UPGRADE_STATUS_ID::RUSH_DURATION] <= 0.0f)
		{
			m_UpgradeStatus[(int)UPGRADE_STATUS_ID::RUSH_DURATION] = m_upgrade_status[(int)UPGRADE_STATUS_ID::RUSH_DURATION] + m_RushAddDurationValue;
			m_UpgradeStatus[(int)UPGRADE_STATUS_ID::RUSH_COOLTIME] = m_upgrade_status[(int)UPGRADE_STATUS_ID::RUSH_COOLTIME];
			m_RushFlg = false;
			m_ControlFlg = false;

			// 速度を止める
			m_Velocity = vivid::Vector2::ZERO;
			m_Accelerator = vivid::Vector2::ZERO;
		}
	}
}

/*
 *	発射
 */
void
CPlayer::
Fire(void)
{
	namespace keyboard = vivid::keyboard;
	namespace controller = vivid::controller;

	CBulletManager& bm = CBulletManager::GetInstance();

	bool space = keyboard::Button(keyboard::KEY_ID::SPACE) ||
		controller::Button(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::A) ||
		controller::Button(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::RIGHT_SHOULDER);

	if (m_BulletFireTimer > 0.0f)
		m_BulletFireTimer -= vivid::GetDeltaTime();

	// スペースキーで弾を発射
	if (space)
	{
		if (m_BulletFireTimer <= 0)
		{
			m_BulletFireTimer = m_upgrade_status[(int)UPGRADE_STATUS_ID::BULLET_COOLTIME];

			vivid::Vector2 bullet_position = m_CenterPosition + vivid::Vector2(cosf(m_Rotation), sinf(m_Rotation)) * (m_Width / 2.0f);
			float dmg = m_Status[(int)STATUS_ID::ATTACKPOWER] * m_UpgradeStatus[(int)UPGRADE_STATUS_ID::BULLET_DAMAGE_MAG];
			float speed = m_UpgradeStatus[(int)UPGRADE_STATUS_ID::BULLET_SPEED];
			float duration = m_UpgradeStatus[(int)UPGRADE_STATUS_ID::BULLET_DURATION];

			// 弾を生成
			bm.Create(m_CharacterCategory, BULLET_ID::NOMAL_BULLET, bullet_position, m_Rotation, dmg, speed, duration);

			//糸の弾サウンド
			CSoundManager::GetInstance().Play(SOUND_ID::THREAD_ATTACK, false);
		}
	}
	else
	{
		// タイマーリセット
		m_BulletFireTimer = 0.0f;
	}
}

/*
 *	操作
 */
void
CPlayer::
Control(void)
{
	namespace keyboard = vivid::keyboard;
	namespace controller = vivid::controller;

	m_MoveFlg = false;
	vivid::Vector2 dir = { 0.0f,0.0f };

	vivid::Vector2 input_dir = { 0.0f,0.0f };
	const float dead_zone = 0.5f; // デッドゾーンの閾値

	// コントローラーの左スティックの入力を取得
	input_dir = controller::GetAnalogStickLeft(controller::DEVICE_ID::PLAYER1);

	// デッドゾーンのチェック
	bool left_stick = abs(input_dir.Length()) > dead_zone;

	bool left = keyboard::Button(keyboard::KEY_ID::LEFT) ||
				keyboard::Button(keyboard::KEY_ID::A) ||
				controller::Button(controller::DEVICE_ID::PLAYER1,controller::BUTTON_ID::LEFT);

	bool right = keyboard::Button(keyboard::KEY_ID::RIGHT) ||
				keyboard::Button(keyboard::KEY_ID::D) ||
				controller::Button(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::RIGHT);

	bool up =	keyboard::Button(keyboard::KEY_ID::UP) ||
				keyboard::Button(keyboard::KEY_ID::W) ||
				controller::Button(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::UP);

	bool down = keyboard::Button(keyboard::KEY_ID::DOWN) ||
				keyboard::Button(keyboard::KEY_ID::S) ||
				controller::Button(controller::DEVICE_ID::PLAYER1, vivid::controller::BUTTON_ID::DOWN);


	// キーボードの入力がある場合
	if (left)
	{
		m_MoveFlg = true;
		dir.x = -1.0f;
	}
	if (right)
	{
		m_MoveFlg = true;
		dir.x = 1.0f;
	}
	if (up)
	{
		m_MoveFlg = true;
		dir.y = -1.0f;
		m_UpdirectionFlg = true;
	}
	if (down)
	{
		m_MoveFlg = true;
		dir.y = 1.0f;
		m_UpdirectionFlg = false;
	}

	// コントローラーの左スティックの入力がデッドゾーンを超えている場合
	if (left_stick)
	{
		m_MoveFlg = true;
		dir = input_dir;
	}

	// 移動加速度の計算
	// 移動フラグが立っている場合
	if (m_MoveFlg)
	{
		// 移動加速度を増加
		m_MoveAccelerator += this->GetStatus(STATUS_ID::SPEED) / 2.0f;

		// 移動加速度の上限チェック
		if (m_MoveAccelerator > this->GetStatus(STATUS_ID::SPEED))
			m_MoveAccelerator = this->GetStatus(STATUS_ID::SPEED);
	}
	else
	{
		m_MoveAccelerator = 0.0f;

		// フラグが立っていない場合、速度を止める
		if(!m_RushChargeFlg)
			// 移動フラグが立っていない場合、体のパーツの移動も止める
			this->StopBodyParts();
	}

	// 移動加速度の設定
	if (dir.Length() != 0.0f)
	{
		// 頭に加速度を与える
		m_Accelerator = dir.Normalize() * m_MoveAccelerator;

		// プレイヤーの画像の頭の回転（仮）
		m_Rotation = atan2f(dir.y, dir.x);
	}
}

/*
 *  満腹度減少更新
 */
void
CPlayer::
FullnessDecreaseUpdate(void)
{
	// 満腹度が0より大きい場合のみ処理
	if (this->GetStatus(STATUS_ID::FULLNESS) <= 0) return;

	// 満腹度減少タイマー処理
	if (m_FullnessDecreaseTimer <= 0)
	{
		m_FullnessDecreaseFlg = true;
		m_FullnessDecreaseTimer = m_fullness_decrease_interval;
	}
	else
	{
		m_FullnessDecreaseTimer -= vivid::GetDeltaTime();
	}

	// 満腹度減少処理
	if (m_FullnessDecreaseFlg)
	{
		m_Status[(int)STATUS_ID::FULLNESS] -= m_fullness_decrease;

		m_FullnessDecreaseFlg = false;
	}
}

/*
 *  体の数更新
 */
void
CPlayer::
BodyQuantityUpdate(void)
{
	// 体のパーツを目標の数まで生成
	while (m_BodyQuantity < m_TargetBodyQuantity)
	{
		this->CreateBodyParts();
	}

	// 体のパーツを目標の数まで削除
	while (m_BodyQuantity > m_TargetBodyQuantity)
	{
		this->DeleteBodyParts();
	}
}

/*
 *	体のパーツ生成
 */
void
CPlayer::
CreateBodyParts(void)
{
	// 体の数が最大値に達している場合、処理を行わない
	if (m_BodyQuantity >= m_max_body_quantity) return;

	CParts* body = nullptr;

	body = new CParts(m_size, m_size, m_body_path_name, CHARACTER_CATEGORY::PLAYER, CHARACTER_ID::PLAYER);

	BodyPartsList::iterator end = m_BodyPartsList.end();

	// パーツの初期化
	if (m_BodyQuantity == 0)
		body->Initialize(BODY_PART::BODY, (ICharacter*)this, m_Position);
	else
		body->Initialize(BODY_PART::BODY, *--end, m_Position);

	// パーツリストに追加
	m_BodyPartsList.push_back(body);

	++m_BodyQuantity;
}

/*
 *	体のパーツ削除
 */
void
CPlayer::
DeleteBodyParts(void)
{
	// 体の数が最小値に達している場合、処理を行わない
	if (m_BodyQuantity <= m_min_body_quantity) return;

	BodyPartsList::iterator end = --m_BodyPartsList.end();
	CParts* parts = (*end);

	parts->Finalize();
	parts->SetActive(false);

	delete parts;

	m_BodyPartsList.erase(end);

	--m_BodyQuantity;
}

/*
 *	体のパーツ停止
 */
void
CPlayer::
StopBodyParts(void)
{
	// 全ての体のパーツの移動を停止
	BodyPartsList::iterator it = m_BodyPartsList.begin();
	while (it != m_BodyPartsList.end())
	{
		CParts* parts = (*it);
		parts->StopMove();
		++it;
	}
}