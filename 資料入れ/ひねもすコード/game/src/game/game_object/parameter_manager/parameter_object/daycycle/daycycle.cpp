
/*!
 *  @file       daycycle.cpp
 *  @brief      一日の時間管理
 *  @author     Ryusei Shimizu
 *  @date       2025/10/21
 */

#include "daycycle.h"
#include "../../../../../utility/utility.h"

//const float CDayCycle::m_max_morning_time = 30.0f;                //!< 朝の最大時間
//const float CDayCycle::m_max_daytime_time = 30.0f;                //!< 昼の最大時間
//const float CDayCycle::m_max_night_time   = 30.0f;                //!< 夜の最大時間

const float CDayCycle::m_max_morning_time     = 25.0f;           //!< 朝の最大時間
const float CDayCycle::m_max_daytime_time     = 25.0f;           //!< 昼の最大時間
const float CDayCycle::m_max_night_time       = 25.0f;           //!< 夜の最大時間

const float CDayCycle::m_max_daycycle_time = m_max_morning_time + m_max_daytime_time + m_max_night_time;   //!< 一日の最大時間

const unsigned int CDayCycle::m_morning_color   = 0x14000000;    //!< 朝の背景色
const unsigned int CDayCycle::m_daytime_color   = 0x00000000;    //!< 昼の背景色
const unsigned int CDayCycle::m_night_color     = 0x44000000;    //!< 夜の背景色
const unsigned int CDayCycle::m_change_color    = 0x02000000;    //!< 背景色の変化量

/*
 *  コンストラクタ
 */
CDayCycle::
CDayCycle(void)
    : m_CountTimer(0)
    , m_CountFlg(false)
    , m_DayCycleState(DAYCYCLE_ID::DUMMY)
{
}

/*
 *  デストラクタ
 */
CDayCycle::
~CDayCycle(void)
{
}

/*
 *  初期化
 */
void
CDayCycle::
Initialize(void)
{
    m_CountTimer = 0.0f;
    m_CountFlg = false;
    m_DayCycleState = DAYCYCLE_ID::MORNING;

    m_BackColor = m_morning_color;
}

/*
 *  更新
 */
void
CDayCycle::
Update(void)
{
    // 時間計測フラグの状態によって秒数を加算する
    if (m_CountFlg)
    {
        m_CountTimer += vivid::GetDeltaTime();
    }

    // 経過時間によって状態を変化させる
    if (m_CountTimer < m_max_morning_time)
    {
        // 朝にする
        m_DayCycleState = DAYCYCLE_ID::MORNING;

        if (m_BackColor != m_morning_color)
        {
            m_BackColor -= m_change_color;
        }
    }
    else if (m_CountTimer < m_max_morning_time + m_max_daytime_time)
    {
        // 昼にする
        m_DayCycleState = DAYCYCLE_ID::DAYTIME;

        if (m_BackColor != m_daytime_color)
        {
            m_BackColor -= m_change_color;
        }
    }
    else if (m_CountTimer < m_max_daycycle_time)
    {
        // 夜にする
        m_DayCycleState = DAYCYCLE_ID::NIGHT;

        if (m_BackColor != m_night_color)
        {
            m_BackColor += m_change_color;
        }
    }
    else
    {
        // ダミーにする
        m_DayCycleState = DAYCYCLE_ID::DUMMY;
    }
}

/*
 *  描画
 */
void
CDayCycle::
Draw(void)
{
    // 現在の状態によって背景色を変更する
    vivid::DrawTexture("data\\object\\white.png", vivid::Vector2::ZERO, m_BackColor);
}

/*
 *  解放
 */
void
CDayCycle::
Finalize(void)
{
}

/*
 *  一日の計測スタート
 */
void
CDayCycle::
DayStart(void)
{
    m_CountFlg = true;
}

/*
 *  一日の計測ストップ
 */
void
CDayCycle::
DayStop(void)
{
    m_CountFlg = false;
}

/*
 *  一日の計測リセット
 */
void
CDayCycle::
DayReset(void)
{
    m_CountTimer = 0.0f;
}

/*
 *  終了判定
 */
bool
CDayCycle::
GetDayFinish(void)const
{
    return m_CountTimer >= m_max_daycycle_time;
}

/*
 *  現在の時間を返す
 */
float
CDayCycle::
GetDayCycleNumber(void)const
{
    return m_CountTimer;
}

/*
 *  一日の最大時間を返す
 */
float
CDayCycle::
GetMaxDayCycleTime(void) const
{
    return m_max_daycycle_time;
}

/*
 *  現在の時間管理IDを返す
 */
DAYCYCLE_ID
CDayCycle::
GetDayCycleID(void)const
{
    return m_DayCycleState;
}