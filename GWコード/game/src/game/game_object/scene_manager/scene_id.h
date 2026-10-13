
/*!
 *  @file			scene_id.h
 *  @brief			シーンID
 *  @author			Ryusei Shimizu
 *  @date			2026/09/30
 */

#pragma once

/*
 *	メインシーンID
 */
enum class MAIN_SCENE_ID
{
	NONE = -1,	//!<　空

	TITLE,			//!<　タイトルシーン
	STAGE_SELECT,	//!<　ステージセレクトシーン
	GAME_MAIN,		//!<　ゲームメインシーン
	RESULT,			//!<　リザルトシーン

	MAX,			//!<　最大数
};

/*
 *	サブシーンID
 */
enum class SUB_SCENE_ID
{
	NONE = -1,	//!<　空

	PAUSE,			//!<　ポーズシーン

	MAX,			//!<　最大数
};