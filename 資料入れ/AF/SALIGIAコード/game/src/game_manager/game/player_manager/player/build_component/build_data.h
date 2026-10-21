
/*!
 *  @file		build_data.h
 *  @brief		ビルドデータ
 *  @author     Ryusei Shimizu
 *  @date       2026/06/26
 */

#pragma once

#include "../../player_id.h"

/*!
 *	@brief	ビルドデータ
 */
struct BuildData
{
	SALIGIA_ID Primary		= SALIGIA_ID::NONE;		//!< 選択された欲IDの1つ目
	SALIGIA_ID Secondary	= SALIGIA_ID::NONE;		//!< 選択された欲IDの2つ目
};

/*!
 *	@brief	得意欲望帯
 */
struct AFFINITY_ZONE
{
	float Min = 0.0f;		//!< 得意欲望帯の最小値
	float Max = 0.0f;		//!< 得意欲望帯の最大値
};