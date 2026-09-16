
/*!
 *  @file       time.cpp
 *  @brief      時間表示
 *  @author     Ryusei Shimizu
 *  @date       2026/02/23
 */

#include "time.h"
#include "../../../parameter_manager/parameter_manager.h"
#include "../../../../../utility/utility.h"


const int               CTime::m_width			= 150;																	//!< 幅
const int               CTime::m_height			= 150;																	//!< 高さ
const vivid::Vector2    CTime::m_position		= vivid::Vector2((vivid::WINDOW_WIDTH - m_width) / 2.0f, 10.0f);		//!< 位置
const vivid::Rect       CTime::m_frame_rect		= { 0, 0, m_width, m_height };											//!< 枠の読み込み範囲
const vivid::Rect		CTime::m_time_rect		= { m_width, 0, m_width * 2, m_height };								//!< 時間の位置
const vivid::Vector2    CTime::m_anchor			= vivid::Vector2(m_width / 2.0f, m_height / 2.0f);						//!< 基準点

/*
 *	コンストラクタ
 */
CTime::
CTime(void)
{
}

/*
 *	デストラクタ
 */
CTime::
~CTime(void)
{
}

/*
 *	初期化
 */
void
CTime::
Initialize(void)
{
	m_Rotation = DEG_TO_RAD(0);
}

/*
 *	更新
 */
void
CTime::
Update(void)
{
	CGameParameterManager& pm = CGameParameterManager::GetInstance();

	DAYCYCLE_ID id = pm.GetDayCycleID();

	// 時間の更新
	switch (id)
	{
	case DAYCYCLE_ID::DUMMY:
		break;
	case DAYCYCLE_ID::MORNING:
		if (m_Rotation <= DEG_TO_RAD(0))
			m_Rotation += DEG_TO_RAD(1);
		else
			m_Rotation = DEG_TO_RAD(0);
		break;
	case DAYCYCLE_ID::DAYTIME:
		if (m_Rotation <= DEG_TO_RAD(120))
			m_Rotation += DEG_TO_RAD(1);
		else
			m_Rotation = DEG_TO_RAD(120);
		break;
	case DAYCYCLE_ID::NIGHT:
		if (m_Rotation <= DEG_TO_RAD(240))
			m_Rotation += DEG_TO_RAD(1);
		else
			m_Rotation = DEG_TO_RAD(240);
		break;
	}
}

/*
 *	描画
 */
void
CTime::
Draw(void)
{
	std::string path = "data\\hud\\time3.png";

	// 時間の描画
	vivid::DrawTexture(path, m_position, 0xffffffff, m_time_rect, m_anchor, m_Rotation);

	// 枠の描画
	vivid::DrawTexture(path, m_position, 0xffffffff, m_frame_rect);

}

/*
 *	解放
 */
void
CTime::
Finalize(void)
{
}
