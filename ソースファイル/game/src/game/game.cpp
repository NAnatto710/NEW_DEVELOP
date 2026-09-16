
/*!
 *  @file       game.cpp
 *  @brief      ゲーム管理
 *  @author     Ryusei Shimizu
 *  @date       2026/09/16
 */

#include "game.h"
#include "../utility/utility.h"

/*
 *	ゲーム初期化
 */
void
CGame::
GameInitialize(void)
{
	// マウスカーソル表示
	SetMouseDispFlag(true);

	// Utility初期化
	Utility::Sound::Init();
	Utility::Movie::Init();
	Utility::Data::Init();

	Utility::Data::Load();
}

/*
 *	ゲーム更新
 */
void
CGame::
GameUpdate(void)
{
	// Utility更新
	Utility::Sound::Update();
}

/*
 *	ゲーム描画
 */
void
CGame::
GameDraw(void)
{
}

/*
 *	ゲーム解放
 */
void
CGame::
GameFinalize(void)
{
}
