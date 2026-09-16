
/*
 *  @file       character.cpp
 *  @brief      キャラクター
 *  @author     Ryusei Shimizu
 *  @date       2025/10/15
 */

#include "character.h"
#include "../../stage_manager/stage_manager.h"
#include "../../camera_manager/camera_manager.h"
#include "../../hud_manager/hud_manager.h"
#include "../../score_manager/score_manager.h"
#include "../../effect_manager/effect_manager.h"
#include "../../sound_manager/sound_manager.h"
#include "../../../../utility/utility.h"


const float				ICharacter::m_max_invincible_time			= 3.0f;			//!< 無敵時間
const float				ICharacter::m_invincible_visible_interval	= 0.1f;			//!< 無敵時間中の点滅間隔
const float				ICharacter::m_friction						= 0.7f; 		//!< 移動時の摩擦力

/*
 *  コンストラクタ
 */
ICharacter::
ICharacter(int width, int height, std::string name, CHARACTER_CATEGORY category, CHARACTER_ID character_id)
	: m_Width(width)
	, m_Height(height)
	, m_PathName(name)
	, m_Accelerator(vivid::Vector2::ZERO)
	, m_PreviousPosition(vivid::Vector2::ZERO)
	, m_CenterPosition(vivid::Vector2::ZERO)
	, m_Position(vivid::Vector2::ZERO)
	, m_Velocity(vivid::Vector2::ZERO)
	, m_Anchor(vivid::Vector2((float)m_Width / 2.0f, (float)m_Height / 2.0f))
	, m_Rect({ 0,0,m_Width,m_Height })
	, m_Scale(vivid::Vector2(1.0f, 1.0f))
	, m_Color(0xffffffff)
	, m_Rotation(0.0f)
	, m_InvincibleTime(0)
	, m_MoveAccelerator(0.0f)
	, m_MoveFlg(false)
	, m_ActiveFlg(true)
	, m_InvincibleFlg(false)
	, m_CharacterCategory(category)
	, m_CharacterID(character_id)
	, m_CharacterAliveState(CHARACTER_ALIVE_STATE::ALIVE)
{
}

/*
 *  デストラクタ
 */
ICharacter::
~ICharacter(void)
{
}

/*
 *  初期化
 */
void
ICharacter::
Initialize(const vivid::Vector2& position)
{
	m_Accelerator = vivid::Vector2::ZERO;
	m_PreviousPosition = position;
	m_Position = position;
	m_CenterPosition = m_Position + vivid::Vector2(m_Width / 2.0f, m_Height / 2.0f);
	m_Velocity = vivid::Vector2::ZERO;
	m_Color = 0xffffffff;
	m_Rotation = 0.0f;
	m_InvincibleTime = 0;
	m_MoveAccelerator = 0.0f;
	m_MoveFlg = false;
	m_ActiveFlg = true;
	m_InvincibleFlg = false;
	m_CharacterAliveState = CHARACTER_ALIVE_STATE::ALIVE;

	for (int i = 0; i < ((int)STATUS_ID::MAX); i++)
	{
		m_MaxStatus[i] = 0;
		m_Status[i] = 0;
	}
	for (int i = 0; i < (int)UPGRADE_STATUS_ID::MAX; i++)
	{
		m_UpgradeStatus[i] = 0.0f;
	}

	CHudManager& ch = CHudManager::GetInstance();
	//アイコンの生成
	ch.GetCreate();
}

/*
 *  更新
 */
void
ICharacter::
Update(void)
{
	// キャラクターの生存状態によって処理を分ける
	switch (m_CharacterAliveState)
	{
	case CHARACTER_ALIVE_STATE::ALIVE:	Alive();	break;
	case CHARACTER_ALIVE_STATE::DEAD:	Dead();		break;
	}
}

/*
 *  描画
 */
void
ICharacter::
Draw(void)
{
	// 無敵時間中は点滅し、平常時は常に表示される
	if (m_InvincibleTime <= 0 || (int)(m_InvincibleTime / m_invincible_visible_interval) % 2 == 0)
	{
		vivid::Vector2 pos = m_Position;
		pos -= CCameraManager::GetInstance().GetPosition();

		vivid::DrawTexture(m_PathName, pos, m_Color, m_Rect, m_Anchor, m_Scale, m_Rotation);
	}
}

/*
 *	解放
 */
void
ICharacter::
Finalize(void)
{
}

/*
 *  キャラクターID取得
 */
