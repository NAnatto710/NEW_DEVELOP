
/*!
 *  @file       character_status_id.h
 *  @brief      キャラクターステータスID
 *  @author     Ryusei Shimizu
 *  @date       2026/01/22
 */

#pragma once

/*
 *	@brief	基本ステータスID
 */
enum class STATUS_ID
{
	HP,				//!< 体力
	FULLNESS,		//!< 満腹度
	ATTACKPOWER,	//!< 攻撃力
	SPEED,			//!< 速度
	TOUTHDAMAGE,	//!< 触れたときのダメージ

	MAX,
};

/*
 *	@brief	ステータス定義
 */
enum class STATUS_DEFINITION
{
	DEFAULT,	//!< 初期値
	MAXIMUM,	//!< 最大値
	MINIMUM,	//!< 最小値

	MAX,
};