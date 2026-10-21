
/*!
 *  @file		resource_component.h
 *  @brief		キャラクターのリソースコンポーネント
 *  @author     Ryusei Shimizu
 *  @date       2026/07/07
 */

#pragma once

#include "resource_id.h"
#include <array>

#include "../../../../../../utility/csv_loader/loader/resource_csv_loader/resource_csv_loader.h"

/*!
 *	@class		CResourceComponent
 *
 *	@brief		リソース管理クラス
 *
 *	@author     Ryusei Shimizu
 *
 *  @date       2026/07/07
 */
class CResourceComponent
{
public:

    /*!
     *  @brief コンストラクタ
	 */
	CResourceComponent(void);

	/*!
	 *  @brief デストラクタ
     */
	~CResourceComponent(void) = default;

    /*!
     *  @brief 初期化
	 */
    void Initialize(const std::string& resource_csv_path);

	/*!
	 *  @brief 更新
	 */
	void Update(void);

    /*!
     *  @brief 現在値取得
     * 
	 *  @param[in] id リソースID
     * 
	 *  @return 現在値
	 */
    float GetCurrent(RESOURCE_ID id) const;

    /*!
	 *  @brief 最大値取得
     * 
	 *  @param[in] id リソースID
     * 
	 *  @return 最大値
     */
    float GetMax(RESOURCE_ID id) const;

	/*!
	 *  @brief 最小値取得
     * 
	 *  @param[in] id リソースID
     * 
	 *  @return 最小値
     */
    float GetMin(RESOURCE_ID id) const;

    /*!
     *  @brief 現在値設定
	 *
	 *  @param[in] id リソースID
	 *  @param[in] value 設定値
     */
    void SetCurrent(RESOURCE_ID id, float value);

    /*!
	 *  @brief 現在値加算
     * 
	 *  @param[in] id リソースID
	 *  @param[in] value 加算値
     */
    void AddCurrent(RESOURCE_ID id, float value);

	/*!
	 *  @brief 現在値リセット
     * 
	 *  @param[in] id リソースID
     */
	void ResetCurrent(RESOURCE_ID id);

	/*!
	 *  @brief 現在値クリア
     * 
	 *  @param[in] id リソースID
     */
	void ClearCurrent(RESOURCE_ID id);

	/*!
	 *  @brief リソースが0かどうかを判定
     * 
	 *  @param[in] id リソースID
     * 
	 *  @return リソースが0かどうか
     */
    bool IsZero(RESOURCE_ID id) const;

	/*!
	 *  @brief リソースが最大値かどうかを判定
     * 
	 *  @param[in] id リソースID
     * 
	 *  @return リソースが最大値かどうか
     */
    bool IsMax(RESOURCE_ID id) const;

private:

	static const int	m_desire_add_interval;	//!< 欲望加算間隔
	static const float	m_desire_add_value;		//!< 欲望加算値

	int					m_DesireAddTimer;		//!< 欲望加算タイマー

	CResourceCSVLoader	m_ResourceCSVLoader;	//!< リソースCSVローダー

	std::array<ResourceValue, static_cast<size_t>(RESOURCE_ID::MAX)> m_Resources;   //!< リソース構造体配列
};