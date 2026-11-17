
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

    if(vivid::keyboard::Button(vivid::keyboard::KEY_ID::D) || vivid::keyboard::Button(vivid::keyboard::KEY_ID::A))
        input.Direction = vivid::keyboard::Button(vivid::keyboard::KEY_ID::D) - vivid::keyboard::Button(vivid::keyboard::KEY_ID::A);

    if (vivid::keyboard::Button(vivid::keyboard::KEY_ID::RIGHT) || vivid::keyboard::Button(vivid::keyboard::KEY_ID::LEFT))
        input.Direction = vivid::keyboard::Button(vivid::keyboard::KEY_ID::RIGHT) - vivid::keyboard::Button(vivid::keyboard::KEY_ID::LEFT);

    input.JumpPressed = vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::W) ||
                        vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::SPACE);

    return input;
}
