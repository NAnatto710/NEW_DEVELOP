
/*!
 *  @file			scene.cpp
 *  @brief			シーン基底
 *  @author			Ryusei Shimizu
 *  @date			2026/09/30
 */

#include "scene.h"

const int			IScene::m_scene_text_size	= 40;			//!< デバックモードで表示されるテキストの文字サイズ
const unsigned int	IScene::m_scene_text_color	= 0xff000000;	//!< デバックモードで表示されるテキストの文字の色

/*
 *	コンストラクタ
 */
IScene::
IScene (std::string scene_id)
	: m_SceneText(scene_id)
	, m_SceneTextPosition({0.0f,0.0f})
{
}

/*
 *	初期化
 */
void
IScene::
Initialize (void)
{
}

/*
 *	更新
 */
void
IScene::
Update (void)
{
}

/*
 *	描画
 */
void
IScene::
Draw (void)
{
#ifdef VIVID_DEBUG

	// 画面左上に各シーンの名前を表示
	vivid::DrawText (m_scene_text_size, m_SceneText, m_SceneTextPosition, m_scene_text_color);

#endif // VIVID_DEBUG
}

/*
 *	解放
 */
void
IScene::
Finalize (void)
{
}