
/*!
 *  @file		pause.cpp
 *  @brief		ポーズ
 *  @author     Ryusei Shimizu
 *  @date       2026/04/10
 */

#include "pause.h"
#include "../../scene_manager.h"
#include "../../../../../utility/utility.h"
#include <string>

 /*
  *	コンストラクタ
  */
CPause::
CPause(void)
	: IScene("Pause")
	, m_IsCountdown(false)
	, m_CountdownTimer(0.0f)
	, m_CountdownCount(3)
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
	m_IsInputLock = false;
	m_IsCountdown = false;
	m_CountdownTimer = 0.0f;
	m_CountdownCount = 3;

	vivid::CreateFont(120, 5);
}

/*
 *	更新
 */
void
CPause::
Update(void)
{
	namespace key = vivid::keyboard;
	namespace controller = vivid::controller;

	// 入力ロック判定
	if (!m_IsInputLock)
	{
		m_IsInputLock = key::Released(key::KEY_ID::X) ||
			controller::Released(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::START) ||
			controller::Released(controller::DEVICE_ID::PLAYER2, controller::BUTTON_ID::START) ||
			controller::Released(controller::DEVICE_ID::PLAYER3, controller::BUTTON_ID::START) ||
			controller::Released(controller::DEVICE_ID::PLAYER4, controller::BUTTON_ID::START);

		return;
	}

	bool is_sub_scene_change = key::Trigger(key::KEY_ID::X) ||
		controller::Trigger(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::START) ||
		controller::Trigger(controller::DEVICE_ID::PLAYER2, controller::BUTTON_ID::START) ||
		controller::Trigger(controller::DEVICE_ID::PLAYER3, controller::BUTTON_ID::START) ||
		controller::Trigger(controller::DEVICE_ID::PLAYER4, controller::BUTTON_ID::START);

	if (!m_IsCountdown)
	{
		if (is_sub_scene_change)
		{
			m_IsCountdown = true;

			m_CountdownTimer = 0.0f;
			m_CountdownCount = 3;
		}

		return;
	}

	m_CountdownTimer += vivid::GetDeltaTime();

	if (m_CountdownTimer >= 1.0f)
	{
		// 1秒分を消費
		m_CountdownTimer -= 1.0f;

		--m_CountdownCount;

		// カウント終了
		if (m_CountdownCount <= 0)
		{
			CSceneManager::GetInstance().ChangeSubScene(SUBSCENE_ID::NONE);

			return;
		}
	}

}

/*
 *	描画
 */
void
CPause::
Draw(void)
{
	unsigned char alpha = 180;

	// 背景の透明度をゆっくり下げる
	if (m_IsCountdown)
	{
		float elapsed_time = static_cast<float>(3 - m_CountdownCount) + m_CountdownTimer;

		float progress = elapsed_time / 3.0f;

		progress = CLAMP(progress, 0.0f, 1.0f);

		float alpha_value = 180.0f * (1.0f - progress);

		alpha =	static_cast<unsigned char>(alpha_value);
	}

	vivid::DrawTexture("data\\object\\white.png", vivid::Vector2(0.0f, 0.0f), Utility::GetColorByIdAlpha(COLOR_ID::BLACK, alpha));

	// カウントダウン
	if (m_IsCountdown)
	{
		const int text_size = 120;

		std::string text = std::to_string(m_CountdownCount);

		int text_width = vivid::GetTextWidth(text_size, text);

		vivid::Vector2 position = vivid::Vector2((vivid::GetWindowWidth() - text_width) / 2.0f, (vivid::GetWindowHeight() - text_size) / 2.0f);

		vivid::DrawText(text_size, text, position);
	}
	else
	{
		vivid::DrawTexture("data\\object\\pause.png", vivid::Vector2((vivid::GetWindowWidth() - 450) / 2.0f, (vivid::GetWindowHeight() - 160) / 2.0f));
	}

	IScene::Draw();
}

/*
 *	解放
 */
void
CPause::
Finalize(void)
{
	m_IsCountdown = false;
	m_CountdownTimer = 0.0f;
	m_CountdownCount = 3;
}
