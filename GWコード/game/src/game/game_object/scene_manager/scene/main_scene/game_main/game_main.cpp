
/*!
 *  @file			game_main.h
 *  @brief			ゲームメインシーン
 *  @author			Ryusei Shimizu
 *  @date			2026/09/30
 */

#include "game_main.h"
#include "../../../scene_manager.h"
#include "../../../../game_object.h"

/*
 *	コンストラクタ
 */
CGameMain::
CGameMain (void)
	: IScene("GameMain")
{
}

/*
 *	初期化
 */
void
CGameMain::
Initialize (void)
{
	CStageManager::GetInstance ().Initialize();
	CCharacterManager::GetInstance ().Initialize({100.0f, 100.0f}, 64, 64);
}

/*
 *	更新
 */
void
CGameMain::
Update (void)
{
	CStageManager::GetInstance ().Update();
	CCharacterManager::GetInstance ().Update ();

	if (vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::Z))
		CSceneManager::GetInstance().ChangeMainScene(MAIN_SCENE_ID::RESULT);
	if(vivid::keyboard::Trigger (vivid::keyboard::KEY_ID::X))
		CSceneManager::GetInstance ().ChangeSubScene(SUB_SCENE_ID::PAUSE);
}

/*
 *	描画
 */
void
CGameMain::
Draw (void)
{
	CStageManager::GetInstance ().Draw();
	CCharacterManager::GetInstance ().Draw ();

	IScene::Draw();
}

/*
 *	解放
 */
void
CGameMain::
Finalize (void)
{
	CStageManager::GetInstance ().Finalize();
	CCharacterManager::GetInstance ().Finalize ();
}
