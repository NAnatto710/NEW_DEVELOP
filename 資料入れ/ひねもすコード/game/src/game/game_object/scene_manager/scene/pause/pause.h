
/*!
 *  @file       pause.h
 *  @brief      ポーズシーン
 *  @author     Ryusei Shimizu
 *  @date       2025/12/18
 */

#pragma once

#include "vivid.h"
#include "../scene.h"


/*!
 *  @class      CPause
 *
 *  @brief      ポーズシーンクラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2025/12/18
 */
class CPause
    :public IScene
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
    void        Initialize(void);

    /*!
     *  @brief      更新
     */
    void        Update(void);

    /*!
     *  @brief      描画
     */
    void        Draw(void);

    /*!
     *  @brief      解放
     */
    void        Finalize(void);

private:

    static const float                  m_scene_change_wait_time;   //!< シーン切り替え待機時間

    float                               m_WaitTime;                 //!< 待機時間
};