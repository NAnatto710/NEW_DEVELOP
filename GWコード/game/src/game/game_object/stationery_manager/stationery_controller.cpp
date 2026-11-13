/*!
 *  @file           stationery_controller.cpp
 *  @brief          文房具のマウス操作
 *  @date           2026/11/10
 */

#include "stationery_controller.h"
#include "stationery/stationery.h"

#include <cmath>

/*
 *  マウス入力による文房具操作の更新
 */
void
CStationeryController::
Update(const std::vector<std::unique_ptr<IStationery>>& stationeries)
{
    const vivid::Point cursor = vivid::mouse::GetCursorPos();
    const vivid::Vector2 mouse_position = {
        static_cast<float>(cursor.x),
        static_cast<float>(cursor.y)
    };

    // 右クリックで操作開始前の位置・角度へ戻す
    if (m_Dragging != nullptr &&
        vivid::mouse::Trigger(vivid::mouse::BUTTON_ID::RIGHT))
    {
        CancelDrag();
        return;
    }

    // 未使用の文房具を左クリックで掴む
    if (m_Dragging == nullptr &&
        vivid::mouse::Trigger(vivid::mouse::BUTTON_ID::LEFT))
    {
        m_Dragging = FindStationery(stationeries, mouse_position);

        if (m_Dragging != nullptr)
        {
            // 掴んだ瞬間に落下速度を破棄する
            m_Dragging->ResetPhysicsVelocity();
            m_StartPosition = m_Dragging->GetPosition();
            m_StartRotation = m_Dragging->GetRotation();
            m_DragOffset = {
                m_StartPosition.x - mouse_position.x,
                m_StartPosition.y - mouse_position.y
            };
        }
    }

    if (m_Dragging == nullptr)
    {
        return;
    }

    // 別システムでUSEDになった場合は操作を停止する
    if (!m_Dragging->CanManipulate())
    {
        m_Dragging = nullptr;
        return;
    }

    // マウスで掴んだ点を維持して移動する
    if (vivid::mouse::Button(vivid::mouse::BUTTON_ID::LEFT))
    {
        m_Dragging->SetPosition({
            mouse_position.x + m_DragOffset.x,
            mouse_position.y + m_DragOffset.y
        });

        // 回転角はラジアン。ホイール1単位で5度回転する（仮値）
        constexpr float ROTATION_STEP = 0.0872664626f;
        const int wheel = vivid::mouse::GetWheel();

        if (wheel != 0)
        {
            m_Dragging->SetRotation(
                m_Dragging->GetRotation() + ROTATION_STEP * wheel);
        }
    }

    // ボタンを離したらその場所に配置する
    if (vivid::mouse::Released(vivid::mouse::BUTTON_ID::LEFT))
    {
        if (m_Dragging->GetState() == STATIONERY_STATE::HELD)
        {
            m_Dragging->Place();
        }

        m_Dragging = nullptr;
    }
}

/*
 *  クリック位置から文房具を検索
 */
IStationery*
CStationeryController::
FindStationery(const std::vector<std::unique_ptr<IStationery>>& stationeries,
               const vivid::Vector2& position) const
{
    // 最後に描画した文房具を優先して選択する
    for (auto it = stationeries.rbegin(); it != stationeries.rend(); ++it)
    {
        IStationery* stationery = it->get();

        if (stationery->CanManipulate() &&
            ContainsPoint(*stationery, position))
        {
            return stationery;
        }
    }

    return nullptr;
}

/*
 *  回転矩形によるクリック判定
 */
bool
CStationeryController::
ContainsPoint(const IStationery& stationery,
              const vivid::Vector2& position) const
{
    const vivid::Vector2& center = stationery.GetPosition();
    const vivid::Vector2& size = stationery.GetSize();
    const float rotation = stationery.GetRotation();

    // マウス座標を定規のローカル座標へ逆回転させる
    const float dx = position.x - center.x;
    const float dy = position.y - center.y;
    const float cosine = std::cos(rotation);
    const float sine = std::sin(rotation);
    const float local_x = dx * cosine + dy * sine;
    const float local_y = -dx * sine + dy * cosine;

    return std::abs(local_x) <= size.x * 0.5f &&
           std::abs(local_y) <= size.y * 0.5f;
}

/*
 *  文房具操作のキャンセル
 */
void
CStationeryController::
CancelDrag(void)
{
    if (m_Dragging == nullptr)
    {
        return;
    }

    // 配置済みの文房具は掴む直前の座標へ戻す
    m_Dragging->SetPosition(m_StartPosition);
    m_Dragging->SetRotation(m_StartRotation);
    m_Dragging = nullptr;
}

/*
 *  操作状態の破棄
 */
void
CStationeryController::
Reset(void)
{
    m_Dragging = nullptr;
    m_DragOffset = {0.0f, 0.0f};
    m_StartPosition = {0.0f, 0.0f};
    m_StartRotation = 0.0f;
}

/*
 *  指定した文房具をドラッグ中か
 */
bool
CStationeryController::
IsDragging(const IStationery* stationery) const
{
    return m_Dragging == stationery && stationery != nullptr;
}

/*
 *  いずれかの文房具をドラッグ中か
 */
bool
CStationeryController::
IsDragging(void) const
{
    return m_Dragging != nullptr;
}
