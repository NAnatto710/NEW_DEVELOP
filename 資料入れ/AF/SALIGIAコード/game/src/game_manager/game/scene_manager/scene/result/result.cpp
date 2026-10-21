
/*!
 *  @file		result.cpp
 *  @brief		リザルト
 *  @author     Hiroto Maniwa
 *  @date       2026/04/22
 */

#include "result.h"
#include "../../scene_manager.h"
#include "../../../player_manager/player_manager.h"
#include "../game_main/game_main.h"
#include "../../../../../utility/sound_manager/sound_manager.h"
#include "../../../../../utility/utility.h"
#include "../../../effect_manager/effect_manager.h"

const std::string CResult::m_ranking_text[] =
{
	"1st",
	"2nd",
	"3rd",
	"4th"
};
const std::string CResult::m_player_text[] =
{
	"1P",
	"2P",
	"3P",
	"4P"
};
const int CResult::m_text_size = 120;
const int CResult::m_result_size = 40;
const unsigned int CResult::m_player_text_color[] =
{
	0xffff0000,		// 赤
	0xff0000ff,		// 青
	0xff00ff00,		// 緑
	0xffffff00 		// 黄色
};
const unsigned int CResult::m_ranking_color[] =
{
	0xffffd700,	// 金
	0xffc0c0c0,	// 銀
	0xffb87333,	// 銅
	0xffa6a4ed  // 青白
};
const vivid::Vector2 CResult::m_text_position[] =
{
	vivid::Vector2(((vivid::GetWindowWidth() / 4 * 0) + vivid::GetWindowWidth() / 4) - (6 * m_text_size / 2),vivid::GetWindowHeight() / 3),
	vivid::Vector2(((vivid::GetWindowWidth() / 4 * 1) + vivid::GetWindowWidth() / 4) - (6 * m_text_size / 2),vivid::GetWindowHeight() / 3),
	vivid::Vector2(((vivid::GetWindowWidth() / 4 * 2) + vivid::GetWindowWidth() / 4) - (6 * m_text_size / 2),vivid::GetWindowHeight() / 3),
	vivid::Vector2(((vivid::GetWindowWidth() / 4 * 3) + vivid::GetWindowWidth() / 4) - (6 * m_text_size / 2),vivid::GetWindowHeight() / 3)
};
const float CResult::m_start_x[] =
{
	429.0f,
	240.0f,
	74.8f
};
const float CResult::m_interval[] =
{
	665.2f,
	521.5f,
	456.5f
};
const float CResult::m_scroll_speed = 2.0;
const float CResult::m_particle_interval = 0.001f;
const int   CResult::m_display_background_width = 400;
const int   CResult::m_display_background_height = 560;
const int	CResult::m_player_display_width = 200;
const int   CResult::m_player_display_height = 170;
const int	CResult::m_crown_width = 300;
const int   CResult::m_crown_height = 200;
const float CResult::m_fade_speed=2.0f;
const float CResult::m_drop_speed=2.0f;
const float CResult::m_swich_change_time = 30.0f;
const unsigned int CResult::m_default_color = 0xffffffff;
const float CResult::m_enlarge_speed = 0.05f;
const float CResult::m_rotation_speed = 1.0f;
 /*
  *	コンストラクタ
  */
