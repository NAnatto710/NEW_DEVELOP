
/*
 *  @file       parts.cpp
 *  @brief      パーツ
 *  @author     Ryusei Shimizu
 *  @date       2026/01/22
 */

#include "parts.h"
#include "../../camera_manager/camera_manager.h"
#include "../../../../utility/utility.h"


const float		CParts::m_friction					= 0.7f; 	//!< 移動時の摩擦力
const float     CParts::m_body_distance				= 28.0f;    //!< 体の間の距離
const float     CParts::m_body_extension_speed		= 0.02f;	//!< 体の伸びる速さ
const float     CParts::m_body_extension_max_timer	= 1.15f;	//!< 体の伸縮最大タイマー
const float     CParts::m_body_extension_min_timer	= 0.85f;	//!< 体の伸縮最小タイマー

/*
 *  コンストラクタ
 */
CParts::
CParts(int width, int height, std::string name, CHARACTER_CATEGORY category, CHARACTER_ID character_id)
	: ICharacter(width, height, name, category, character_id)
	, m_TargetPosition(vivid::Vector2::ZERO)
	, m_TargetParts(nullptr)
	, m_ForrowMoveFlg(false)
	, m_UpdirectionFlg(true)
	, m_ExtensionFlg(false)
	, m_ExtensionTimer(0.0f)
{
}

/*
 *  デストラクタ
 */
CParts::
~CParts(void)
{
}

/*
 *  初期化
 */
void
CParts::
Initialize(BODY_PART parts, ICharacter* target_parts, const vivid::Vector2& position)
{
	m_BodyPart = parts;
	m_TargetParts = target_parts;
	m_Position = position;
	m_PreviousPosition = m_Position;
	m_TargetPosition = m_TargetParts->GetCenterPosition();
	m_CenterPosition = m_Position + vivid::Vector2(m_Width / 2.0f, m_Height / 2.0f);

	m_ForrowMoveFlg = false;
	m_UpdirectionFlg = true;
	m_ExtensionTimer = m_body_extension_min_timer;
	m_ExtensionFlg = false;

	m_ActiveFlg = true;
	m_CharacterAliveState = CHARACTER_ALIVE_STATE::ALIVE;
}

/*
 *  更新
 */
void
CParts::
Update(void)
{
	// 移動
	this->Move();

	// 追従移動
	this->ForrowMove(m_body_distance);
}

/*
 *  更新
 */
void
CParts::
Update(float bodydistance)
{
	// 移動
	this->Move();
	// 追従移動
	this->ForrowMove(bodydistance);
}

/*
 *  描画
 */
void
CParts::
Draw(void)
{
	vivid::Vector2 pos = m_Position;
	pos -= CCameraManager::GetInstance().GetPosition();

	vivid::DrawTexture(m_PathName, pos, m_Color, m_Rect, m_Anchor, m_Scale, m_Rotation);
}

/*
 *  描画
 */
void
CParts::
Draw(unsigned int color)
{
	m_Color = color;

	vivid::Vector2 pos = m_Position;
	pos -= CCameraManager::GetInstance().GetPosition();

	vivid::DrawTexture(m_PathName, pos, m_Color, m_Rect, m_Anchor, m_Scale, m_Rotation);
}

/*
 *	解放
 */
void
CParts::
Finalize(void)
{
}

/*
 *	移動停止
 */
void
CParts::
StopMove(void)
{
	m_Accelerator = vivid::Vector2::ZERO;
}

/*
 *	目標パーツ取得
 */
ICharacter*
CParts::
GetTargetParts(void) const
{
	return m_TargetParts;
}

/*
 *	対象パーツ設定
 */
void
CParts::
SetTarget(CParts& parts)
{
	m_TargetParts = &parts;
}

/*
 *	速度設定
 */
void
CParts::
SetVeloctiy(vivid::Vector2 velocity)
{
	m_Velocity = velocity;
}

/*
 *	回転設定
 */
void
CParts::
SetRotation(float rotation)
{
	m_Rotation = rotation;
}

/*
 *  部位取得
 */
BODY_PART
CParts::
GetBodyPart(void) const
{
	return m_BodyPart;
}

/*
 *	キャラクター識別子取得
 */
CHARACTER_CATEGORY
CParts::
GetCharacterCategory(void) const
{
	return m_CharacterCategory;
}

/*
 *	動作
 */
void
CParts::
Move(void)
{
	// 前フレームの位置を保存
	m_PreviousPosition = m_Position;

	// 加速度を速度に反映
	m_Velocity += m_Accelerator;

	// 速度を位置に反映
	m_Position += m_Velocity;

	// 中心位置の更新
	m_CenterPosition = m_Position + vivid::Vector2(m_Width / 2.0f, m_Height / 2.0f);
	m_TargetPosition = m_TargetParts->GetCenterPosition();

	// 摩擦力を速度に反映
	m_Velocity *= m_friction;

	// 加速度リセット
	m_Accelerator = vivid::Vector2(0.0f, 0.0f);
}

/*
 *	追従移動
 */
void
CParts::
ForrowMove(float bodydistance)
{
	// ひとつ前の体との差分を求める
	vivid::Vector2 distance = m_TargetPosition - m_CenterPosition;

	// 体の向きの設定
	vivid::Vector2 dir = distance;
	if (dir.Length() != 0.0f)
	{
		m_Rotation = atan2f(dir.y, dir.x);
	}

	// 体の間の距離以上離れていたら加速度を与える
	if (distance.Length() >= bodydistance)
	{
		// 伸縮タイマーの更新
		if (m_ExtensionFlg == false)
		{
			if (m_ExtensionTimer < m_body_extension_max_timer)
				m_ExtensionTimer += m_body_extension_speed;
			else
				m_ExtensionFlg = true;
		}
		else
		{
			if (m_ExtensionTimer > m_body_extension_min_timer)
				m_ExtensionTimer -= m_body_extension_speed;
			else
				m_ExtensionFlg = false;
		}

		// 加速度の設定
		if (dir.Length() != 0.0f)
		{
			// プレイヤーの速度取得
			ICharacter* cm = CCharacterManager::GetInstance().GetPlayer();
			float speed = cm->GetStatus(STATUS_ID::SPEED);

			m_Accelerator = distance.Normalize() * (speed / m_ExtensionTimer);
		}
	}
	else
	{
		// 加速度リセット
		m_Accelerator = vivid::Vector2::ZERO;
		m_Velocity -= vivid::Vector2(m_Velocity.x / 2, m_Velocity.y / 2);
	}
}

