
/*!
 *  @file       rush.h
 *  @brief      突進クラス
 *  @author     Misaki Kawada
 *  @date       2026/02/17
 */

#pragma once

#include "vivid.h"
#include "../../../../../utility/utility.h"
#include <list>

/*!
 *  @class	  CRush
 *
 *  @brief      攻撃ベースクラス
 *
 *  @author     Misaki Kawada
 *
 *  @date       2026/02/17
 */
class CRush
{
public:

	/*!
	 *  @brief      コンストラクタ
	 */
	CRush();

	/*!
	 *  @brief      デストラクタ
	 */
	~CRush(void) = default;

	/*
	 *	@breif	突進
	 */
	vivid::Vector2 Rush(vivid::Vector2 velocity, float speed, float direction, float val);
};
