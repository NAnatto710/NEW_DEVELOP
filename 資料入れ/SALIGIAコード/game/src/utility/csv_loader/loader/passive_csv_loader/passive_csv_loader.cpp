/*!
 *  @file       passive_csv_loader.cpp
 *  @brief      パッシブCSVローダー
 *  @author     Ryusei Shimizu
 *  @date       2026/07/21
 */

#include "passive_csv_loader.h"

#include "../../csv_helper/csv_helper.h"
#include "../../csv_convert/csv_convert.h"

const std::unordered_map<std::string, SALIGIA_ID>
CPassiveCSVLoader::m_SaligiaTable =
{
    { "SUPERBIA", SALIGIA_ID::SUPERBIA },
    { "AVARITIA", SALIGIA_ID::AVARITIA },
    { "LUXURIA",  SALIGIA_ID::LUXURIA  },
    { "INVIDIA",  SALIGIA_ID::INVIDIA  },
    { "GULA",     SALIGIA_ID::GULA     },
    { "IRA",      SALIGIA_ID::IRA      },
    { "ACEDIA",   SALIGIA_ID::ACEDIA   },
};

const std::unordered_map<std::string, PASSIVE_CONDITION>
CPassiveCSVLoader::m_ConditionTable =
{
    { "NORMAL",         PASSIVE_CONDITION::NORMAL    },
    { "SAME",           PASSIVE_CONDITION::SAME      },
    { "MUYOKU",         PASSIVE_CONDITION::MUYOKU    },
};

const std::unordered_map<std::string, ATTRIBUTE_ID>
CPassiveCSVLoader::m_AttributeTable =
{
    { "SPEED",             ATTRIBUTE_ID::SPEED             },
    { "JUMP_POWER",        ATTRIBUTE_ID::JUMP_POWER        },
    { "ATTACK_POWER",      ATTRIBUTE_ID::ATTACK_POWER      },
    { "DESIRE_GAIN",       ATTRIBUTE_ID::DESIRE_GAIN       },
    { "COST_DESIRE",       ATTRIBUTE_ID::COST_DESIRE       },
    { "KNOCKBACK_POWER",   ATTRIBUTE_ID::KNOCKBACK_POWER   },
    { "KNOCKBACK_RESIST",  ATTRIBUTE_ID::KNOCKBACK_RESIST  },
    { "GUARD_POWER",       ATTRIBUTE_ID::GUARD_POWER       },
};

const std::unordered_map<std::string, ATTRIBUTE_MODIFIER_TYPE>
CPassiveCSVLoader::m_ModifierTypeTable =
{
    { "ADD",  ATTRIBUTE_MODIFIER_TYPE::ADD  },
    { "RATE", ATTRIBUTE_MODIFIER_TYPE::RATE },
};

/*
 * コンストラクタ
 */
CPassiveCSVLoader::
CPassiveCSVLoader()
{
}

/*
 * CSVロード
 */
bool
CPassiveCSVLoader::
Load(const std::string& filepath)
{
    // パッシブ情報を初期化
    m_PassiveInfos.clear();

    return LoadFile(filepath,
        [&](const std::string& line)
        {
            return LoadLine(line);
        });
}

/*
 * 1行ロード
 */
bool
CPassiveCSVLoader::
LoadLine(const std::string& line)
{
    // CSVを分割
    auto values = CSV::Split(line);

    // 列数が不正な場合は失敗
    if (!CSV::CheckColumnCount(values, ToIndex(PASSIVE_CSV::MAX)))
    {
        return false;
    }

    PassiveInfo passive;

    // 大罪
    if (!CSV::ParseEnum(m_SaligiaTable, values[ToIndex(PASSIVE_CSV::SALIGIA)], passive.Saligia))
    {
        return false;
    }

    // 条件
    if (!CSV::ParseEnum(m_ConditionTable, values[ToIndex(PASSIVE_CSV::CONDITION)], passive.Condition))
    {
        return false;
    }

	// 属性
    if (!CSV::ParseEnum(m_AttributeTable, values[ToIndex(PASSIVE_CSV::ATTRIBUTE)], passive.Attribute))
    {
        return false;
    }

	// 補正タイプ
    if (!CSV::ParseEnum(m_ModifierTypeTable, values[ToIndex(PASSIVE_CSV::TYPE)], passive.Modifier.Type))
    {
        return false;
    }

	// 補正値
    passive.Modifier.Value = CSV::Get<float>( values, ToIndex(PASSIVE_CSV::VALUE));

	// 補正値は有効
    passive.Modifier.Enable = true;

    // パッシブは永続
    passive.Modifier.IsPermanent = true;

    // パッシブ情報追加
    m_PassiveInfos.emplace_back(passive);

    return true;
}

/*
 * パッシブ情報取得
 */
PassiveInfo
CPassiveCSVLoader::
GetPassiveInfos(SALIGIA_ID id, PASSIVE_CONDITION condition) const
{
    PassiveInfo info{};
    info.Modifier.Enable = false;

    for (const auto& passive : m_PassiveInfos)
    {
        if (passive.Saligia != id)
            continue;

        if (passive.Condition != condition)
            continue;

        return passive;
    }

    return info;
}