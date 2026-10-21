
/*!
 *  @file       sound_id.h
 *  @brief      サウンドID
 *  @author     Ryusei Shimizu
 *  @date       2026/03/20
 */

#pragma once
#include "vivid.h"

/*!
 *	@brief  サウンドID
 */
enum class SOUND_ID
{
	HIT,		   //!< ヒット
	MISS,		   //!< ミス
	DROP,		   //!< 落下
	GUARD,		   //!< ガード
	GUARD_BREAK,   //!< ガードブレイク
	JUMP,		   //!< ジャンプ
	LANDING,	   //!< 着地
	RESPAWN,	   //!< リスポーン
	JOIN,		   //!< 参加
	PUSH,		   //!< ボタン
	BUILD,		   //!< ビルド
	CANCEL,		   //!< キャンセル
	START,		   //!< スタート
	SELECT,		   //!< カーソル移動
	OK,

	TITLE_BGM,
	PLAYER_JOIN_BGM,
	MAIN_BGM,	   //!< メインBGM
	BUILD_SELECT_BGM,
	RESULY_BGM,

	MAX			   //!< 最大値
};
