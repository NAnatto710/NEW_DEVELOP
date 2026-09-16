
/*!
 *  @file       game_main.h
 *  @brief      ゲームメインシーン
 *  @author     Ryusei Shimizu
 *  @date       2025/10/08
 */

#pragma once

#include "vivid.h"
#include "../scene.h"

/*!
  *  @class      CGameMain
  *
  *  @brief      ゲームメインシーンクラス
  *
  *  @author     Ryusei Shimizu
  *
  *  @date       2025/10/08
  */
class CGameMain
    :public IScene
{
public:

    /*!
     *  @brief      コンストラクタ
     */
    CGameMain(void);

    /*!
     *  @brief      デストラクタ
     */
    ~CGameMain(void);

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

    static const float    m_scene_change_wait_time;      //!< シーン切り替え待機時間

    float		          m_WaitTime;                    //!< 待機時間
};