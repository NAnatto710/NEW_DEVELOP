
/*!
 *  @file		initial_point.cpp
 *  @brief		初期位置
 *  @author     Ryusei Shimizu
 *  @date       2026/11/20
 */

#include "initial_point.h"

/*
 *	コンストラクタ
 */
CInitialPoint::
CInitialPoint (void)
{
}

/*
 *	初期化
 */
void
CInitialPoint::
Initialize (void)
{
    // 読み込み範囲指定
    m_Rect.left = 0;
    m_Rect.top = 0;
    m_Rect.right = 0;
    m_Rect.bottom = 0;

    // ノーマルブロックID設定
    m_StageObjectID = STAGE_OBJECT_ID::INITIAL_POINT;

    // アクティブフラグをON
    m_ActiveFlg = true;

    // コリジョンフラグON
    m_CollisionFlg = false;
}
