
/*!
 *  @file		title.cpp
 *  @brief		タイトル
 *  @author     Hiroto Maniwa
 *  @date       2026/04/22
 */

#include "title.h"
#include "../../scene_manager.h"
#include "../../../../../utility/sound_manager/sound_manager.h"
#include "../../../effect_manager/effect_manager.h"
#include "../../../../../utility/utility.h"
#include "../../../../../utility/movie_manager/movie_manager.h"

namespace
{
	bool HasTitleInput(void)
	{
		using namespace vivid;

		if (keyboard::Button(keyboard::KEY_ID::Z) ||
			keyboard::Button(keyboard::KEY_ID::RETURN) ||
			keyboard::Button(keyboard::KEY_ID::SPACE) ||
			keyboard::Button(keyboard::KEY_ID::UP) ||
			keyboard::Button(keyboard::KEY_ID::DOWN) ||
			keyboard::Button(keyboard::KEY_ID::LEFT) ||
			keyboard::Button(keyboard::KEY_ID::RIGHT))
		{
			return true;
		}

		const controller::BUTTON_ID buttons[] =
		{
			controller::BUTTON_ID::UP,
			controller::BUTTON_ID::DOWN,
			controller::BUTTON_ID::LEFT,
			controller::BUTTON_ID::RIGHT,
			controller::BUTTON_ID::START,
			controller::BUTTON_ID::BACK,
			controller::BUTTON_ID::LEFT_THUMB,
			controller::BUTTON_ID::RIGHT_THUMB,
			controller::BUTTON_ID::LEFT_SHOULDER,
			controller::BUTTON_ID::RIGHT_SHOULDER,
			controller::BUTTON_ID::A,
			controller::BUTTON_ID::B,
			controller::BUTTON_ID::X,
			controller::BUTTON_ID::Y,
		};

		for (int i = 0; i < static_cast<int>(controller::DEVICE_ID::MAX); ++i)
		{
			const controller::DEVICE_ID device = static_cast<controller::DEVICE_ID>(i);

			for (const controller::BUTTON_ID button : buttons)
			{
				if (controller::Button(device, button))
				{
					return true;
				}
			}

			if (controller::TriggerAnalogStickLeft(device, controller::STICK_DIRECTION::UP) ||
				controller::TriggerAnalogStickLeft(device, controller::STICK_DIRECTION::DOWN) ||
				controller::TriggerAnalogStickLeft(device, controller::STICK_DIRECTION::LEFT) ||
				controller::TriggerAnalogStickLeft(device, controller::STICK_DIRECTION::RIGHT))
			{
				return true;
			}
		}

		return false;
	}
}

const int CTitle::m_width = 840.0f;
const int CTitle::m_height = 500.0f;
const int CTitle::m_b_push_width = 600.0f;
const int CTitle::m_b_push_height = 80.0f;
const float CTitle::m_interval = 0.01;
const float CTitle::m_b_push_rotation = 0.0f;
const float CTitle::m_demo_start_time = 10.0f;
const char* CTitle::m_demo_movie_path = "data\\movie\\wait_movie.mp4";
const float CTitle::m_fade_speed = 1.5f;
const vivid::Vector2 CTitle::m_title_position = vivid::Vector2((vivid::GetWindowWidth() - m_width) / 2, (vivid::GetWindowHeight() - m_height) / 2 - 100.0f);
const vivid::Vector2 CTitle::m_b_push_position = vivid::Vector2((vivid::GetWindowWidth() - m_b_push_width) / 2, (vivid::GetWindowHeight() - m_b_push_height) / 2 + 200);
const vivid::Vector2 CTitle::m_b_push_anchor = vivid::Vector2(m_b_push_width / 2.0f, m_b_push_height / 2.0f);
const vivid::Vector2 CTitle::m_b_push_scale = vivid::Vector2(1.0f, 1.0f);
const vivid::Rect CTitle::m_b_push_rect = { 0,0,m_b_push_width ,m_b_push_height };
const float CTitle::m_flash_time = 50.0f;

/*
 *	コンストラクタ
 */
CTitle::
CTitle(void)
	:IScene("title")
	, m_Timer(0.0f)
	, m_IdleTimer(0.0f)
	, m_RectTimer(0.0f)
	, m_AlphaFlg(false)
	, m_IsMovieMode(false)
	, m_ChangeFlg(false)
	, m_BPushColor(0xffffffff)
	, m_BPushColorFlash(0xffffffff)
	, m_BPushPosition{ 0.0f,0.0f }
	, m_BPushScale{ 0.0f,0.0f }
	, m_BPushRect{ 0,0,0,0 }
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
	m_Timer = 0.0f;
	m_IdleTimer = 0.0f;
	m_RectTimer = 0.0f;
	m_BPushPosition = m_b_push_position;
	m_BPushRect = { 0,0,0,0 };
	m_BPushColor = 0xffffffff;
	m_BPushColorFlash = 0x80ffffff;
	m_AlphaFlg = false;
	m_IsMovieMode = false;
	m_ChangeFlg = false;
	CMovieManager::GetInstance().Stop();
}

/*
 *	更新
 */
