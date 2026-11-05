/*!
 *  @file           stationery_manager.cpp
 *  @brief          文房具管理
 *  @date           2026/11/05
 */

#include "stationery_manager.h"
#include "stationery/stationery.h"
#include "stationery/ruler/ruler.h"

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
