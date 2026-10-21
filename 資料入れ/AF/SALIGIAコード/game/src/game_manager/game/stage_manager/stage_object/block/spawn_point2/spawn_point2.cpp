
/*!
 *  @file		spawn_point2.cpp
 *  @brief		スポーンポイント２
 *  @author     Ryusei Shimizu
 *  @date       2026/04/17
 */

#include "spawn_point2.h"

 /*
  *  コンストラクタ
  */
CSpawnPoint2::
CSpawnPoint2(void)
{
}

/*
 *  デストラクタ
 */
CSpawnPoint2::
~CSpawnPoint2()
{
}

/*
 *  初期化
 */
void
CSpawnPoint2::
Initialize(void)
{
    // 読み込み範囲指定
    m_Rect.left = 0;
    m_Rect.top = 0;
    m_Rect.right = 0;
    m_Rect.bottom = 0;

    // スポーンポイントID設定
    m_StageObjectID = STAGE_OBJECT_ID::SPAWN_POINT_2;

    // アクティブフラグをON
    m_ActiveFlg = true;

	// コリジョンフラグはOFF
    m_CollisionFlg = false;
}
