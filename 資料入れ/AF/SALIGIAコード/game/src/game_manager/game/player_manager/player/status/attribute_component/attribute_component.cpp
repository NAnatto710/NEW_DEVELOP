
/*!
 *  @file		attribute_component.cpp
 *  @brief		キャラクターの属性コンポーネント
 *  @author     Ryusei Shimizu
 *  @date       2026/07/07
 */

#include "attribute_component.h"
#include "vivid.h"

/*
 *  @brief リソースIDをインデックスに変換
 *
 *  @param[in] id リソースID
 *
 *  @return インデックス
 */
static constexpr size_t
ToIndex(ATTRIBUTE_ID id)
{
	return static_cast<size_t>(id);
}

/*
 *  コンストラクタ
 */
CAttributeComponent::
CAttributeComponent()
{
}

/*
 *  初期化
 */
void
CAttributeComponent::
Initialize(const std::string& attribute_csv_path)
{
    // 基本値を完全初期化
    m_Attribute = {};

    // 前試合の一時補正を全削除
    m_Modifier.clear();

    // パッシブ完全初期化
    for (auto& passive : m_Passive)
    {
        passive.Enable = false;
        passive.IsPermanent = true;
        passive.Time = 0;
        passive.Type = ATTRIBUTE_MODIFIER_TYPE::ADD;
        passive.Value = 0.0f;
    }

    m_AttributeCSVLoader.Load(attribute_csv_path, m_Attribute);
}

/*
 *  更新
 */
void
CAttributeComponent::
Update()
{
	// 補正値の更新
    for (auto itr = m_Modifier.begin(); itr != m_Modifier.end();)
    {
		// 補正値の参照を取得
        auto& modifiers = itr->second;

		// 補正値の時間を減少
        for (auto& modifier : modifiers)
        {
			// 補正値が永続でない場合は時間を減少
            if (!modifier.IsPermanent)
            {
                --modifier.Time;
            }
        }

		// 有効期限切れの補正値を削除
        modifiers.erase(std::remove_if(modifiers.begin(), modifiers.end(),
                [](const AttributeModifier& modifier)
                {
                    return !modifier.IsPermanent && modifier.IsExpired();
                }),
            modifiers.end());

		// 補正値が空の場合はマップから削除
        if (modifiers.empty())
        {
            itr = m_Modifier.erase(itr);
        }
        else
        {
            ++itr;
        }
    }
}

/*
 *  基本値取得
 */
float
CAttributeComponent::
GetBase(ATTRIBUTE_ID id) const
{
    return m_Attribute[ToIndex(id)].Base;
}

/*
 *  補正値取得
 */
float
CAttributeComponent::
GetValue(ATTRIBUTE_ID id) const
{
    float value = GetBase(id);
    float add = 0.0f;
    float rate = 1.0f;

	// パッシブ補正値を取得
    const auto& passive = m_Passive[ToIndex(id)];

    if (passive.Enable)
    {
        switch (passive.Type)
        {
        case ATTRIBUTE_MODIFIER_TYPE::ADD:      add += passive.Value;   break;
        case ATTRIBUTE_MODIFIER_TYPE::RATE:     rate *= passive.Value;  break;
        }
    }

    // その他の補正
    const auto modifiers = m_Modifier.find(id);

    if (modifiers != m_Modifier.end())
    {
        for (const auto& modifier : modifiers->second)
        {
            if (!modifier.Enable)
                continue;

            switch (modifier.Type)
            {
            case ATTRIBUTE_MODIFIER_TYPE::ADD:      add += modifier.Value;      break;
            case ATTRIBUTE_MODIFIER_TYPE::RATE:     rate *= modifier.Value;     break;
            }
        }
    }

    return (value + add) * rate;
}

/*
 *  基本値設定
 */
void
CAttributeComponent::
SetBase(ATTRIBUTE_ID id, float value)
{
	m_Attribute[ToIndex(id)].Base = value;
}

/*
 *  補正値追加
 */
void
CAttributeComponent::
AddModifier(ATTRIBUTE_ID id, const AttributeModifier& modifier)
{
	m_Modifier[id].push_back(modifier);
}

/*
 *  補正値クリア
 */
void
CAttributeComponent::
ClearModifier(ATTRIBUTE_ID id)
{
    m_Modifier.erase(id);
}

/*
 *  パッシブ補正値設定
 */
void
CAttributeComponent::
SetPassiveModifier(ATTRIBUTE_ID id, const AttributeModifier& modifier)
{
	m_Passive[ToIndex(id)] = modifier;
}

/*
 *  パッシブ補正値クリア
 */
void
CAttributeComponent::
ClearPassiveModifier(ATTRIBUTE_ID id)
{
    auto& passive = m_Passive[ToIndex(id)];

    passive.Enable = false;
    passive.Type = ATTRIBUTE_MODIFIER_TYPE::ADD;
    passive.Value = 0.0f;
}
