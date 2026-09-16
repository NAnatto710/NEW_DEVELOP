
/*!
 *  @file       parameter_manager.cpp
 *  @brief      ゲームパラメータ管理
 *  @author     Ryusei Shimizu
 *  @date       2025/12/03
 */

#include "parameter_manager.h"
#include "../stage_manager/stage_manager.h"
#include "../scene_manager/scene_manager.h"
#include "../character_manager/character_manager.h"
#include "../effect_manager/effect_manager.h"
#include "../character_manager/character/player/player.h"
#include "../score_manager/score_manager.h"
#include "../sound_manager/sound_manager.h"
#include "../../../utility/utility.h"


const float CGameParameterManager::m_season_change_effect_time	= 3.0f;		//!< シーズンチェンジエフェクト時間
const int   CGameParameterManager::m_season_change_need_feed	= 10;		//!< シーズンチェンジに必要なフィード数

/*
 *  インスタンスの取得
 */
CGameParameterManager&
CGameParameterManager::
GetInstance(void)
{
	static CGameParameterManager instance;

	return instance;
}

/*
 *  初期化
 */
void
CGameParameterManager::
Initialize(void)
{
	m_DayCycle.Initialize();
	m_DayCycle.DayStart();

	m_GameClearFlg = false;
	m_ChangeSeasonFlg = false;
	m_SeasonChangeEffectFlg = false;

	m_SeasonChangeEffectTimer = 0;

	m_Color = 0xffffffff;

	m_NextSeason = SEASON_ID::WINTER;
	m_State = STATE::FADEOUT;
}

/*
 *  更新
 */
void
CGameParameterManager::
Update(void)
{
	switch (m_State)
	{
	case STATE::FADEIN:          FadeIn();          break;
	case STATE::SCENE_UPDATE:    SeasonUpdate();    break;
	case STATE::FADEOUT:         FadeOut();         break;
	case STATE::SCENE_CHANGE:    SeasonChange();	break;
	case STATE::BREAK:								break;
	}

#ifdef VIVID_DEBUG

#endif
}

/*
 *  描画
 */
void
CGameParameterManager::
Draw(void)
{
	CSceneManager& scm = CSceneManager::GetInstance();

	if (m_CurrentSeason != SEASON_ID::WINTER)
		m_DayCycle.Draw();

	if(m_CurrentSeason == SEASON_ID::WINTER && scm.GetMainSceneID() == MAINSCENE_ID::GAMEMAIN)
	{
		// アルファ値を減少させる
		int alpha = (m_Color & 0xff000000) >> 24;
		alpha -= 0.1;

		// アルファ値が0未満にならないようにする
		if (alpha < 0)
		{
			alpha = 0;
		}

		// アルファ値をカラーに反映させる
		m_Color = (alpha << 24) | (m_Color & 0x00ffffff);

		vivid::DrawTexture("data\\background\\button2.png", vivid::Vector2(420, 150),m_Color);
	}
}

/*
 *  解放
 */
void
CGameParameterManager::
Finalize(void)
{
	m_DayCycle.Finalize();
}

/*
 *  季節変更
 */
void
CGameParameterManager::
ChangeSeason(SEASON_ID id)
{
	m_NextSeason = id;
	m_ChangeSeasonFlg = true;
}

/*
 *	季節IDの取得
 */
SEASON_ID
CGameParameterManager::
GetSeasonId(void) const
{
	return m_CurrentSeason;
}

/*
 *  現在の時間を返す
 */
float
CGameParameterManager::
GetDayCycleNumber(void) const
{
	return m_DayCycle.GetDayCycleNumber();
}

/*
 *  一日の最大時間を返す
 */
float
CGameParameterManager::
GetMaxDayCycleTime(void) const
{
	return m_DayCycle.GetMaxDayCycleTime();
}

/*
 *  現在の時間管理IDを返す
 */
DAYCYCLE_ID
CGameParameterManager::
GetDayCycleID(void) const
{
	return m_DayCycle.GetDayCycleID();
}

/*
 *  コンストラクタ
 */
CGameParameterManager::
CGameParameterManager(void)
	: m_GameClearFlg(false)
{
}

/*
 *  コピーコンストラクタ
 */
CGameParameterManager::
CGameParameterManager(const CGameParameterManager& rhs)
{
	(void)rhs;
}

/*
 *  デストラクタ
 */
CGameParameterManager::
~CGameParameterManager(void)
{
}

/*
 *  代入演算子
 */
CGameParameterManager&
CGameParameterManager::
operator=(const CGameParameterManager& rhs)
{
	(void)rhs;

	return *this;
}

/*
 *  フェードイン
 */
