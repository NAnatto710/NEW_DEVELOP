
/*!
 *  @file       hp_gauge.cpp
 *  @brief      HPゲージ
 *  @author     Ryusei Shimizu
 *  @date       2026/02/03
 */

#include "hp_gauge.h"
#include "../../../character_manager/character_manager.h"

const int               CHpGauge::m_width               = 600;										                        //!< 幅 
const int               CHpGauge::m_bar_height          = 38;										                        //!< バーの高さ 
const int               CHpGauge::m_frame_height        = 42;										                        //!< 枠の高さ 
const vivid::Vector2    CHpGauge::m_bar_position        = vivid::Vector2(10.0f, 12.0f);				                        //!< 位置 
const vivid::Vector2    CHpGauge::m_frame_position      = vivid::Vector2(10.0f, 10.0f);				                        //!< 位置 
const vivid::Rect       CHpGauge::m_frame_rect          = { 0, 0, m_width, m_frame_height };				                //!< 枠の読み込み範囲 
const vivid::Rect       CHpGauge::m_bar_rect            = { 0, m_frame_height, m_width, m_frame_height + m_bar_height };	//!< バーの読み込み範囲 
const int               CHpGauge::m_bar_speed           = 2;										                        //!< バーの移動速度 

/*
 *  コンストラクタ
 */
CHpGauge::
CHpGauge(void)
{
}

/*
 *  デストラクタ
 */
CHpGauge::
~CHpGauge(void)
{
}

/*
 *  初期化
 */
void
CHpGauge::
Initialize(void)
{
    m_Rect = m_bar_rect;
}

/*
 *  更新
 */
void
CHpGauge::
Update(void)
{
    // プレイヤーキャラクターを取得
    ICharacter* player = CCharacterManager::GetInstance().GetPlayer();

    float rate = 0.0f;

    // プレイヤーのライフの割合を求める
    if (player)
        rate = (float)player->GetStatus(STATUS_ID::HP) / (float)player->GetMaxStatus(STATUS_ID::HP);

    // 割合をもとに画像の幅(目標値)を計算
    int life = (int)(rate * (float)m_width);

    // 目標値を下回るまでRectのrightを減らす
    if (m_Rect.right > life)
    {
        m_Rect.right -= m_bar_speed;
    }

	// 目標値を上回るまでRectのrightを増やす
    if (m_Rect.right < life)
    {
        m_Rect.right += m_bar_speed;
    }
}

/*
 *	描画
 */
void
CHpGauge::
Draw(void)
{
	std::string path = "data\\hud\\hp.png";

    // 外枠
    vivid::DrawTexture(path, m_frame_position, 0xffffffff, m_frame_rect);

    // 内側
    vivid::DrawTexture(path, m_bar_position, 0xffffffff, m_Rect);
}

/*
 *  解放
 */
void
CHpGauge::
Finalize(void)
{
}
