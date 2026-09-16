
/*!
 *  @file       score_manager.h
 *  @brief      ゲームスコア管理
 *  @author     Ryusei Shimizu
 *  @date       2026/02/20
 */

#pragma once

#include "vivid.h"
#include "score_id.h"

/*!
 *  @class      CScoreManager
 *
 *  @brief      ゲームスコア管理クラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2026/02/20
 */
class CScoreManager
{
public:
	/*!
	 *  @brief      インスタンスの取得
	 *
	 *  @return     インスタンス
	 */
	static CScoreManager&	GetInstance(void);

	/*!
	 *  @brief      初期化
	 */
	void					Initialize(void);

	/*!
	 *  @brief      描画
	 */
	void                    Draw(void);

	/*!
	 *  @brief      解放
	 */
	void                    Finalize(void);

	/*!
	 *  @brief      スコアの加算
	 *
	 *  @param[in]  id  スコアID
	 */
	void					AddScore(SCORE_ID id);

	/*!
	 *  @brief      加算スコアの取得
	 *
	 *  @param[in]  id  スコアID
	 *
	 *  @return     加算スコア
	 */
	int                     GetAddScore(SCORE_ID id) const;

	/*!
	 *  @brief      スコアの取得
	 *
	 *  @return     スコア
	 */
	int						GetScore(void) const;

	/*!
	 *  @brief      スコアの取得
	 *
	 *  @param[in]  id  スコアID
	 *
	 *  @return     スコア
	 */
	int                     GetScore(SCORE_ID id) const { return m_Score[(int)id]; }

private:

	/*!
	 *  @brief      コンストラクタ
	 */
	CScoreManager(void);

	/*!
	 *  @brief      コピーコンストラクタ
	 *
	 *  @param[in]  rhs     オブジェクト
	 */
	CScoreManager(const CScoreManager& rhs);

	/*!
	 *  @brief      ムーブコンストラクタ
	 *
	 *  @param[in]  rhs     オブジェクト
	 */
	CScoreManager(CScoreManager&& rhs);

	/*!
	 *  @brief      デストラクタ
	 */
	~CScoreManager(void);

	/*!
	 *  @brief      代入演算子
	 *
	 *  @param[in]  rhs 代入オブジェクト
	 *
	 *  @return     自身のオブジェクト
	 */
	CScoreManager& operator=(const CScoreManager& rhs);


	static const int	m_add_score[(int)SCORE_ID::MAX];	//!< スコア加算値

	int					m_Score[(int)SCORE_ID::MAX];		//!< スコア
	int					m_TotalScore;						//!< 総スコア
};