void
CGameParameterManager::
FadeIn(void)
{
	// シーズンチェンジエフェクトの生成
	if (m_SeasonChangeEffectTimer > 0)
	{
		CEffectManager& em = CEffectManager::GetInstance();

		m_SeasonChangeEffectTimer -= vivid::GetDeltaTime();

		float tmp_pos_x = -vivid::WINDOW_WIDTH * 1.5f;
		int loop_count = (m_SeasonChangeEffectTimer / m_season_change_effect_time * 15);

		for (int i = 0; i < loop_count; i++)
		{
			// エフェクトの生成位置をランダムに設定
			float effect_pos_x = u_RandomInt((int)tmp_pos_x, (int)vivid::WINDOW_WIDTH);
			em.Create(EFFECT_ID::SEASON_CHANGE, vivid::Vector2(effect_pos_x, -330.0f), 0xffffffff, 0.0f);
		}

		// シーズンチェンジエフェクトフラグの設定
		m_SeasonChangeEffectFlg = true;
	}
	else
	{
		m_State = STATE::SCENE_UPDATE;

		// シーズンチェンジエフェクトタイマーのリセット
		m_SeasonChangeEffectTimer = 0;
		// シーズンチェンジエフェクトフラグのリセット
		m_SeasonChangeEffectFlg = false;
	}
}

/*
 *  シーズン更新
 */
void
CGameParameterManager::
SeasonUpdate(void)
{
	if (m_GameClearFlg)
	{
		return;
	}

	m_DayCycle.Update();

	CScoreManager& sm = CScoreManager::GetInstance();
	int score = sm.GetScore(SCORE_ID::FEED);
	int add_score = sm.GetAddScore(SCORE_ID::FEED);

	int feed_count = score / add_score;

	// 一日が終了した、またはフィード数が規定数以上で冬の季節の場合
	if (m_DayCycle.GetDayFinish() && m_CurrentSeason != SEASON_ID::WINTER ||
		feed_count >= m_season_change_need_feed && m_CurrentSeason == SEASON_ID::WINTER)
	{
		// 次の季節を設定
		int next_season = ((int)m_CurrentSeason + 1) % (int)SEASON_ID::MAX;
		this->ChangeSeason(SEASON_ID(next_season));
	}

	// シーズン変更が発生
	if (m_CurrentSeason != m_NextSeason || m_ChangeSeasonFlg)
	{
		// フェードアウト
		m_State = STATE::FADEOUT;

		// シーズンチェンジエフェクトタイマーのリセット
		m_SeasonChangeEffectTimer = 0;

		m_ChangeSeasonFlg = false;
	}
}

/*
 *  フェードアウト
 */
void
CGameParameterManager::
FadeOut(void)
{
	// シーズンチェンジエフェクトの生成
	if (m_SeasonChangeEffectTimer < m_season_change_effect_time)
	{
		CEffectManager& em = CEffectManager::GetInstance();

		m_SeasonChangeEffectTimer += vivid::GetDeltaTime();

		float tmp_pos_x = -vivid::WINDOW_WIDTH * 1.5f;
		int loop_count = (m_SeasonChangeEffectTimer / m_season_change_effect_time * 15);

		for (int i = 0; i < loop_count; i++)
		{
			// エフェクトの生成位置をランダムに設定
			float effect_pos_x = u_RandomInt((int)tmp_pos_x, (int)vivid::WINDOW_WIDTH);
			em.Create(EFFECT_ID::SEASON_CHANGE, vivid::Vector2(effect_pos_x, -330.0f), 0xffffffff, 0.0f);
		}

		// シーズンチェンジエフェクトフラグの設定
		m_SeasonChangeEffectFlg = true;
	}
	else
	{
		m_State = STATE::SCENE_CHANGE;

		// シーズンチェンジエフェクトタイマーのリセット
		m_SeasonChangeEffectTimer = m_season_change_effect_time;
		// シーズンチェンジエフェクトフラグのリセット
		m_SeasonChangeEffectFlg = false;
	}
}

/*
 *  シーズン変更
 */
void
CGameParameterManager::
SeasonChange(void)
{
	CStageManager& sm = CStageManager::GetInstance();
	CSceneManager& scm = CSceneManager::GetInstance();
	CCharacterManager& cm = CCharacterManager::GetInstance();
	CEffectManager& em = CEffectManager::GetInstance();
	CPlayer* player = (CPlayer*)cm.GetPlayer();

	// ゲームクリア判定
	if (m_CurrentSeason == SEASON_ID::AUTUMN &&
		m_NextSeason == SEASON_ID::WINTER)
	{
		m_GameClearFlg = true;
		m_State = STATE::BREAK;

		// 季節を切り替える
		m_CurrentSeason = SEASON_ID::WINTER;

		// サブシーンをダミーシーンに変更
		scm.ChangeSubScene(SUBSCENE_ID::DUMMY);

		return;
	}

	if (scm.GetMainSceneID() != MAINSCENE_ID::GAMEMAIN)
	{
		// メインシーンをゲームメインに変更
		scm.ChangeMainScene(MAINSCENE_ID::GAMEMAIN);
	}
	else
	{
		// サブシーンを選択シーンに変更
		scm.ChangeSubScene(SUBSCENE_ID::SELECT);
	}

	// エネミーの削除
	cm.EnemyDelete();

	// プレイヤーの体力を回復
	player->IncreaseStatus(STATUS_ID::HP, 50);

	// プレイヤーに無敵を付与する
	player->Invincible();

	// 一日のリセット
	m_DayCycle.DayReset();

	// 季節を切り替える
	m_CurrentSeason = m_NextSeason;

	// オブジェクトの再配置
	sm.ReinstallationObject();

	// フェードイン
	m_State = STATE::FADEIN;
}