
/*!
 *  @file       character_id.h
 *  @brief      キャラクターID
 *  @author     Ryusei Shimizu
 *  @date       2025/10/08
 */

#pragma once

/*
 *	@brief	キャラクター識別ID
 */
enum class CHARACTER_CATEGORY
{
	PLAYER,			//!< プレイヤー
	ENEMY,			//!< 敵

	UNKNOW			//!< 所属不明
};

/*
 *	@brief	キャラクターID
 */
enum class CHARACTER_ID
{
	PLAYER,				//!< プレイヤー
	CATERPILLAR,		//!< 毛虫
	CENTIPEDE,			//!< ムカデ
	BEE,				//!< ハチ

	MAX,				//!< 最大値
};

/*!
 *	@brief	キャラクターの生存状態ID
 */
enum class CHARACTER_ALIVE_STATE
{
	ALIVE,		//!< 生存
	DEAD,		//!< 死亡
};