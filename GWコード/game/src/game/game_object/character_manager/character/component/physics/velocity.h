
/*!
 *  @file           vector.h
 *  @brief          キャラクターの速度情報
 *  @author         Ryusei Shimizu
 *  @date           2026/10/13
 */


#pragma once

#include "vivid.h"

/*
 *  キャラクターの速度情報を管理する構造体
 */
struct VelocityInfo
{
    vivid::Vector2 Move = { 0.0f, 0.0f };
    vivid::Vector2 Gravity = { 0.0f, 0.0f };

    /*!
     *  @brief  最終的な速度を取得する
     *
     *  @return 移動速度 + 重力による速度
     */
    vivid::Vector2
    GetFinalVelocity(void) const
    {
        return Move + Gravity;
    }

    /*!
     *  @brief  すべての速度を初期化する
     */
    void
    Clear(void)
    {
        Move = { 0.0f, 0.0f };
        Gravity = { 0.0f, 0.0f };
    }
};
