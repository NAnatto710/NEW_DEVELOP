
/*!
 *  @file       upgrade_status.h
 *  @brief      プレイヤーの強化ステータス
 *  @author     Ryusei Shimizu
 *  @date       2026/02/04
 */

#pragma once

#include "vivid.h"
#include "upgrade_status_id.h"
#include <vector>

/*!
 *  @class      CUpgradeStatus
 *
 *  @brief      プレイヤーの強化ステータスクラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2026/02/04
 */
class CUpgradeStatus
{
public:

	/*!
	 *  @brief      コンストラクタ
	 */
	CUpgradeStatus(void);

	/*!
	 *  @brief      デストラクタ
	 */
	~CUpgradeStatus(void);

	/*!
	 *  @brief      初期化
	 */
	void				Initialize(void);

	/*!
	 *  @brief      更新
	 */
	void				Update(void);

	/*!
	 *	@brief		描画
	 */
	void				Draw(void);

	/*!
	 *  @brief      解放
	 */
	void				Finalize(void);

	/*!
	 *  @brief      強化ステータスID取得
	 *
	 *  @return     強化ステータスID
	 */
	UPGRADE_STATUS_ID   GetUpgradeStatusID(UPGRADE_STATUS_ID id) const;

	/*!
	 *  @brief      強化ステータスID設定
	 *
	 *  @param[in]  id  強化ステータスID
	 */
	void 				SetUpgradeStatusID(UPGRADE_STATUS_ID id);

	/*!
	 *  @brief      強化ステータスカウント取得
	 *
	 *  @return     強化ステータスカウント
	 */
	int                 GetUpgradeStatusCount(UPGRADE_STATUS_ID id) const;

	/*!
	 *  @brief      強化ステータス取得
	 *
	 *  @return     強化ステータス
	 */
	float               GetUpgrade(UPGRADE_STATUS_ID id) const;

	/*!
	 *  @brief      強化ステータスID取得
	 *
	 *  @return     強化ステータスID
	 */
	UPGRADE_STATUS_ID   GetUpgradeStatusID() const;

	/*!
	 *  @brief      満腹度設定
	 *
	 *  @param[in]  value   満腹度
	 */
	void				SetFullness(float value);

private:

	static const float		m_default_status_mag;									//!< ステータス倍率の初期値
	static const float      m_status_mag[(int)UPGRADE_STATUS_ID::MAX];				//!< ステータス倍率

	float					m_Fullness;												//!< 満腹度
	float					m_StatusMag[(int)UPGRADE_STATUS_ID::MAX];				//!< ステータス倍率
	int						m_UpgradeStatus[(int)UPGRADE_STATUS_ID::MAX];			//!< 強化ステータスID

	UPGRADE_STATUS_ID		m_UpgradeStatusID;										//!< 強化ステータスID
};
