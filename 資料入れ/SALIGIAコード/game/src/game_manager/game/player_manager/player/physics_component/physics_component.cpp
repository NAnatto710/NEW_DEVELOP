
/*!
 *  @file		physics_component.cpp
 *  @brief		物理演算コンポーネント
 *  @author     Ryusei Shimizu
 *  @date       2026/05/21
 */

#include "physics_component.h"
#include "../../../stage_manager/stage_manager.h"
#include "../status/attribute_component/attribute_component.h"
#include "../../../../../utility/sound_manager/sound_manager.h"
#include "../../../effect_manager/effect_manager.h"
#include "../../../camera_manager/camera_manager.h"
#include "../../../../../utility/utility.h"

const float CPhysicsComponent::m_friction            = 0.9f;     //!< 移動時の摩擦力
const float CPhysicsComponent::m_gravity             = 1.6f;     //!< 重力
const float CPhysicsComponent::m_gravity_increase    = 0.08f;    //!< 重力による速度の増加量
const float CPhysicsComponent::m_max_fall_speed      = 42.0f;    //!< 落下速度の上限

/*
 *  コンストラクタ
 */
CPhysicsComponent::
CPhysicsComponent()
{
}

/*
 *	初期化
 */ 
void
CPhysicsComponent::
Initialize(int width, int height, const vivid::Vector2& position)
{
    m_Width = width;
    m_Height = height;
    m_Position = position;
    m_InitialPosition = position;

	m_Velocity.Clear();
    m_MoveAccelerator = 0.0f;
	m_GravityPower = 0.0f;

    m_LandingFlg = false;
    m_WallFlg = false;
    m_PlummetFlg = false;
	m_LandingStiffnessFlg = false;
}

/*
 *  更新
 */
void
CPhysicsComponent::
Update(void)
{
	// 衝突チェック
	this->CheckCollision();

	// 速度の減衰
    this->VelocityDecay();

    // 当たり判定の更新
    this->HitCapsuleUpdate();
}

/*
 *  解放
 */
void
CPhysicsComponent::
Finalize(void)
{
}

/*
 *  重力加速度の適用
 */
void
CPhysicsComponent::
ApplyGravity(void)
{
    // 重力による速度の増加
    m_GravityPower += m_gravity_increase;
    
    // 重力速度の上限チェック
    if (m_GravityPower > m_gravity)
    {
        m_GravityPower = m_gravity;
    }
    
    // 重力加速度をY成分に追加（上方向を負とする）
    m_Velocity.Gravity.y += m_GravityPower;

    // 落下速度の上限チェック
    if (m_Velocity.Gravity.y > m_max_fall_speed)
    {
        m_Velocity.Gravity.y = m_max_fall_speed;
    }

	if (m_LandingFlg) return;

    // 急落下の処理
    if (m_PlummetFlg)
    {
        m_Velocity.Gravity.y = m_max_fall_speed;
    }
}

/*
 *  衝突チェック
 */
