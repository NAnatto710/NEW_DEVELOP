
/*!
 *  @file       game_main.cpp
 *  @brief      ゲームメインシーン
 *  @author     Ryusei Shimizu
 *  @date       2025/10/08
 */

#include "game_main.h"
#include "../../scene_manager.h"
#include "../../../../game_object/game_object.h"


const float CGameMain::m_scene_change_wait_time = 2.0f;		//!< シーン切り替え待機時間

/*
 *	コンストラクタ
 */
CGameMain::
CGameMain(void)
	:IScene("GameMainScene")
{
}

/*
 *	デストラクタ
 */
CGameMain::
~CGameMain(void)
{
}

/*
 *	初期化
 */
void
CGameMain::
Initialize(void)
{
	m_WaitTime = 0;
	
	CSoundManager::GetInstance().SetVolume(SOUND_ID::GAMEMAIN_BGM, 10000);
	//ゲームメインサウンド
	CSoundManager::GetInstance().Play(SOUND_ID::GAMEMAIN_BGM, true);
}

/*
 *	更新
 */
void
CGameMain::
Update(void)
{
	namespace keyboard = vivid::keyboard;
	namespace controller = vivid::controller;

	// ゲームクリア判定
	if (CGameParameterManager::GetInstance().IsGameClear())
	{
		CSceneManager::GetInstance().ChangeMainScene(MAINSCENE_ID::RESULT);

		//ゲームメインサウンド停止
		CSoundManager::GetInstance().Stop(SOUND_ID::GAMEMAIN_BGM);
	}

	// ゲームオーバー判定
	if (CCharacterManager::GetInstance().GetPlayer()->GetStatus(STATUS_ID::HP) <= 0)
	{
		CSceneManager::GetInstance().ChangeMainScene(MAINSCENE_ID::RESULT);

		//ゲームメインサウンド停止
		CSoundManager::GetInstance().Stop(SOUND_ID::GAMEMAIN_BGM);
	}

	CGameParameterManager::GetInstance().Update();

	// シーズン変更エフェクト中はキャラクター、バレットの更新を行わない
	if (!CGameParameterManager::GetInstance().IsSeasonChangeEffect())
	{
		CCharacterManager::GetInstance().Update();
		CBulletManager::GetInstance().Update();
	}

	CCameraManager::GetInstance().Update();
	CStageManager::GetInstance().Update();
	CHudManager::GetInstance().Update();
	CEffectManager::GetInstance().Update();
	CUpgraeStatusManager::GetInstance().Update();

	if (keyboard::Trigger(keyboard::KEY_ID::X) ||
		controller::Trigger(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::START))
		CSceneManager::GetInstance().ChangeSubScene(SUBSCENE_ID::PAUSE);

	bool natto = keyboard::Button(keyboard::KEY_ID::SEVEN) &&
		keyboard::Button(keyboard::KEY_ID::ONE) &&
		keyboard::Button(keyboard::KEY_ID::ZERO) ||
		controller::Button(controller::DEVICE_ID::PLAYER1,controller::BUTTON_ID::X) &&
		controller::Button(controller::DEVICE_ID::PLAYER1,controller::BUTTON_ID::Y) &&
		controller::Button(controller::DEVICE_ID::PLAYER1,controller::BUTTON_ID::LEFT_SHOULDER) &&
		controller::Button(controller::DEVICE_ID::PLAYER1,controller::BUTTON_ID::RIGHT_SHOULDER);

	// 数字の７と１と０を同時に一定時間以上押すとリザルト画面へ
	if (natto)
	{
		m_WaitTime += vivid::GetDeltaTime();

		if (m_WaitTime >= m_scene_change_wait_time)
			CSceneManager::GetInstance().ChangeMainScene(MAINSCENE_ID::RESULT);

		//サウンド停止
		CSoundManager::GetInstance().Stop(SOUND_ID::GAMEMAIN_BGM);
	}
	else
	{
		m_WaitTime = 0.0f;
	}

#ifdef VIVID_DEBUG
	if (vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::Z))
	{
		CSceneManager::GetInstance().ChangeMainScene(MAINSCENE_ID::RESULT);

		//サウンド停止
		CSoundManager::GetInstance().Stop(SOUND_ID::GAMEMAIN_BGM);
	}
#endif
}

/*
 *	描画
 */
void
CGameMain::
Draw(void)
{
	CStageManager::GetInstance().Draw();
	CEffectManager::GetInstance().Draw();
	CCharacterManager::GetInstance().Draw();
	CBulletManager::GetInstance().Draw();
	CHudManager::GetInstance().Draw();
	CEffectManager::GetInstance().SceneEffectDraw();
	CGameParameterManager::GetInstance().Draw();
	CUpgraeStatusManager::GetInstance().Draw();

	//IScene::Draw();
}

/*
 *	解放
 */
void
CGameMain::
Finalize(void)
{
}