
/*!
 *  @file           stationery.cpp
 *  @brief          文房具共通基底
 *  @author         Ryusei Shimizu
 *  @date           2026/11/05
 */

#include "stationery.h"

/*
 *  コンストラクタ
 */
IStationery::
IStationery(STATIONERY_TYPE type)
    : m_Type(type)
    , m_State(STATIONERY_STATE::HELD)
    , m_Position(0.0f, 0.0f)
    , m_Size(0.0f, 0.0f)
    , m_Rotation(0.0f)
{
}

/*
 *  初期化
 */
void
IStationery::
Initialize(const vivid::Vector2& position, const vivid::Vector2& size)
{
    m_Position = position;
    m_Size = size;
    m_Rotation = 0.0f;
    m_State = STATIONERY_STATE::HELD;
}

/*
 *  配置確定
 */
bool
IStationery::
Place(void)
{
    // 購入後・配置前の文房具だけを配置確定できる
    if (m_State != STATIONERY_STATE::HELD)
    {
        return false;
    }

    m_State = STATIONERY_STATE::PLACED;
    return true;
}

/*
 *  使用済みへの変更
 */
bool
IStationery::
MarkUsed(void)
{
    // 配置確定前の接触では使用済みにしない
    if (m_State != STATIONERY_STATE::PLACED)
    {
        return false;
    }

    m_State = STATIONERY_STATE::USED;
    return true;
}

/*
 *  プレイヤー操作による位置変更
 */
bool
IStationery::
SetPosition(const vivid::Vector2& position)
{
    // 使用済みの文房具はプレイヤーが再配置できない
    if (!CanManipulate())
    {
        return false;
    }

    m_Position = position;
    return true;
}

/*
 *  プレイヤー操作による回転変更
 */
bool
IStationery::
SetRotation(float rotation)
{
    // 使用済みの文房具はプレイヤーが回転できない
    if (!CanManipulate())
    {
        return false;
    }

    m_Rotation = rotation;
    return true;
}

/*
 *  プレイヤー操作の可否
 */
bool
IStationery::
CanManipulate(void) const
{
    return m_State != STATIONERY_STATE::USED;
}

/*
 *  物理演算による位置変更
 */
void
IStationery::
SetPhysicsPosition(const vivid::Vector2& position)
{
    // 使用済みでも、重力や衝突によって位置は変化する
    m_Position = position;
}

/*
 *  物理演算による回転変更
 */
void
IStationery::
SetPhysicsRotation(float rotation)
{
    // 使用済みでも、衝突による回転は継続する
    m_Rotation = rotation;
}
