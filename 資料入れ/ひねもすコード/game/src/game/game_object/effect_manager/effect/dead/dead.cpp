
/*!
 *  @file       dead.cpp
 *  @brief      死亡エフェクトクラス
 *  @author     Ryusei Shimizu
 *  @date       2026/02/26
 */

#include "dead.h"
#include "../../../camera_manager/camera_manager.h"

const int			CDead::m_width			= 80;									//!< 幅
const int			CDead::m_height			= 120;									//!< 高さ
const int			CDead::m_move_speed		= -4;									//!< エフェクトの移動速度
const int			CDead::m_fade_speed		= 10;									//!< フェード速度
const std::string	CDead::m_texture_path	= "data\\effect\\dead\\dead.png";		//!< テクスチャ名


/*
 *	コンストラクタ
 */
CDead::
CDead(void)
	: IEffect(m_width, m_height, EFFECT_ID::DEAD)
{
}

/*
 *	デストラクタ
 */
CDead::
~CDead(void)
{
}

/*
 *  初期化
 */
void
CDead::
Initialize(const vivid::Vector2& position, unsigned int color, float rotation)
{
	vivid::Vector2 pos = position;
	pos -= vivid::Vector2(m_width / 2, m_height / 2);
	IEffect::Initialize(pos, color, rotation);
}

void
CDead::
Update(void)
{
	vivid::Vector2 velocity = vivid::Vector2(0, m_move_speed);
	m_Position += velocity;


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

void
CDead::
Draw(void)
{
	vivid::Vector2 pos = m_Position;
	pos -= CCameraManager::GetInstance().GetPosition();

	vivid::DrawTexture(m_texture_path, pos, m_Color, m_Rect, m_Anchor, m_Scale, m_Rotation);
}
