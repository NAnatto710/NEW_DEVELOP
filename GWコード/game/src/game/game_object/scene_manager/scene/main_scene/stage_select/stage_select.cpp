
/*!
 *  @file			stage_select.h
 *  @brief			ステージセレクトシーン
 *  @author			Ryusei Shimizu
 *  @date			2026/09/30
 */

#include "stage_select.h"
#include "../../../scene_manager.h"

/*
 *	コンストラクタ
 */
CStageSelect::
CStageSelect (void)
	: IScene ("StageSelect")
{
}

/*
 *	初期化
 */
void
CStageSelect::
Initialize (void)
{
}

/*
 *	更新
 */
void
CStageSelect::
Update (void)
{
	if(vivid::keyboard::Trigger (vivid::keyboard::KEY_ID::Z))
		CSceneManager::GetInstance().ChangeMainScene(MAIN_SCENE_ID::GAME_MAIN);
}

/*
 *	描画
 */
void
CStageSelect::
Draw (void)
{
	IScene::Draw ();
}

/*
 *	解放
 */
void
CStageSelect::
Finalize (void)
{
}
