/*!
 *  @file       passive_csv_loader.h
 *  @brief      パッシブCSVローダー
 *  @author     Ryusei Shimizu
 *  @date       2026/07/21
 */

#pragma once

#include "../../csv_loader.h"

#include "passive_id.h"

#include <vector>
#include <unordered_map>

/*
 *  パッシブCSVをインデックスに変換
 */
namespace
{
    constexpr size_t ToIndex(PASSIVE_CSV id)
    {
        return static_cast<size_t>(id);
    }
}

/*!
 *	@class		CPassiveCSVLoader
 *
 *	@brief		パッシブCSVローダー
 *
 *	@author     Ryusei Shimizu
 *
 *  @date       2026/07/21
 */
class CPassiveCSVLoader
    : public CCSVLoaderBase
{
public:

    /*!
     *  @brief コンストラクタ
     */
    CPassiveCSVLoader();

    /*!
     *  @brief デストラクタ
     */
    ~CPassiveCSVLoader() = default;

    /*!
     *  @brief CSVロード
     *
     *  @param[in] filepath CSVファイル
     *
     *  @return 読み込み成功
     */
    bool Load(const std::string& filepath);

    /*!
     *  @brief パッシブ情報取得
     *
     *  @param[in] id 大罪ID
     *  @param[in] condition 発動条件
     *
     *  @return パッシブ情報
     */
    PassiveInfo GetPassiveInfos(SALIGIA_ID id, PASSIVE_CONDITION condition) const;

private:

    /*!
     *  @brief 1行ロード
     *
     *  @param[in] line CSV1行
     *
     *  @return 読み込み成功
     */
    bool LoadLine(const std::string& line);

    static const std::unordered_map<std::string, SALIGIA_ID>                m_SaligiaTable;         //! 大罪ID変換テーブル
    static const std::unordered_map<std::string, PASSIVE_CONDITION>         m_ConditionTable;       //! パッシブ条件変換テーブル
    static const std::unordered_map<std::string, ATTRIBUTE_ID>              m_AttributeTable;       //! 属性ID変換テーブル
    static const std::unordered_map<std::string, ATTRIBUTE_MODIFIER_TYPE>   m_ModifierTypeTable;    //! 補正タイプ変換テーブル

    std::vector<PassiveInfo> m_PassiveInfos;    //! パッシブ情報
};