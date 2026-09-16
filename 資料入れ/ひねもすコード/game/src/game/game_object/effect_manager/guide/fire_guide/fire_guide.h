
/*!
 *  @file       fire_guide.h
 *  @brief      発射ガイドクラス
 *  @author     Ryusei Shimizu
 *  @date       2026/02/24
 */

#pragma once

#include "vivid.h"
#include "../guide.h"

 /*!
  *  @class      CFireGuide
  *
  *  @brief      発射ガイドクラス
  *
  *  @author     Ryusei Shimizu
  *
  *  @date       2026/02/24
  */
class CFireGuide
	: public IGuide
{
public:

	/*!
	 *  @brief      コンストラクタ
	 */
	CFireGuide();

private:

	static const int				m_width;           //!< 幅
	static const int				m_height;          //!< 高さ
	static const std::string		m_texture_path;    //!< テクスチャパス
};