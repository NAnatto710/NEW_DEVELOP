
/*!
 *  @file       game.cpp
 *  @brief      ゲーム管理
 *  @author     Ryusei Shimizu
 *  @date       2025/10/08
 */

#include "game.h"
#include "../game/game_object/scene_manager/scene_manager.h"
#include "../game/game_object/sound_manager/sound_manager.h"

/*
 *	ゲーム初期化
 */
void
CGame::
GameInitialize(void)
{
	SetMouseDispFlag(true);	// マウスカーソル表示

	CSoundManager::GetInstance().Initialize();

	// シーンマネージャー初期化
	CSceneManager::GetInstance().Initialize();
}

/*
 *	ゲーム更新
 */
void
CGame::
GameUpdate(void)
{
	// シーンマネージャー初期化
	CSceneManager::GetInstance().Update();
}

/*
 *	ゲーム描画
 */
void
CGame::
GameDraw(void)
{
	// シーンマネージャー取得
	CSceneManager& sm = CSceneManager::GetInstance();

	// シーン描画
	sm.Draw();

	// シーンエフェクト描画
	sm.DrawSceneEffect();
}

/*
 *	ゲーム解放
 */
void
CGame::
GameFinalize(void)
{
	// シーンマネージャー初期化
	CSceneManager::GetInstance().Finalize();
}