CResult::
CResult(void)
	:IScene("Result")
	,m_JoinCount(0)
	,m_ScrollTimer(0.0f)
	,m_CrownPosition{0.0f,0.0f}
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
	m_JoinCount = CSceneManager::GetInstance().GetJoinCount();
	//m_JoinCount = 4;
	vivid::CreateFont(m_text_size, 10);
	for (int i = 0;i < 2;i++)
	{
		m_IconRect[i] = { 0,0,2940,420 };
	}
	m_IconPosition[0] = { 0,0 };
	m_IconPosition[1] = { 2940,0 };
	m_SwitchSceneTimer = 0.0f;
	m_ScrollTimer = 0.0f;
	m_ParticleTimer = 0.0f;
	m_PlayerDisplayRect = VIVID_NEW vivid::Rect[m_JoinCount];
	m_DisplayFlag = VIVID_NEW bool[m_JoinCount];
	m_RankingFlag = VIVID_NEW bool[m_JoinCount];
	m_DisplayBackGroundRect  =VIVID_NEW vivid::Rect[m_JoinCount];
	m_DisplayBackGroundAnchor=VIVID_NEW vivid::Vector2[m_JoinCount];
	m_DisplayBackGroundScale =VIVID_NEW vivid::Vector2[m_JoinCount];
	m_DisplayBackGroundRotation = VIVID_NEW float[m_JoinCount];
	m_RankingColor = VIVID_NEW unsigned int[m_JoinCount];
	m_PlayerColor = VIVID_NEW unsigned int[m_JoinCount];
	for (int i = 0;i < m_JoinCount;i++)
	{
		m_PlayerDisplayRect[i] = vivid::Rect{i * m_player_display_width,0,i * m_player_display_width + m_player_display_width,m_player_display_height};
		m_DisplayFlag[i] = false;
		m_RankingFlag[i] = false;
		m_DisplayBackGroundRect[i] = vivid::Rect{ 0,0,m_display_background_width,m_display_background_height };
		m_DisplayBackGroundAnchor[i] = vivid::Vector2(m_display_background_width / 2, m_display_background_height / 2);
		m_DisplayBackGroundScale[i] = vivid::Vector2(0.0f, 0.0f);
		m_DisplayBackGroundRotation[i] = DEG_TO_RAD(180);
		m_RankingColor[i] = 0xffffffff;
		m_PlayerColor[i] = 0x00ffffff;
	}
	m_CrownColor = 0x00ffffff;
	m_DropFlag = true;

	// 1位(DeadCount == 0)のプレイヤー位置に王冠を表示する。
	int winner_index = 0;
	for (int i = 0; i < m_JoinCount; ++i)
	{
		if (CSceneManager::GetInstance().GetDeadCount((PLAYER_ID)i) == 0)
		{
			winner_index = i;
			break;
		}
	}
	m_CrownPosition.x = m_start_x[m_JoinCount - 2] + m_interval[m_JoinCount - 2] * winner_index+40;
	m_DisplayFlag[0] = true;
}

/*
 *	更新
 */
void
CResult::
Update(void)
{
	bool IsMainSceneChange = vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::Z) ||
		vivid::controller::Button(vivid::controller::DEVICE_ID::PLAYER1, vivid::controller::BUTTON_ID::B) ||
		vivid::controller::Button(vivid::controller::DEVICE_ID::PLAYER2, vivid::controller::BUTTON_ID::B) ||
		vivid::controller::Button(vivid::controller::DEVICE_ID::PLAYER3, vivid::controller::BUTTON_ID::B) ||
		vivid::controller::Button(vivid::controller::DEVICE_ID::PLAYER4, vivid::controller::BUTTON_ID::B) ||
		m_SwitchSceneTimer > m_swich_change_time;

	if (IsMainSceneChange)
	{
		CSceneManager::GetInstance().ChangeMainScene(MAINSCENE_ID::TITLE);
	}
	m_SwitchSceneTimer += vivid::GetDeltaTime();
	m_ScrollTimer += vivid::GetDeltaTime();
	m_ParticleTimer += vivid::GetDeltaTime();

	DisPlay();

	if (vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::Q))
		CEffectManager::GetInstance().Create(EFFECT_ID::IMPACT_FLASH, PLAYER_ID::MAX, vivid::Vector2(900.0f, 500.0f), DIRECTION::TOP, vivid::Vector2(1.0f, 1.0f), m_default_color, 0.0f);
}

/*
 *	描画
 */
void
CResult::
Draw(void)
{
	IScene::Draw();	

	vivid::DrawTexture("data\\map\\background_layer\\01_sky.png", vivid::Vector2::ZERO);


	for (int i = 0;i < m_JoinCount;i++)
	{
		vivid::DrawTexture("data\\object\\display_background.png", vivid::Vector2(m_start_x[m_JoinCount - 2] + m_interval[m_JoinCount - 2] * i, 200.0f),m_default_color,m_DisplayBackGroundRect[i],
			m_DisplayBackGroundAnchor[i],m_DisplayBackGroundScale[i],m_DisplayBackGroundRotation[i]);
		if (IsDisplayFlag()&&m_DisplayBackGroundScale[m_JoinCount-1].x>=1)
		{
			vivid::DrawTexture("data\\object\\player_display.png", vivid::Vector2(m_start_x[m_JoinCount - 2] + m_interval[m_JoinCount - 2] * i+(m_player_display_width/2), 400.0f), m_PlayerColor[i], m_PlayerDisplayRect[i]);
			vivid::DrawTexture("data\\object\\ranking_k.png", vivid::Vector2(m_start_x[m_JoinCount - 2] + m_interval[m_JoinCount - 2] * i, 200.0f), m_RankingColor[i],
				{ 200 * CSceneManager::GetInstance().GetDeadCount((PLAYER_ID)i), 0, 200 + CSceneManager::GetInstance().GetDeadCount((PLAYER_ID)i) * 200, 150 });
			vivid::DrawTexture("data\\object\\crown.png", m_CrownPosition+vivid::Vector2(0.0f,0.0f), m_CrownColor);
		}
	}

	
	
}

