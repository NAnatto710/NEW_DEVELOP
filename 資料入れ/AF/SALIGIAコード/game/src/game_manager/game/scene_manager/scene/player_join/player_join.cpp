
/*!
 *  @file		player_join.cpp
 *  @brief		プレイヤー参加
 *  @author     Hiroto Maniwa
 *  @date       2026/06/30
 */

#include "player_join.h"
#include "../../scene_manager.h"
#include "../../../../../utility/sound_manager/sound_manager.h"
#include "../../../effect_manager/effect_manager.h"
#include "../../../../../utility/utility.h"

const int CPlayerJoin::m_text_size = 40;
const int CPlayerJoin::m_max_count = 3;
const int CPlayerJoin::m_width = 900;
const int CPlayerJoin::m_height = 200;
const int CPlayerJoin::m_frame_size=400;
const int CPlayerJoin::m_player_display_width=200;
const int CPlayerJoin::m_player_display_height=170;

const float CPlayerJoin::m_change_speed = 50.0f;
const float CPlayerJoin::m_frame_distance = ((float)m_frame_size + 50.0f);
const float CPlayerJoin::m_max_scale=1.0f;
const float CPlayerJoin::m_min_scale=0.1;
const float CPlayerJoin::m_enlarge_speed = 0.05f;
const float CPlayerJoin::m_interval = 7.0f;

const std::string CPlayerJoin::m_text[] =
{
	"1P",
	"2P",
	"3P",
	"4P"
};
const vivid::Vector2 CPlayerJoin::m_text_position[] =
{
	vivid::Vector2(200.0f,300.0f),
	vivid::Vector2(200.0f,350.0f),
	vivid::Vector2(200.0f,400.0f),
	vivid::Vector2(200.0f,450.0f)
};

const vivid::Vector2 CPlayerJoin::m_position = vivid::Vector2((vivid::GetWindowWidth() - m_width) / 2, 100.0f);
const vivid::Vector2 CPlayerJoin::m_anchor = vivid::Vector2(m_width / 2, m_height / 2);
const vivid::Vector2 CPlayerJoin::m_scale = vivid::Vector2(1.0f, 1.0f);
const vivid::Vector2 CPlayerJoin::m_frame_position = vivid::Vector2(75.0f, 400.0f);
const vivid::Vector2 CPlayerJoin::m_player_display_position = vivid::Vector2(m_frame_position.x + ((m_frame_size + m_player_display_width) / 2), 420.0f);
const vivid::Rect CPlayerJoin::m_player_display_rect[] =
{
	vivid::Rect{0,0,m_player_display_width,m_player_display_height},
	vivid::Rect{m_player_display_width,0,m_player_display_width*2,m_player_display_height},
	vivid::Rect{m_player_display_width*2,0,m_player_display_width*3,m_player_display_height},
	vivid::Rect{m_player_display_width*3,0,m_player_display_width*4,m_player_display_height}
};
const int CPlayerJoin::m_ready_width = 350;
const int CPlayerJoin::m_ready_height = 65;


CPlayerJoin::CPlayerJoin(void)
	: IScene("PlayerJoin")
	,m_JoinCount(0)
{
}

void CPlayerJoin::Initialize(void)
{
	m_JoinCount = 0;
	for (int i = 0;i < (int)PLAYER_ID::MAX;i++)
	{
		m_JoinController[i] = (int)PLAYER_ID::MAX;
		m_ReadyFlag[i] = false;
		m_ReadyRect[i] = { 0,0,m_ready_width,m_ready_height };
	}
	for (int i = 0;i < vivid::controller::GetConnectCount();i++)
	{
		m_JoinColor[i] = 0xffffffff;
		m_JoinFlag[i] = false;
		m_JoinText[i] = " NoPlayer ";
	}

	m_Rect = { 0,0,m_width,m_height };
	m_Scale = vivid::Vector2(m_min_scale, m_min_scale);

	m_Timer = 0.0f;
	CSoundManager::GetInstance().PlayBGM(SOUND_ID::PLAYER_JOIN_BGM);
}

