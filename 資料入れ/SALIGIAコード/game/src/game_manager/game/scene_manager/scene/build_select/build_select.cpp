
/*!
 *  @file		build_select.cpp
 *  @brief		ビルドセレクト
 *  @author     Hiroto Maniwa
 *  @date       2026/04/22
 */

#include "build_select.h"
#include "../../scene_manager.h"
#include "../player_join/player_join.h"
#include "../../../../../utility/sound_manager/sound_manager.h"
#include "../../../effect_manager/effect_manager.h"
#include "../../../../../utility/utility.h"
#include "../../../player_manager/player_manager.h"


const int			CBuildSelect::m_text_size = 40;			//!< デバックモードで表示されるテキストの文字サイズ
const unsigned int	CBuildSelect::m_text_color = 0xff000000;//!< デバックモードで表示されるテキストの文字の色
const int			CBuildSelect::m_icon_width = 160;		//!< ボールの幅
const int			CBuildSelect::m_icon_height = 165;		//!< ボールの高さ
const int			CBuildSelect::m_background_width = 960;	//!< 背景の幅
const int			CBuildSelect::m_background_height = 540;	//!< 背景の高さ
const float			CBuildSelect::m_up_position = 15.0f;
const float			CBuildSelect::m_select_frame_scale = 1.1;	//!< 枠の大きさ
const unsigned int  CBuildSelect::m_default_color = 0xffffffff;
const unsigned int  CBuildSelect::m_ball_color[] =				//!< ボールの色
{
	0x88FFD700,		//!< 傲慢
	0x88C0C0C0,		//!< 強欲
	0x88800080,		//!< 色欲
	0x8800FF00,		//!< 嫉妬
	0x88FF1493,		//!< 暴食
	0x88FF0000,		//!< 憤怒
	0x8800BFFF,		//!< 怠惰
};
const unsigned int CBuildSelect::m_select_frame_color[] =		//!< 枠の色
{
	0xff0000ff,		//!< 青
	0xffff0000,		//!< 赤
	0xff00ff00,		//!< 緑
	0xffffff00,		//!< 黄色
};
const vivid::Vector2 CBuildSelect::m_icon_position = vivid::Vector2(m_icon_width/2-10 , 600.0f);	//!< ボールの位置
const vivid::Vector2 CBuildSelect::m_text_position[] =
{
	vivid::Vector2(0.0f,0.0f),
	vivid::Vector2(vivid::GetWindowWidth() - 70.0f,0.0f),
	vivid::Vector2(0.0f,vivid::GetWindowHeight() - 16.0f),
	vivid::Vector2(vivid::GetWindowWidth() - 70.0f,0.0f)
};
const vivid::Vector2 CBuildSelect::m_current_saligia_position[]
{
	vivid::Vector2(200.0f , 200.0f),
	vivid::Vector2(200.0f + vivid::GetWindowWidth() / 4, 200.0f),
	vivid::Vector2(200.0f + (vivid::GetWindowWidth() / 4) * 2, 200.0f),
	vivid::Vector2(200.0f + (vivid::GetWindowWidth() / 4) * 3, 200.0f)
};
const int			 CBuildSelect::m_character_text_size = 16;						//!< 表示されるテキストのサイズ

const std::string CBuildSelect::m_player_name[4] =		//!< 表示されるプレイヤーのテキスト
{
	"Player1",
	"Player2",
	"Player3",
	"Player4",
};

const std::string CBuildSelect::m_character_name[7] =	//!< 表示されるテキスト
{
	"SUPERBIA",		//!< 傲慢
	"AVARITIA",		//!< 強欲
	"LUXURIA",		//!< 色欲
	"INVIDIA",		//!< 嫉妬
	"GULA",			//!< 暴食
	"IRA",			//!< 憤怒
	"ACEDIA"		//!< 怠惰
};
const vivid::Rect CBuildSelect::m_background_rect = { 0,0,m_background_width ,m_background_height };
const int	CBuildSelect::m_max_select = 2;
const int	CBuildSelect::m_width = 180;
const int	CBuildSelect::m_height = 90;
const int	CBuildSelect::m_frame_width = 160;
const int	CBuildSelect::m_frame_height = 160;
const int	CBuildSelect::m_select_hand_width = 60;
const int   CBuildSelect::m_select_hand_height = 85;
const int   CBuildSelect::m_player_display_width = 200;
const int   CBuildSelect::m_player_display_height = 170;
const vivid::Rect	CBuildSelect::m_player_display_rect[] =
{
	vivid::Rect{0,0,m_player_display_width,m_player_display_height},
	vivid::Rect{m_player_display_width,0,m_player_display_width * 2,m_player_display_height},
	vivid::Rect{m_player_display_width * 2,0,m_player_display_width * 3,m_player_display_height},
	vivid::Rect{m_player_display_width * 3,0,m_player_display_width * 4,m_player_display_height},

};

