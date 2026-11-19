
/*!
 *  @file           stationery.h
 *  @brief          定規
 *  @author         文房具共通基底
 *  @date           2026/11/05
 */

#include "stationery.h"
#include "../../../../utility/utility.h"

#include <cmath>

// 1フレームを何回に分割するかはManagerで管理する。
const float IStationery::m_gravity           = 0.35f;
const float IStationery::m_max_fall_speed    = 12.0f;
const float IStationery::m_linear_damping    = 0.999f;
const float IStationery::m_angular_damping   = 0.998f;
const float IStationery::m_max_angular_speed = 0.06f;

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
    , m_PhysicsVelocity(0.0f, 0.0f)
    , m_PhysicsAngularSpeed(0.0f)
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
    ResetPhysicsVelocity();
}

/*
 *  重力と現在の速度で位置・角度を更新する 
 */
void
IStationery::
UpdatePhysics (float delta_time)
{
    // つかみ状態であれば物理演算処理はしない
    if(m_State == STATIONERY_STATE::HELD) return;

    // 最大落下速度内で重力を適応
    m_PhysicsVelocity.y = Utility::Min (m_PhysicsVelocity.y + m_gravity * delta_time, m_max_fall_speed);

    // 1フレームの物理演算を何回に分けて計算するかを変えても減衰率が大きく変わらないようにするためにべき乗を使用する
    // std::pow はべき乗の計算に使用できる（引数１の引数２乗）
    
    // 横方向の速度を減退
    m_PhysicsVelocity.x *= std::pow (m_linear_damping, delta_time);

    // 回転速度を減退
    m_PhysicsAngularSpeed *= std::pow (m_angular_damping, delta_time);
    m_PhysicsAngularSpeed = Utility::Clamp (m_PhysicsAngularSpeed, -m_max_angular_speed, m_max_angular_speed);

    // 速度を適応させて位置と角度を更新
    m_Position = m_Position + m_PhysicsVelocity * delta_time;
    m_Rotation += m_PhysicsAngularSpeed * delta_time;
}

/*
 *  掴み直したときに以前の速度を消す
 */
void
IStationery::
ResetPhysicsVelocity (void)
{
    m_PhysicsVelocity = vivid::Vector2 (0.0f, 0.0f);
    m_PhysicsAngularSpeed = 0.0f;
}

/*
 *  配置確定
 */
bool
IStationery::
Place(void)
{
    if (m_State != STATIONERY_STATE::HELD) return false;
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
    if (m_State != STATIONERY_STATE::PLACED) return false;
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
    if (!CanManipulate()) return false;
    m_Position = position;
    ResetPhysicsVelocity();
    return true;
}

/*
 *  プレイヤー操作による回転変更
 */
bool
IStationery::
SetRotation(float rotation)
{
    if (!CanManipulate()) return false;
    m_Rotation = rotation;
    ResetPhysicsVelocity();
    return true;
}

/*
 *  プレイヤーが操作できるか
 */
bool
IStationery::
CanManipulate(void) const
{
    return m_State != STATIONERY_STATE::USED;
}

/*
 *  物理処理から位置を変更する
 */
void
IStationery::
SetPhysicsPosition(const vivid::Vector2& position)
{
    m_Position = position;
}

/*
 *  物理処理から角度を変更する
 */
void
IStationery::
SetPhysicsRotation(float rotation)
{
    m_Rotation = rotation;
}