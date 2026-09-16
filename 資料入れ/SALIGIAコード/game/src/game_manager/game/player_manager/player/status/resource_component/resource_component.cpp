
/*!
 *  @file		resource_component.cpp
 *  @brief		キャラクターのリソースコンポーネント
 *  @author     Ryusei Shimizu
 *  @date       2026/07/07
 */

#include "resource_component.h"
#include "../../../../../../utility/utility.h"

const int	CResourceComponent::m_desire_add_interval	= 60;		//!< 欲望加算間隔
const float	CResourceComponent::m_desire_add_value		= 1.0f;		//!< 欲望加算値

/*
 *  @brief リソースIDをインデックスに変換
 * 
 *  @param[in] id リソースID
 * 
 *  @return インデックス
 */
static constexpr size_t
ToIndex(RESOURCE_ID id)
{
	return static_cast<size_t>(id);
}

/*
 *  コンストラクタ
 */
CResourceComponent::
CResourceComponent(void)
{
}

/*
 *  初期化
 */
void
CResourceComponent::
Initialize(const std::string& resource_csv_path)
{
	m_ResourceCSVLoader.Load(resource_csv_path, m_Resources);

	m_DesireAddTimer = 0;
}

/*
 *  更新
 */
void
CResourceComponent::
Update(void)
{
	// 欲望の自動回復
	if(m_DesireAddTimer >= m_desire_add_interval)
	{
		m_DesireAddTimer = 0;
		AddCurrent(RESOURCE_ID::DESIRE, m_desire_add_value);
	}
	else
	{
		++m_DesireAddTimer;
	}
}

/*
 *  現在値取得
 */
float
CResourceComponent::
GetCurrent(RESOURCE_ID id) const
{
	return m_Resources[ToIndex(id)].Current;
}

/*
 *  最大値取得
 */
float
CResourceComponent::
GetMax(RESOURCE_ID id) const
{
	return m_Resources[ToIndex(id)].Max;
}

/*
 *  最小値取得
 */
float
CResourceComponent::
GetMin(RESOURCE_ID id) const
{
	return m_Resources[ToIndex(id)].Min;
}

/*
 *  現在値設定
 */
void
CResourceComponent::
SetCurrent(RESOURCE_ID id, float value)
{
	m_Resources[ToIndex(id)].Current = CLAMP(value, m_Resources[ToIndex(id)].Min, m_Resources[ToIndex(id)].Max);
}

/*
 *  現在値加算
 */
void
CResourceComponent::
AddCurrent(RESOURCE_ID id, float value)
{
	m_Resources[ToIndex(id)].Current = CLAMP(m_Resources[ToIndex(id)].Current + value, m_Resources[ToIndex(id)].Min, m_Resources[ToIndex(id)].Max);
}

/*
 *  現在値リセット
 */
void
CResourceComponent::
ResetCurrent(RESOURCE_ID id)
{
	m_Resources[ToIndex(id)].Current = m_Resources[ToIndex(id)].Max;
}

/*
 *  現在値クリア
 */
void
CResourceComponent::
ClearCurrent(RESOURCE_ID id)
{
	m_Resources[ToIndex(id)].Current = m_Resources[ToIndex(id)].Min;
}

/*
 *  リソースが0かどうかを判定
 */
bool
CResourceComponent::
IsZero(RESOURCE_ID id) const
{
	return m_Resources[ToIndex(id)].Current <= m_Resources[ToIndex(id)].Min;
}

/*
 *  リソースが最大値かどうかを判定
 */
bool
CResourceComponent::
IsMax(RESOURCE_ID id) const
{
	return m_Resources[ToIndex(id)].Current >= m_Resources[ToIndex(id)].Max;
}