const float CBuildSelect::m_interval = 0.05f;
const int CBuildSelect::m_arm_width = 70;
const int CBuildSelect::m_arm_height = 140;
const vivid::Rect CBuildSelect::m_arm_rect[] =
{
	vivid::Rect{0,0,m_arm_width,m_arm_height},
	vivid::Rect{m_arm_width,0,m_arm_width*2,m_arm_height},
	vivid::Rect{m_arm_width*2,0,m_arm_width * 3,m_arm_height},
	vivid::Rect{m_arm_width*3,0,m_arm_width * 4,m_arm_height},
	vivid::Rect{m_arm_width * 4,0,m_arm_width * 5,m_arm_height},
	vivid::Rect{m_arm_width * 5,0,m_arm_width * 6,m_arm_height},
	vivid::Rect{m_arm_width * 6,0,m_arm_width * 7,m_arm_height},
};

const float CBuildSelect::m_up_down_interval = 1.5f;
const float CBuildSelect::m_up_down_position = 0.3f;
const int CBuildSelect::m_player_width = 70;
const int CBuildSelect::m_player_height = 140;


/*
 *	コンストラクタ
 */
CBuildSelect::
CBuildSelect(void)
	:IScene("CharacterSelect")
	, m_Position({ 0.0f,0.0f })
	, m_Rect({ 0, 0, 0, 0 })
	, m_JoinCount(0)
	, m_ReadyFlag(false)
	, m_JoinFlag(false)
	, m_Timer(0.0f)
{
}

/*
 *	デストラクタ
 */
CBuildSelect::
~CBuildSelect(void)
{
}

/*
 *	初期化
 */
void
CBuildSelect::
Initialize(void)
{
	CSceneManager::GetInstance().ClearBuildData();

	//m_JoinCount = CSceneManager::GetInstance().GetJoinCount();
	m_JoinCount = 2;

	m_ReadyFlag = VIVID_NEW bool[m_JoinCount];
	m_JoinFlag = VIVID_NEW bool[(int)PLAYER_ID::MAX];

	for (int i = 0;i < (int)PLAYER_ID::MAX;i++)
	{
		m_JoinFlag[i] = false;
		
	}

	for (int i = 0;i < m_JoinCount;i++)
	{
		for (int j = 0;j < m_max_select;j++)
		{
			m_SelectSaligia[i] = 0;
			m_SelectColor[i][j] = 0xff808080;
			m_SaligiaId[i][j] = SALIGIA_ID::NONE;
			m_SelectWardRect[i][j] = { 0,0,0,0 };
			m_SelectIconRect[i][j] = { 0,0,0,0 };
			m_SelectArmRect[i][j] = { 0,0,0,0 };
			
		}
		m_ReadyFlag[i] = false;
		m_JoinFlag[i] = true;
		m_SelectCount[i] = 0;
	}

	for (int i = 0;i < (int)SALIGIA_ID::MAX;i++)
	{
		CEffectManager::GetInstance().Create(EFFECT_ID::SALIGIA_AURA, PLAYER_ID::MAX,
			m_icon_position+vivid::Vector2((i * 272)-15, 50.0f), DIRECTION::TOP, vivid::Vector2(1.0f, 1.0f), m_ball_color[i], 0.0f);
		m_IconPosition[i] = m_icon_position + vivid::Vector2(i * 272-10, 0.0f);
		m_IconRect[i] = {i * m_icon_width,0,m_icon_width + (i * m_icon_width),m_icon_height};
		m_SelectFlag[i] = false;
		m_WardRect[i] = { i * m_width,0,i * m_width + m_width,m_height };
		m_SaligiaArmPosition[i] = m_icon_position + vivid::Vector2(i * 272 + 25, +200.0f);
	}
	m_Timer = 0.0f;
	m_UpDownTimer = 0.0f;
	m_UpDownFlag = false;
}

/*
 *	更新
 */
