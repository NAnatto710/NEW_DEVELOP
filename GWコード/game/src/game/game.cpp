
/*!
 *  @file			game.cpp
 *  @brief			ゲーム管理
 *  @author			Ryusei Shimizu
 *  @date			2026/09/16
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
	// リリース版でもマウスカーソルを表示できる
	SetMouseDispFlag(true);

	// Utility初期化
	Utility::Sound::Init();
	Utility::Movie::Init();
	Utility::Data::Init();

	// セーブデータのロード
	Utility::Data::Load();

	// シーンの初期化
	CSceneManager::GetInstance().Initialize();
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

	// シーンの更新
	CSceneManager::GetInstance().Update();
}

/*
 *	ゲーム描画
 */
void
CGame::
GameDraw(void)
{
	// シーンの描画（エフェクトと別で描画する）
	CSceneManager::GetInstance().Draw();
	CSceneManager::GetInstance().DrawSceneEffect();
}

/*
 *	ゲーム解放
 */
void
CGame::
GameFinalize(void)
{
	// シーンの解放
	CSceneManager::GetInstance().Finalize();

	// Utility解放
	Utility::Sound::Finalize();
	Utility::Movie::Finalize();
}
