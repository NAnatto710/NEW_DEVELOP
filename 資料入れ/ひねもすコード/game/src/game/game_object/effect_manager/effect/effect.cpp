
/*!
 *  @file       effect.cpp
 *  @brief      エフェクトベースクラス
 *  @author     Ryusei Shimizu
 *  @date       2026/02/19
 */

#include "effect.h"

/*
 *  コンストラクタ
 */
IEffect::
IEffect(int width, int height, EFFECT_ID id)
    : m_Width(width)
    , m_Height(height)
    , m_Position(vivid::Vector2(0.0f, 0.0f))
    , m_Color(0xffffffff)
    , m_Anchor(vivid::Vector2((float)m_Width / 2.0f, (float)m_Height / 2.0f))
    , m_Rect({ 0, 0, m_Width, m_Height })
    , m_Scale(vivid::Vector2(1.0f, 1.0f))
    , m_Rotation(0.0f)
    , m_ActiveFlg(true)
	, m_EffectID(id)
{
}

/*
 *  デストラクタ
 */
IEffect::
~IEffect(void)
{
}

/*
 *  初期化
 */
void
IEffect::
Initialize(const vivid::Vector2& position, unsigned int color, float rotation)
{
    m_Position = position;
    m_Color = color;
    m_Rotation = rotation;
    m_ActiveFlg = true;
}

/*
 *  更新
 */
void
IEffect::
Update(void)
{
}

/*
 *  描画
 */
void
IEffect::
Draw(void)
{
}

/*
 *  解放
 */
void
IEffect::
Finalize(void)
{
}

/*
 *  位置の取得
 */
vivid::Vector2
IEffect::
GetPosition(void)const
{
    return m_Position;
}

/*
 *  位置の設定
 */
void
IEffect::
SetPosition(const vivid::Vector2& position)
{
	m_Position = position;
}

/*
 *  アクティブフラグの取得
 */
bool
IEffect::
IsActive(void)const
{
    return m_ActiveFlg;
}

/*
 *  アクティブフラグの設定
 */
void
IEffect::
SetActive(bool active)
{
    m_ActiveFlg = active;
}

/*
 *  エフェクトIDの取得
 */
EFFECT_ID
IEffect::
GetEffectID(void) const
{
    return m_EffectID;
}