CHARACTER_ID
ICharacter::
GetCharacterID(void)const
{
	return m_CharacterID;
}

/*
 *  位置の取得
 */
vivid::Vector2
ICharacter::
GetPosition(void)const
{
	return m_Position;
}

/*
 *  位置の設定
 */
void
ICharacter::
SetPosition(const vivid::Vector2& position)
{
	m_Position = position;
}

/*
 *  中心位置の取得
 */
vivid::Vector2
ICharacter::
GetCenterPosition(void)const
{
	return m_CenterPosition;
}

/*
 *  横幅取得
 */
int
ICharacter::
GetWidth(void)const
{
	return m_Width;
}

/*
 *  高さ取得
 */
int
ICharacter::
GetHeight(void)const
{
	return m_Height;
}

/*
 *	回転値取得
 */
float
ICharacter::
GetRotation(void)const
{
	return m_Rotation;
}

/*
 *  アクティブフラグ取得
 */
bool
ICharacter::
IsActive(void)const
{
	return m_ActiveFlg;
}

/*
 *  アクティブフラグ設定
 */
void
ICharacter::
SetActive(bool active)
{
	m_ActiveFlg = active;
}

/*
 *  キャラクター識別子取得
 */
CHARACTER_CATEGORY
ICharacter::
GetCharacterCategory(void)const
{
	return m_CharacterCategory;
}

/*
 *	ステータス取得
 */
float
ICharacter::
GetStatus(STATUS_ID id) const
{
	return m_Status[(int)id];
}

/*
 *	ステータス最大値取得
 */
float
ICharacter::
GetMaxStatus(STATUS_ID id) const
{
	return m_MaxStatus[(int)id];
}

/*
 *  強化ステータス取得
 */
float
ICharacter::
GetUpgradeStatus(UPGRADE_STATUS_ID id) const
{
	return m_UpgradeStatus[(int)id];
}

/*
 *  ステータス上昇
 */
void
ICharacter::
IncreaseStatus(STATUS_ID status_id, float value)
{
	// ステータスの範囲チェック
	if (m_Status[(int)status_id] + value > m_MaxStatus[(int)status_id])
	{
		m_Status[(int)status_id] = m_MaxStatus[(int)status_id];
		return;
	}

	m_Status[(int)status_id] += value;
}

/*
 *	弾との当たり判定
 */
bool
ICharacter::
CheckHitBullet(IBullet* bullet)
{
	// 弾が存在しない、同じキャラクターカテゴリの弾、無敵状態、死亡状態の場合は当たり判定を行わない
	if (!bullet || m_CharacterCategory == bullet->GetBulletCategory() || m_InvincibleFlg || m_CharacterAliveState == CHARACTER_ALIVE_STATE::DEAD)
		return false;

	//敵ダメージサウンド調節
	CSoundManager::GetInstance().SetVolume(SOUND_ID::THREAD_ATTACK_DAMAGE, 9000);

	// 当たり判定
	if (u_CheckHitObject(bullet->GetPosition(), bullet->GetWidth(), bullet->GetHeight(),
		this->GetPosition(), this->GetWidth(), this->GetHeight(), this->GetRotation()))
	{
		// 当たっていたら弾を消す
		bullet->SetActive(false);

		// プレイヤーの弾が当たった場合はスコア加算
		if (bullet->GetBulletCategory() == CHARACTER_CATEGORY::PLAYER)
		{
			CScoreManager::GetInstance().AddScore(SCORE_ID::HITBULLET);
		}
		//敵のヒットサウンド再生
		CSoundManager::GetInstance().Play(SOUND_ID::THREAD_ATTACK_DAMAGE, false);

		m_Status[(int)STATUS_ID::HP] -= bullet->GetStatus(BULLET_STATUS_ID::DAMAGE);

		m_InvincibleTime = m_max_invincible_time;
		m_InvincibleFlg = true;

		return true;
	}
	return false;
}

/*
 *  ラッシュの当たり判定
 */
