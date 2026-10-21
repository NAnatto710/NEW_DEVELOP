
/*!
 *  @file       resource_csv_loader.h
 *  @brief      リソースCSVローダー
 *  @author     Ryusei Shimizu
 *  @date       2026/07/07
 */

#pragma once

#include "../../csv_loader.h"

#include "../../../../game_manager/game/player_manager/player/status/resource_component/resource_id.h"

#include <array>
#include <unordered_map>

/*!
 *	@class		CResourceCSVLoader
 *
 *	@brief		リソースCSVローダー
 *
 *	@author     Ryusei Shimizu
 *
 *  @date       2026/07/07
 */
class CResourceCSVLoader
    : public CCSVLoaderBase
{
public:

    /*!
     *  @brief コンストラクタ
     */
    CResourceCSVLoader();

    /*!
     *  @brief デストラクタ
     */
    ~CResourceCSVLoader() = default;

    /*!
     *  @brief CSVロード
     *
     *  @param[in] filepath CSVファイル
     *  @param[out] resources リソース
     *
     *  @return 読み込み成功
     */
    bool Load(const std::string& filepath, std::array<ResourceValue, static_cast<size_t>(RESOURCE_ID::MAX)>& resources);

private:

    /*!
     *  @brief 1行ロード
     * 
	 *  @param[in] line CSVの1行
	 *  @param[out] resources リソース
     * 
	 *  @return 読み込み成功
     */
    bool LoadLine(const std::string& line, std::array<ResourceValue, static_cast<size_t>(RESOURCE_ID::MAX)>& resources);

	static const std::unordered_map<std::string, RESOURCE_ID> m_Table;      //!< リソースID変換テーブル
};