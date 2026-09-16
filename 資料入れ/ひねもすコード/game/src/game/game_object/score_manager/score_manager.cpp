
/*!
 *  @file       score_manager.cpp
 *  @brief      ゲームスコア管理
 *  @author     Ryusei Shimizu
 *  @date       2026/02/20
 */

#include "score_manager.h"

const int CScoreManager::m_add_score[(int)SCORE_ID::MAX] =			//!< スコア加算値
{
	2000,	// FEED
	5000,	// ENEMY
	 500,	// HITBULLET
	-300,	// MISSBULLET
};

/*
 *  インスタンスの取得
 */
CScoreManager&
CScoreManager::
GetInstance(void)
{
	static CScoreManager instance;

	return instance;
}

/*
 *  初期化
 */
void
CScoreManager::
Initialize(void)
{
	m_TotalScore = 0;
	for (int i = 0; i < (int)SCORE_ID::MAX; ++i)
		m_Score[i] = 0;
}

/*
 *  描画
 */
void
CScoreManager::
Draw(void)
{
	int text_size = 80;
	vivid::Vector2 position = { 550.0f, 700.0f };
	std::string text = "Total Score: " + std::to_string(m_TotalScore);

	vivid::DrawText(text_size, text, position,0xff000000);
}

/*
 *  解放
 */
void
CScoreManager::
Finalize(void)
{
}

/*
 *  スコアの加算
 */
void
CScoreManager::
AddScore(SCORE_ID id)
{
	m_Score[(int)id] += m_add_score[(int)id];
	m_TotalScore += m_add_score[(int)id];
}

/*
 *  スコアの取得
 */
int
CScoreManager::
GetAddScore(SCORE_ID id) const
{
	return m_add_score[(int)id];
}

/*
 *  スコアの取得
 */
int
CScoreManager::
GetScore(void) const
{
	return m_TotalScore;
}

/*
 *  コンストラクタ
 */
CScoreManager::
CScoreManager(void)
	: m_TotalScore(0)
{
}

/*
 *  コピーコンストラクタ
 */
CScoreManager::
CScoreManager(const CScoreManager& rhs)
{
	(void)rhs;
}

/*
 *  デストラクタ
 */
CScoreManager::
~CScoreManager(void)
{
}

/*
 *  代入演算子
 */
CScoreManager&
CScoreManager::
operator=(const CScoreManager& rhs)
{
	(void)rhs;
	return *this;
}
