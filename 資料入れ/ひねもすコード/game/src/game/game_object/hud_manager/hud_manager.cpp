
/*!
 *  @file       hud_manager.cpp
 *  @brief      HUD管理
 *  @author     Ryusei Shimizu
 *  @date       2026/02/03
 */

#include "hud_manager.h"
#include "../parameter_manager/parameter_manager.h"

CMap				m_Map;				//!< マップ

/*
 *  インスタンスの取得
 */
CHudManager&
CHudManager::
GetInstance(void)
{
	static CHudManager instance;

	return instance;
}

/*
 *  初期化
 */
void
CHudManager::
Initialize(void)
{
	m_HpGauge.Initialize();
	m_FullnessGauge.Initialize();
	m_Map.Initialize();
	m_Time.Initialize();
	m_RushCoolTime.Initialize();
}

/*
 *  更新
 */
void
CHudManager::
Update(void)
{
	m_HpGauge.Update();
	m_FullnessGauge.Update();
	m_Map.Update();
	m_Time.Update();
	m_RushCoolTime.Update();
}

/*
 *	描画
 */
void
CHudManager::
Draw(void)
{
	m_HpGauge.Draw();
	m_FullnessGauge.Draw();
	m_Map.Draw();
	m_RushCoolTime.Draw();

	// 冬の季節以外の場合は時間を描画する
	if (CGameParameterManager::GetInstance().GetSeasonId() != SEASON_ID::WINTER)
		m_Time.Draw();
}

/*
 *  解放
 */
void
CHudManager::
Finalize(void)
{
	m_HpGauge.Finalize();
	m_FullnessGauge.Finalize();
	m_Map.Finalize();
	m_Time.Finalize();
	m_RushCoolTime.Finalize();
}

/*
 *  ミニマップポジション取得
 */
vivid::Vector2
CHudManager::
MapPosition(void)
{
	return m_Map.GetMapPosition();
}

/*
 *  マップサイズ
 */
float CHudManager::MapSize(void)
{
	return m_Map.GetSize();
}

/*
 *  ミニマップ描画範囲
 */
float CHudManager::DrawRange(void)
{
	return m_Map.GetDrawRange();
}

/*
 *	ミニマップ描画範囲
 */
void CHudManager::GetCreate(void)
{
	return m_Map.Create();
}

/*
 *  コンストラクタ
 */
CHudManager::
CHudManager(void)
{
}

/*
 *  コピーコンストラクタ
 */
CHudManager::
CHudManager(const CHudManager& rhs)
{
	(void)rhs;
}

/*
 *  デストラクタ
 */
CHudManager::
~CHudManager(void)
{
}

/*
 *  代入演算子
 */
CHudManager&
CHudManager::
operator=(const CHudManager& rhs)
{
	(void)rhs;
	return *this;
}