void
CBuildSelect::
Update(void)
{
	bool IsMainSceneChange = vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::Z) ||
		vivid::controller::Button(vivid::controller::DEVICE_ID::PLAYER1, vivid::controller::BUTTON_ID::START) ||
		vivid::controller::Button(vivid::controller::DEVICE_ID::PLAYER2, vivid::controller::BUTTON_ID::START)||
		vivid::controller::Button(vivid::controller::DEVICE_ID::PLAYER3, vivid::controller::BUTTON_ID::START)||
		vivid::controller::Button(vivid::controller::DEVICE_ID::PLAYER4, vivid::controller::BUTTON_ID::START);

	if (IsMainSceneChange && IsAllReady())
	{
		for (int i = 0;i < m_JoinCount;i++)
		{
			BuildData new_data;
			new_data.Primary = m_SaligiaId[i][0];
			new_data.Secondary = m_SaligiaId[i][1];
			CSceneManager::GetInstance().SetBuildData(new_data);
		}
		CSceneManager::GetInstance().ChangeMainScene(MAINSCENE_ID::GAME_MAIN);
	}

	Select();

	if (vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::Q))
	{
		
		vivid::effekseer::StartEffect("data\\attack.efk", vivid::Vector2(500.0f, 500.0f));
	}

	m_Timer += vivid::GetDeltaTime();
	m_UpDownTimer += vivid::GetDeltaTime();

	if (m_Timer >= m_interval)
	{
		for (int i = 0;i < (int)SALIGIA_ID::MAX;i++)
		{
			CEffectManager::GetInstance().Create(EFFECT_ID::AURA_PARTICLE, PLAYER_ID::MAX, 
				vivid::Vector2(Utility::GetRandomInt(0,268)+i*268-10, Utility::GetRandomInt(0, 540)+500)
				, DIRECTION::TOP, vivid::Vector2(0.5f, 0.5f), m_ball_color[i], 0.0f);
		}
		m_Timer = 0.0f;
	}

	if (m_UpDownFlag)
	{
		for (int i = 0;i < (int)SALIGIA_ID::MAX;i++)
		{
			m_SaligiaArmPosition[i].y += m_up_down_position;
		}
	}

	if (!m_UpDownFlag)
	{
		for (int i = 0;i < (int)SALIGIA_ID::MAX;i++)
		{
			m_SaligiaArmPosition[i].y -= m_up_down_position;
		}
	}

	if (m_UpDownTimer > m_up_down_interval)
	{
		m_UpDownTimer = 0.0f;
		m_UpDownFlag = !m_UpDownFlag;
	}
	
}

/*
 *	描画
 */
