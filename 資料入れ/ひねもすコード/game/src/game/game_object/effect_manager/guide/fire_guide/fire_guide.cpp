
/*!
 *  @file       fire_guide.cpp
 *  @brief      突進ガイドクラス
 *  @author     Ryusei Shimizu
 *  @date       2026/02/24
 */

#include "fire_guide.h"

const int				CFireGuide::m_width			= 200;								//!< 幅
const int				CFireGuide::m_height		= 200;								//!< 高さ
const std::string		CFireGuide::m_texture_path	= "data\\guide\\fire_guide.png";    //!< テクスチャパス


/*
 *  コンストラクタ
 */
CFireGuide::
CFireGuide()
	: IGuide(m_width, m_height, m_texture_path)
{
}