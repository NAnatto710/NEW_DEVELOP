
/*!
 *  @file           physics_component.cpp
 *  @brief          キャラクター用物理演算コンポーネント
 *  @author         Ryusei Shimizu
 *  @date           2026/10/13
 */

#include "physics_component.h"
#include "../../../../stage_manager/stage_manager.h"
#include "../../../../stationery_manager/stationery_manager.h"
#include "../../../../stationery_manager/stationery/stationery.h"
#include "../../../../../../utility/utility.h"

const float CPhysicsComponent::m_friction           =  0.9f;    //!< 摩擦
const float CPhysicsComponent::m_gravity_increase   =  0.09f;   //!< 重力加算値
const float CPhysicsComponent::m_max_gravity        =  1.2f;    //!< 最大重力値
const float CPhysicsComponent::m_max_fallspeed      = 15.0f;    //!< 最大落下速度

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

    // 地面についていて重力が加算されていたら
    if(m_IsGrounded && m_Velocity.Gravity.y > 0.0f)
    {
        // 垂直方向の速度リセット
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

    // 接地中の定規上では、水平移動を斜面に沿わせる
    float slopeMoveY = 0.0f;
    const bool hasSlope = GetStationerySlopeMove(velocity.x, slopeMoveY);

    // 上り坂では先に上へ移動して、定規の側面へ押し戻されるのを防ぐ
    if(hasSlope && slopeMoveY < 0.0f)
    {
        this->MoveAxis(slopeMoveY, false);
    }

    // X方向の移動、衝突判定
    this->MoveAxis(velocity.x, true);

    // 下り坂では横移動してから斜面へ沿って下がる
    if(hasSlope && slopeMoveY > 0.0f)
    {
        this->MoveAxis(slopeMoveY, false);
    }

    // ジャンプ・落下によるY方向の移動は従来どおり実行する
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

    return IsBlocked (probe) || HasStationeryBelow ();
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

        // 定規の斜面に沿うように衝突法線で位置を補正
        if(!ResolveStationery(next, horizontal))
        {
            if(horizontal) m_Velocity.Move.x = 0.0f;
            else this->ResetVerticalVelocity ();
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

/*
 *  文房具との衝突解消
 */
bool
CPhysicsComponent::
ResolveStationery(vivid::Vector2& position, bool horizontal)
{
    CStationeryManager& manager = CStationeryManager::GetInstance();

    // 複数オブジェクトとの重なりを数回に分けて解消する
    constexpr int MAX_ITERATIONS = 4;
    constexpr float CONTACT_MARGIN = 0.001f;

    for(int i = 0; i < MAX_ITERATIONS; ++i)
    {
        const Utility::AABB player = {
            position,
            static_cast<float>(m_Width),
            static_cast<float>(m_Height)
        };

        Utility::CollisionResult hit;
        IStationery* stationery = nullptr;
        if(!manager.CheckCollision(player, hit, stationery))
        {
            return true;
        }

        // NormalはPlayerから文房具へ向かう方向
        // Player側を離すためNormalの逆方向へ動かす
        const vivid::Vector2 correction = {
            -hit.Normal.x * (hit.PenetrationDepth + CONTACT_MARGIN),
            -hit.Normal.y * (hit.PenetrationDepth + CONTACT_MARGIN)
        };
        const vivid::Vector2 corrected = {
            position.x + correction.x,
            position.y + correction.y
        };

        // ステージ地形へ押し込む補正は行わない
        if(IsBlocked(corrected))
        {
            return false;
        }

        position = corrected;
        stationery->MarkUsed();

        // 上面へ押し戻されたら着地、下面へ押し戻されたら頭を衝突
        if(!horizontal && std::fabs(hit.Normal.y) > 0.5f)
        {
            const float vertical = GetVelocity().y;
            // 法線はPlayerから文房具への向き。上面ならNormal.yは正
            if((hit.Normal.y > 0.0f && vertical >= 0.0f) ||
               (hit.Normal.y < 0.0f && vertical <= 0.0f))
            {
                ResetVerticalVelocity();
            }
        }
    }

    // 反復後も深く重なっていた場合は今回の移動を拒否する
    const Utility::AABB player = {
        position,
        static_cast<float>(m_Width),
        static_cast<float>(m_Height)
    };
    Utility::CollisionResult hit;
    IStationery* stationery = nullptr;
    return !manager.CheckCollision(player, hit, stationery) ||
           hit.PenetrationDepth <= 0.01f;
}

/*
 *  定規上の接地判定
 */
bool
CPhysicsComponent::
HasStationeryBelow(void) const
{
    // 1px下の足場を調べる。水平面に近い法線だけを地面として扱う
    const Utility::AABB player = {
        {m_Position.x, m_Position.y + 1.1f},
        static_cast<float>(m_Width),
        static_cast<float>(m_Height)
    };
    Utility::CollisionResult result;
    IStationery* stationery = nullptr;
    if(!CStationeryManager::GetInstance().CheckCollision(player, result, stationery))
    {
        return false;
    }

    // Playerから文房具へ向かう法線なので、床はY正方向
    return result.Normal.y > 0.5f;
}

/*
 *  定規の斜面に沿った移動量の取得
 */
bool
CPhysicsComponent::
GetStationerySlopeMove(float horizontalMove, float& slopeMoveY) const
{
    slopeMoveY = 0.0f;

    // 接地していない・ジャンプ中・横入力なしなら斜面移動しない
    if(!m_IsGrounded || horizontalMove == 0.0f ||
       m_Velocity.Gravity.y < 0.0f)
    {
        return false;
    }

    // 足元をわずかに下へずらして、接地している定規の法線を取得する
    const Utility::AABB probe = {
        {m_Position.x, m_Position.y + 1.1f},
        static_cast<float>(m_Width),
        static_cast<float>(m_Height)
    };

    Utility::CollisionResult result;
    IStationery* stationery = nullptr;
    if(!CStationeryManager::GetInstance().CheckCollision(probe, result, stationery))
    {
        return false;
    }

    // 法線Y正は上面接触。急角度・側面接触は斜面扱いしない
    constexpr float MIN_GROUND_NORMAL_Y = 0.65f;
    if(result.Normal.y < MIN_GROUND_NORMAL_Y)
    {
        return false;
    }

    // 斜面の接線は法線に垂直: dy / dx = -Normal.x / Normal.y
    slopeMoveY = -horizontalMove * result.Normal.x / result.Normal.y;
    return true;
}