void
CBuildSelect::
Draw(void)
{
	IScene::Draw();

	for (int i = 0;i < (int)PLAYER_ID::MAX;i++)
	{
		vivid::DrawTexture("data\\object\\build_background_white.png", vivid::Vector2(i * 480, 0.0f), 0xffffffff);
		vivid::DrawTexture("data\\object\\build_background.png", vivid::Vector2(i * 480, 0.0f),m_select_frame_color[i]);
		vivid::DrawTexture("data\\object\\player_display.png", vivid::Vector2(i * 480, 0.0f), 0xffffffff, m_player_display_rect[i],
			vivid::Vector2(0.0f,0.0f),vivid::Vector2(0.5f,0.5f));
		if (m_JoinFlag[i]==false)
		{
			vivid::DrawTexture("data\\object\\no_player.png", vivid::Vector2(i * 480, 0.0f),0x88ffffff);
		}
	}
	
	vivid::DrawTexture("data\\object\\saligia_background.png", vivid::Vector2(0.0f, 500.0f),0x88ffffff);
	vivid::DrawTexture("data\\object\\kettei.png", vivid::Vector2(730.0f, 1020.0f));
	vivid::DrawTexture("data\\object\\kyancel.png", vivid::Vector2(960.0f, 1020.0f));

	for (int i = 0;i < m_JoinCount;i++)
	{
		//vivid::DrawTexture("data\\player\\player\\new_player.png", vivid::Vector2(((480-m_player_width)/2)+(i*480), 20.0f));
		vivid::DrawTexture("data\\player\\player\\texture\\body.png", vivid::Vector2(((480 - m_player_width) / 2) + (i * 480), 20.0f));
		vivid::DrawTexture("data\\player\\player\\texture\\body_light.png", vivid::Vector2(((480 - m_player_width) / 2) + (i * 480), 20.0f),
			m_select_frame_color[i]);
		vivid::DrawTexture("data\\player\\player\\texture\\head.png", vivid::Vector2(((480 - m_player_width) / 2) + (i * 480), 20.0f));
		vivid::DrawTexture("data\\player\\player\\texture\\head_light.png", vivid::Vector2(((480 - m_player_width) / 2) + (i * 480), 20.0f),
			m_select_frame_color[i]);
		for (int j = 0;j < m_max_select;j++)
		{
			vivid::DrawTexture("data\\object\\frame.png", m_current_saligia_position[i] + vivid::Vector2((j * 160)-100, 0.0f), m_SelectColor[i][j]);
			//vivid::DrawTexture("data\\object\\saligia_ward.png", m_current_saligia_position[i] + vivid::Vector2((j*m_width)-150, 200.0f),0xffffffff,m_SelectWardRect[i][j]);
			vivid::DrawTexture("data\\object\\saligia_color_icon.png", m_current_saligia_position[i] + vivid::Vector2((j * 160) - 100, 0.0f), 0xffffffff, m_SelectIconRect[i][j]);
			vivid::DrawTexture("data\\object\\saligia_arms.png", vivid::Vector2(((480 - m_player_width) / 2-70) + (i * 480)+j*160, 20.0f), 0xffffffff, m_SelectArmRect[i][j]);
		}
	}

	// saligia描画
	for (int i = 0;i < (int)SALIGIA_ID::MAX;i++)
	{
		vivid::DrawTexture("data\\object\\saligia_color_icon.png", m_IconPosition[i], m_default_color, m_IconRect[i]);
		vivid::DrawTexture("data\\object\\saligia_ward.png", m_icon_position + vivid::Vector2(i * 272-80, -90.0f),0xffffffff,m_WardRect[i],vivid::Vector2(m_width/2,m_height/2),vivid::Vector2(0.7,0.7));
		vivid::DrawTexture("data\\object\\saligia_arms.png", m_SaligiaArmPosition[i], 0xffffffff, m_arm_rect[i]);
		//vivid::DrawText(m_character_text_size, m_character_name[i], m_icon_position + vivid::Vector2(100.0f * i, 50.0f), m_text_color);
	}

	// プレイヤーの数だけ枠を描画
	for (int i = 0;i < m_JoinCount;i++)
	{
		//vivid::DrawText(m_text_size, m_player_name[i], m_text_position[i]);
		m_framePosition[i] = m_icon_position + vivid::Vector2(m_SelectSaligia[i] * 272.0f, 0.0f);
		vivid::DrawTexture("data\\object\\ball.png", m_framePosition[i], m_select_frame_color[i], m_Rect, vivid::Vector2(16.0f, 16.0f), vivid::Vector2(m_select_frame_scale, m_select_frame_scale));
		vivid::DrawTexture("data\\object\\select_hand.png", m_framePosition[i] - vivid::Vector2(-i * 60, 50.0f), m_select_frame_color[i]);
		
		//vivid::DrawText(m_text_size, m_player_name[i], m_framePosition[i] - vivid::Vector2(0.0f, 50.0f), m_select_frame_color[i]);
		
	}

	

#ifdef VIVID_DEBUG

	// 画面左上にゲームモードの名前を表示
	vivid::DrawText(m_text_size, m_GameModeName, m_Position, m_text_color);

#endif // VIVID_DEBUG
}

/*
 *	解放
 */
void
CBuildSelect::
Finalize(void)
{
	delete[] m_ReadyFlag;
	m_ReadyFlag = nullptr;

	delete[] m_JoinFlag;
	m_JoinFlag = nullptr;

	CEffectManager::GetInstance().Finalize();
}

/*
 *	選択
 */
