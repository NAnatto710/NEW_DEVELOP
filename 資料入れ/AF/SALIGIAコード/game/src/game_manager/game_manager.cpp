
/*!
 *  @file		game_manager.cpp
 *  @brief		ゲーム管理
 *  @author     Ryusei Shimizu
 *  @date       2026/03/20
 */

#include "game_manager.h"
#include "game/scene_manager/scene_manager.h"
#include "../utility/sound_manager/sound_manager.h"
#include "game/effect_manager/effect_manager.h"
#include "../utility/utility_object.h"

/*
 *	インスタンス取得
 */
CGameManager&
CGameManager::
GetInstance()
{
	static CGameManager instance;

	return instance;
}

/*
 *	初期化
 */
void
CGameManager::
Initialize()
{
	// utilityオブジェクトの初期化
	CMovieManager::GetInstance().Initialize();
	CSoundManager::GetInstance().Initialize();

	// シーンマネージャーの初期化
	CSceneManager::GetInstance().Initialize();

	// エフェクトマネージャーの初期化
	CEffectManager::GetInstance().Initialize();
}

/*
 *	更新
 */
void
CGameManager::
Update()
{
	// utilityオブジェクトの更新
	CSoundManager::GetInstance().Update();

	// シーンマネージャーの更新
	CSceneManager::GetInstance().Update();

	// エフェクトマネージャーの更新
	CEffectManager::GetInstance().Update();
}

/*
 *	描画
 */
void
CGameManager::
Draw()
{
	// シーンマネージャーの描画
	CSceneManager::GetInstance().Draw();
	CSceneManager::GetInstance().DrawSceneEffect();

	// エフェクトマネージャーの描画
	CEffectManager::GetInstance().Draw();
}

/*
 *	解放
 */
void
CGameManager::
Finalize()
{
	//// utilityオブジェクトの解放
	//CMovieManager::GetInstance().Finalize();
	//CSoundManager::GetInstance().Finalize();

	//// シーンマネージャーの解放
	//CSceneManager::GetInstance().Finalize();

	//// エフェクトマネージャーの解放
	//CEffectManager::GetInstance().Finalize();
}