void CPlayerJoin::Update(void)
{
	/*bool IsMainSceneChange = vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::Z) ||
		vivid::controller::Button(vivid::controller::DEVICE_ID::PLAYER1, vivid::controller::BUTTON_ID::START) ||
		vivid::controller::Button(vivid::controller::DEVICE_ID::PLAYER2, vivid::controller::BUTTON_ID::START) ||
		vivid::controller::Button(vivid::controller::DEVICE_ID::PLAYER3, vivid::controller::BUTTON_ID::START) ||
		vivid::controller::Button(vivid::controller::DEVICE_ID::PLAYER4, vivid::controller::BUTTON_ID::START);*/

	//bool IsMainSceneChange = IsAllReady();

	/*if (IsMainSceneChange&&m_JoinCount>1)
	{
		CSceneManager::GetInstance().SetGamePlayer(m_JoinCount);
		CSceneManager::GetInstance().ChangeMainScene(MAINSCENE_ID::BUILD_SELECT);
	}*/

	if (IsAllReady() && m_JoinCount > 1)
	{
		CSceneManager::GetInstance().SetGamePlayer(m_JoinCount);
		CSceneManager::GetInstance().ChangeMainScene(MAINSCENE_ID::BUILD_SELECT);
	}

	if (m_Scale.x < m_max_scale)
	{
		m_Scale += vivid::Vector2(m_enlarge_speed, m_enlarge_speed);
	}
	
	/*if (m_Rect.left < m_JoinCount * m_width && m_JoinCount < 4)
	{
		m_Rect.left += m_change_speed;
		m_Rect.right += m_change_speed;
	}
	if (m_Rect.right > (m_JoinCount * m_width)+m_width && m_JoinCount > 2)
	{
		m_Rect.left -= m_change_speed;
		m_Rect.right -= m_change_speed;
	}*/

	int player_count = m_JoinCount - 1;
	m_Rect = { player_count*m_width,0,(player_count* m_width)+m_width,m_height };

	m_Timer += vivid::GetDeltaTime();
	if (m_Timer>m_interval)
	{
		CEffectManager::GetInstance().Create(EFFECT_ID::ICON_DROP, PLAYER_ID::MAX, vivid::Vector2(Utility::GetRandomInt(0, vivid::GetWindowWidth() - 160), -200.0f),
			DIRECTION::TOP, vivid::Vector2(1.0f, 1.0f), 0xccffffff, 0.0f);
		m_Timer = 0.0f;
	}
		

	if (vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::S) && m_JoinCount!=0)
	{
		m_JoinCount--;
		
	}

	if (vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::A) && m_JoinCount < 4)
	{
		m_JoinCount++;

	}
		
	Join();

	Ready();
}

void CPlayerJoin::Draw(void)
{
#ifdef _DEBUG
	/*for (int i = 0;i < (int)PLAYER_ID::MAX;i++)
	{
		vivid::DrawTexture("data\\object\\ball.png", m_text_position[i] + vivid::Vector2(-40.0f, 0.0f), m_JoinColor[i]);
		vivid::DrawText(m_text_size, m_text[i], m_text_position[i]);
		vivid::DrawText(m_text_size, m_JoinText[i] + std::to_string(m_JoinController[i]), m_text_position[i] + vivid::Vector2(50.0f, 0.0f));
	}*/
#endif
	vivid::DrawTexture("data\\map\\background_layer\\background2.png", vivid::Vector2::ZERO);
	vivid::DrawTexture("data\\object\\player_count.png", m_position , 0xffffffff, m_Rect, m_anchor, m_scale);
	vivid::DrawTexture("data\\object\\l_r.png", vivid::Vector2(((vivid::GetWindowWidth() - 1500) / 2), 300.0f));
	for (int i = 0;i < (int)PLAYER_ID::MAX;i++)
	{
		vivid::DrawTexture("data\\object\\player_join_frame.png", m_frame_position+vivid::Vector2(m_frame_distance * i, 0.0f),0x88ffffff,
			vivid::Rect{0,0,m_frame_size,m_frame_size},vivid::Vector2(m_frame_size/2,m_frame_size/2),m_Scale);
		if (m_JoinFlag[i]==true)
		{
			vivid::DrawTexture("data\\object\\player_display.png", m_frame_position + vivid::Vector2(m_frame_distance * i +( (400-m_player_display_width) / 2), 0.0f), 0xffffffff, m_player_display_rect[i]);
			vivid::DrawTexture("data\\object\\conected.png", m_frame_position + vivid::Vector2(m_frame_distance * i+((400 - 300) / 2), 170.0f));
			vivid::DrawTexture("data\\object\\ready.png", m_frame_position + vivid::Vector2(m_frame_distance * i+ ((400 - m_ready_width) / 2), 250.0f), 0xffffffff, m_ReadyRect[i]);
			
		}

	}
	
}

void CPlayerJoin::Finalize(void)
{
}

