
/*!
 *  @file		pause.h
 *  @brief		ポーズ
 *  @author     Ryusei Shimizu
 *  @date       2026/04/10
 */

#pragma once

#include "vivid.h"
#include "../scene.h"

/*!
 *	@class		CPause
 *
 *	@brief		ポーズシーン
 *
 *	@author     Ryusei Shimizu
 *
 *  @date       2026/04/10
 */
class CPause
    : public IScene
{
public:

    /*!
     *  @brief      コンストラクタ
     */
    CPause(void);

    /*!
     *  @brief      デストラクタ
     */
    ~CPause(void);

    /*!
     *  @brief      初期化
     */
    void            Initialize(void);

    /*!
     *  @brief      更新
     */
    void            Update(void);

    /*!
     *  @brief      描画
     */
    void            Draw(void);

    /*!
     *  @brief      解放
     */
    void            Finalize(void);

private:

    bool    m_IsInputLock;      //!< 入力ロック
    bool    m_IsCountdown;      //!< カウントダウン中か
    float   m_CountdownTimer;   //!< 1カウント分の時間
    int     m_CountdownCount;   //!< 3,2,1

};