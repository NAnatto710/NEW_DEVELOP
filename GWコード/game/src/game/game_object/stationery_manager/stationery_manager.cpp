/*!
 *  @file           stationery_manager.cpp
 *  @brief          文房具管理
 *  @date           2026/11/05
 */

#include "stationery_manager.h"
#include "stationery/stationery.h"
#include "stationery/ruler/ruler.h"

#include <cmath>

/*
 *  インスタンスの取得
 */
CStationeryManager&
CStationeryManager::
GetInstance(void)
{
    static CStationeryManager instance;
    return instance;
}

/*
 *  デストラクタ
 */
CStationeryManager::
~CStationeryManager(void) = default;

/*
 *  初期化
 */
void
CStationeryManager::
Initialize(void)
{
    // シーン再入場時に前回の文房具を残さない
    Finalize();
}

/*
 *  更新
 */
void
CStationeryManager::
Update(void)
{
    UpdateMouse();

    for (const auto& stationery : m_Stationeries)
    {
        stationery->Update();
    }
}

/*
 *  描画
 */
void
CStationeryManager::
Draw(void)
{
    for (const auto& stationery : m_Stationeries)
    {
        stationery->Draw();
    }
}

/*
 *  解放
 */
void
CStationeryManager::
Finalize(void)
{
    // 選択中のポインタが解放済みオブジェクトを指さないようにする
    m_Dragging = nullptr;
    m_Stationeries.clear();
}

/*
 *  定規生成
 */
CRuler*
CStationeryManager::
CreateRuler(const vivid::Vector2& position)
{
    auto ruler = std::make_unique<CRuler>();
    ruler->Initialize(position);

    // 所有権の移動後も実体のアドレスは変わらない
    CRuler* result = ruler.get();
    m_Stationeries.emplace_back(std::move(ruler));

    return result;
}

/*
 *  文房具の個数取得
 */
std::size_t
CStationeryManager::
GetCount(void) const
{
    return m_Stationeries.size();
}

/*
 *  定規を掴んでいるか
 */
bool
CStationeryManager::
IsDragging(void) const
{
    return m_Dragging != nullptr;
}

/*
 *  マウス入力による文房具操作
 */
void
CStationeryManager::
UpdateMouse(void)
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
        m_Dragging = FindStationery(mouse_position);

        if (m_Dragging != nullptr)
        {
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
CStationeryManager::
FindStationery(const vivid::Vector2& position) const
{
    // 最後に描画した文房具を優先して選択する
    for (auto it = m_Stationeries.rbegin(); it != m_Stationeries.rend(); ++it)
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
CStationeryManager::
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
CStationeryManager::
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
