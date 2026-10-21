
/*!
 *  @file		spawn_point1.cpp
 *  @brief		スポーンポイント１
 *  @author     Ryusei Shimizu
 *  @date       2026/04/17
 */

#include "spawn_point1.h"

/*
 *  コンストラクタ
 */
CSpawnPoint1::
CSpawnPoint1(void)
{
}

/*
 *  デストラクタ
 */
CSpawnPoint1::
~CSpawnPoint1()
{
}

/*
 *  初期化
 */
void
CSpawnPoint1::
Initialize(void)
{
    // 読み込み範囲指定
    m_Rect.left = 0;
    m_Rect.top = 0;
    m_Rect.right = 0;
    m_Rect.bottom = 0;

    // スポーンポイントID設定
    m_StageObjectID = STAGE_OBJECT_ID::SPAWN_POINT_1;

    // アクティブフラグをON
    m_ActiveFlg = true;

	// コリジョンフラグはOFF
    m_CollisionFlg = false;
}
