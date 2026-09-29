
/*!
 *  @file       game.h
 *  @brief      ゲーム管理
 *  @author		Ryusei Shimizu
 *  @date       2026/09/16
 */

#pragma once

#include "vivid.h"

class CGame
{
public:

    /*!
     *  @brief      ゲーム初期化
     */
    void        GameInitialize(void);

    /*!
     *  @brief      ゲーム更新
     */
    void        GameUpdate(void);

    /*!
     *  @brief      ゲーム描画
     */
    void        GameDraw(void);

    /*!
     *  @brief      ゲーム解放
     */
    void        GameFinalize(void);
};