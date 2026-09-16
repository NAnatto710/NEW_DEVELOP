
/*!
 *  @file       rush_cooltime.cpp
 *  @brief      突進クールタイム表示
 *  @author     Ryusei Shimizu
 *  @date       2026/02/23
 */

#include "rush_cooltime.h"
#include "../../../character_manager/character_manager.h"
#include "../../../character_manager/character/player/player.h"

const int               CRushCoolTime::m_width				= 150;																	//!< 幅
const int               CRushCoolTime::m_height				= 150;																	//!< 高さ
const vivid::Vector2    CRushCoolTime::m_position			= vivid::Vector2( 10.0f, vivid::WINDOW_HEIGHT - m_height - 10 );		//!< 位置
const vivid::Rect		CRushCoolTime::m_frame_rect			= { 0,0,m_width ,m_height };											//!< 読み込み範囲
const vivid::Rect		CRushCoolTime::m_cool_time_rect		= { 0,m_height, m_width ,m_height * 2 };									//!< クールタイムの読み込み範囲
const std::string		CRushCoolTime::m_texture_path		= "data\\hud\\hp_gauge.png";											//!< テクスチャファイルパス

/*
 *  コンストラクタ
 */
CRushCoolTime::
CRushCoolTime(void)
{
}

/*
 *  デストラクタ
 */
CRushCoolTime::
~CRushCoolTime(void)
{
}

/*
 *  初期化
 */
void
CRushCoolTime::
Initialize(void)
{
	CCharacterManager& cm = CCharacterManager::GetInstance();
	CPlayer* player = (CPlayer*)cm.GetPlayer();

	m_Rect = m_cool_time_rect;
}

/*
 *  更新
 */
void
CRushCoolTime::
Update(void)
{
	CCharacterManager& cm = CCharacterManager::GetInstance();
	CPlayer* player = (CPlayer*)cm.GetPlayer();

	m_CoolTime = player->GetPlayerUpgradeStatus(UPGRADE_STATUS_ID::RUSH_COOLTIME);
	m_CoolTimer = player->GetUpgradeStatus(UPGRADE_STATUS_ID::RUSH_COOLTIME);

	float rate = 0.0f;

	rate = (((float)m_CoolTime - (float)m_CoolTimer) / (float)m_CoolTime);

	// 割合をもとに画像の幅(目標値)を計算
	int cool_time = (int)(rate * (float)m_width);

	// 目標値を下回るまでRectのrightを減らす
	if (m_Rect.right > cool_time)
	{
		m_Rect.right -= 5;
	}

	// 目標値を上回るまでRectのrightを増やす
	if (m_Rect.right < cool_time)
	{
		m_Rect.right += 5;
	}
}

/*
 *  描画
 */
void
CRushCoolTime::
Draw(void)
{
	vivid::DrawTexture(m_texture_path, m_position, 0xffffffff, m_Rect);
	vivid::DrawTexture(m_texture_path, m_position, 0xffffffff, m_frame_rect);
}

/*
 *  解放
 */
void
CRushCoolTime::
Finalize(void)
{
}