
/*!
 *  @file		attack_component.cpp
 *  @brief		攻撃コンポーネント
 *  @author     Ryusei Shimizu
 *  @date       2026/08/25
 */

#include "attack_component.h"
#include "../../status/resource_component/resource_component.h"
#include "../../status/attribute_component/attribute_component.h"

// 腕の基準位置
const vivid::Vector2 CAttackComponent::m_left_arm_position = vivid::Vector2(35.0f, -5.0f);
const vivid::Vector2 CAttackComponent::m_right_arm_position = vivid::Vector2(-40.0f, -5.0f);

/*
 *  コンストラクタ
 */
CAttackComponent::
CAttackComponent()
	: m_LeftArm(ARM_ID::LEFT, m_left_arm_position)
	, m_RightArm(ARM_ID::RIGHT, m_right_arm_position)
{
}

/*
 *  初期化
 */
void
CAttackComponent::
Initialize(const vivid::Vector2& player_position, bool facing_right, const BuildData& build_data, std::string file_path)
{
	m_RightArm.Initialize(player_position, facing_right, build_data.Primary);
	m_LeftArm.Initialize(player_position, facing_right, build_data.Secondary);

	m_AttackCSVLoader.Load(file_path);
}

/*
 *  更新
 */
void
CAttackComponent::
Update(const vivid::Vector2& player_position, bool facing_right, const vivid::Vector2& attack_input, bool guarding)
{
	m_LeftArm.SetGuard(guarding);
	m_RightArm.SetGuard(guarding);

	m_LeftArm.Update(player_position, facing_right, attack_input);
	m_RightArm.Update(player_position, facing_right, attack_input);
}

/*
 *  右腕の描画
 */
void
CAttackComponent::
RightArmDraw(void)
{
	m_RightArm.Draw();
}

/*
 *  左腕の描画
 */
void
CAttackComponent::
LeftArmDraw(void)
{
	m_LeftArm.Draw();
}

/*
 *  解放
 */
void
CAttackComponent::
Finalize(void)
{
}

/*
 *  攻撃開始
 */
void
CAttackComponent::
Attack(ATTACK_ID attack_id, CResourceComponent& resource_component, CAttributeComponent& attribute_component, bool facing_right, ARM_ID arm_id)
{
	if (attack_id == ATTACK_ID::NONE) return;

	const AttackData& attack = m_AttackCSVLoader.GetAttackData(attack_id);

	m_RightArm.ClearHitTargets();
	m_LeftArm.ClearHitTargets();

	auto StartArmAttack = [&](CArm& arm, const AttackInfo& attack_info)
		{
			if (attack_info.ID == ATTACK_ID::NONE) return;

			// スキルの場合のみ欲望を消費
			if (!this->IsAttack(attack_info.ID))
			{
				const float cost_rate = attribute_component.GetValue(ATTRIBUTE_ID::COST_DESIRE);

				resource_component.AddCurrent(RESOURCE_ID::DESIRE, -attack_info.Damage.DesireCost * cost_rate);
			}

			arm.StartAttack(attack_info, facing_right);
		};


	/*
	 * 指定腕がRIGHTの場合
	 */
	if (arm_id == ARM_ID::RIGHT)
	{
		// RIGHT用データがあれば使用
		if (attack.RightArm.ID != ATTACK_ID::NONE)
		{
			StartArmAttack(m_RightArm, attack.RightArm);
		}
		// RIGHT用がなければLEFT用データを流用
		else if (attack.LeftArm.ID != ATTACK_ID::NONE)
		{
			StartArmAttack(m_RightArm, attack.LeftArm);
		}

		return;
	}


	/*
	 * 指定腕がLEFTの場合
	 */
	if (arm_id == ARM_ID::LEFT)
	{
		// LEFT用データがあれば使用
		if (attack.LeftArm.ID != ATTACK_ID::NONE)
		{
			StartArmAttack(m_LeftArm, attack.LeftArm);
		}
		// LEFT用がなければRIGHT用データを流用
		else if (attack.RightArm.ID != ATTACK_ID::NONE)
		{
			StartArmAttack(m_LeftArm, attack.RightArm);
		}

		return;
	}


	/*
	 * BOTHの場合
	 *
	 * 通常攻撃・同一ビルド用。
	 * CSVに書かれている左右の腕をそのまま使用する。
	 */

	if (attack.RightArm.ID != ATTACK_ID::NONE)
	{
		StartArmAttack(m_RightArm,attack.RightArm);
	}

	if (attack.LeftArm.ID != ATTACK_ID::NONE)
	{
		StartArmAttack(m_LeftArm, attack.LeftArm);
	}
}

/*
 *  攻撃キャンセル
 */
void
CAttackComponent::
Cancel(void)
{
	m_LeftArm.CancelAttack();
	m_RightArm.CancelAttack();
}

/*
 *  ガードブレイク演出開始
 */
void
CAttackComponent::
StartGuardBreak(int duration)
{
	m_LeftArm.StartGuardBreak(duration);
	m_RightArm.StartGuardBreak(duration);
}

/*
 *  攻撃中かどうかの取得
 */
bool
CAttackComponent::
IsAttacking(void) const
{
	return m_LeftArm.GetState() != ARM_STATE::IDLE ||
		m_RightArm.GetState() != ARM_STATE::IDLE;
}

/*
 *  攻撃中の腕の取得
 */
CArm*
CAttackComponent::
GetActiveAttackArm(void)
{
	if (m_RightArm.IsActiveFrame())
		return &m_RightArm;

	if (m_LeftArm.IsActiveFrame())
		return &m_LeftArm;

	return nullptr;
}

/*
 *  通常攻撃かどうかの取得
 */
bool
CAttackComponent::
IsAttack(ATTACK_ID attack_id) const
{
	return attack_id == ATTACK_ID::ATTACK_NEUTRAL ||
		attack_id == ATTACK_ID::ATTACK_SIDE ||
		attack_id == ATTACK_ID::ATTACK_UP ||
		attack_id == ATTACK_ID::ATTACK_DOWN;
}
