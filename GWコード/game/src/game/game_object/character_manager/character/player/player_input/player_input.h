
/*!
 *  @file           player_input.h
 *  @brief          プレイヤー
 *  @author         Ryusei Shimizu
 *  @date           2026/10/16
 */

#pragma once

// 入力データ
struct CharacterMoveInput
{
    float   Direction   = 0.0f;     //!< 方向
    bool    JumpPressed = false;    //!< ジャンプ
};

class CPlayerInput
{
public:

    /*!
     *  @brief  入力情報の取得
     * 
     *  @return 入力データ
     */
    CharacterMoveInput GetInput (void) const;
};
