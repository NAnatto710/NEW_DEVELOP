
/*!
 *  @file           player_input.h
 *  @brief          プレイヤー
 *  @author         Ryusei Shimizu
 *  @date           2026/10/16
 */

#pragma once

// 入力データ。将来の敵AIも同じ形式を出力できる。
struct CharacterMoveInput
{
    float Direction = 0.0f;
    bool JumpPressed = false;
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
