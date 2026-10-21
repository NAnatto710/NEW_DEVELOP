
/*!
 *  @file		resource_id.h
 *  @brief		キャラクターのリソースID
 *  @author     Ryusei Shimizu
 *  @date       2026/07/07
 */

#pragma once

/*!
 *	@brief	リソース構造体
 */
struct ResourceValue
{
	float Current	= 0.0f;		//!< 現在の値
	float Max		= 0.0f;		//!< 最大値
	float Min		= 0.0f;		//!< 最小値
};

/*!
 *	@brief	ステータスID
 */
enum class RESOURCE_ID
{
	HP,			//!< HP
	DESIRE,		//!< 欲望
	GUARD,		//!< ガード値(ガードすると減少する)
	LIFE,		//!< 残機

	MAX		//<! 最大値
};