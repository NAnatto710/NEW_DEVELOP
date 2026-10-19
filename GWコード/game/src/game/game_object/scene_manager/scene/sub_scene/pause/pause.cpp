
/*!
 *  @file			pause.cpp
 *  @brief			ポーズシーン
 *  @author			Ryusei Shimizu
 *  @date			2026/09/30
 */

#include "pause.h"
#include "../../../scene_manager.h"

 /*
  *	コンストラクタ
  */
CPause::
CPause (void)
	:IScene ("Pause")
{
}

/*
 *	初期化
 */
void
CPause::
Initialize (void)
{
	m_SceneTextPosition = { 0.0f, 40.0f };
}

/*
 *	更新
 */
void
CPause::
Update (void)
{
	if(vivid::keyboard::Trigger (vivid::keyboard::KEY_ID::X))
		CSceneManager::GetInstance ().ChangeSubScene (SUB_SCENE_ID::NONE);
}

/*
 *	描画
 */
void
CPause::
Draw (void)
{
	IScene::Draw ();
}

/*
 *	解放
 */
void
CPause::
Finalize (void)
{
}
