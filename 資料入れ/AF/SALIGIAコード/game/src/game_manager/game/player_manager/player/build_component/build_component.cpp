
/*!
 *  @file		build_component.cpp
 *  @brief		ビルドコンポーネント
 *  @author     Ryusei Shimizu
 *  @date       2026/06/26
 */

#include "build_component.h"
#include "../status/attribute_component/attribute_component.h"
#include "../state_controller/state_controller.h"

namespace
{
	void ResetPassiveModifiers(std::array<AttributeModifier, ToIndex(ATTRIBUTE_ID::MAX)>& modifiers)
	{
		for (auto& modifier : modifiers)
		{
			modifier.Enable = false;
			modifier.IsPermanent = true;
			modifier.Time = 0;

			modifier.Type = ATTRIBUTE_MODIFIER_TYPE::ADD;
			modifier.Value = 0.0f;
		}
	}
}

/*
 *	コンストラクタ
 */
CBuildComponent::
CBuildComponent()
	: m_IsPassiveApplied(false)
	, m_StateController(nullptr)
	, m_PreviousDesireState(DESIRE_STATE::NORMAL)
{
}

/*
 *	初期化
 */
void
CBuildComponent::
Initialize(const BuildData& build, const std::string& passive_csv_path,	CStateController* state_controller)
{
	m_Build = build;
	m_StateController = state_controller;

	ResetPassiveModifiers(m_NormalPassive);
	ResetPassiveModifiers(m_MuyokuPassive);

	m_PreviousDesireState = DESIRE_STATE::NORMAL;

	// 初回はパッシブを適用する
	m_IsPassiveApplied = false;

	// パッシブCSVを読み込み
	m_PassiveCSVLoader.Load(passive_csv_path);

	// パッシブ情報を読み込み
	LoadPassive();
}

/*
 *	パッシブ読み込み
 */
void
CBuildComponent::
LoadPassive(void)
{
	ResetPassiveModifiers(m_NormalPassive);
	ResetPassiveModifiers(m_MuyokuPassive);

	// 1つ目の欲望の通常パッシブを取得
	const auto passive = m_PassiveCSVLoader.GetPassiveInfos(m_Build.Primary, PASSIVE_CONDITION::NORMAL);

	ApplyPassive(m_NormalPassive, passive);

	// 2つ目の欲望が1つ目の欲望と同じ場合は、SAME条件のパッシブを取得する
	if (IsSameBuild())
	{
		// 2つ目の欲望の重複パッシブを取得
		const auto passive = m_PassiveCSVLoader.GetPassiveInfos(m_Build.Primary, PASSIVE_CONDITION::SAME);

		ApplyPassive(m_NormalPassive, passive);
	}
	else
	{
		// 2つ目の欲望の通常パッシブを取得
		const auto passive = m_PassiveCSVLoader.GetPassiveInfos(m_Build.Secondary, PASSIVE_CONDITION::NORMAL);

		ApplyPassive(m_NormalPassive, passive);
	}

	// 無欲パッシブを取得
	const auto passive_p = m_PassiveCSVLoader.GetPassiveInfos(m_Build.Primary, PASSIVE_CONDITION::MUYOKU);

	ApplyPassive(m_MuyokuPassive, passive_p);
	
	const auto passive_s = m_PassiveCSVLoader.GetPassiveInfos(m_Build.Secondary, PASSIVE_CONDITION::MUYOKU);

	ApplyPassive(m_MuyokuPassive, passive_s);
}

/*
 *	パッシブ更新
 */
void
CBuildComponent::
PassiveUpdate(CAttributeComponent& attribute_component)
{
	// StateControllerが存在しない場合は処理しない
	if (m_StateController == nullptr) return;

	// 現在の欲望状態を取得
	const DESIRE_STATE currentDesireState =	m_StateController->GetDesireState();

	// 初回適用後で、状態も変わっていないなら更新不要
	if (m_IsPassiveApplied && currentDesireState == m_PreviousDesireState) return;

	// 前回の欲望状態を更新
	m_PreviousDesireState = currentDesireState;
	m_IsPassiveApplied = true;

	const auto& passives = (currentDesireState == DESIRE_STATE::MUYOKU) ? m_MuyokuPassive : m_NormalPassive;

	// パッシブを属性コンポーネントに適用
	for (size_t i = 0; i < passives.size(); ++i)
	{
		attribute_component.SetPassiveModifier(static_cast<ATTRIBUTE_ID>(i), passives[i]);
	}
}

/*
 *	パッシブ適用
 */
void
CBuildComponent::
ApplyPassive(std::array<AttributeModifier, ToIndex(ATTRIBUTE_ID::MAX)>& passiveModifiers, const PassiveInfo& passiveInfo)
{
	// パッシブが存在しない
	if (!passiveInfo.Modifier.Enable)
		return;

	auto& modifier = passiveModifiers[ToIndex(passiveInfo.Attribute)];

	// この属性にまだパッシブが設定されていない
	if (!modifier.Enable)
	{
		modifier = passiveInfo.Modifier;

		modifier.Enable = true;
		modifier.IsPermanent = true;
		modifier.Time = 0;

		return;
	}

	// 同じ属性でタイプが違う場合
	// 現在の構造では同時保持できないため処理しない
	if (modifier.Type != passiveInfo.Modifier.Type)	return;

	switch (modifier.Type)
	{
	case ATTRIBUTE_MODIFIER_TYPE::ADD:		modifier.Value += passiveInfo.Modifier.Value;	break;
	case ATTRIBUTE_MODIFIER_TYPE::RATE:		modifier.Value *= passiveInfo.Modifier.Value;	break;
	}
}

/*
 *	ビルドデータ取得
 */
const BuildData&
CBuildComponent::
GetBuild() const
{
	return m_Build;
}

/*
 *	選ばれたビルドが同じかどうかを判定
 */
bool
CBuildComponent::
IsSameBuild(void) const
{
	return m_Build.Primary == m_Build.Secondary;
}