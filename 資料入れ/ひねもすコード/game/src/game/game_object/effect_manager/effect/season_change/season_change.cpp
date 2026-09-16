
/*!
 *  @file       season_change.cpp
 *  @brief      シーン変更エフェクト
 *  @author     Ryusei Shimizu
 *  @date       2026/02/19
 */

#include "season_change.h"
#include "../../../parameter_manager/parameter_manager.h"
#include "../../../../../utility/utility.h"

const int			CSeasonChange::m_width			= 300;										//!< 幅
const int			CSeasonChange::m_height			= 300;										//!< 高さ
const int			CSeasonChange::m_fade_speed		= 2;										//!< フェード速度
const vivid::Rect	CSeasonChange::m_left_rect		= { 0, 0, m_width, m_height };				//!< 描画矩形
const vivid::Rect	CSeasonChange::m_right_rect		= { m_width, 0, m_width * 2, m_height };	//!< 描画矩形
const std::string	CSeasonChange::m_texture_path[(int)SEASON_ID::MAX] =						//!< テクスチャ名
{
	"data\\effect\\season_change\\winter.png",	//!< 冬
	"data\\effect\\season_change\\spring.png",	//!< 春
	"data\\effect\\season_change\\summer.png",	//!< 夏
	"data\\effect\\season_change\\autumn.png",	//!< 秋
};

/*
 *  コンストラクタ
 */
CSeasonChange::
CSeasonChange(void)
	: IEffect(m_width,m_height,EFFECT_ID::SEASON_CHANGE)
{
}

/*
 *  デストラクタ
 */
CSeasonChange::
~CSeasonChange(void)
{
}

/*
 *  初期化
 */
void
CSeasonChange::
Initialize(const vivid::Vector2& position, unsigned int color, float rotation)
{
	IEffect::Initialize(position, color, rotation);

	CGameParameterManager& pm = CGameParameterManager::GetInstance();

	// 現在のシーズンを取得
	m_Season = pm.GetSeasonId();

	// 左右どちらのテクスチャを表示するのかランダムに決定
	bool side = u_RandomInt(0, 1);

	// 描画矩形と表示するテクスチャを設定
	if (side)
	{
		m_Rect = m_left_rect;
		m_Side = SEASON_CHANGE_SIDE::LEFT;
	}
	else
	{
		m_Rect = m_right_rect;
		m_Side = SEASON_CHANGE_SIDE::RIGHT;
	}
}

/*
 *  更新
 */
void
CSeasonChange::
Update(void)
{
	float x = vivid::WINDOW_WIDTH / 80.0f;
	float y = vivid::WINDOW_HEIGHT / 80.0f;

	m_Position += vivid::Vector2(x, y);

	// フェードアウト
	if (m_Position.y > 0)
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
CSeasonChange::
Draw(void)
{
	vivid::DrawTexture(m_texture_path[(int)m_Season], m_Position, m_Color, m_Rect);
}
