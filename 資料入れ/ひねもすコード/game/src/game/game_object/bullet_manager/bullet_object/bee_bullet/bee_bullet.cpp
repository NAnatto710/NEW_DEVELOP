
/*!
 *  @file       bee_bullet.cpp
 *  @brief      ハチ弾クラス
 *  @author     Ryusei Shimizu
 *  @date       2026/02/24
 */

#include "bee_bullet.h"

const std::string	CBeeBullet::m_texture_name	= "data\\object\\bee_bullet.png";	//!< テクスチャパス
const int			CBeeBullet::m_width			= 45;								//!< 幅
const int			CBeeBullet::m_height		= 22;								//!< 高さ


/*
 *  コンストラクタ
 */
CBeeBullet::
CBeeBullet(void)
	: IBullet(m_texture_name, m_width, m_height)
{
}

/*
 *  デストラクタ
 */
CBeeBullet::
~CBeeBullet(void)
{
}

/*
 *  初期化
 */
void
CBeeBullet::
Initialize(CHARACTER_CATEGORY category, const vivid::Vector2& position, float direction, float damage, float speed, float duration)
{
	IBullet::Initialize(category, position, direction, damage, speed, duration);
}
