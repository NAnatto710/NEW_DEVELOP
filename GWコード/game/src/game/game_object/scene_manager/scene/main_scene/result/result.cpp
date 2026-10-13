
/*!
 *  @file			result.cpp
 *  @brief			リザルトシーン
 *  @author			Ryusei Shimizu
 *  @date			2026/09/30
 */

#include "result.h"
#include "../../../scene_manager.h"

/*
 *	コンストラクタ
 */
CResult::
CResult (void)
	: IScene ("Result")
{
}

/*
 *	初期化
 */
void
CResult::
Initialize (void)
{
}

/*
 *	更新
 */
void
CResult::
Update (void)
{
	if (vivid::mouse::Trigger(vivid::mouse::BUTTON_ID::LEFT))
		CSceneManager::GetInstance().ChangeMainScene(MAIN_SCENE_ID::TITLE);
}

/*
 *	描画
 */
void
CResult::
Draw (void)
{
	IScene::Draw ();
}

/*
 *	解放
 */
void
CResult::
Finalize (void)
{
}
