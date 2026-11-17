
/*!
  *  @file           ruler.cpp
  *  @brief          定規
  *  @author         Ryusei Shimizu
  *  @date           2026/11/05
  */

#include "ruler.h"

#include <string>

const int           CRuler::m_texture_width     = 300;                              //!< 画像の幅
const int           CRuler::m_texture_height    = 50;                               //!< 画像の高さ
const std::string   CRuler::m_texture_path      = "data\\stationery\\ruler.png";    //!< 画像パス

/*
 *  コンストラクタ
 */
CRuler::
CRuler(void)
    : IStationery(STATIONERY_TYPE::RULER)
{
}

/*
 *  初期化
 */
void
CRuler::
Initialize(const vivid::Vector2& position)
{
    // 基底クラスで中心座標とサイズ、配置状態を初期化する
    IStationery::Initialize(position, {static_cast<float>(m_texture_width), static_cast<float>(m_texture_height)});
}

/*
 *  更新
 */
void
CRuler::
Update(void)
{
}

/*
 *  描画
 */
void
CRuler::
Draw(void)
{
    const vivid::Vector2 anchor = { m_texture_width * 0.5f, m_texture_height * 0.5f};
    const vivid::Rect rect = { 0, 0, m_texture_width, m_texture_height};
    const vivid::Vector2 scale = {1.0f, 1.0f};

    const vivid::Vector2 draw_position = { m_Position.x - anchor.x, m_Position.y - anchor.y};

    vivid::DrawTexture( m_texture_path, draw_position, 0xffffffff, rect, anchor, scale, m_Rotation);
}