void
CPhysicsComponent::
CheckCollision(void)
{
    /*
     *		ステージとの当たり判定と移動処理
     */

    CStageManager& stage = CStageManager::GetInstance();

    // プレイヤーの位置と大きさ
    int x = (int)(m_Position.x);
    int y = (int)(m_Position.y);
    int nx = (int)(m_Position.x + m_Velocity.GetFinalVelocity().x);
    int ny = (int)(m_Position.y + m_Velocity.GetFinalVelocity().y);
    int w = (int)m_Width;
    int h = (int)m_Height;

    // ステージオブジェクトの大きさ
    int size = stage.GetBlockSize();

    // 地上にいる
    if (m_LandingFlg)
    {
        // 足元を調べてブロックがなければ落下
        if (!stage.IsHit(x, y + h) &&
            !stage.IsHit(x + w, y + h) &&
            !stage.IsHitScaffolding(x, y + h) &&
            !stage.IsHitScaffolding(x + w, y + h) ||
            m_PlummetFlg)
        {
            // 足元にブロックがないので着地していない
            m_LandingFlg = false;
        }
    }
    // 空中にいる
    else
    {
        // 重力加速度の適用
        this->ApplyGravity();

        // 天井判定
        if (stage.IsHit(x, ny) ||
            stage.IsHit(x + w - 1, ny))
        {
            // 上に動いている
            if (m_Velocity.GetFinalVelocity().y < 0)
            {
                ny = (ny / size + 1) * size;
            }
        }

        // 着地判定
        if (stage.IsHit(x, ny + h - 1) ||
            stage.IsHit(x + w - 1, ny + h - 1) ||
            (stage.IsHitScaffolding(x, ny + h - 1) && !m_PlummetFlg) ||
            (stage.IsHitScaffolding(x + w - 1, ny + h - 1) && !m_PlummetFlg))
        {
            // 下に動いている
            if (m_Velocity.GetFinalVelocity().y > 0)
            {
                ny = ((ny + h) / size) * size - h;

                // 着地した
                this->SetLandingFlg(true);
            }
        }
    }

    // 位置の決定
    m_Position.y = (float)ny;

    // 左の判定
    if (stage.IsHit(nx, y) ||
        stage.IsHit(nx, y + h / 2) ||
        stage.IsHit(nx, y + h - 1))
    {
        // 左に移動している
        if (m_Velocity.GetFinalVelocity().x < 0)
        {
            nx = (nx / size + 1) * size;

            // ブロックにあたっているので速度を消す
            this->SetVelocityX(VelocityID::Move, 0.0f);
        }
    }

    // 右の判定
    if (stage.IsHit(nx + w - 1, y) ||
        stage.IsHit(nx + w - 1, y + h / 2) ||
        stage.IsHit(nx + w - 1, y + h - 1))
    {
        // 右に移動している
        if (m_Velocity.GetFinalVelocity().x > 0)
        {
            nx = ((nx + w) / size) * size - w;

            // ブロックにあたっているので速度を消す
            this->SetVelocityX(VelocityID::Move, 0.0f);
        }
    }

    m_Position.x = (float)nx;

}

/*
 *  速度リセット
 */
void
CPhysicsComponent::
ResetVelocity(VelocityID id)
{
    switch (id)
    {
    case VelocityID::Move:      m_Velocity.Move = vivid::Vector2::ZERO;     
                                break;

    case VelocityID::Gravity:   m_Velocity.Gravity = vivid::Vector2::ZERO;
                                m_GravityPower = 0.0f;
                                break;

    case VelocityID::KnockBack: m_Velocity.KnockBack = vivid::Vector2::ZERO;
                                break;

    case VelocityID::Final:     m_Velocity.Clear();
                                break;
	}
}

/*
 *  速度の減衰
 */
void
CPhysicsComponent::
VelocityDecay(void)
{
	m_Velocity.Move *= m_friction;
	m_Velocity.KnockBack *= m_friction;
	m_MoveAccelerator *= m_friction;

    // 速度が小さい場合は0にする
	if (abs(m_Velocity.Move.x) < m_friction * 2)
		this->SetVelocityX(VelocityID::Move, 0.0f);

    if (abs(m_Velocity.KnockBack.x) < m_friction)
        this->SetVelocityX(VelocityID::KnockBack, 0.0f);
}

/*
 *  当たり判定の更新
 */
void
CPhysicsComponent::
HitCapsuleUpdate(void)
{
    m_Capsule.Start     = m_Position + vivid::Vector2(m_Width / 2.0f, 0);
    m_Capsule.End       = m_Position + vivid::Vector2(m_Width / 2.0f, m_Height);
	m_Capsule.Radius    = m_Width / 2.0f;
}

/*
 *  速度の加算
 */
void
CPhysicsComponent::
AddVelocity(VelocityID id, vivid::Vector2 velocity)
{
    switch (id)
    {
    case VelocityID::Move:      m_Velocity.Move += velocity;
                                break;
    case VelocityID::Gravity:   m_Velocity.Gravity += velocity;
                                break;
    case VelocityID::KnockBack: m_Velocity.KnockBack += velocity;
                                break;
    case VelocityID::Final:     break;
	}
}

