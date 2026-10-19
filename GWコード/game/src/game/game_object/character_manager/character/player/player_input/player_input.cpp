
/*!
 *  @file           player_input.cpp
 *  @brief          プレイヤー
 *  @author         Ryusei Shimizu
 *  @date           2026/10/16
 */

#include "player_input.h"
#include "vivid.h"

/*
 *  入力情報の取得
 */
CharacterMoveInput
CPlayerInput::
GetInput (void) const
{
    CharacterMoveInput input;

    if(vivid::keyboard::Button (vivid::keyboard::KEY_ID::A)) input.Direction -= 1.0f;
    if(vivid::keyboard::Button (vivid::keyboard::KEY_ID::D)) input.Direction += 1.0f;

    input.JumpPressed = vivid::keyboard::Trigger (vivid::keyboard::KEY_ID::W) ||
                        vivid::keyboard::Trigger (vivid::keyboard::KEY_ID::SPACE);

    return input;
}