void 
CBuildSelect::
Select(void)
{
	for (int i = 0;i < vivid::controller::GetConnectCount();i++)
	{
		vivid::controller::DEVICE_ID device_id = CSceneManager::GetInstance().GetController((PLAYER_ID)i);

		if (vivid::controller::TriggerAnalogStickLeft(device_id, vivid::controller::STICK_DIRECTION::RIGHT)
			&& m_SelectSaligia[i] < (int)SALIGIA_ID::ACEDIA)
		{
			CSoundManager::GetInstance().PlaySE(SOUND_ID::SELECT);
			m_SelectSaligia[i]++;
		}

		if (vivid::controller::TriggerAnalogStickLeft(device_id, vivid::controller::STICK_DIRECTION::LEFT)
			&& m_SelectSaligia[i] > 0)
		{
			CSoundManager::GetInstance().PlaySE(SOUND_ID::SELECT);
			m_SelectSaligia[i]--;
		}

		if (vivid::controller::Trigger(device_id, vivid::controller::BUTTON_ID::B))
		{
			
			if (m_SelectCount[i] == 1)
			{
				m_SelectColor[i][1] = m_ball_color[m_SelectSaligia[i]];
				m_SaligiaId[i][1] = (SALIGIA_ID)m_SelectSaligia[i];
				m_SelectWardRect[i][1].bottom = m_height;
				m_SelectWardRect[i][1].right = m_SelectSaligia[i] * m_width + m_width;
				m_SelectWardRect[i][1].left = m_SelectSaligia[i] * m_width;
				m_SelectIconRect[i][1].bottom = m_icon_height;
				m_SelectIconRect[i][1].right = m_SelectSaligia[i] * m_icon_width + m_icon_width;
				m_SelectIconRect[i][1].left = m_SelectSaligia[i] * m_icon_width;
				m_SelectArmRect[i][1].bottom = m_arm_height;
				m_SelectArmRect[i][1].right = m_SelectSaligia[i] * m_arm_width + m_arm_width;
				m_SelectArmRect[i][1].left = m_SelectSaligia[i] * m_arm_width;
				m_SelectCount[i]++;
				m_ReadyFlag[i] = true;
				CSoundManager::GetInstance().PlaySE(SOUND_ID::BUILD);
				CEffectManager::GetInstance().Create(EFFECT_ID::FRAME_FLASH, PLAYER_ID::MAX, m_current_saligia_position[i] + vivid::Vector2((160) - 100, 0.0f)
					, DIRECTION::TOP, vivid::Vector2(1.0f, 1.0f), m_ball_color[m_SelectSaligia[i]], 0.0f);
			}

			if (m_SelectCount[i] == 0)
			{
				m_SelectColor[i][0] = m_ball_color[m_SelectSaligia[i]];
				m_SaligiaId[i][0] = (SALIGIA_ID)m_SelectSaligia[i];
				m_SelectWardRect[i][0].bottom = m_height;
				m_SelectWardRect[i][0].right = m_SelectSaligia[i] * m_width + m_width;
				m_SelectWardRect[i][0].left = m_SelectSaligia[i] * m_width;
				m_SelectIconRect[i][0].bottom = m_icon_height;
				m_SelectIconRect[i][0].right = m_SelectSaligia[i] * m_icon_width + m_icon_width;
				m_SelectIconRect[i][0].left = m_SelectSaligia[i] * m_icon_width;
				m_SelectArmRect[i][0].bottom = m_arm_height;
				m_SelectArmRect[i][0].right = m_SelectSaligia[i] * m_arm_width + m_arm_width;
				m_SelectArmRect[i][0].left = m_SelectSaligia[i] * m_arm_width;
				m_SelectCount[i]++;
				CSoundManager::GetInstance().PlaySE(SOUND_ID::BUILD);
				CEffectManager::GetInstance().Create(EFFECT_ID::FRAME_FLASH, PLAYER_ID::MAX, m_current_saligia_position[i]-vivid::Vector2(80,0.0f)
					, DIRECTION::TOP, vivid::Vector2(1.0f, 1.0f), m_ball_color[m_SelectSaligia[i]], 0.0f);
			}	
		}

		if (m_SelectCount[i] == 1 && vivid::controller::Trigger(device_id, vivid::controller::BUTTON_ID::A))
		{
			m_SelectColor[i][0] = 0xff808080;
			m_SelectWardRect[i][0].bottom = 0;
			m_SelectIconRect[i][0].bottom = 0;
			m_SelectArmRect[i][0].bottom = 0;
			m_SelectCount[i]--;
			CSoundManager::GetInstance().PlaySE(SOUND_ID::CANCEL);
		}
		if (m_SelectCount[i] == 2 && vivid::controller::Trigger(device_id, vivid::controller::BUTTON_ID::A))
		{
			m_SelectColor[i][1] = 0xff808080;
			m_SelectWardRect[i][1].bottom = 0;
			m_SelectIconRect[i][1].bottom = 0;
			m_SelectArmRect[i][1].bottom = 0;
			m_SelectCount[i]--;
			m_ReadyFlag[i] = false;
			CSoundManager::GetInstance().PlaySE(SOUND_ID::CANCEL);
		}

		for (int j = 0;j < (int)SALIGIA_ID::MAX;j++)
		{
			if (m_SelectSaligia[i] == j)
				m_IconPosition[j].y = m_icon_position.y - m_up_position;
			if (m_SelectSaligia[i] != j)
				m_IconPosition[j].y = m_icon_position.y;
		}
	}
	
}

/*
 *	キャラ選択フラグ
 */
bool CBuildSelect::IsAllReady(void)
{
	for (int i = 0;i < m_JoinCount;i++)
	{
		if (!m_ReadyFlag[i])
			return false;
	}

	return true;
}

