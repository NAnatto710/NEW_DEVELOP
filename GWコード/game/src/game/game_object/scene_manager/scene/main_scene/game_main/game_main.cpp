
/*!
 *  @file			game_main.h
 *  @brief			ゲームメインシーン
 *  @author			Ryusei Shimizu
 *  @date			2026/09/30
 */

#include "game_main.h"
#include "../../../scene_manager.h"

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
}

/*
 *	更新
 */
void
CGameMain::
Update (void)
{
	if (vivid::mouse::Trigger(vivid::mouse::BUTTON_ID::LEFT))
		CSceneManager::GetInstance().ChangeMainScene(MAIN_SCENE_ID::RESULT);
}

/*
 *	描画
 */
void
CGameMain::
Draw (void)
{
	IScene::Draw();
}

/*
 *	解放
 */
void
CGameMain::
Finalize (void)
{
}
