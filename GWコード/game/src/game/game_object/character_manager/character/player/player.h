
/*!
 *  @file           player.h
 *  @brief          プレイヤー
 *  @author         Ryusei Shimizu
 *  @date           2026/10/13
 */

#pragma once

#include "vivid.h"
#include "../charater.h"

class CPlayer
	: public ICharacter
{
public:

	/*!
	 *	@brief		コンストラクタ
	 */
	CPlayer(void) = default;

	/*!
	 *	@brief		デストラクタ
	 */
	~CPlayer(void) override = default;

	/*!
	 *	@brief		更新
	 */
	void			Update(void) override;

	/*!
	 *	@brief		描画
	 */
	void			Draw(void) override;
};