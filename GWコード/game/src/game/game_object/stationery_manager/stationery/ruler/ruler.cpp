/*!
 *  @file           ruler.cpp
 *  @brief          定規
 *  @date           2026/11/05
 */

#include "ruler.h"

#include <string>

namespace
{
    const std::string RULER_TEXTURE_PATH = "data\\stationery\\ruler.png";
}

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
    IStationery::Initialize(
        position,
        {static_cast<float>(TEXTURE_WIDTH), static_cast<float>(TEXTURE_HEIGHT)});
}

/*
 *  更新
 */
void
CRuler::
Update(void)
{
    // マウス操作・物理処理は今後追加する
}

/*
 *  描画
 */
void
CRuler::
Draw(void)
{
    // vivid2DのDrawTextureは、position + anchorを回転中心として扱う
    // 基底クラスの中心座標を渡すため、画像半分を引いておく
    const vivid::Vector2 anchor = {
        TEXTURE_WIDTH * 0.5f,
        TEXTURE_HEIGHT * 0.5f
    };

    const vivid::Vector2 draw_position = {
        m_Position.x - anchor.x,
        m_Position.y - anchor.y
    };

    const vivid::Rect rect = {
        0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT
    };

    const vivid::Vector2 scale = {1.0f, 1.0f};

    vivid::DrawTexture(
        RULER_TEXTURE_PATH,
        draw_position,
        0xffffffff,
        rect,
        anchor,
        scale,
        m_Rotation);
}
