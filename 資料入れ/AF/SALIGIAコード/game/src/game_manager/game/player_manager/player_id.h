
/*!
 *  @file		player_id.h
 *  @brief		プレイヤーID
 *  @author     Ryusei Shimizu
 *  @date       2026/04/13
 */

#pragma once

/*
 *	@brief	キャラクター識別ID
 */
enum class CATEGORY_ID
{
	PLAYER,				//!< プレイヤー
	ENEMY,				//!< 敵

	UNKNOW			//!< 所属不明
};

/*!
 *	プレイヤーID
 */
enum class PLAYER_ID
{
	NONE = -1,			//!< 空
	
	PLAYER1,			//!< プレイヤー1
	PLAYER2,			//!< プレイヤー2
	PLAYER3,			//!< プレイヤー3
	PLAYER4,			//!< プレイヤー4

	MAX,			//!< 最大数
};

/*!
 *	脱落した順番
 */
enum class DEAD_COUNT
{
	FIRST,		//!< 一番目
	SECOND,
	THERD,
	FORTH,
};

/*!
 *	選択された欲ID
 */
enum class SALIGIA_ID
{
	NONE=-1,		//!< 空

	SUPERBIA,		//!< 傲慢
	AVARITIA,		//!< 強欲
	LUXURIA,		//!< 色欲
	INVIDIA,		//!< 嫉妬
	GULA,			//!< 暴食
	IRA,			//!< 憤怒
	ACEDIA,			//!< 怠惰

	MAX,		//!< 最大数
};

