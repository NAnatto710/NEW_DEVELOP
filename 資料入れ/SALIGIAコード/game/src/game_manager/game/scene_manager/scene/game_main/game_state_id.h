/*!
 *  @file		game_mode_id.h
 *  @brief		ゲームモードID
 *  @author     Ryusei Shimizu
 *  @date       2026/04/13
 */

#pragma once

 /*!
  *	ゲームモードID
  */
enum class GAME_STATE_ID
{
	START,
	PLAY,
	FINISH,

	MAX,				//!< 最大数
};