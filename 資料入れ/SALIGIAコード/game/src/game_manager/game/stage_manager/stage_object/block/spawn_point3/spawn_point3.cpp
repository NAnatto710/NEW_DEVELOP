
/*!
 *  @file		spawn_point3.cpp
 *  @brief		スポーンポイント３
 *  @author     Ryusei Shimizu
 *  @date       2026/04/17
 */

#include "spawn_point3.h"

 /*
  *  コンストラクタ
  */
CSpawnPoint3::
CSpawnPoint3(void)
{
}

/*
 *  デストラクタ
 */
CSpawnPoint3::
~CSpawnPoint3()
{
}

/*
 *  初期化
 */
void
CSpawnPoint3::
Initialize(void)
{
    // 読み込み範囲指定
    m_Rect.left = 0;
    m_Rect.top = 0;
    m_Rect.right = 0;
    m_Rect.bottom = 0;

    // スポーンポイントID設定
    m_StageObjectID = STAGE_OBJECT_ID::SPAWN_POINT_3;

    // アクティブフラグをON
    m_ActiveFlg = true;

    // コリジョンフラグはOFF
    m_CollisionFlg = false;
}
