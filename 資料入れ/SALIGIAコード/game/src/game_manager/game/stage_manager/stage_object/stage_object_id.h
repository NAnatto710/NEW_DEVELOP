
/*!
 *  @file		stage_object_id.h
 *  @brief		ステージオブジェクトID
 *  @author     Ryusei Shimizu
 *  @date       2026/04/13
 */

#pragma once

/*
 *	@brief	ステージオブジェクトID
 */
enum class STAGE_OBJECT_ID
{
	NONE,				//!< 空

	NORMAL_BLOCK,		//!< ノーマルブロック
	UNDER_BLOCK,		//!< 地中のブロック
	SCAFFOLDING,		//!< 足場
	SPAWN_POINT_1,		//!< スポーンポイント1
	SPAWN_POINT_2,		//!< スポーンポイント2
	SPAWN_POINT_3,		//!< スポーンポイント3
	SPAWN_POINT_4,		//!< スポーンポイント4

	MAX					//!< 最大数
};