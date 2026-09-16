
/*!
 *  @file       rush_guide.h
 *  @brief      突進ガイドクラス
 *  @author     Ryusei Shimizu
 *  @date       2026/02/24
 */

#pragma once

#include "vivid.h"
#include "../guide.h"

 /*!
  *  @class      CRushGuide
  *
  *  @brief      突進ガイドクラス
  *
  *  @author     Ryusei Shimizu
  *
  *  @date       2026/02/24
  */
class CRushGuide
	: public IGuide
{
public:

	/*!
	 *  @brief      コンストラクタ
	 */
	CRushGuide();

private:

	static const int				m_width;           //!< 幅
	static const int				m_height;          //!< 高さ
	static const std::string		m_texture_path;    //!< テクスチャパス
};