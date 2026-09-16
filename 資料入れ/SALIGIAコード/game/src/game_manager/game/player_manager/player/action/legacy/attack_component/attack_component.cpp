
/*!
 *  @file		attack_component.cpp
 *  @brief		攻撃コンポーネント
 *  @author     Ryusei Shimizu
 *  @date       2026/05/26
 */

#include "attack_component.h"
#include "../../player.h"
#include "../../../../../../utility/utility.h"
#include "../../../../camera_manager/camera_manager.h"

/*
 *	コンストラクタ
 */
CAttackComponent::
CAttackComponent()
{
}

/*
 *	初期化
 */
void
CAttackComponent::
Initialize(CPlayer* owner)
{
	m_Owner = owner;

	m_AttackActive = false;
	m_CurrentAttack = nullptr;
	m_HitTargets.clear();

	Clear();
}

/*
 *	更新
 */
void
CAttackComponent::
Update()
{
	// 毎フレームヒットボックスを再生成するため、まずはクリアする
	m_HitCircles.clear();

    if (!m_Owner || !m_AttackActive) return;

    auto state = m_Owner->GetStateController().GetActionState();

	// 攻撃行動以外は処理しない
	switch (state)
	{
	case ACTION_STATE::ATTACK_NEUTRAL:
	case ACTION_STATE::ATTACK_SIDE:
	case ACTION_STATE::ATTACK_UP:
	case ACTION_STATE::ATTACK_DOWN:
	case ACTION_STATE::SKILLX:
	case ACTION_STATE::SKILLA:
		break;

	default:
		return;
	}

    int frame = m_Owner->GetStateController().GetFrame();

    // 攻撃発生中のみ有効
    if (m_CurrentAttack && m_CurrentAttack->IsActiveFrame(frame))
    {
        vivid::Vector2 position = m_Owner->GetPhysicsComponent().GetCenterPosition();
        float		   dir = m_Owner->GetPlayerData().Direction ? 1.0f : -1.0f;

		// ヒットボックスの生成
		for (const auto& circle : m_CurrentAttack->AttackCircles)
		{
			CircleHitBox hitBox;

			// 座標
			hitBox.Position.x = position.x + (circle.Offset.x * dir);
			hitBox.Position.y =	position.y + circle.Offset.y;

			// 半径
			hitBox.Radius = circle.Radius;

			float damage = m_Owner->GetAttributeComponent().GetValue(ATTRIBUTE_ID::ATTACK_POWER);
			float guard = m_Owner->GetAttributeComponent().GetValue(ATTRIBUTE_ID::GUARD_POWER);
			float knockback = m_Owner->GetAttributeComponent().GetValue(ATTRIBUTE_ID::KNOCKBACK_POWER);
			float desire = m_Owner->GetAttributeComponent().GetValue(ATTRIBUTE_ID::DESIRE_GAIN);
			float cost_desire = m_Owner->GetAttributeComponent().GetValue(ATTRIBUTE_ID::COST_DESIRE);

			// ダメージ情報
			hitBox.Damage.HpDamage = circle.Damage.HpDamage * damage;
			hitBox.Damage.GuardDamage = circle.Damage.GuardDamage * damage * guard;

			hitBox.Damage.KnockBack.x = circle.Damage.KnockBack.x * knockback * dir;
			hitBox.Damage.KnockBack.y =	circle.Damage.KnockBack.y * knockback;

			hitBox.Damage.Desire = circle.Damage.Desire + desire;
			hitBox.Damage.CostDesire = circle.Damage.CostDesire + cost_desire;

			hitBox.Damage.HitStun = circle.Damage.HitStun;
			hitBox.Damage.HitStop = circle.Damage.HitStop;

			hitBox.Damage.CameraShake.x = circle.Damage.CameraShake.x * dir;
			hitBox.Damage.CameraShake.y = circle.Damage.CameraShake.y;

			hitBox.Damage.InvincibleTime = circle.Damage.InvincibleTime;
			hitBox.Damage.IgnoreInvincible = circle.Damage.IgnoreInvincible;
			hitBox.Damage.CanGuard = circle.Damage.CanGuard;

			m_HitCircles.push_back(hitBox);
		}
	}
}

/*
 *	描画
 */
void
CAttackComponent::
DebugDraw()
{
	if (!m_AttackActive) return;

	int frame = m_Owner->GetStateController().GetFrame();

	vivid::Vector2 position = m_Owner->GetPhysicsComponent().GetCenterPosition();

	float dir = m_Owner->GetPlayerData().Direction ? 1.0f : -1.0f;

	// ヒットボックスの生成
	for (const auto& circle : m_CurrentAttack->AttackCircles)
	{
		CircleHitBox hitBox;

		// 座標
		hitBox.Position.x = position.x + (circle.Offset.x * dir);
		hitBox.Position.y = position.y + circle.Offset.y;

		// 半径
		hitBox.Radius = circle.Radius;

		vivid::Vector2 scroll = CCameraManager::GetInstance().GetScroll();
		float    camera_scale = CCameraManager::GetInstance().GetCameraScale();

		vivid::Vector2 draw_position = hitBox.Position;

		// カメラの拡大率を考慮して描画位置を調整
		draw_position *= camera_scale;
		// カメラのスクロールを考慮して描画位置を調整
		draw_position -= scroll;

		// 攻撃発生中のみ赤色で描画
		if (m_CurrentAttack && m_CurrentAttack->IsActiveFrame(frame))
		{
			DrawCircle(draw_position.x, draw_position.y, hitBox.Radius, Utility::GetColorById(COLOR_ID::RED));
		}
		else
		{
			DrawCircle(draw_position.x, draw_position.y, hitBox.Radius, Utility::GetColorById(COLOR_ID::WHITE));
		}
	}	
}

/*
 *	解放
 */
void
CAttackComponent::
Finalize()
{
}

/*
 *	現在の行動情報を設定
 */
void
CAttackComponent::
SetAttack(const AttackInfo& attack)
{
	m_AttackActive = true;
	m_HitTargets.clear();
	m_CurrentAttack = &attack;
}

/*
 *	行動情報のクリア
 */
void
CAttackComponent::
Clear()
{
	m_HitCircles.clear();
	m_HitTargets.clear();
	m_AttackActive = false;
	m_CurrentAttack = nullptr;
}

/*
 *	現在のヒットボックスを取得
 */
const std::vector<CircleHitBox>&
CAttackComponent::
GetHitCircles() const
{
    return m_HitCircles;
}

/*
 *	特定のキャラクターに既にヒットしているかどうかを確認
 */
bool
CAttackComponent::
HasHit(CPlayer* target) const
{
	return m_HitTargets.find(target) != m_HitTargets.end();
}

/*
 *	特定のキャラクターをヒット対象として追加
 */
void
CAttackComponent::
AddHitTarget(CPlayer* target)
{
	m_HitTargets.insert(target);
}
