
/*!
 *  @file       hud_manager.h
 *  @brief      HUD管理
 *  @author     Ryusei Shimizu
 *  @date       2026/02/03
 */

#pragma once

#include "vivid.h"
#include "hud/hud_object.h"

/*!
 *  @class      CHudManager
 *
 *  @brief      HUD管理クラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2026/02/03
 */
class CHudManager
{
public:

	/*!
	  *  @brief      インスタンスの取得
	  *
	  *  @return     インスタンス
	  */
	static CHudManager& GetInstance(void);

	/*!
	 *  @brief      初期化
	 */
	void				Initialize(void);

	/*!
	 *  @brief      更新
	 */
	void				Update(void);

	/*!
	 *	@brief 		描画
	 */
	void				Draw(void);

	/*!
	 *  @brief      解放
	 */
	void				Finalize(void);

	/*!
	 *  @brief      ミニマップポジション取得
	 */
	vivid::Vector2 		MapPosition(void);

	/*!
	 *  @brief      ミニマップサイズ取得
	 */
	float				MapSize(void);

	/*!
	 *  @brief      ミニマップ描画範囲
	 */
	float				DrawRange(void);

	/*!
	 *  @brief      ミニマップ描画範囲
	 */
	void				GetCreate(void);

private:

	/*!
	 *  @brief      コンストラクタ
	 */
	CHudManager(void);

	/*!
	 *  @brief      コピーコンストラクタ
	 *
	 *  @param[in]  rhs     オブジェクト
	 */
	CHudManager(const CHudManager& rhs);

	/*!
	 *  @brief      ムーブコンストラクタ
	 *
	 *  @param[in]  rhs     オブジェクト
	 */
	CHudManager(CHudManager&& rhs);

	/*!
	 *  @brief      デストラクタ
	 */
	~CHudManager(void);

	/*!
	 *  @brief      代入演算子
	 *
	 *  @param[in]  rhs 代入オブジェクト
	 *
	 *  @return     自身のオブジェクト
	 */
	CHudManager& operator=(const CHudManager& rhs);

	CHpGauge			m_HpGauge;			//!< HPゲージ
	CFullnessGauge		m_FullnessGauge;	//!< 満腹度ゲージ
	CTime				m_Time;				//!< 時間表示
	CRushCoolTime		m_RushCoolTime;		//!< 突進クールタイム表示
};