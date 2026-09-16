/*
 *  @file       upgrade_status.cpp
 *  @brief      プレイヤーの強化ステータス
 *  @author     Ryusei Shimizu
 *  @date       2026/02/04
 */

#include "upgrade_status.h"

const float CUpgradeStatus::m_default_status_mag	= 0.5f;		//!< 強化ステータス倍率初期値

const float CUpgradeStatus::m_status_mag[(int)UPGRADE_STATUS_ID::MAX] =
{
	0.6f,					//!< 弾のダメージ倍率
	0.0f,							//!< 弾のクールタイム
	0.0f,							//!< 弾の持続時間
	0.0f,							//!< 弾のスピード
	0.8f,					//!< 突進のダメージ倍率
	0.0f, 							//!< 突進のクールタイム
	0.02f, 					//!< 突進の持続時間
	0.1f, 					//!< 突進のスピード
};

/*
 *  コンストラクタ
 */
CUpgradeStatus::
CUpgradeStatus(void)
	: m_Fullness(0.0f)
{
}

/*
 *  デストラクタ
 */
CUpgradeStatus::
~CUpgradeStatus(void)
{
}

/*
 *  初期化
 */
void
CUpgradeStatus::
Initialize(void)
{
	// 強化ステータス倍率初期化
	for (int i = 0; i < (int)UPGRADE_STATUS_ID::MAX; i++)
	{
		m_StatusMag[i] = m_default_status_mag;
		m_UpgradeStatus[i] = 0;
	}

	// 強化ステータスID初期化
	m_UpgradeStatusID = UPGRADE_STATUS_ID::MAX;

	// 満腹度初期化
	m_Fullness = 0.0f;
}

/*
 *  更新
 */
void
CUpgradeStatus::
Update(void)
{
	// 満腹度に応じて倍率を上げる
	for (int i = 0; i < (int)UPGRADE_STATUS_ID::MAX; i++)
	{
		m_StatusMag[i] = m_default_status_mag + (((m_Fullness / 10.0f) * (m_UpgradeStatus[i] * m_status_mag[i])) / 10.0f);
	}
}

/*
 *	描画
 */
void
CUpgradeStatus::
Draw(void)
{
}

/*
 *  解放
 */
void
CUpgradeStatus::
Finalize(void)
{
}

/*
 *  強化ステータスID取得
 */
UPGRADE_STATUS_ID
CUpgradeStatus::
GetUpgradeStatusID(UPGRADE_STATUS_ID id) const
{
	if (m_UpgradeStatus[(int)id] == 0)			return UPGRADE_STATUS_ID::MAX;
	else                                        return id;
}

/*
 *  強化ステータスID設定
 */
void
CUpgradeStatus::
SetUpgradeStatusID(UPGRADE_STATUS_ID id)
{
	m_UpgradeStatus[(int)id] += 1;
	m_UpgradeStatusID = id;
}

/*
 *  強化ステータスカウント取得
 */
int
CUpgradeStatus::
GetUpgradeStatusCount(UPGRADE_STATUS_ID id) const
{
	return m_UpgradeStatus[(int)id];
}

/*
 *	強化倍率取得
 */
float
CUpgradeStatus::
GetUpgrade(UPGRADE_STATUS_ID id) const
{
	return m_StatusMag[(int)id];
}

/*
 *  強化ステータスID取得
 */
UPGRADE_STATUS_ID
CUpgradeStatus::
GetUpgradeStatusID() const
{
	return m_UpgradeStatusID;
}

/*
 *  満腹度設定
 */
void
CUpgradeStatus::
SetFullness(float value)
{
	m_Fullness = value;
}
