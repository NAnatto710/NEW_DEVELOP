
/*!
 *  @file       fullness_gauge.cpp
 *  @brief      満腹度ゲージ
 *  @author     Ryusei Shimizu
 *  @date       2026/02/03
 */

#include "fullness_gauge.h"
#include "../../../character_manager/character_manager.h"
#include "../../../upgrade_manager/upgrade_manager.h"

const int               CFullnessGauge::m_width         = 80;										    //!< 幅 
const int               CFullnessGauge::m_height        = 80;										    //!< 高さ 

const vivid::Vector2     CFullnessGauge::m_default_position = { 10.0f, 50.0f };                         //!< デフォルトの位置
const vivid::Vector2     CFullnessGauge::m_offset			= { 50.0f, 0.0f };                          //!< オフセット

const vivid::Rect       CFullnessGauge::m_frame_rect    = { 0, 0, m_width, m_height };				    //!< 枠の読み込み範囲 
const vivid::Rect       CFullnessGauge::m_bar_rect      = { 0, m_height, m_width, m_height * 2 };	    //!< バーの読み込み範囲 

const std::string       CFullnessGauge::m_fullness_head_path    = "data\\hud\\fullness_head.png";       //!< テクスチャのパス
const std::string       CFullnessGauge::m_fullness_gauge_path   = "data\\hud\\fullness_gauge.png";      //!< テクスチャのパス
const std::string       CFullnessGauge::m_fullness_body_path    = "data\\hud\\fullness_body.png";       //!< テクスチャのパス



/*
 *  コンストラクタ
 */
CFullnessGauge::
CFullnessGauge(void)
{
}

/*
 *  デストラクタ
 */
CFullnessGauge::
~CFullnessGauge(void)
{
}

/*
 *  初期化
 */
void
CFullnessGauge::
Initialize(void)
{
	for (int i = 0; i < 10; i++)
	{
		m_Rect[i] = m_bar_rect;
	}

    for (int i = 0; i < 12; i++)
    {
		m_Position[i] = m_default_position + (m_offset * i);
		m_Scale[i] = vivid::Vector2(1.0f, 1.0f);
    }
}

/*
 *  更新
 */
void
CFullnessGauge::
Update(void)
{
    // プレイヤーキャラクターを取得
    ICharacter* player = CCharacterManager::GetInstance().GetPlayer();

    float rate = 0.0f;

    // プレイヤーの満腹度の割合を求める
    if (player)
        rate = ((float)player->GetStatus(STATUS_ID::FULLNESS) / (float)player->GetMaxStatus(STATUS_ID::FULLNESS)) * 10.0f;
    

	// バーの読み込み範囲を更新
	for (int i = 0; i < 10; i++)
	{
		if (i < static_cast<int>(rate))
            m_Rect[i].right = m_bar_rect.right;
		else
			m_Rect[i].right = 0.0f;
	}

	m_Timer += vivid::GetDeltaTime(); // 経過時間を取得

	int index = static_cast<int>(rate);

	// 跳ねるゲージの拡大率を計算
	for (int i = 1; i < 11; i++)
	{
		if (i == index)
		{
			m_Scale[i].x = 1.0f + (sin(m_Timer * (16.0f - i)) * 0.1f);
			m_Scale[i].y = 1.0f + (sin(m_Timer * (16.0f - i)) * 0.1f);
		}
		else
		{
			m_Scale[i].x = 1.0f; // その他のゲージは通常の拡大率
			m_Scale[i].y = 1.0f; // その他のゲージは通常の拡大率
		}
	}
}

/*
 *  描画
 */
void
CFullnessGauge::
Draw(void)
{
	CUpgraeStatusManager& um = CUpgraeStatusManager::GetInstance();
	unsigned int color = um.GetUpgradeStatusNameColor();

	for (int i = 0; i < 12; i++)
	{
        if(i == 0)
			vivid::DrawTexture(m_fullness_body_path, m_Position[i], color, m_frame_rect, vivid::Vector2(m_width / 2, m_height / 2), m_Scale[i]);
		else if (i == 11)
			vivid::DrawTexture(m_fullness_head_path, m_Position[i], color, m_frame_rect, vivid::Vector2(m_width / 2, m_height / 2), m_Scale[i]);
		else
		{
			vivid::DrawTexture(m_fullness_gauge_path, m_Position[i], 0xffffffff, m_frame_rect, vivid::Vector2(m_width / 2, m_height / 2), m_Scale[i]);
			vivid::DrawTexture(m_fullness_gauge_path, m_Position[i], 0xffffffff, m_Rect[i - 1],vivid::Vector2(m_width/2,m_height/2),m_Scale[i]);
		}
	}
}

/*
 *  解放
 */
void
CFullnessGauge::
Finalize(void)
{
}
