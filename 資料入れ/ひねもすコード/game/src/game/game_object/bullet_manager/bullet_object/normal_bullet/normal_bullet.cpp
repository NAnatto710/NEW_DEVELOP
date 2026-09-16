
/*!
 *  @file       nomal_bullet.cpp
 *  @brief      通常弾クラス
 *  @author     Ryusei Shimizu
 *  @date       2025/12/22
 */

#include "normal_bullet.h"

const std::string	CNomalBullet::m_texture_name	= "data\\object\\bullet.png";	//!< テクスチャパス
const int			CNomalBullet::m_width			= 90;							//!< 幅
const int			CNomalBullet::m_height			= 30;							//!< 高さ
const int			CNomalBullet::m_animation		= 10;							//!< Animationの区切り


/*
 *  コンストラクタ
 */
CNomalBullet::
CNomalBullet(void)
	: IBullet(m_texture_name, m_width, m_height)
{
}

/*
 *  デストラクタ
 */
CNomalBullet::
~CNomalBullet(void)
{
}

/*
 *  初期化
 */
void
CNomalBullet::
Initialize(CHARACTER_CATEGORY category, const vivid::Vector2& position, float direction, float damage, float speed, float duration)
{
	IBullet::Initialize(category, position, direction, damage, speed, duration);

	m_AnimationFrame = 0;
	m_AnimationTimer = 0;
}

/*
 *  更新
 */
void
CNomalBullet::
Update(void)
{
	IBullet::Update();

	// アニメーション
	this->Animation();
}

/*
 *  アニメーション
 */
void
CNomalBullet::
Animation(void)
{
	m_AnimationFrame++;
	//フレームの方が大きくなった時に動かす
	if (m_AnimationFrame > m_animation)
	{
		m_AnimationTimer += 1;
		m_AnimationFrame = 0;
	}

	// Rectの更新
	m_Rect.top = 0;
	m_Rect.bottom = m_Rect.top + m_height;
	m_Rect.left = (m_AnimationTimer % 3) * m_width;
	m_Rect.right = m_Rect.left + m_width;

}