void
CTitle::
Update(void)
{
	CMovieManager& movie = CMovieManager::GetInstance();

	if (m_IsMovieMode)
	{
		if (HasTitleInput())
		{
			movie.Stop();
			m_IsMovieMode = false;
			m_IdleTimer = 0.0f;
			return;
		}

		if (!movie.IsPlaying())
		{
			m_IsMovieMode = false;
			m_IdleTimer = 0.0f;
		}
		else
		{
			return;
		}
	}

	const float delta_time = vivid::GetDeltaTime();
	m_Timer += delta_time;
	m_RectTimer += delta_time;

	if (HasTitleInput())
	{
		m_IdleTimer = 0.0f;
	}
	else
	{
		m_IdleTimer += delta_time;
	}

	bool IsMainSceneChange = vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::Z) ||
		vivid::controller::Button(vivid::controller::DEVICE_ID::PLAYER1, vivid::controller::BUTTON_ID::B) ||
		vivid::controller::Button(vivid::controller::DEVICE_ID::PLAYER2, vivid::controller::BUTTON_ID::B) ||
		vivid::controller::Button(vivid::controller::DEVICE_ID::PLAYER3, vivid::controller::BUTTON_ID::B) ||
		vivid::controller::Button(vivid::controller::DEVICE_ID::PLAYER4, vivid::controller::BUTTON_ID::B);

	if (IsMainSceneChange)
	{
		CSoundManager::GetInstance().PlaySE(SOUND_ID::PUSH);
		CSceneManager::GetInstance().ChangeMainScene(MAINSCENE_ID::PLAYER_JOIN);
		CEffectManager::GetInstance().Finalize();
		return;
	}

	if (m_IdleTimer >= m_demo_start_time)
	{
		m_IdleTimer = 0.0f;

		if (movie.Play(m_demo_movie_path))
		{
			m_IsMovieMode = true;
			CEffectManager::GetInstance().Finalize();
			return;
		}
	}

	if (m_Timer >= m_interval)
	{
		float i = 1 * Utility::GetRandomInt(1, 5);
		int j = Utility::GetRandomInt(1, 22);
		CEffectManager::GetInstance().Create(EFFECT_ID::TITLE_PARTICLE, PLAYER_ID::PLAYER1, vivid::Vector2(Utility::GetRandomInt(0, vivid::GetWindowWidth()), Utility::GetRandomInt(0, vivid::GetWindowHeight())), DIRECTION::RIGHT, vivid::Vector2(i, i), Utility::GetColorByIdAlpha((COLOR_ID)Utility::GetRandomInt(0, 11), Utility::GetRandomInt(128, 255)), 0.0f);
		CEffectManager::GetInstance().Create(EFFECT_ID::TWINKLE, PLAYER_ID::PLAYER1, vivid::Vector2(Utility::GetRandomInt(0, vivid::GetWindowWidth()), Utility::GetRandomInt(0, vivid::GetWindowHeight())), DIRECTION::RIGHT, vivid::Vector2(i, i), Utility::GetColorById((COLOR_ID)Utility::GetRandomInt(0, 11)), DEG_TO_RAD(45));

		m_Timer = 0.0f;
		if (j == 22)
		{
			CEffectManager::GetInstance().Create(EFFECT_ID::SIRCLE, PLAYER_ID::MAX, vivid::Vector2(Utility::GetRandomInt(0, vivid::GetWindowWidth()), Utility::GetRandomInt(0, vivid::GetWindowHeight())), DIRECTION::RIGHT, vivid::Vector2(0.5, 0.5), Utility::GetColorByIdAlpha((COLOR_ID)Utility::GetRandomInt(0, 11), Utility::GetRandomInt(64, 255)), 0.0f);
		}
	}

	int i = (int)(m_RectTimer * 15) * m_flash_time;
	m_BPushRect = { i,0,i + 100,m_b_push_height };
	m_BPushPosition.x = m_b_push_position.x + i;

	if (m_RectTimer >= 3.0f)
	{
		m_RectTimer = 0.0f;
		m_ChangeFlg = !m_ChangeFlg;
	}

	/*if (m_ChangeFlg == false)
	{
		m_BPushRect.left = i + 100;
	}
	if (m_ChangeFlg==true)
	{
		m_BPushRect.right = i;
	}*/

	float alpha = (m_BPushColor & 0xff000000) >> 24;
	float max_alpha = (0xffffffff & 0xff000000) >> 24;

	if (m_AlphaFlg == false)
	{
		alpha -= m_fade_speed;
		if (alpha < 0)
		{
			alpha = 0;
			m_AlphaFlg = true;

		}
	}

	if (m_AlphaFlg == true)
	{
		alpha += m_fade_speed;
		if (alpha >= max_alpha)
		{
			m_AlphaFlg = false;
		}
	}
	m_BPushColor = ((unsigned int)alpha << 24) | (m_BPushColor & 0x00ffffff);
	m_BPushColorFlash = ((unsigned int)alpha << 24) | (m_BPushColor & 0x00ffffff);

}

/*
 *	描画
 */
void
CTitle::
Draw(void)
{
	IScene::Draw();

	if (m_IsMovieMode)
	{
		CMovieManager::GetInstance().Display(0, 0, vivid::GetWindowWidth(), vivid::GetWindowHeight());
		return;
	}

	vivid::DrawTexture("data\\map\\background_layer\\background2.png", vivid::Vector2::ZERO);
	vivid::DrawTexture("data\\object\\title.png", m_title_position);
	vivid::DrawTexture("data\\object\\b_push.png", m_b_push_position, m_BPushColor, m_b_push_rect, m_b_push_anchor, m_b_push_scale, m_b_push_rotation);
	vivid::DrawTexture("data\\object\\b_push.png", m_BPushPosition, m_BPushColorFlash, m_BPushRect, m_b_push_anchor, m_b_push_scale, m_b_push_rotation);

}

/*
 *	解放
 */
void
CTitle::
Finalize(void)
{
	CMovieManager::GetInstance().Stop();
	m_IsMovieMode = false;
	CEffectManager::GetInstance().Finalize();

}
