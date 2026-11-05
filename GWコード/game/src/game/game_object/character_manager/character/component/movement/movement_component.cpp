
/*!
 *  @file           movement_component.cpp
 *  @brief          キャラクターの移動制御コンポーネント
 *  @author         Ryusei Shimizu
 *  @date           2026/10/13
 */

#include "movement_component.h"
#include "../physics/physics_component.h"
#include "../../../../../../utility/utility.h"

const float CMovementComponent::m_move_speed    = 6.0f;     //!< 移動速度
const float CMovementComponent::m_acceleration  = 1.5f;     //!< 加速度
const float CMovementComponent::m_deceleration  = 1.2f;     //!< 減速度
const float CMovementComponent::m_jump_power    = 12.0f;    //!< ジャンプ力

/*
 *  初期化
 */
void
CMovementComponent::
Initialize (void)
{
    m_CurrentSpeed = 0.0f;
}

/*
 *  更新
 */
void
CMovementComponent::
Update (float direction, bool jumpPressed, CPhysicsComponent& physics)
{
    // 移動入力
    this->Move (direction, physics);

    // ジャンプ入力
    this->Jump (jumpPressed, physics);
}

/*
 *  左右移動
 */
void
CMovementComponent::
Move (float direction, CPhysicsComponent& physics)
{
    direction = Utility::Clamp(direction, -1.0f, 1.0f);

    if(direction != 0.0f)
    {
        // 目標速度
        const float targetSpeed = direction * m_move_speed;

        // 目標速度へ徐々に近づける
        if(m_CurrentSpeed < targetSpeed)
        {
            m_CurrentSpeed = Utility::Min(m_CurrentSpeed + m_acceleration, targetSpeed);
        }
        else
        {
            m_CurrentSpeed = Utility::Max(m_CurrentSpeed - m_acceleration, targetSpeed);
        }
    }
    else
    {
        // 入力がなければ減速
        if(m_CurrentSpeed > 0.0f)
        {
            m_CurrentSpeed = Utility::Max(0.0f, m_CurrentSpeed - m_deceleration);
        }
        else if(m_CurrentSpeed < 0.0f)
        {
            m_CurrentSpeed = Utility::Min(0.0f, m_CurrentSpeed + m_deceleration);
        }
    }

    // Physicsへ水平移動速度を渡す
    physics.SetMoveVelocity ({m_CurrentSpeed, 0.0f});
}

/*
 *  ジャンプ
 */
void
CMovementComponent::
Jump (bool jumpPressed, CPhysicsComponent& physics)
{
    // キーが押されたとき
    if(!jumpPressed) return;

    // 接地中のみジャンプ
    if(!physics.IsGrounded ()) return;

    // Y軸正方向が下なので負の速度
    physics.SetVerticalVelocity (-m_jump_power);
}
