
/*!
 *  @file       parameter_manager.h
 *  @brief      ゲームパラメータ管理
 *  @author     Ryusei Shimizu
 *  @date       2025/12/03
 */

#pragma once

#include "vivid.h"
#include "season_id.h"
#include "parameter_object/parameter_object.h"

/*!
 *  @class      CGameParameterManager
 *
 *  @brief      ゲームパラメータ管理クラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2025/12/03
 */
class CGameParameterManager
{
public:

	/*!
	 *  @brief      インスタンスの取得
	 *
	 *  @return     インスタンス
	 */
	static CGameParameterManager& GetInstance(void);

	/*!
	 *  @brief      初期化
	 */
	void				Initialize(void);

	/*!
	 *  @brief      更新
	 */
	void				Update(void);

	/*!
	 *  @brief      描画
	 */
	void				Draw(void);

	/*!
	 *  @brief      解放
	 */
	void				Finalize(void);

	/*!
	 *  @brief      季節変更
	 */
	void                ChangeSeason(SEASON_ID id);

	/*!
	 *	@brief		ゲーム終了判定
	 *
	 *	@return		ゲームクリアフラグ
	 */
	bool				IsGameClear(void) const { return m_GameClearFlg; }

	/*!
	 *  @brief      季節変更フラグ取得
	 *
	 *  @return     季節変更フラグ
	 */
	bool                IsSeasonChangeEffect(void) const { return m_SeasonChangeEffectFlg; }

	/*
	 * @brief		季節IDの取得
	 *
	 * @return		季節ID
	 */
	SEASON_ID			GetSeasonId(void)const;

	/*!
	 *  @brief      現在の時間を返す
	 *
	 *  @return     時間の数値
	 */
	float				GetDayCycleNumber(void)const;

	/*!
	 *  @brief      一日の最大時間を返す
	 *
	 *  @return     一日の最大時間
	 */
	float				GetMaxDayCycleTime(void)const;

	/*!
	 *  @brief      現在の時間管理IDを返す
	 *
	 *  @return     時間管理ID
	 */
	DAYCYCLE_ID			GetDayCycleID(void)const;

private:

	/*!
	 *  @brief      コンストラクタ
	 */
	CGameParameterManager(void);

	/*!
	 *  @brief      コピーコンストラクタ
	 *
	 *  @param[in]  rhs     オブジェクト
	 */
	CGameParameterManager(const CGameParameterManager& rhs);

	/*!
	 *  @brief      ムーブコンストラクタ
	 *
	 *  @param[in]  rhs     オブジェクト
	 */
	CGameParameterManager(CGameParameterManager&& rhs);

	/*!
	 *  @brief      デストラクタ
	 */
	~CGameParameterManager(void);

	/*!
	 *  @brief      代入演算子
	 *
	 *  @param[in]  rhs 代入オブジェクト
	 *
	 *  @return     自身のオブジェクト
	 */
	CGameParameterManager& operator=(const CGameParameterManager& rhs);

	/*!
	 *  @brief      フェードイン
	 */
	void    FadeIn(void);

	/*!
	 *  @brief      シーズン更新
	 */
	void    SeasonUpdate(void);

	/*!
	 *  @brief      フェードアウト
	 */
	void    FadeOut(void);

	/*!
	 *  @brief      シーズン変更
	 */
	void    SeasonChange(void);

	/*!
	 *  @brief      状態ID
	 */
	enum class STATE
	{
		FADEIN              //!< フェードイン
		, SCENE_UPDATE      //!< シーズン更新
		, FADEOUT           //!< フェードアウト
		, SCENE_CHANGE      //!< シーズン変更
		, BREAK             //!< ブレイク
	};


	static const float	m_season_change_effect_time;				//!< シーズンチェンジエフェクト時間
	static const int    m_season_change_need_feed;					//!< シーズンチェンジに必要なフィード数

	float				m_SeasonChangeEffectTimer;					//!< シーズンチェンジエフェクトタイマー

	bool				m_GameClearFlg;								//!< ゲームクリアフラグ
	bool                m_ChangeSeasonFlg;							//!< 季節変更フラグ
	bool                m_SeasonChangeEffectFlg;					//!< シーズンチェンジエフェクトフラグ

	unsigned int        m_Color;										//!< 色

	SEASON_ID			m_NextSeason;								//!< 次の季節
	SEASON_ID           m_CurrentSeason;							//!< 現在の季節
	STATE               m_State;									//!< 状態

	CDayCycle			m_DayCycle;									//!< 一日サイクル
};