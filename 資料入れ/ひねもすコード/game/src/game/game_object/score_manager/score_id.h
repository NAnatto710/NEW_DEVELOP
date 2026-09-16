
/*!
 *  @file       score_id.h
 *  @brief      スコアID
 *  @author     Ryusei Shimizu
 *  @date       2026/02/20
 */

#pragma once

/*!
 *  @brief      スコアID
 */
enum class SCORE_ID
{	
	FEED,         //!< エサスコア
	ENEMY,        //!< エネミースコア
	HITBULLET,    //!< 弾スコア
	MISSBULLET,   //!< 弾ミススコア

	MAX           //!< スコアID数
};