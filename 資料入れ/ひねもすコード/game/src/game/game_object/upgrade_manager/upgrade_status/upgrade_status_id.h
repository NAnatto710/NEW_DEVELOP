
/*!
 *  @file       upgrade_status_id.h
 *  @brief      プレイヤーの強化ステータスID
 *  @author     Ryusei Shimizu
 *  @date       2026/02/04
 */

#pragma once

/*
 *	@brief		強化ステータス管理ID
 */
enum class UPGRADE_STATUS_ID
{
	BULLET_DAMAGE_MAG,		//!< 弾のダメージ倍率
	BULLET_COOLTIME,		//!< 弾のクールタイム
	BULLET_DURATION,		//!< 弾の持続時間
	BULLET_SPEED,			//!< 弾のスピード

	RUSH_DAMAGE_MAG,		//!< 突進のダメージ倍率
	RUSH_COOLTIME,			//!< 突進のクールタイム
	RUSH_DURATION,			//!< 突進の持続時間
	RUSH_SPEED,				//!< 突進のスピード

	MAX					//!< ステータスID数
};



