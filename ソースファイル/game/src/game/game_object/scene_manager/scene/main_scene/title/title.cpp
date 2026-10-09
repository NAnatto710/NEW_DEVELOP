
/*!
 *  @file			title.cpp
 *  @brief			タイトルシーン
 *  @author			Ryusei Shimizu
 *  @date			2026/09/30
 */

#include "title.h"
#include "../../../scene_manager.h"

/*
 *	コンストラクタ
 */
CTitle::
CTitle (void)
	:IScene("Title")
{
}

/*
 *	初期化
 */
void
CTitle::
Initialize (void)
{
}

/*
 *	更新
 */
void
CTitle::
Update (void)
{
	if (vivid::mouse::Trigger(vivid::mouse::BUTTON_ID::LEFT))
		CSceneManager::GetInstance().ChangeMainScene(MAIN_SCENE_ID::STAGE_SELECT);
}

/*
 *	描画
 */
void
CTitle::
Draw (void)
{
	IScene::Draw();
}

/*
 *	解放
 */
void
CTitle::
Finalize (void)
{
}
