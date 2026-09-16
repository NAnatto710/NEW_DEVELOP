
/*!
 *  @file       result.cpp
 *  @brief      リザルトシーン
 *  @author     Ryusei Shimizu
 *  @date       2025/10/08
 */

#include "result.h"
#include "../../scene_manager.h"
#include "../../../../game_object/game_object.h"


const float				CResult::m_scene_change_wait_time	= 10.0f;		//!< シーン切り替え待機時間
const std::string		CResult::m_result_logo_path			= "data\\background\\result_scene_logo.png";	//!< リザルト背景のファイル名
const std::string		CResult::m_result_background_path	= "data\\background\\result_background.png";	//!< リザルト背景のファイル名

/*
 *	コンストラクタ
 */
CResult::
CResult(void)
	: IScene("ResultScene")
{
}

/*
 *	デストラクタ
 */
CResult::
~CResult(void)
{
}

/*
 *	初期化
 */
void
CResult::
Initialize(void)
{
	m_WaitTime = 0;

	CSoundManager::GetInstance().SetVolume(SOUND_ID::RESULT, 8000);
	CSoundManager::GetInstance().Play(SOUND_ID::RESULT, true);

	CGameParameterManager& pm = CGameParameterManager::GetInstance();

	bool is_clear = pm.IsGameClear();

	if (is_clear)
	{
		m_ResultPath = "data\\background\\gameclear.png";
	}
	else
	{
		m_ResultPath = "data\\background\\gameover.png";
	}
}

/*
 *	更新
 */
void
CResult::
Update(void)
{
	namespace keyboard = vivid::keyboard;
	namespace controller = vivid::controller;
	namespace mouse = vivid::mouse;

	bool change_scene = false;
	
	// シーン切り替え待機時間が経過したらタイトルに戻る
	if (m_WaitTime >= m_scene_change_wait_time)change_scene = true;
	else										m_WaitTime += vivid::GetDeltaTime();

	if (keyboard::Trigger(keyboard::KEY_ID::Z) ||
		keyboard::Trigger(keyboard::KEY_ID::RETURN) ||
		mouse::Trigger(mouse::BUTTON_ID::LEFT) ||
		controller::Trigger(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::B) ||
		controller::Trigger(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::A))
		change_scene = true;

	// タイトルに戻る
	if (change_scene)
	{
		CSoundManager::GetInstance().Stop(SOUND_ID::RESULT);
		CSceneManager::GetInstance().ChangeMainScene(MAINSCENE_ID::TITLE);
	}
}

/*
 *	描画
 */
void
CResult::
Draw(void)
{
	// タイトル背景の描画
	vivid::DrawTexture(m_result_background_path, vivid::Vector2(0, 0));

	// タイトルロゴの描画
	vivid::DrawTexture(m_result_logo_path, vivid::Vector2( 760, 200));

	// ゲームクリア・ゲームオーバーの描画
	vivid::DrawTexture(m_ResultPath, vivid::Vector2(460, 400));

	// スコアの描画
	CScoreManager::GetInstance().Draw();
}

/*
 *	解放
 */
void
CResult::
Finalize(void)
{
	CBulletManager::GetInstance().Finalize();
	CCharacterManager::GetInstance().Finalize();
	CStageManager::GetInstance().Finalize();
	CCameraManager::GetInstance().Finalize();
	CGameParameterManager::GetInstance().Finalize();
	CHudManager::GetInstance().Finalize();
	CAttackManager::GetInstance().Finalize();
	CEffectManager::GetInstance().Finalize();
	CScoreManager::GetInstance().Finalize();
	CUpgraeStatusManager::GetInstance().Finalize();
}