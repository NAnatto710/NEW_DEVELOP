
/*!
 *  @file           movement_component.cpp
 *  @brief          キャラクターの移動制御コンポーネント
 *  @author         Ryusei Shimizu
 *  @date           2026/10/13
 */

#include "movement_component.h"
#include "../physics/physics_component.h"
#include "../../../../../../utility/utility.h"

const float CMovementComponent::m_MoveSpeed     = 5.0f;     //!< 移動速度
const float CMovementComponent::m_Acceleration  = 1.2f;     //!< 加速度
const float CMovementComponent::m_Deceleration  = 1.5f;     //!< 減速度
const float CMovementComponent::m_JumpPower     = 12.0f;    //!< ジャンプ力

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
    this->Move (direction, physics);
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
        const float targetSpeed = direction * m_MoveSpeed;

        // 目標速度へ徐々に近づける
        if(m_CurrentSpeed < targetSpeed)
        {
            m_CurrentSpeed = Utility::Min(m_CurrentSpeed + m_Acceleration, targetSpeed);
        }
        else
        {
            m_CurrentSpeed = Utility::Max(m_CurrentSpeed - m_Acceleration, targetSpeed);
        }
    }
    else
    {
        // 入力がなければ減速
        if(m_CurrentSpeed > 0.0f)
        {
            m_CurrentSpeed = Utility::Max(0.0f, m_CurrentSpeed - m_Deceleration);
        }
        else if(m_CurrentSpeed < 0.0f)
        {
            m_CurrentSpeed = Utility::Min(0.0f, m_CurrentSpeed + m_Deceleration);
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
Jump (bool jumpPressed, CPhysicsComponent& physics
)
{
    if(!jumpPressed) return;

    // 接地中のみジャンプ
    if(!physics.IsGrounded ()) return;

    // Y軸正方向が下なので負の速度
    physics.SetVerticalVelocity (-m_JumpPower);
}
