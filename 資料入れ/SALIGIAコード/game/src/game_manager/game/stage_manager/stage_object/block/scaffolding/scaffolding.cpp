
/*!
 *  @file		scaffolding.cpp
 *  @brief		足場ブロック
 *  @author     Ryusei Shimizu
 *  @date       2026/04/14
 */

#include "scaffolding.h"

 /*
  *  コンストラクタ
  */
CScaffolding::
CScaffolding(void)
{
}

/*
 *  デストラクタ
 */
CScaffolding::
~CScaffolding(void)
{
}

/*
 *  初期化
 */
void
CScaffolding::
Initialize(void)
{
    // 読み込み範囲指定
    m_Rect.left = m_default_size * 3;
    m_Rect.top = 0;
    m_Rect.right = m_Rect.left + m_default_size;
    m_Rect.bottom = m_Rect.top + m_default_size;

    // 足場ID設定
    m_StageObjectID = STAGE_OBJECT_ID::SCAFFOLDING;

    // アクティブフラグをON
    m_ActiveFlg = true;

    // コリジョンフラグON
    m_CollisionFlg = false;
}
