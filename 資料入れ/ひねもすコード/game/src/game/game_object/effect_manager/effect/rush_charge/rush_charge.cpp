
/*!
 *  @file       rush_charge.cpp
 *  @brief      突進チャージエフェクトクラス
 *  @author     Ryusei Shimizu
 *  @date       2026/02/26
 */

#include "rush_charge.h"
#include "../../../camera_manager/camera_manager.h"
#include "../../../../../utility/utility.h"

const int			CRushCharge::m_width			= 24;													//!< 幅
const int			CRushCharge::m_height			= 24;													//!< 高さ
const int			CRushCharge::m_move_speed		= 8;													//!< エフェクトの移動速度
const int			CRushCharge::m_fade_speed		= 30;													//!< フェード速度
const std::string	CRushCharge::m_texture_path		= "data\\effect\\rush_charge\\charge_particle.png";		//!< テクスチャ名
const int			CRushCharge::m_spawn_range		= 300.0f;												//!< エフェクトの出現範囲

/*
 *	コンストラクタ
 */
CRushCharge::
CRushCharge(void)
	: IEffect(m_width, m_height,EFFECT_ID::RUSH_CHARGE)
{
}

/*
 *	デストラクタ
 */
CRushCharge::
~CRushCharge(void)
{
}

/*
 *  初期化
 */
void
CRushCharge::
Initialize(const vivid::Vector2& position, unsigned int color, float rotation)
{
	m_TargetPos = position;

	vivid::Vector2 pos = m_TargetPos;

	float x = u_RandomInt(0, m_spawn_range);
	float y = u_RandomInt(0, m_spawn_range);

	x -= m_spawn_range / 2;
	y -= m_spawn_range / 2;

	pos += vivid::Vector2(x, y);

	IEffect::Initialize(pos, color, rotation);
}

/*
 *  更新
 */
void
CRushCharge::
Update(void)
{
	vivid::Vector2 m_center_pos = m_Position + vivid::Vector2(m_Width / 2, m_Height / 2);

	vivid::Vector2 v = m_TargetPos - m_center_pos;

	float angle = atan2(v.y, v.x);

	vivid::Vector2 m_velocity;

	m_velocity.x = cos(angle) * m_move_speed;
	m_velocity.y = sin(angle) * m_move_speed;

	m_Position += m_velocity;

	// アルファ値を減少させる
	int alpha = (m_Color & 0xff000000) >> 24;
	alpha -= m_fade_speed;

	// アルファ値が0未満にならないようにする
	if (alpha < 0)
	{
		alpha = 0;
		m_ActiveFlg = false;
	}

	// アルファ値をカラーに反映させる
	m_Color = (alpha << 24) | (m_Color & 0x00ffffff);
}

/*
 *  描画
 */
void
CRushCharge::
Draw(void)
{
	vivid::Vector2 pos = m_Position;
	pos -= CCameraManager::GetInstance().GetPosition();

	vivid::DrawTexture(m_texture_path, pos, m_Color, m_Rect, m_Anchor, m_Scale, m_Rotation);
}
