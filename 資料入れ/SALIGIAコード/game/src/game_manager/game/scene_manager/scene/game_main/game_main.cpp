
/*!
 *  @file		game_main.cpp
 *  @brief		ゲームメイン
 *  @author     Ryusei Shimizu
 *  @date       2026/04/10
 */

#include "game_main.h"
#include "../../scene_manager.h"
#include "../../../../../utility/sound_manager/sound_manager.h"
#include "../../../game_object.h"

const float CGameMain::m_no_operation_time = 30.0f;
const int CGameMain::m_text_width = 620;
const int CGameMain::m_text_height = 160;
const int CGameMain::m_center_line_width = 1980;
const int CGameMain::m_center_line_height = 1080;
const vivid::Vector2 CGameMain::m_text_position = vivid::Vector2((vivid::GetWindowWidth() - m_text_width) / 2, (vivid::GetWindowHeight() - m_text_height) / 2);
const vivid::Vector2 CGameMain::m_text_anchor = vivid::Vector2(m_text_width / 2, m_text_height / 2);
const vivid::Vector2 CGameMain::m_text_scale = vivid::Vector2(1.0f, 1.0f);
const float CGameMain::m_fade_speed = 5.0f;
const int	CGameMain::m_load_speed = 20.0f;

/*
 *	コンストラクタ
 */
CGameMain::
CGameMain(void)
	:IScene("GameMain")
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
	CSceneManager::GetInstance().ResetBattleData();

	m_JoinCount = CSceneManager::GetInstance().GetJoinCount();
	m_GameState = GAME_STATE_ID::START;
	m_Timer = 0;
	m_text_count = 0;
	m_Count = m_JoinCount - 1;

	m_TextColor = 0xffffffff;
	m_CenterLineColor = 0xffffffff;

	m_NoOperationTimer = 0.0f;
	m_IsNoOperationFinish = false;

	m_TextRect = { 0,0,0,m_text_height };
	m_CenterLineRect={ 0,0,m_center_line_width,m_center_line_height };


	CStageManager::GetInstance().Initialize();

	CPlayerManager::GetInstance().Initialize();

	CCameraManager::GetInstance().Initialize();

	CHUDManager::GetInstance().Initialize();

}

/*
 *	更新
 */
