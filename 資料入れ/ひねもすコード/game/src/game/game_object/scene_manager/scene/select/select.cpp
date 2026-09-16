
/*!
 *  @file       select.cpp
 *  @brief      セレクトシーン
 *  @author     Ryusei Shimizu
 *  @date       2026/02/04
 */

#include "select.h"
#include "../../scene_manager.h"
#include "../../../upgrade_manager/upgrade_manager.h"


const float	CSelect::m_scene_change_wait_time = 2.0f;		//!< シーン切り替え待機時間

/*
 *	コンストラクタ
 */
CSelect::
CSelect(void)
	:IScene("SelectScene")
{
}

/*
 *	デストラクタ
 */
CSelect::
~CSelect(void)
{
}

/*
 *	初期化
 */
void
CSelect::
Initialize(void)
{
}

/*
 *	更新
 */
void
CSelect::
Update(void)
{
	namespace keyboard = vivid::keyboard;
	namespace controller = vivid::controller;

	bool natto = keyboard::Button(keyboard::KEY_ID::SEVEN) &&
		keyboard::Button(keyboard::KEY_ID::ONE) &&
		keyboard::Button(keyboard::KEY_ID::ZERO) ||
		controller::Button(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::X) &&
		controller::Button(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::Y) &&
		controller::Button(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::LEFT_SHOULDER) &&
		controller::Button(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::RIGHT_SHOULDER);

	// 数字の７と１と０を同時に一定時間以上押すとリザルト画面へ
	if (natto)
	{
		m_WaitTime += vivid::GetDeltaTime();

		if (m_WaitTime >= m_scene_change_wait_time)
		{
			CSceneManager::GetInstance().ChangeSubScene(SUBSCENE_ID::DUMMY);
			CSceneManager::GetInstance().ChangeMainScene(MAINSCENE_ID::RESULT);
		}
	}
	else
	{
		m_WaitTime = 0.0f;
	}

	// 強化ステータスの選択
	CUpgraeStatusManager::GetInstance().SelectUpgradeStatus();
}

/*
 *	描画
 */
void
CSelect::
Draw(void)
{
	//IScene::Draw();

	// 強化ステータスの描画
	// 引数は、選択中の強化ステータスと選択ボタンを描画するかどうか
	CUpgraeStatusManager::GetInstance().DrawUpgradeStatusGraph(true);
}

/*
 *	解放
 */
void
CSelect::
Finalize(void)
{
}