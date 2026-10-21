
/*!
 *  @file		guard_component.cpp
 *  @brief		ガードコンポーネント
 *  @author     Ryusei Shimizu
 *  @date       2026/07/01
 */

#include "guard_component.h"
#include "../../player.h"

const int   CGuardComponent::m_just_guard_time	        = 5;	//!< ジャストガード時間
const int   CGuardComponent::m_break_stiffness_time     = 120;	//!< ガードブレイク時の硬直時間
const int   CGuardComponent::m_guard_reduce_interval    = 15;	//!< ガード耐久減少間隔
const float CGuardComponent::m_guard_reduce_value       = 1.5f;	//!< ガード耐久減少量


/*
 *	コンストラクタ
 */
CGuardComponent::
CGuardComponent(void)
{
}

/*
 *	初期化
 */
void
CGuardComponent::
Initialize(void)
{
	m_Guarding = false;
	m_Break = false;

	m_JustGuard = false;
	m_GuardTimer = m_guard_reduce_interval;
	m_JustGuardTimer = 0;
}

/*
 *	更新
 */
void
CGuardComponent::
Update(CResourceComponent& resource_component)
{
	// ガード中でなければ処理しない
	if (!m_Guarding)
		return;

	if (m_JustGuardTimer > 0)
		m_JustGuardTimer--;

	// ガード耐久減少タイマーを加算
    m_GuardTimer++;

	// ガード耐久減少
    if (m_GuardTimer >= m_guard_reduce_interval)
    {
        m_GuardTimer = 0;

		resource_component.AddCurrent(RESOURCE_ID::GUARD, -m_guard_reduce_value);
    }

	// ガード値が0以下になった場合、ガードブレイク状態にする
    if (resource_component.IsZero(RESOURCE_ID::GUARD))
    {
        m_Break = true;

        m_Guarding = false;
    }
}

/*
 *	ガード開始
 */
void
CGuardComponent::
Start(void)
{
	// ガードブレイク中はガードを開始しない
	if (m_Break || m_Guarding) return;

	// ガード中フラグを立てる
	m_Guarding = true;

	// ガード耐久減少タイマーをリセット
	m_GuardTimer = 0;

	// ジャストガードフラグをリセット
	m_JustGuard = false;

	// ジャストガードタイマーをリセット
	m_JustGuardTimer = m_just_guard_time;
}

/*
 *	ガード終了
 */
void
CGuardComponent::
End(void)
{
	m_Guarding = false;
}

/*
 *	ガード処理
 */
void
CGuardComponent::
Guard(const DamageInfo& damage, CPlayer& player)
{
	// ジャストガードタイマーが0より大きい場合、ジャストガード成功
    if (m_JustGuardTimer > 0)
    {
		m_JustGuard = true;

        return;
    }

	// ガード値を減少させる
	player.GetResourceComponent().AddCurrent(RESOURCE_ID::GUARD, -damage.GuardDamage);

	// 少しノックバックを加える
	player.GetPhysicsComponent().AddVelocity(VelocityID::KnockBack, damage.KnockBack * 0.2f);

	// ガード値が0以下になった場合、ガードブレイク状態にする
    if (player.GetResourceComponent().IsZero(RESOURCE_ID::GUARD))
    {
        m_Break = true;
        m_Guarding = false;
    }
}

/*
 *	ガードブレイクフラグリセット
 */
void
CGuardComponent::
ResetBreak(CResourceComponent& resource_component)
{
	m_Guarding = false;
	m_Break = false;
	resource_component.ResetCurrent(RESOURCE_ID::GUARD);
}
