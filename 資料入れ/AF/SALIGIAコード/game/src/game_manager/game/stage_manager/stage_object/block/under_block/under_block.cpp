
/*!
 *  @file		under_block.cpp
 *  @brief		地中のブロック
 *  @author     Ryusei Shimizu
 *  @date       2026/04/14
 */

#include "under_block.h"

 /*
  *  コンストラクタ
  */
CUnderBlock::
CUnderBlock(void)
{
}

/*
 *  デストラクタ
 */
CUnderBlock::
~CUnderBlock(void)
{
}

/*
 *  初期化
 */
void
CUnderBlock::
Initialize(void)
{
    // 読み込み範囲指定
    m_Rect.left = m_default_size * 2;
    m_Rect.top = 0;
    m_Rect.right = m_Rect.left + m_default_size;
    m_Rect.bottom = m_Rect.top + m_default_size;

    // 地中ブロックID設定
    m_StageObjectID = STAGE_OBJECT_ID::UNDER_BLOCK;

    // アクティブフラグをON
    m_ActiveFlg = true;

    // コリジョンフラグON
    m_CollisionFlg = true;
}
