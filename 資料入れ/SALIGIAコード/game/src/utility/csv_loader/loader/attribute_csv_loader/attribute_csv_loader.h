
/*!
 *  @file       attribute_csv_loader.h
 *  @brief      属性CSVローダー
 *  @author     Ryusei Shimizu
 *  @date       2026/07/07
 */

#pragma once

#include "../../csv_loader.h"

#include "../../../../game_manager/game/player_manager/player/status/attribute_component/attribute_id.h"

#include <array>
#include <unordered_map>

/*!
 *	@class		CAttributeCSVLoader
 *
 *	@brief		属性CSVローダー
 *
 *	@author     Ryusei Shimizu
 *
 *  @date       2026/07/07
 */
class CAttributeCSVLoader
    : public CCSVLoaderBase
{
public:

    /*!
     *  @brief コンストラクタ
     */
    CAttributeCSVLoader();

    /*!
     *  @brief デストラクタ
     */
    ~CAttributeCSVLoader() = default;

    /*!
     *  @brief CSVロード
     *
     *  @param[in] filepath CSVファイル
     *  @param[out] attributes 属性
     *
     *  @return 読み込み成功
     */
    bool Load(const std::string& filepath, std::array<AttributeValue, static_cast<size_t>(ATTRIBUTE_ID::MAX)>& attributes);

private:

    /*!
     *  @brief 1行ロード
     *
     *  @param[in] line CSVの1行
     *  @param[out] attributes 属性
     *
     *  @return 読み込み成功
     */
    bool LoadLine( const std::string& line, std::array<AttributeValue, static_cast<size_t>(ATTRIBUTE_ID::MAX)>& attributes);

	static const std::unordered_map<std::string, ATTRIBUTE_ID> m_Table;     //!< 属性ID変換テーブル
};