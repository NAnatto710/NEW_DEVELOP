
/*!
 *  @file       daycycle.h
 *  @brief      一日の時間管理
 *  @author     Ryusei Shimizu
 *  @date       2025/10/21
 */

#pragma once

#include "vivid.h"
#include "daycycle_id.h"

/*!
 *  @class      CDayCycle
 *
 *  @brief      一日の時間管理クラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2025/10/21
 */
class CDayCycle
{
public:

    /*!
     *  @brief      コンストラクタ
     */
    CDayCycle();

    /*!
     *  @brief      デストラクタ
     */
    ~CDayCycle();

    /*!
     *  @brief      初期化
     */
    void        Initialize(void);

    /*!
     *  @brief      更新
     */
    void        Update(void);

    /*!
     *  @brief  描画
     */
    void        Draw(void);

    /*!
     *  @brief      解放
     */
    void        Finalize(void);

    /*!
     *  @brief      一日の計測スタート
     */
    void        DayStart(void);

    /*!
     *  @brief      一日の計測ストップ
     */
    void        DayStop(void);

    /*!
     *  @brief      一日の計測リセット
     */
    void        DayReset(void);

    /*!
     *  @brief      終了判定
     *
     *  @return     終了判定
     */
    bool        GetDayFinish(void)const;

    /*!
     *  @brief      現在の時間を返す
     *
     *  @return     時間の数値
     */
    float       GetDayCycleNumber(void)const;

	/*!
	 *  @brief      一日の最大時間を返す
	 *
	 *  @return     一日の最大時間
	 */
	float       GetMaxDayCycleTime(void)const;

    /*!
     *  @brief      現在の時間管理IDを返す
     *
     *  @return     時間管理ID
     */
    DAYCYCLE_ID GetDayCycleID(void)const;

private:

    static const float  m_max_morning_time;     //!< 朝の最大時間
    static const float  m_max_daytime_time;     //!< 昼の最大時間
    static const float  m_max_night_time;       //!< 夜の最大時間
    static const float  m_max_daycycle_time;    //!< 一日の最大時間

    static const unsigned int m_morning_color;  //!< 朝の背景色
    static const unsigned int m_daytime_color;  //!< 昼の背景色
    static const unsigned int m_night_color;    //!< 夜の背景色   
    static const unsigned int m_change_color;   //!< 背景色の変化量

    float               m_CountTimer;           //!< 時間計測タイマー
    bool                m_CountFlg;             //!< 時間計測をするかどうかの判別フラグ

    unsigned int        m_BackColor;            //!< 背景色

    DAYCYCLE_ID         m_DayCycleState;        //!< 時間管理ID
};