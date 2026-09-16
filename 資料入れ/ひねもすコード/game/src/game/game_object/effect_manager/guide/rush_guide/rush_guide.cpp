
/*!
 *  @file       rush_guide.cpp
 *  @brief      突進ガイドクラス
 *  @author     Ryusei Shimizu
 *  @date       2026/02/24
 */

#include "rush_guide.h"

const int				CRushGuide::m_width			= 200;								//!< 幅
const int				CRushGuide::m_height		= 200;								//!< 高さ
const std::string		CRushGuide::m_texture_path	= "data\\guide\\rush_guide.png";    //!< テクスチャパス


/*
 *  コンストラクタ
 */
CRushGuide::
CRushGuide()
	: IGuide(m_width, m_height, m_texture_path)
{
}