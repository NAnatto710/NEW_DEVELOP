
/*!
 *  @file       pause.cpp
 *  @brief      ポーズシーン
 *  @author     Ryusei Shimizu
 *  @date       2025/12/18
 */

#include "pause.h"
#include "../../scene_manager.h"
#include "../../../upgrade_manager/upgrade_manager.h"


const float	CPause::m_scene_change_wait_time = 2.0f;						//!< シーン切り替え待機時間

/*
 *	コンストラクタ
 */
CPause::
CPause(void)
	:IScene("PauseScene")
{
}

/*
 *	デストラクタ
 */
CPause::
~CPause(void)
{
}

/*
 *	初期化
 */
void
CPause::
Initialize(void)
{
}

/*
 *	更新
 */
void
CPause::
Update(void)
{
	namespace keyboard = vivid::keyboard;
	namespace controller = vivid::controller;

	if (keyboard::Trigger(keyboard::KEY_ID::X) ||
		vivid::controller::Trigger(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::START) ||
		vivid::controller::Trigger(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::A) || 
		vivid::controller::Trigger(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::B))
		CSceneManager::GetInstance().ChangeSubScene(SUBSCENE_ID::DUMMY);

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

#ifdef VIVID_DEBUG
	if (vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::Z))
		CSceneManager::GetInstance().ChangeMainScene(MAINSCENE_ID::RESULT);
#endif
}

/*
 *	描画
 */
void
CPause::
Draw(void)
{
	//IScene::Draw();

	// アップグレードのステータスグラフを描画
	// 引数は、アップグレードのステータスグラフを描画するかどうか
	CUpgraeStatusManager::GetInstance().DrawUpgradeStatusGraph(false);
}

/*
 *	解放
 */
void
CPause::
Finalize(void)
{
}