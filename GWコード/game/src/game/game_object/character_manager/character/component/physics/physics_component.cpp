
/*!
 *  @file   physics_component.cpp
 *  @brief  キャラクター用物理演算コンポーネント
 */

#include "physics_component.h"
#include "../../../../stage_manager/stage_manager.h"
#include "../../../../../../utility/utility.h"

const float CPhysicsComponent::m_friction           = 0.9f;     //!< 摩擦
const float CPhysicsComponent::m_gravity_increase   = 0.08f;    //!< 重力加算値
const float CPhysicsComponent::m_max_gravity        = 1.8f;     //!< 最大重力値
const float CPhysicsComponent::m_max_fallspeed      = 20.0f;    //!< 最大落下速度

/*
 *  初期化
 */
void
CPhysicsComponent::
Initialize (int width, int height, const vivid::Vector2& position)
{
    m_Width = width;
    m_Height = height;

    m_Position = position;
    m_InitialPosition = position;

    m_Velocity.Clear ();

    m_GravityPower = 0.0f;
    m_IsGrounded = false;
}

/*
 *  更新
 */
void
CPhysicsComponent::
Update (void)
{
    if(m_Width <= 0 || m_Height <= 0) return;

    m_IsGrounded = HasGroundBelow ();

    // 重力の適用
    ApplyGravity ();

    // 移動、衝突判定
    CheckCollision ();

    m_IsGrounded = HasGroundBelow ();

    if(m_IsGrounded && m_Velocity.Gravity.y > 0.0f)
    {
        ResetVerticalVelocity ();
    }
}

/*
 *  重力の適用
 */
void
CPhysicsComponent::
ApplyGravity (void)
{
    // 接地中は重力を加算しない
    if(m_IsGrounded)
    {
        if(m_Velocity.Gravity.y > 0.0f) this->ResetVerticalVelocity ();
        return;
    }

    m_GravityPower += m_gravity_increase;

    m_GravityPower = Utility::Min(m_GravityPower,m_max_gravity);

    m_Velocity.Gravity.y += m_GravityPower;

    m_Velocity.Gravity.y = Utility::Min (m_Velocity.Gravity.y, m_max_fallspeed);
}

/*
 *  衝突判定と位置補正
 */
void
CPhysicsComponent::
CheckCollision (void)
{
    const vivid::Vector2 velocity = this->GetVelocity();

    // X方向の移動、衝突判定
    this->MoveAxis(velocity.x, true);

    // Y方向の移動、衝突判定
    this->MoveAxis(velocity.y, false);
}

/*
 *  速度の減衰
 */
void
CPhysicsComponent::
VelocityDecay (void)
{
    m_Velocity.Move.x *= m_friction;

    // 速度が少ない値になったら０にする
    if(std::fabs (m_Velocity.Move.x) < 0.01f) m_Velocity.Move.x = 0.0f;
}

/*
 *  速度の加算
 */
void
CPhysicsComponent::
AddMoveVelocity (const vivid::Vector2& velocity)
{
    m_Velocity.Move = m_Velocity.Move + velocity;
}

/*
 *  速度の設定
 */
void
CPhysicsComponent::
SetMoveVelocity (const vivid::Vector2& velocity)
{
    m_Velocity.Move = velocity;
}

/*
 *  すべての速度をリセット
 */
void
CPhysicsComponent::
ResetVelocity (void)
{
    m_Velocity.Clear ();
    m_GravityPower = 0.0f;
}

/*
 *  位置の設定
 */
void
CPhysicsComponent::
SetPosition (const vivid::Vector2& position)
{
    m_Position = position;
    m_IsGrounded = false;
}

/*
 *  接地状態の設定
 */
void
CPhysicsComponent::
SetGrounded (bool grounded)
{
    m_IsGrounded = grounded;

    // 着地している場合は重力の速度をリセットする
    if(grounded && m_Velocity.Gravity.y > 0.0f) ResetVerticalVelocity ();
}

/*
 *  ステージとの当たり判定
 */
bool
CPhysicsComponent::
IsBlocked (const vivid::Vector2& position) const
{
    if(m_Width <= 0 || m_Height <= 0) return false;

    const CStageManager& stage = CStageManager::GetInstance ();
    const int blockSize = stage.GetBlockSize ();

    const float eps = 0.001f;
    
    // キャラクターと接触しているタイル情報の取得
    const int left = static_cast<int>(std::floor (position.x / blockSize));
    const int right = static_cast<int>(std::floor ((position.x + m_Width - eps) / blockSize));
    const int top = static_cast<int>(std::floor (position.y / blockSize));
    const int bottom = static_cast<int>(std::floor ((position.y + m_Height - eps) / blockSize));

    // 接触しているタイルとの当たり判定を行う
    for(int ty = top; ty <= bottom; ++ty)
    {
        for(int tx = left; tx <= right; ++tx)
        {
            if(stage.IsHit (tx * blockSize, ty * blockSize)) return true;
        }
    }
    return false;
}

/*
 *  接地判定
 */
bool
CPhysicsComponent::
HasGroundBelow () const
{
    vivid::Vector2 probe = m_Position;
    // 移動処理で生じる微小な隙間を考慮
    probe.y += 1.1f;

    return IsBlocked (probe);
}

/*
 *  移動接触判定
 */
void
CPhysicsComponent::
MoveAxis (float delta, bool horizontal)
{
    if(delta == 0.0f) return;

    float remaining = std::fabs (delta);
    const float direction = (delta > 0.0f) ? 1.0f : -1.0f;

    // 1ピクセル以下に分割して通り抜けを抑制（暫定方式）
    while(remaining > 0.0f)
    {
        const float step = Utility::Min(1.0f, remaining) * direction;
        vivid::Vector2 next = m_Position;

        if(horizontal)  next.x += step;
        else            next.y += step;

        if(IsBlocked (next))
        {
            if(horizontal) m_Velocity.Move.x = 0.0f;
            else
            {
                // 天井・床のどちらでも垂直速度を止める
                this->ResetVerticalVelocity ();
            }
            return;
        }
        m_Position = next;
        remaining -= std::fabs (step);
    }
}

/*
 *  垂直方向の速度の設定
 */
void CPhysicsComponent::SetVerticalVelocity (float velocity)
{
    m_Velocity.Move.y = 0.0f;
    m_Velocity.Gravity.y = velocity;
    m_GravityPower = 0.0f;
    m_IsGrounded = false;
}

/*
 *  垂直方向の速度をリセット
 */
void
CPhysicsComponent::
ResetVerticalVelocity (void)
{
    m_Velocity.Move.y = 0.0f;
    m_Velocity.Gravity.y = 0.0f;

    m_GravityPower = 0.0f;
}
