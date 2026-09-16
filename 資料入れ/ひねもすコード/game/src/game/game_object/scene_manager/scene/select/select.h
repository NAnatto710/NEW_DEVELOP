
/*!
 *  @file       select.h
 *  @brief      セレクトシーン
 *  @author     Ryusei Shimizu
 *  @date       2026/02/04
 */

#pragma once

#include "vivid.h"
#include "../scene.h"
#include "../../../upgrade_manager/upgrade_manager.h"

/*!
 *  @class      CSelect
 *
 *  @brief      セレクトシーンクラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2026/02/04
 */
class CSelect
    :public IScene
{
public:

    /*!
     *  @brief      コンストラクタ
     */
    CSelect(void);

    /*!
     *  @brief      デストラクタ
     */
    ~CSelect(void);

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

    static const float                  m_scene_change_wait_time;       //!< シーン切り替え待機時間

    float	                            m_WaitTime;                     //!< 待機時間
};