void
CGameMain::
Update(void)
{
	namespace key = vivid::keyboard;	

	m_Timer += vivid::GetDeltaTime();

	/*bool finish = !CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER1)->IsActive() ||
		!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER2)->IsActive();*/

	bool finish = false;

	for (int i = 0; i < m_JoinCount; i++)
	{
		auto* player = CPlayerManager::GetInstance().GetPlayer((PLAYER_ID)i);

		// 脱落した瞬間に一度だけ順位を確定する。
		// DeadCount は 0=1位, 1=2位... の順位インデックスとして扱う。
		if (!player->IsActive() &&
			CSceneManager::GetInstance().GetDeadCount((PLAYER_ID)i) == -1)
		{
			CSceneManager::GetInstance().SetDeadCount((PLAYER_ID)i, m_Count);
			m_Count--;
		}
	}
	
	if (CSceneManager::GetInstance().GetJoinCount() == 4)
	{
		if (!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER1)->IsActive() &&
			!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER2)->IsActive() &&
			!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER3)->IsActive() &&
			CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER4)->IsActive())
			finish = true;

		if (!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER1)->IsActive() &&
			!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER2)->IsActive() &&
			CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER3)->IsActive() &&
			!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER4)->IsActive())
			finish = true;

		if (!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER1)->IsActive() &&
			CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER2)->IsActive() &&
			!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER3)->IsActive() &&
			!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER4)->IsActive())
			finish = true;

		if (CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER1)->IsActive() &&
			!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER2)->IsActive() &&
			!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER3)->IsActive() &&
			!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER4)->IsActive())
			finish = true;
	}

	if (CSceneManager::GetInstance().GetJoinCount() == 3)
	{
		if (!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER1)->IsActive() &&
			!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER2)->IsActive() &&
			CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER3)->IsActive())
			finish = true;

		if (!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER1)->IsActive() &&
			CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER2)->IsActive() &&
			!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER3)->IsActive())
			finish = true;

		if (CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER1)->IsActive() &&
			!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER2)->IsActive() &&
			!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER3)->IsActive())
			finish = true;
	}

	if (CSceneManager::GetInstance().GetJoinCount() == 2)
	{
		if (!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER1)->IsActive() ||
			!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER2)->IsActive())
			finish = true;
	}

	// 勝敗が決まったら、生き残ったプレイヤーを1位(0)として確定する。
	if (finish)
	{
		for (int i = 0; i < m_JoinCount; i++)
		{
			auto* player = CPlayerManager::GetInstance().GetPlayer((PLAYER_ID)i);
			if (player->IsActive() &&
				CSceneManager::GetInstance().GetDeadCount((PLAYER_ID)i) == -1)
			{
				CSceneManager::GetInstance().SetDeadCount((PLAYER_ID)i, 0);
			}
		}
	}

	// Zキーが押されたか、勝敗が決まった場合、メインシーンをRESULTに変更する
	bool IsMainSceneChange = key::Trigger(key::KEY_ID::Z) || finish;

	bool IsSubSceneChange = key::Trigger(key::KEY_ID::X) ||
		controller::Trigger(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::START) ||
		controller::Trigger(controller::DEVICE_ID::PLAYER2, controller::BUTTON_ID::START) ||
		controller::Trigger(controller::DEVICE_ID::PLAYER3, controller::BUTTON_ID::START) ||
		controller::Trigger(controller::DEVICE_ID::PLAYER4, controller::BUTTON_ID::START);
	
	float alpha = (m_CenterLineColor & 0xff000000) >> 24;
	float max_alpha = (0xffffffff & 0xff000000) >> 24;

	CHUDManager::GetInstance().Update();

	switch (m_GameState)
	{
	case GAME_STATE_ID::START:

		if (m_Timer > 1.2f&& m_TextRect.right >= m_text_width)
		{
			m_text_count = 1;
			CSoundManager::GetInstance().PlaySE(SOUND_ID::START);
			m_GameState = GAME_STATE_ID::PLAY;
			CCameraManager::GetInstance().SetCameraShake(8, vivid::Vector2(8.0f, 5.0f), vivid::Vector2(5.0f, 3.0f));

		}

		if (m_TextRect.right <= m_text_width)
		{
			m_TextRect.right += m_load_speed;
		}
		
		break;
	case GAME_STATE_ID::PLAY:

		CSoundManager::GetInstance().PlayBGM(SOUND_ID::MAIN_BGM);

		CStageManager::GetInstance().Update();

		CPlayerManager::GetInstance().Update();

		CCameraManager::GetInstance().Update();
		m_TextRect = { m_text_width,0,m_text_width * 2,m_text_height };

		alpha -= m_fade_speed;
		if (alpha < 0)
		{
			alpha = 0;
		}
		m_CenterLineColor = ((unsigned int)alpha << 24) | (m_CenterLineColor & 0x00ffffff);
		m_TextColor= ((unsigned int)alpha << 24) | (m_CenterLineColor & 0x00ffffff);

		if (CPlayerManager::GetInstance().IsAnyPlayerOperating())
		{
			m_NoOperationTimer = 0.0f;
			return;
		}

		m_NoOperationTimer += vivid::GetDeltaTime();

		if (m_NoOperationTimer >= m_no_operation_time)
		{
			m_NoOperationTimer = m_no_operation_time;
			m_IsNoOperationFinish = true;
		}

		if (IsSubSceneChange)
		{
			CSceneManager::GetInstance().ChangeSubScene(SUBSCENE_ID::PAUSE);
		}

		if (finish || m_IsNoOperationFinish)
		{
			m_text_count = 2;
			m_Timer = 0;
			CSoundManager::GetInstance().PlaySE(SOUND_ID::START);
			m_GameState = GAME_STATE_ID::FINISH;
		}

		break;
	case GAME_STATE_ID::FINISH:
		m_TextRect = { m_text_width * 2,0,m_text_width * 3,m_text_height };
		m_TextColor = 0xffffffff;
		if (m_Timer > 1)
		{
			if (IsMainSceneChange || m_IsNoOperationFinish)
			{
				CSceneManager::GetInstance().ChangeMainScene(MAINSCENE_ID::RESULT);
			}
		}
	
		break;
	case GAME_STATE_ID::MAX:
		break;
	default:
		break;
	}

	

	//// Zキーが押されたか、プレイヤー1の残機が0以下か、プレイヤー2の残機が0以下の場合、メインシーンをRESULTに変更する
	//bool IsMainSceneChange = key::Trigger(key::KEY_ID::Z) ||
	//	!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER1)->IsActive() ||
	//	!CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER2)->IsActive();

	//if (IsMainSceneChange)
	//{
	//	CSceneManager::GetInstance().ChangeMainScene(MAINSCENE_ID::RESULT);
	//}

	

#ifdef VIVID_DEBUG

	// デバッグ用のキー入力
	if (key::Trigger(key::KEY_ID::ONE))
		CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER1)->GetResourceComponent().SetCurrent(RESOURCE_ID::DESIRE, 10);
	if (key::Trigger(key::KEY_ID::TWO))
		CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER1)->GetResourceComponent().SetCurrent(RESOURCE_ID::DESIRE, 30);
	if (key::Trigger(key::KEY_ID::THREE))
		CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER1)->GetResourceComponent().SetCurrent(RESOURCE_ID::DESIRE, 50);
	if (key::Trigger(key::KEY_ID::FOUR))
		CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER1)->GetResourceComponent().SetCurrent(RESOURCE_ID::DESIRE, 80);
#endif // VIVID_DEBUG
}

/*
 *	描画
 */
