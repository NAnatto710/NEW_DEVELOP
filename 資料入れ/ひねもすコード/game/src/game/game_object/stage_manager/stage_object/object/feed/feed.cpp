
/*!
 *  @file       feed.cpp
 *  @brief      エサクラス
 *  @author     Ryusei Shimizu
 *  @date       2026/01/28
 */

#include "feed.h"
#include "../../../stage_manager.h"
#include "../../../../character_manager/character_manager.h"
#include "../../../../character_manager/character/player/player.h"
#include "../../../../score_manager/score_manager.h"
#include "../../../../sound_manager/sound_manager.h"

/*
 *  コンストラクタ
 */
CFeed::
CFeed(void)
{
}

/*
 *  デストラクタ
 */
CFeed::
~CFeed(void)
{
}

/*
 *  初期化
 */
void
CFeed::
Initialize(void)
{
    // 読み込み範囲指定
    m_Rect.left = m_default_size * ((int)STAGE_OBJECT_ID::FEED_OBJECT);
    m_Rect.top = 0;
    m_Rect.right = m_Rect.left + m_default_size;
    m_Rect.bottom = m_default_size;

    // ID設定
    m_StageObjectID = STAGE_OBJECT_ID::FEED_OBJECT;

    // アクティブフラグをON
    m_ActiveFlg = true;

    // コリジョンフラグOFF
    m_CollisionFlg = false;
}

/*
 *  更新
 */
void
CFeed::
Update(void)
{
    if (m_ActiveFlg == false)return;

    // プレイヤー取得
    CPlayer* player = (CPlayer*)CCharacterManager::GetInstance().GetPlayer();

    vivid::Vector2 pos = player->GetCenterPosition();

    vivid::Vector2 center = this->GetCenterPosition();

    float radius = (float)this->GetSize() / 2.0f;

    vivid::Vector2 v = pos - center;

    // プレイヤーとの距離を比較
    if (v.Length() < radius)
    {
        // 取得された
        m_ActiveFlg = false;

        // プレイヤーの満腹度を上昇させる
        player->IncreaseFullness();

		// スコア加算
        CScoreManager::GetInstance().AddScore(SCORE_ID::FEED);

        //餌食べるサウンド再生
        CSoundManager::GetInstance().Play(SOUND_ID::SUBSIST, false);
    }

    // ベースの更新処理
    IStageObject::Update();
}
