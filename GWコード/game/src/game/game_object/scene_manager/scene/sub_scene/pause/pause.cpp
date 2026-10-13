
/*!
 *  @file			pause.cpp
 *  @brief			ポーズシーン
 *  @author			Ryusei Shimizu
 *  @date			2026/09/30
 */

#include "pause.h"

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
