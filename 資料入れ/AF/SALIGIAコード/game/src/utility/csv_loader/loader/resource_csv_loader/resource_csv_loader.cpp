/*!
 *  @file       resource_csv_loader.cpp
 *  @brief      リソースCSVローダー
 *  @author     Ryusei Shimizu
 *  @date       2026/07/07
 */

#include "resource_csv_loader.h"

#include "../../csv_helper/csv_helper.h"
#include "../../csv_convert/csv_convert.h"

#include "../../../utility.h"

const std::unordered_map<std::string, RESOURCE_ID> CResourceCSVLoader::m_Table =
{
    { "HP", RESOURCE_ID::HP },
    { "DESIRE", RESOURCE_ID::DESIRE },
    { "GUARD", RESOURCE_ID::GUARD },
    { "LIFE", RESOURCE_ID::LIFE }
};

/*
 *  属性IDをインデックスに変換
 */
namespace
{
    constexpr size_t ToIndex(RESOURCE_ID id)
    {
        return static_cast<size_t>(id);
    }
}

/*
 *  コンストラクタ
 */
CResourceCSVLoader::
CResourceCSVLoader()
{
}

/*
 *  CSVロード
 */
bool
CResourceCSVLoader::
Load(const std::string& filepath, std::array<ResourceValue, static_cast<size_t>(RESOURCE_ID::MAX)>& resources)
{
	// CSVをロード
    return LoadFile(filepath,
        [&](const std::string& line)
        {
            return LoadLine(line, resources);
        });
}

/*
 *  1行ロード
 */
bool
CResourceCSVLoader::
LoadLine(const std::string& line, std::array<ResourceValue, static_cast<size_t>(RESOURCE_ID::MAX)>& resources)
{
	// CSVを分割
    auto values = CSV::Split(line);

	// 列数が4でない場合は失敗
    if (!CSV::CheckColumnCount(values, 4))
    {
        return false;
    }

    RESOURCE_ID id;

	// リソースIDを変換できなかった場合は失敗
    if (!CSV::ParseEnum(m_Table, values[0], id)) 
    {
        return false;
    }

	// リソースを取得
    auto& resource = resources[ToIndex(id)];

	// リソースの値を取得
    resource =
    {
        CSV::Get<float>(values,1),
        CSV::Get<float>(values,2),
        CSV::Get<float>(values,3)
    };

	// 現在値を最小値と最大値の範囲に収める
    resource.Current = CLAMP(resource.Current, resource.Min, resource.Max);

    return true;
}