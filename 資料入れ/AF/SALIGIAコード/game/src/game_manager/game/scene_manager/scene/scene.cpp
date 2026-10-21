
/*!
 *  @file		scene.cpp
 *  @brief		シーン基底
 *  @author     Ryusei Shimizu
 *  @date       2026/04/10
 */

#include "scene.h"

const int			IScene::m_text_size		= 40;			//!< デバックモードで表示されるテキストの文字サイズ
const unsigned int	IScene::m_text_color	= 0xff000000;	//!< デバックモードで表示されるテキストの文字の色

/*
 *	コンストラクタ
 */
IScene::
IScene(std::string SceneName)
	: m_SceneName(SceneName)
	, m_Position({ 0.0f,0.0f })
{
}

/*
 *	デストラクタ
 */
IScene::
~IScene(void)
{
}

/*
 *	初期化
 */
void
IScene::
Initialize(void)
{
}

/*
 *	更新
 */
void
IScene::
Update(void)
{
}

/*
 *	描画
 */
void
IScene::
Draw(void)
{
#ifdef VIVID_DEBUG

	// 画面左上に各シーンの名前を表示
	vivid::DrawText(m_text_size, m_SceneName, m_Position, m_text_color);

#endif // VIVID_DEBUG
}

/*
 *	解放
 */
void
IScene::
Finalize(void)
{
}