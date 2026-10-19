
/*!
 *  @file           character.h
 *  @brief          キャラクター基底
 *  @author         Ryusei Shimizu
 *  @date           2026/10/13
 */

#pragma once

#include "vivid.h"

class ICharacter
{
public:

	/*!
	 *	@brief		コンストラクタ
	 */
	ICharacter(void) = default;

	/*!
	 *	@brief		デストラクタ
	 */
	virtual			~ICharacter(void) = default;

	/*!
	 *	@brief		更新
	 */
	virtual void	Update(void) = 0;

	/*!
	 *	@brief		描画
	 */
	virtual void	Draw(void) = 0;
};