void
CGameMain::
Draw(void)
{
	CStageManager::GetInstance().Draw();

	CPlayerManager::GetInstance().Draw();

	CHUDManager::GetInstance().Draw();

	vivid::DrawTexture("data\\object\\gamemain_text.png", m_text_position, m_TextColor, m_TextRect);
	switch (m_GameState)
	{
	case GAME_STATE_ID::START:
		break;
	case GAME_STATE_ID::PLAY:

		vivid::DrawTexture("data\\effect\\center_line.png", vivid::Vector2::ZERO,m_CenterLineColor);

		break;
	case GAME_STATE_ID::FINISH:

		break;
	case GAME_STATE_ID::MAX:
		break;
	default:
		break;
	}

	

#ifdef VIVID_DEBUG

	IScene::Draw();

	int text_size = 40;
	float text_x = 10.0f;
	float text_y = 50.0f;

	std::string player1_character_state_text = "p1_chara_state:";
	std::string player1_character_action_state_text = "p1_action_state:";	
	std::string player1_player_state_text = "p1_player_state:";

	switch (CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER1)->GetStateController().GetMoveState())
	{
	case MOVE_STATE::NONE:
		player1_character_state_text += "NONE";
		break;
	case MOVE_STATE::IDLE:
		player1_character_state_text += "IDLE";
		break;
	case MOVE_STATE::DASH:
		player1_character_state_text += "DASH";
		break;
	case MOVE_STATE::JUMP:
		player1_character_state_text += "JUMP";
		break;
	case MOVE_STATE::FALL:
		player1_character_state_text += "FALL";
		break;
	case MOVE_STATE::ATTACK:
		player1_character_state_text += "ATTACK";
		break;
	case MOVE_STATE::SKILL:
		player1_character_state_text += "SKILL";
		break;
	case MOVE_STATE::GUARD:
		player1_character_state_text += "GUARD";
		break;
	case MOVE_STATE::STIFFNESS:
		player1_character_state_text += "STIFFNESS";
		break;
	case MOVE_STATE::MAX:
		player1_character_state_text += "MAX";
		break;
	}

	switch (CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER1)->GetStateController().GetActionState())
	{
	case ACTION_STATE::IDLE:
		player1_character_action_state_text += "IDLE";
		break;
	case ACTION_STATE::ATTACK_NEUTRAL:
		player1_character_action_state_text += "ATTACK_NEUTRAL";
		break;
	case ACTION_STATE::ATTACK_SIDE:
		player1_character_action_state_text += "ATTACK_SIDE";
		break;
	case ACTION_STATE::ATTACK_UP:
		player1_character_action_state_text += "ATTACK_UP";
		break;
	case ACTION_STATE::SKILLX:
		player1_character_action_state_text += "SKILLX";
		break;
	case ACTION_STATE::SKILLA:
		player1_character_action_state_text += "SKILLA";
		break;
	case ACTION_STATE::GUARD:
		player1_character_action_state_text += "GUARD";
		break;
	case ACTION_STATE::STIFFNESS:
		player1_character_action_state_text += "STIFFNESS";
		break;
	case ACTION_STATE::RESPAWN:
		player1_character_action_state_text += "RESPAWN";
		break;
	}

	switch (CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER1)->GetStateController().GetDesireState())
	{
	case DESIRE_STATE::MUYOKU:
		player1_player_state_text += "MUYOKU";
		break;

	case DESIRE_STATE::NORMAL:
		player1_player_state_text += "NORMAL";
		break;
	}

	/*vivid::DrawText(text_size, player1_character_state_text, vivid::Vector2(text_x, text_y),0xff000000);
	vivid::DrawText(text_size, player1_character_action_state_text, vivid::Vector2(text_x, text_y + text_size),0xff000000);
	vivid::DrawText(text_size, player1_player_state_text, vivid::Vector2(text_x, text_y + text_size * 2),0xff000000);	

	vivid::DrawText(text_size, "p1_HP:" + std::to_string(CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER1)->GetResourceComponent().GetCurrent(RESOURCE_ID::HP)), vivid::Vector2(text_x, text_y + text_size * 4), 0xff000000);
	vivid::DrawText(text_size, "p1_LIFE:" + std::to_string(CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER1)->GetResourceComponent().GetCurrent(RESOURCE_ID::LIFE)), vivid::Vector2(text_x, text_y + text_size * 5), 0xff000000);
	vivid::DrawText(text_size, "p1_DESIRE:" + std::to_string(CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER1)->GetResourceComponent().GetCurrent(RESOURCE_ID::DESIRE)), vivid::Vector2(text_x, text_y + text_size * 6), 0xff000000);
	vivid::DrawText(text_size, "p1_GUARD:" + std::to_string(CPlayerManager::GetInstance().GetPlayer(PLAYER_ID::PLAYER1)->GetResourceComponent().GetCurrent(RESOURCE_ID::GUARD)), vivid::Vector2(text_x, text_y + text_size * 7), 0xff000000);*/

#endif // VIVID_DEBUG

}

/*
 *	解放
 */
void
CGameMain::
Finalize(void)
{
	// プレイヤーを参照しているものから先に終了
	CEffectManager::GetInstance().Finalize();

	CHUDManager::GetInstance().Finalize();

	CCameraManager::GetInstance().Finalize();

	CPlayerManager::GetInstance().Finalize();

	CStageManager::GetInstance().Finalize();
}