/*
 *  ジャンプ処理
 */
void
CPhysicsComponent::
Jump(float direction, float power)
{
    // ジャンプ力を移動速度のY成分に設定（上方向を負とする）
    this->AddVelocity(VelocityID::Move, vivid::Vector2(direction * (power / 5), -power));

    // ジャンプしたので着地フラグを下ろす
    m_LandingFlg = false;

    // ジャンプしたので重力加速度をリセット
    this->SetVelocityY(VelocityID::Gravity, 0.0f);

    CSoundManager::GetInstance().PlaySE(SOUND_ID::JUMP);
}

/*
 *  移動処理
 */
void
CPhysicsComponent::
Move(vivid::Vector2 direction, const CAttributeComponent& attribute_component)
{
	float speed = attribute_component.GetValue(ATTRIBUTE_ID::SPEED);

    float base_speed = attribute_component.GetBase(ATTRIBUTE_ID::SPEED);

    // 移動加速度を増加
	m_MoveAccelerator += speed;

    // 移動加速度の上限チェック
    if (m_MoveAccelerator > speed * 2.0f)
        m_MoveAccelerator = speed * 2.0f;

    // 移動加速度の設定
    if (direction.Length() != 0.0f)
    {
        // 移動方向を正規化して移動加速度を掛ける
        // 加速度を速度に加算して、加速度をリセット
        this->AddVelocity(VelocityID::Move, direction.Normalize() * m_MoveAccelerator);
    }
}

/*
 *  X速度の設定
 */
void
CPhysicsComponent::
SetVelocityX(VelocityID id, float velocity)
{
    switch (id)
    {
    case VelocityID::Move:      m_Velocity.Move.x = velocity;
                                break;

    case VelocityID::Gravity:   m_Velocity.Gravity.y = velocity;
                                break;

    case VelocityID::KnockBack: m_Velocity.KnockBack.x = velocity;
                                break;

    case VelocityID::Final:     break;
    }
}

/*
 *  Y速度の設定
 */
void
CPhysicsComponent::
SetVelocityY(VelocityID id, float velocity)
{
    switch (id)
    {
    case VelocityID::Move:      m_Velocity.Move.y = velocity;
                                break;

    case VelocityID::Gravity:   m_Velocity.Gravity.y = velocity;
                                m_GravityPower = 0.0f;
                                break;

    case VelocityID::KnockBack: m_Velocity.KnockBack.y = velocity;
                                break;

    case VelocityID::Final:     break;
	}
}

/*
 *  地面接地フラグの設定
 */
void
CPhysicsComponent::
SetLandingFlg(bool flg)
{
    m_LandingFlg = flg;

    if (flg)
    {
        if(m_Velocity.Gravity.y >= m_max_fall_speed)
        {
            // 着地硬直を設定
            if (!m_PlummetFlg)
            {
				this->SetLandingStiffnessFlg(true);

                CSoundManager::GetInstance().PlaySE(SOUND_ID::LANDING);

                vivid::Vector2 effect_pos = this->GetPosition();
                vivid::Vector2 scale = { CCameraManager::GetInstance().GetCameraScale() ,CCameraManager::GetInstance().GetCameraScale() };

                effect_pos *= CCameraManager::GetInstance().GetCameraScale();
                effect_pos -= CCameraManager::GetInstance().GetScroll();

                CEffectManager::GetInstance().Create(EFFECT_ID::JAMP, PLAYER_ID::MAX, effect_pos, DIRECTION::RIGHT, scale, Utility::GetColorById(COLOR_ID::WHITE), 0.0f);
            }
        }

        // 着地時にY速度をクリア
        this->SetVelocityY(VelocityID::Move, 0.0f);
        this->SetVelocityY(VelocityID::Gravity, 0.0f);
        this->SetVelocityY(VelocityID::KnockBack, 0.0f);
    }
}