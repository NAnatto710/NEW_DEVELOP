
/*!
 *  @file       guide.cpp
 *  @brief      ガイドベースクラス
 *  @author     Ryusei Shimizu
 *  @date       2026/02/24
 */

#include "guide.h"

const float		IGuide::m_default_achive_time	= 15.0f;      //!< デフォルトのアクティブ時間
const int		IGuide::m_fade_speed			= 5;            //!< フェード速度

 /*
  *  コンストラクタ
  */
IGuide::
IGuide(int width, int height, std::string path)
    : m_Width(width)
    , m_Height(height)
	, m_TextureName(path)
    , m_Position(vivid::Vector2(0.0f, 0.0f))
    , m_Color(0xffffffff)
    , m_Anchor(vivid::Vector2((float)m_Width / 2.0f, (float)m_Height / 2.0f))
    , m_Rect({ 0, 0, m_Width, m_Height })
    , m_Scale(vivid::Vector2(1.0f, 1.0f))
    , m_Rotation(0.0f)
    , m_ActiveFlg(true)
{
}

/*
 *  デストラクタ
 */
IGuide::
~IGuide(void)
{
}

/*
 *  初期化
 */
void
IGuide::
Initialize(const vivid::Vector2& position)
{
	m_Position = position;
	m_AchiveTime = m_default_achive_time;
	m_ActiveFlg = true;
}

/*
 *  更新
 */
void
IGuide::
Update(void)
{
	m_AchiveTime -= vivid::GetDeltaTime();

	if (m_AchiveTime <= 0.0f)
	{
		// アルファ値を減少させる
		int alpha = (m_Color & 0xff000000) >> 24;
		alpha -= m_fade_speed;

		// アルファ値が0未満にならないようにする
		if (alpha < 0)
		{
			alpha = 0;
			m_ActiveFlg = false;
		}

		// アルファ値をカラーに反映させる
		m_Color = (alpha << 24) | (m_Color & 0x00ffffff);
	}
}

/*
 *  描画
 */
void
IGuide::
Draw(void)
{
	vivid::DrawTexture(m_TextureName, m_Position, m_Color, m_Rect, m_Anchor, m_Scale, m_Rotation);
}

/*
 *  解放
 */
void
IGuide::
Finalize(void)
{
}

/*
 *  位置の取得
 */
vivid::Vector2
IGuide::
GetPosition(void) const
{
	return m_Position;
}

/*
 *  位置の設定
 */
void
IGuide::
SetPosition(const vivid::Vector2& position)
{
	m_Position = position;
}

/*
 *  アクティブ状態の取得
 */
bool
IGuide::
IsActive(void) const
{
	return m_ActiveFlg;
}

/*
 *  アクティブ状態の設定
 */
void
IGuide::
SetActive(bool active)
{
	m_ActiveFlg = active;
}
