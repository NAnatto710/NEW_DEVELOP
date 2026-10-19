
/*!
 *  @file           player.cpp
 *  @brief          プレイヤー
 *  @author         Ryusei Shimizu
 *  @date           2026/10/13
 */

#include "player.h"

/*
 *	初期化
 */
void 
CPlayer::
Initialize (int width, int height, const vivid::Vector2& position)
{
    m_Movement.Initialize ();
    m_Physics.Initialize (width, height, position);
    m_CachedPosition = position;
}

/*
 *	更新
 */
void 
CPlayer::
Update(void)
{
    const CharacterMoveInput input = m_Input.GetInput ();
    m_Movement.Update (input.Direction, input.JumpPressed, m_Physics);
    m_Physics.Update ();
    m_CachedPosition = m_Physics.GetPosition ();
}

/*
 *	描画
 */
void
CPlayer::
Draw(void)
{
    // 仮描画：画像・カメラを接続する前の当たり判定確認用
    const auto pos = m_Physics.GetPosition ();
    vivid::DrawTexture("data\\character\\player.png",pos);
}
