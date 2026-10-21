
/*!
 *  @file		spawn_point4.cpp
 *  @brief		スポーンポイント４
 *  @author     Ryusei Shimizu
 *  @date       2026/04/17
 */

#include "spawn_point4.h"

 /*
  *  コンストラクタ
  */
CSpawnPoint4::
CSpawnPoint4(void)
{
}

/*
 *  デストラクタ
 */
CSpawnPoint4::
~CSpawnPoint4()
{
}

/*
 *  初期化
 */
void
CSpawnPoint4::
Initialize(void)
{
    // 読み込み範囲指定
    m_Rect.left = 0;
    m_Rect.top = 0;
    m_Rect.right = 0;
    m_Rect.bottom = 0;

    // スポーンポイントID設定
    m_StageObjectID = STAGE_OBJECT_ID::SPAWN_POINT_4;

    // アクティブフラグをON
    m_ActiveFlg = true;

    // コリジョンフラグはOFF
    m_CollisionFlg = false;
}
