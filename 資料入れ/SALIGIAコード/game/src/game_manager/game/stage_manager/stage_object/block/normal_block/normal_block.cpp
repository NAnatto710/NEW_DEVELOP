
/*!
 *  @file		normal_block.cpp
 *  @brief		ノーマルブロック
 *  @author     Ryusei Shimizu
 *  @date       2026/04/14
 */

#include "normal_block.h"

 /*
  *  コンストラクタ
  */
CNormalBlock::
CNormalBlock(void)
{
}

/*
 *  デストラクタ
 */
CNormalBlock::
~CNormalBlock(void)
{
}

/*
 *  初期化
 */
void
CNormalBlock::
Initialize(void)
{
    // 読み込み範囲指定
    m_Rect.left = m_default_size;
    m_Rect.top = 0;
    m_Rect.right = m_Rect.left + m_default_size;
    m_Rect.bottom = m_default_size;

    // ノーマルブロックID設定
    m_StageObjectID = STAGE_OBJECT_ID::NORMAL_BLOCK;

    // アクティブフラグをON
    m_ActiveFlg = true;

    // コリジョンフラグON
    m_CollisionFlg = true;
}