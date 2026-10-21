
/*!
 *  @file       attribute_csv_loader.cpp
 *  @brief      属性CSVローダー
 *  @author     Ryusei Shimizu
 *  @date       2026/07/07
 */

#include "attribute_csv_loader.h"

#include "../../csv_helper/csv_helper.h"
#include "../../csv_convert/csv_convert.h"

const std::unordered_map<std::string, ATTRIBUTE_ID> CAttributeCSVLoader::m_Table =
{
    {"SPEED", ATTRIBUTE_ID::SPEED},
    {"JUMP_POWER", ATTRIBUTE_ID::JUMP_POWER},
    {"ATTACK_POWER", ATTRIBUTE_ID::ATTACK_POWER},
	{"DESIRE_GAIN", ATTRIBUTE_ID::DESIRE_GAIN},
	{"COST_DESIRE", ATTRIBUTE_ID::COST_DESIRE},
    {"KNOCKBACK_POWER", ATTRIBUTE_ID::KNOCKBACK_POWER},
    {"KNOCKBACK_RESIST", ATTRIBUTE_ID::KNOCKBACK_RESIST},
    {"GUARD_POWER", ATTRIBUTE_ID::GUARD_POWER},
};

/*
 *  属性IDをインデックスに変換
 */
namespace
{
    constexpr size_t ToIndex(ATTRIBUTE_ID id)
    {
        return static_cast<size_t>(id);
    }
}

/*
 *  コンストラクタ
 */
CAttributeCSVLoader::
CAttributeCSVLoader()
{
}

/*
 *  CSVロード
 */
bool
CAttributeCSVLoader::
Load(const std::string& filepath, std::array<AttributeValue, static_cast<size_t>(ATTRIBUTE_ID::MAX)>& attributes)
{
	// CSVをロード
    return LoadFile(filepath,
        [&](const std::string& line)
        {
            return LoadLine(line, attributes);
        });
}

/*
 *  1行ロード
 */
bool
CAttributeCSVLoader::
LoadLine(const std::string& line, std::array<AttributeValue, static_cast<size_t>(ATTRIBUTE_ID::MAX)>& attributes)
{
	// CSVを分割
    auto values = CSV::Split(line);

	// 列数が不正な場合は失敗
    if (!CSV::CheckColumnCount(values, 2))
        return false;

    ATTRIBUTE_ID id;

	// 属性IDを変換
    if (!CSV::ParseEnum(m_Table, values[0], id))
    {
        return false;
    }

    attributes[ToIndex(id)].Base = CSV::Get<float>(values, 1);

    return true;
}