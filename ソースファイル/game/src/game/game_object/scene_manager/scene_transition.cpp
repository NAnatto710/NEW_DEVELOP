
/*!
 *  @file           scene_transition.cpp
 *  @brief          シーンチェンジエフェクト
 *  @date           2026/10/09
 */

#include "scene_transition.h"
#include "../../../utility/utility.h"
#include "vivid.h"

#include <algorithm>
#include <cmath>

const int CSceneTransition::m_tile_size     = 32;   //!< タイルサイズ
const int CSceneTransition::m_change_frame  = 30;   //!< シーンチェンジのフレーム数

/*
 *  初期化
 */
void
CSceneTransition::
Initialize(void)
{
    m_Cover = false;
    m_Width = vivid::GetWindowWidth();
    m_Height = vivid::GetWindowHeight();
    m_Count = 0;

	// タイル作成
    this->CreateTiles();
}

/*
 *  外周から中央へ黒くする
 */
void
CSceneTransition::
Cover(void)
{
    m_Cover = true;

	// 演出が完了していない場合はカウントを進める
    if (m_Count < m_change_frame)
        ++m_Count;
}

/*
 *  中央から外周へ黒を消す
 */
void
CSceneTransition::
Reveal(void)
{
    m_Cover = false;

	// 演出が完了していない場合はカウントを減らす
    if (m_Count > 0)
        --m_Count;
}

/*
 *  シーンエフェクト描画
 */
void
CSceneTransition::
Draw(void)
{
    if (m_Tiles.empty()) return;

    const int color = Utility::Color(Utility::ColorID::Black);

    // 完全に開いた後は何も描かない
    if (!m_Cover && m_Count <= 0) return;

	// 完全に閉じた後は全面を描画する
    const float progress = static_cast<float>(m_Count) / static_cast<float>(m_change_frame);

	// タイルごとに描画するか判定
    for (const auto& tile : m_Tiles)
    {
        bool draw = false;

        if (m_Cover)
        {
            // 外周から中央へ黒くする
            draw = (progress >= 1.0f - tile.distance);
        }
        else
        {
            // 中央から外周へ黒を消す
            // revealProgress : 0.0 -> 1.0
            const float revealProgress = 1.0f - progress;
            draw = (revealProgress < tile.distance);
        }

        if (draw)
            DxLib::DrawBox(tile.x, tile.y, tile.x + tile.width, tile.y + tile.height, color, TRUE);
    }
}

/*
 *  全面が黒いか
 */
bool
CSceneTransition::
IsCovered(void) const
{
    return m_Count >= m_change_frame;
}

/*
 *  全面を表示したか
 */
bool
CSceneTransition::
IsRevealed(void) const
{
    return m_Count <= 0;
}

/*
 *  全面を黒に設定
 */
void
CSceneTransition::
SetCovered(void)
{
    m_Count = m_change_frame;
}

/*
 *  タイル生成
 */
void
CSceneTransition::
CreateTiles(void)
{
    m_Tiles.clear();
    if (m_Width <= 0 || m_Height <= 0) return;

	// 画面中心座標
    const float cx = m_Width * 0.5f;
    const float cy = m_Height * 0.5f;

	// 中心からの距離を正規化するための係数
    const float rx = Utility::Max(cx, 1.0f);
    const float ry = Utility::Max(cy, 1.0f);

	// タイルの行数と列数を計算
    const int columns = (m_Width + m_tile_size - 1) / m_tile_size;
    const int rows = (m_Height + m_tile_size - 1) / m_tile_size;

    float maxDistance = 0.0f;

	// タイルを作成し、中心からの距離を計算する
    for (int row = 0; row < rows; ++row)
    {
		// タイルの左上座標とサイズを計算
        for (int column = 0; column < columns; ++column)
        {
            TILE tile{};

            tile.x = column * m_tile_size;
            tile.y = row * m_tile_size;
            tile.width  = Utility::Min(m_tile_size, m_Width - tile.x);
            tile.height = Utility::Min(m_tile_size, m_Height - tile.y);

			// タイルの中心座標から画面中心までの距離を計算し、正規化する
            const float dx = (tile.x + tile.width * 0.5f - cx) / rx;
            const float dy = (tile.y + tile.height * 0.5f - cy) / ry;

            tile.distance = std::sqrt(dx * dx + dy * dy);

            maxDistance = Utility::Max(maxDistance, tile.distance);

            m_Tiles.push_back(tile);
        }
    }

    // 正規化により、すべてのタイルが規定フレーム内で切り替わる。
    if (maxDistance > 0.0f)
    {
		// 中心からの距離を正規化する
        for (auto& tile : m_Tiles)
            tile.distance /= maxDistance;
    }
}