/*
 *	解放
 */
void
CResult::
Finalize(void)
{
	delete[] m_PlayerDisplayRect;
	m_PlayerDisplayRect = nullptr;

	CEffectManager::GetInstance().Finalize();
}

void CResult::DisPlay(void)
{

	if (m_ParticleTimer >= m_particle_interval&&IsDisplayFlag())
	{
		for (int i = 0;i < m_JoinCount;i++)
		{

			CEffectManager::GetInstance().Create(EFFECT_ID::RANKING_TWINKLE, PLAYER_ID::MAX,
				vivid::Vector2(m_start_x[m_JoinCount - 2] + m_interval[m_JoinCount - 2] * i, 200.0f) + vivid::Vector2(Utility::GetRandomInt(0, 200), Utility::GetRandomInt(0, 150))
				, DIRECTION::TOP, vivid::Vector2(1.0f, 1.0f),
				m_ranking_color[CSceneManager::GetInstance().GetDeadCount((PLAYER_ID)i)], Utility::GetRandomInt(0, 45));

		}
		CEffectManager::GetInstance().Create(EFFECT_ID::RANKING_TWINKLE, PLAYER_ID::MAX,
			m_CrownPosition + vivid::Vector2(Utility::GetRandomInt(0, m_crown_width), Utility::GetRandomInt(0, m_crown_height)),
			DIRECTION::TOP, vivid::Vector2(1.0f, 1.0f), m_ranking_color[0], Utility::GetRandomInt(0, 45));
		m_ParticleTimer = 0.0f;
	}


	float crown_alpha = (m_CrownColor & 0xff000000) >> 24;
	float max_alpha = (0xffffffff & 0xff000000) >> 24;

	if (m_DropFlag == true && IsDisplayFlag())
	{
		m_CrownPosition.y += m_drop_speed;

		crown_alpha += m_fade_speed;
		if (crown_alpha >= max_alpha)
		{
			crown_alpha = max_alpha;
			m_DropFlag = false;

		}
	}
	m_CrownColor = ((unsigned int)crown_alpha << 24) | (m_CrownColor & 0x00ffffff);

	if (IsDisplayFlag())
	{
		for (int i = 0; i < m_JoinCount; i++)
		{
			float player_alpha = (m_PlayerColor[i] & 0xff000000) >> 24;
			player_alpha += m_fade_speed;
			if (player_alpha >= max_alpha)
				player_alpha = max_alpha;
			m_PlayerColor[i] = ((unsigned int)player_alpha << 24) | (m_PlayerColor[i] & 0x00ffffff);

			float ranking_alpha = (m_RankingColor[i] & 0xff000000) >> 24;
			ranking_alpha += m_fade_speed;
			if (ranking_alpha >= max_alpha)
				ranking_alpha = max_alpha;
			m_RankingColor[i] = ((unsigned int)ranking_alpha << 24) | (m_RankingColor[i] & 0x00ffffff);
		}
	}

	for (int i = 0;i < m_JoinCount;i++)
	{
		if (m_DisplayBackGroundScale[i].x <= 1&&m_DisplayFlag[i])
		{
			m_DisplayBackGroundScale[i] += vivid::Vector2(m_enlarge_speed, m_enlarge_speed);
		}
		if (i + 1 < m_JoinCount &&
			!m_DisplayFlag[m_JoinCount - 1] &&
			m_DisplayBackGroundScale[i].x >= 1)
		{
			m_DisplayFlag[i + 1] = true;
		}
	}
}

/*!
 *  表示フラグ
 */
bool 
CResult::
IsDisplayFlag(void)
{
	for (int i = 0;i < m_JoinCount;i++)
	{
		if (!m_DisplayFlag[i])
			return false;
	}

	return true;
}

bool CResult::DrawFlag(void)
{
	/*for (int i = 0;i < m_JoinCount;i++)
	{
		if (CSceneManager::GetInstance().GetDeadCount((PLAYER_ID)i)== CSceneManager::GetInstance().GetDeadCount((PLAYER_ID)(i+1)))
			return true;
	}*/
	return false;
}
