
/*!
 *  @file		scene_id.h
 *  @brief		シーンID
 *  @author     Ryusei Shimizu
 *  @date       2026/04/10
 */

#pragma once

/*!
 *	メインシーンID
 */
enum class MAINSCENE_ID
{
	NONE,				//!< 空

	TITLE,				//!< タイトル
	PLAYER_JOIN,		//!< プレイヤー参加
	BUILD_SELECT,		//!< ビルド選択
	GAME_MAIN,			//!< ゲーム
	RESULT,				//!< リザルト

	MAX,				//!< 最大数
};

/*!
 *	サブシーンID
 */
enum class SUBSCENE_ID
{
	NONE,				//!< 空

	PAUSE,				//!< ポーズ

	MAX,				//!< 最大数
};