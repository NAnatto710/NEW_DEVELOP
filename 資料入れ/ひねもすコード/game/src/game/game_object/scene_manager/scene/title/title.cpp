
/*!
 *  @file       title.cpp
 *  @brief      タイトルシーン
 *  @author     Ryusei Shimizu
 *  @date       2025/10/08
 */

#include "title.h"
#include "../../scene_manager.h"
#include "../../../../../utility/utility.h"
#include "../../../../game_object/game_object.h"

const int				CTitle::m_title_logo_width	= 1500;								//!< タイトルロゴの幅
const int				CTitle::m_title_logo_height = 565;								//!< タイトルロゴの高さ
const vivid::Vector2	CTitle::m_title_logo_pos	= vivid::Vector2(80, 250);			//!< タイトルロゴの位置
const std::string		CTitle::m_title_logo_path			= "data\\background\\title.png";	//!< タイトルロゴのファイル名
const std::string		CTitle::m_title_background_path		= "data\\background\\title_background.png";	//!< タイトル背景のファイル名
const std::string		CTitle::m_start_button_path			= "data\\background\\start_button.png";		//!< スタートボタンのファイル名
const float				CTitle::m_scene_change_wait_time	= 2.0f;										//!< シーン切り替え待機時間

/*
 *	コンストラクタ
 */
CTitle::
CTitle(void)
	:IScene("TitleScene")
{
}

/*
 *	デストラクタ
 */
CTitle::
~CTitle(void)
{
}

/*
 *	初期化
 */
void
CTitle::
Initialize(void)
{
	m_ChangeSceneFlg = false;

	CBulletManager::GetInstance().Initialize();
	CGameParameterManager::GetInstance().Initialize();
	CStageManager::GetInstance().Initialize();
	CCharacterManager::GetInstance().Initialize();
	CCameraManager::GetInstance().Initialize();
	CHudManager::GetInstance().Initialize();
	CAttackManager::GetInstance().Initialize();
	CEffectManager::GetInstance().Initialize();
	CScoreManager::GetInstance().Initialize();
	CUpgraeStatusManager::GetInstance().Initialize();
	CSoundManager::GetInstance().Initialize();

	//タイトルのサウンド音量調節
	CSoundManager::GetInstance().SetVolume(SOUND_ID::TITLE, 8000);
	//タイトルのサウンド
	CSoundManager::GetInstance().Play(SOUND_ID::TITLE, false);
}

/*
 *	更新
 */
void
CTitle::
Update(void)
{
	namespace keyboard = vivid::keyboard;
	namespace controller = vivid::controller;
	namespace mouse = vivid::mouse;
	
	// シーン切り替え
	if (!m_ChangeSceneFlg)
		if (keyboard::Released(keyboard::KEY_ID::Z) ||
			keyboard::Released(keyboard::KEY_ID::RETURN) ||
			mouse::Released(mouse::BUTTON_ID::LEFT) ||
			controller::Released(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::B) ||
			controller::Released(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::A))
		{
			CSceneManager::GetInstance().ChangeSubScene(SUBSCENE_ID::SELECT);

			//タイトルサウンド停止
			CSoundManager::GetInstance().Stop(SOUND_ID::TITLE);

			m_ChangeSceneFlg = true;
		}

	if (m_ChangeSceneFlg)
	{
		CGameParameterManager::GetInstance().Update();
		CEffectManager::GetInstance().Update();
	}

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
}

/*
 *	描画
 */
void
CTitle::
Draw(void)
{
	// タイトル背景の描画
	vivid::DrawTexture(m_title_background_path, vivid::Vector2(0, 0));

	// タイトルロゴの描画
	vivid::DrawTexture(m_title_logo_path, m_title_logo_pos);

	// スタートボタンの描画
	vivid::DrawTexture(m_start_button_path, vivid::Vector2(420, 850));

	// シーン切り替えフラグが立っている場合
	if (m_ChangeSceneFlg)
	{
		// シーズンチェンジエフェクトの描画
		CGameParameterManager::GetInstance().Draw();
		CEffectManager::GetInstance().SceneEffectDraw();
	}
}

/*
 *	解放
 */
void
CTitle::
Finalize(void)
{
}