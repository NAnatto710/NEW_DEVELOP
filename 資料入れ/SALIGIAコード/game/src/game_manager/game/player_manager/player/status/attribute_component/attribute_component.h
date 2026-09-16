
/*!
 *  @file		attribute_component.h
 *  @brief		キャラクターの属性コンポーネント
 *  @author     Ryusei Shimizu
 *  @date       2026/07/07
 */

#pragma once

#include "attribute_id.h"
#include <array>
#include <unordered_map>

#include "../../../../../../utility/csv_loader/loader/attribute_csv_loader/attribute_csv_loader.h"

/*!
 *	@class		CAttributeComponent
 *
 *	@brief		属性管理コンポーネント
 *
 *	@author     Ryusei Shimizu
 *
 *  @date       2026/07/07
 */
class CAttributeComponent
{
public:

    /*!
	 *  @brief コンストラクタ
     */
	CAttributeComponent();

	/*!
	 *  @brief デストラクタ
     */
    ~CAttributeComponent() = default;

    /*!
     *  @brief 初期化
	 */
    void Initialize(const std::string& attribute_csv_path);

    /*!
	 *  @brief 更新
     */
	void Update();

    /*!
     *  @brief 基本値取得
     * 
	 *  @param[in] id 補正ID
     * 
	 *  @return 基本値
	 */
    float GetBase(ATTRIBUTE_ID id) const;

    /*!
     *  @brief 補正値取得
     * 
	 *  @param[in] id 補正ID
     * 
	 *  @return 補正値
	 */
    float GetValue(ATTRIBUTE_ID id) const;

    /*!
     *  @brief 基本値設定
	 *  
	 *  @param[in] id 補正ID
	 *  @param[in] value 基本値
     */
    void SetBase(ATTRIBUTE_ID id, float value);

    /*!
     *  @brief 補正値追加
     * 
	 *  @param[in] id 補正ID
	 *  @param[in] modifier 補正値
	 */
    void AddModifier(ATTRIBUTE_ID id, const AttributeModifier& modifier);

    /*!
     *  @brief 補正値クリア
     * 
	 *  @param[in] id 補正ID
	 */
    void ClearModifier(ATTRIBUTE_ID id);

    /*!
     *  @brief パッシブ補正値設定
     * 
	 *  @param[in] id 補正ID
	 *  @param[in] modifier 補正値
	 */
    void SetPassiveModifier(ATTRIBUTE_ID id, const AttributeModifier& modifier);

    /*!
     *  @brief パッシブ補正値クリア
     * 
	 *  @param[in] id 補正ID
	 */
    void ClearPassiveModifier(ATTRIBUTE_ID id);

private:

	CAttributeCSVLoader m_AttributeCSVLoader;    //!< 属性CSVローダー

	std::array<AttributeValue, static_cast<size_t>(ATTRIBUTE_ID::MAX)>      m_Attribute;    //!< 基本ステータス
	std::array<AttributeModifier, static_cast<size_t>(ATTRIBUTE_ID::MAX)>   m_Passive;      //!< パッシブ補正値
	std::unordered_map<ATTRIBUTE_ID, std::vector<AttributeModifier>>        m_Modifier;     //!< 属性補正値マップ
};