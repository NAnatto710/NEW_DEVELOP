
/*!
 *  @file       stage_object_id.h
 *  @brief      ステージオブジェクトID
 *  @author     Ryusei Shimizu
 *  @date       2025/12/18
 */

#pragma once

/*
 *	@brief	ステージオブジェクトID
 */
enum class STAGE_OBJECT_ID
{
	EMPTY_OBJECT = 0,		//!< 空オブジェクト
	FEED_OBJECT = 1,		//!< エサ
	ENEMY_SPAWN_OBJECT = 2,		//!< 敵スポーン地点


	MAX,							//!< 最大値
};

/*
 *	@brief	ステージブロックID
 */
enum class STAGE_BLOCK_ID
{
	EMPTY_BLOCK = 0,		//!< 空ブロック
	WALL_BLOCK = 1,		//!< 壁ブロック
	PLAYER_SPAWN_BLOCK = 2,		//!< プレイヤースポーン地点

	MAX,							//!< 最大値
};