void CPlayerJoin::Join(void) {
	namespace controller = vivid::controller;
	namespace keyboard = vivid::keyboard;

	for (int i = 0;i < vivid::controller::GetConnectCount();i++) {
		for (int j = 0;j < (int)PLAYER_ID::MAX;j++) {/*if ((vivid::controller::Button((vivid::controller::DEVICE_ID)i, vivid::controller::BUTTON_ID::RIGHT_SHOULDER) && 				vivid::controller::Trigger((vivid::controller::DEVICE_ID)i, vivid::controller::BUTTON_ID::LEFT_SHOULDER)|| 				(vivid::controller::Trigger((vivid::controller::DEVICE_ID)i, vivid::controller::BUTTON_ID::RIGHT_SHOULDER) &&					vivid::controller::Button((vivid::controller::DEVICE_ID)i, vivid::controller::BUTTON_ID::LEFT_SHOULDER))				&& m_JoinFlag[j] == false))			{				if ([this](int device_id) ->bool {					for (int i = 0;i < (int)PLAYER_ID::MAX;i++) {						if (m_JoinController[i] == device_id) {							return true;						}					}					return false;					}(i))break;				m_JoinFlag[j] = true;				m_JoinController[j] = i;				CSoundManager::GetInstance().PlaySE(SOUND_ID::JOIN);				m_JoinCount++;				vivid::controller::StartVibration((vivid::controller::DEVICE_ID)i, 300, 0.5);				CSceneManager::GetInstance().SetController((PLAYER_ID)j, (vivid::controller::DEVICE_ID)i);				break;							}*/const auto device_id = static_cast<vivid::controller::DEVICE_ID>(i);

		const bool join_input =
			(vivid::controller::Button(device_id, vivid::controller::BUTTON_ID::RIGHT_SHOULDER) &&
				vivid::controller::Trigger(device_id, vivid::controller::BUTTON_ID::LEFT_SHOULDER)) ||
			(vivid::controller::Trigger(device_id, vivid::controller::BUTTON_ID::RIGHT_SHOULDER) &&
				vivid::controller::Button(device_id, vivid::controller::BUTTON_ID::LEFT_SHOULDER));

		if (join_input && m_JoinFlag[j] == false) {
			bool already_joined = false;

			for (int k = 0; k < static_cast<int>(PLAYER_ID::MAX); ++k) {
				if (m_JoinController[k] == i) {
					already_joined = true;
					break;
				}
			}if (already_joined)break;

			m_JoinFlag[j] = true;
			m_JoinController[j] = i;

			CSoundManager::GetInstance().PlaySE(SOUND_ID::JOIN);

			++m_JoinCount;

			vivid::controller::StartVibration(device_id, 300, 0.5f);

			CSceneManager::GetInstance().SetController(static_cast<PLAYER_ID>(j), device_id);

			break;
		}if (vivid::controller::Trigger((vivid::controller::DEVICE_ID)i, vivid::controller::BUTTON_ID::A) && m_JoinController[j] == i) {
			m_JoinFlag[j] = false;
			m_JoinController[j] = (int)PLAYER_ID::MAX;
			m_JoinCount--;
			CSoundManager::GetInstance().PlaySE(SOUND_ID::CANCEL);
			break;

		}
		}
	}for (int i = 0;i < vivid::controller::GetConnectCount();i++) {
		if (m_JoinFlag[i] == true) {
			m_JoinColor[i] = 0xff7fff00;
			m_JoinText[i] = " Connected ";

		}
		else {
			m_JoinColor[i] = 0xffffffff;
			m_JoinText[i] = " Not Connected ";
			m_ReadyFlag[i] = false;
		}
	}
}


void CPlayerJoin::Ready(void)
{
	for (int i = 0;i < vivid::controller::GetConnectCount();i++)
	{
		vivid::controller::DEVICE_ID device_id = CSceneManager::GetInstance().GetController((PLAYER_ID)i);

		if (vivid::controller::Trigger(device_id, vivid::controller::BUTTON_ID::B))
		{
			m_ReadyFlag[i] = true;
			CSoundManager::GetInstance().PlaySE(SOUND_ID::OK);
		}

		if (m_ReadyFlag[i])
		{
			m_ReadyRect[i] = { m_ready_width,0,m_ready_width * 2,m_ready_height };
		}

		if (!m_ReadyFlag[i])
		{
			m_ReadyRect[i] = { 0,0,m_ready_width,m_ready_height };
		}
	}

	
}

bool CPlayerJoin::IsAllReady(void)
{
	for (int i = 0;i < m_JoinCount;i++)
	{
		if (!m_ReadyFlag[i])
			return false;
	}

	return true;
}