bool
ICharacter::
RushCheckHit(ICharacter* player)
{
	// 自キャラが存在しない、もしくは同じカテゴリ、無敵状態、死亡状態なら処理を行わない
	if (!player || m_CharacterCategory == player->GetCharacterCategory() || m_InvincibleFlg || m_CharacterAliveState == CHARACTER_ALIVE_STATE::DEAD)
		return false;

	CSoundManager::GetInstance().SetVolume(SOUND_ID::ENEMY_RUSH_DAMAGE, 8000);

	// 当たり判定チェック
	// 体のパーツ、もしくは頭に当たったら処理
	if (u_CheckHitObject(player->GetPosition(), player->GetWidth(), player->GetHeight(),
		this->GetPosition() + vivid::Vector2(15.0f, 15.0f), this->GetWidth() - 15, this->GetHeight() - 15, this->GetRotation()))
	{
		m_Status[(int)STATUS_ID::HP] -= player->GetStatus(STATUS_ID::ATTACKPOWER) * player->GetUpgradeStatus(UPGRADE_STATUS_ID::RUSH_DAMAGE_MAG);

		//敵のヒットサウンド再生
		CSoundManager::GetInstance().Play(SOUND_ID::ENEMY_RUSH_DAMAGE, false);

		m_InvincibleTime = m_max_invincible_time;
		m_InvincibleFlg = true;

		return true;
	}

	return false;
}

/*
 *  生存
 */
void
ICharacter::
Alive(void)
{
	// 死亡判定
	if (m_Status[(int)STATUS_ID::HP] <= 0)
	{
		m_CharacterAliveState = CHARACTER_ALIVE_STATE::DEAD;

		CEffectManager::GetInstance().Create(EFFECT_ID::DEAD, this->GetCenterPosition(), 0xffffffff, 0.0f);

		// スコア加算
		if (m_CharacterCategory == CHARACTER_CATEGORY::ENEMY)
			CScoreManager::GetInstance().AddScore(SCORE_ID::ENEMY);

		return;
	}

	// 攻撃
	this->Attack();

	// 発射
	this->Fire();

	// 無敵中は位置を反映しない
	if (!m_InvincibleFlg)
	{
		// 位置反映
		this->Move();
	}

	// ブロックとの当たり判定
	this->CheckHitBlock();

	// 無敵更新
	this->invincibleUpdate();
}

/*
 *  死亡
 */
void
ICharacter::
Dead(void)
{
	m_ActiveFlg = false;
}

/*
 *	攻撃
 */
void
ICharacter::
Attack(void)
{
}

/*
 *	発射
 */
void
ICharacter::
Fire(void)
{
}

/*
 *  無敵更新
 */
void
ICharacter::
invincibleUpdate(void)
{
	if (m_InvincibleTime > 0)
	{
		m_InvincibleTime -= vivid::GetDeltaTime();
	}

	// 無敵時間処理
	if (m_InvincibleTime < 0)
	{
		m_InvincibleTime = 0;
		m_InvincibleFlg = false;
	}
}

/*
 *  動作
 */
void
ICharacter::
Move(void)
{
	// 前フレームの位置を保存
	m_PreviousPosition = m_Position;

	// 加速度を速度に反映
	m_Velocity += m_Accelerator;

	// 速度を位置に反映
	m_Position += m_Velocity;

	// 中心位置更新
	m_CenterPosition = m_Position + vivid::Vector2(m_Width / 2.0f, m_Height / 2.0f);

	// 摩擦力を速度に反映
	m_Velocity *= m_friction;

	// 加速度リセット
	m_Accelerator = vivid::Vector2(0.0f, 0.0f);
}

/*
 *  ブロックとの当たり判定
 */
void
ICharacter::
CheckHitBlock(void)
{
	CStageManager& sm = CStageManager::GetInstance();
	int blockSize = sm.GetBlockSize();

	vivid::Vector2& pos = m_Position;

	// キャラクターの四隅の座標を取得
	int left = ((int)pos.x);
	int top = ((int)pos.y);
	int right = ((int)pos.x + m_Width);
	int bottom = ((int)pos.y + m_Height);

	// キャラクターがいるブロックの座標を取得
	int blockLeft = left / blockSize;
	int blockTop = top / blockSize;
	int blockRight = (right - 1) / blockSize;
	int blockBottom = (bottom - 1) / blockSize;

	bool hit = false;

	// 四隅のブロックと当たり判定を行う
	if (sm.IsWall(blockLeft, blockTop))    hit = true;
	if (sm.IsWall(blockRight, blockTop))   hit = true;
	if (sm.IsWall(blockLeft, blockBottom)) hit = true;
	if (sm.IsWall(blockRight, blockBottom))hit = true;

	// 当たっていたら
	if (hit)
	{
		// 当たっていたら前フレームの位置に戻す
		m_Position = m_PreviousPosition;

		// 速度も止める
		m_Accelerator = vivid::Vector2::ZERO;
		m_Velocity = vivid::Vector2::ZERO;
	}
}
