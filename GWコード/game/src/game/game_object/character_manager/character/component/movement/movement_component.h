
/*!
 *  @file           movement_component.h
 *  @brief          キャラクターの移動制御コンポーネント
 *  @author         Ryusei Shimizu
 *  @date           2026/10/13
 */

#pragma once

class CPhysicsComponent;

class CMovementComponent
{
public:

    CMovementComponent (void) = default;
    ~CMovementComponent (void) = default;

    /*!
     *  @brief      初期化
     */
    void            Initialize (void);

    /*!
     *  @brief      更新
     * 
     *  @param[in]  direction   方向
     *  @param[in]  jumpPressed ジャンプ入力
     *  @param[in]  physics     物理演算コンポーネント
     */
    void Update (float direction, bool jumpPressed, CPhysicsComponent& physics);

private:
    
    /*!
     *  @brief      左右移動
     * 
     *  @param[in]  direction   方向
     *  @param[in]  physics     物理演算コンポーネント
     */
    void Move (float direction, CPhysicsComponent& physics);

    /*!
     *  @brief      ジャンプ
     * 
     *  @param[in]  jumpPressed ジャンプ入力
     *  @param[in]  physics     物理演算コンポーネント
     */
    void Jump (bool jumpPressed, CPhysicsComponent& physics);

private:
    
    static const float m_MoveSpeed;         //!< 移動速度
    static const float m_Acceleration;      //!< 加速度
    static const float m_Deceleration;      //!< 減速度
    static const float m_JumpPower;         //!< ジャンプ力

    float m_CurrentSpeed;           //!< 現在の速度
};
