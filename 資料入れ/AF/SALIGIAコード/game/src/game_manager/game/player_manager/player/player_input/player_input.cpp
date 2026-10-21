
/*!
 *  @file		player_input.cpp
 *  @brief		キャラクターの入力情報
 *  @author     Ryusei Shimizu
 *  @date       2026/06/29
 */

#include "player_input.h"

/*
 *	コンストラクタ
 */
CPlayerInput::
CPlayerInput()
{
}

/*
 *	初期化
 */
void
CPlayerInput::
Initialize(vivid::controller::DEVICE_ID device)
{
	m_Device = device;

	m_Input = Input{};

	m_IsJump = false;
	m_IsPlummet = false;
}

/*
 *	更新
 */
void
CPlayerInput::
Update()
{
	// 入力をリセット
	m_Input = {};

	UpdateMove();
	UpdateAction();
}

/*
 *	入力情報の取得
 */
const Input&
CPlayerInput::
GetInput() const
{
	return m_Input;
}

/*
 *  操作入力があるか
 */
bool
CPlayerInput::
IsOperating(void) const
{
    return
        m_Input.MoveX != 0.0f ||
        m_Input.MoveY != 0.0f ||
        m_Input.Jump ||
        m_Input.Attack ||
        m_Input.SkillA ||
        m_Input.SkillX ||
        m_Input.Guard ||
        m_Input.Throw ||
        m_Input.Plummet ||
        m_Input.PlummetReleased;
}

/*
 *	移動入力の更新
 */
void
CPlayerInput::
UpdateMove()
{
    constexpr float dead_zone = 0.75f;

    vivid::Vector2 stick = controller::GetAnalogStickLeft(m_Device);

    // 十字キー

    if (controller::Button(m_Device, controller::BUTTON_ID::LEFT) ||
        controller::Button(m_Device, controller::BUTTON_ID::RIGHT))
    {
        m_Input.MoveX =
            controller::Button(m_Device, controller::BUTTON_ID::RIGHT) -
            controller::Button(m_Device, controller::BUTTON_ID::LEFT);
    }

    if (controller::Button(m_Device, controller::BUTTON_ID::UP) ||
        controller::Button(m_Device, controller::BUTTON_ID::DOWN))
    {
        m_Input.MoveY =
            controller::Button(m_Device, controller::BUTTON_ID::DOWN) -
            controller::Button(m_Device, controller::BUTTON_ID::UP);
    }

	// デバッグ用（キーボード）
    if(m_Device == vivid::controller::DEVICE_ID::PLAYER1)
    {
        if (vivid::keyboard::Button(vivid::keyboard::KEY_ID::A))
            m_Input.MoveX = -1.0f;
        if (vivid::keyboard::Button(vivid::keyboard::KEY_ID::D))
            m_Input.MoveX = 1.0f;

        if (vivid::keyboard::Button(vivid::keyboard::KEY_ID::W))
            m_Input.MoveY = -1.0f;
        if (vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::SPACE))
            if (!m_IsJump)
            {
                m_Input.Jump = true;
                m_IsJump = true;
            }
        if (vivid::keyboard::Button(vivid::keyboard::KEY_ID::S))
        {
            m_Input.MoveY = 1.0f;
            m_Input.Plummet = true;
            m_IsPlummet = true;
        }
    }
    if (m_Device == vivid::controller::DEVICE_ID::PLAYER2)
    {
        if (vivid::keyboard::Button(vivid::keyboard::KEY_ID::LEFT))
            m_Input.MoveX = -1.0f;
        if (vivid::keyboard::Button(vivid::keyboard::KEY_ID::RIGHT))
            m_Input.MoveX = 1.0f;

        if (vivid::keyboard::Button(vivid::keyboard::KEY_ID::UP))
            m_Input.MoveY = -1.0f;
        if (vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::UP))
            if (!m_IsJump)
            {
                m_Input.Jump = true;
                m_IsJump = true;
            }
        if (vivid::keyboard::Button(vivid::keyboard::KEY_ID::DOWN))
        {
            m_Input.MoveY = 1.0f;
            m_Input.Plummet = true;
            m_IsPlummet = true;
        }
    }

    // 左スティック

    if (stick.Length() < dead_zone)
    {
        m_IsJump = false;

        if (m_IsPlummet)
        {
            m_Input.PlummetReleased = true;
            m_IsPlummet = false;
        }

        return;
    }

    // 横移動

    if (stick.x >= dead_zone ||
        stick.x <= -dead_zone)
        m_Input.MoveX = stick.x;

    // 上入力

    if (stick.y <= -dead_zone)
    {
        m_Input.MoveY = stick.y;

       /* if (!m_IsJump)
        {
            m_Input.Jump = true;
            m_IsJump = true;
        }*/
    }

    // 下入力

    if (stick.y >= dead_zone)
    {
        m_Input.MoveY = stick.y;

        m_Input.Plummet = true;
        m_IsPlummet = true;
    }
}

/*
 *	アクション入力の更新
 */
void
CPlayerInput::
UpdateAction()
{
    m_Input.Jump |= controller::Trigger(m_Device, controller::BUTTON_ID::Y) ||
        vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::W) && m_Device == vivid::controller::DEVICE_ID::PLAYER1 ||
        vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::UP) && m_Device == vivid::controller::DEVICE_ID::PLAYER2;

    m_Input.Attack = controller::Trigger(m_Device, controller::BUTTON_ID::B) ||
        vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::J) && m_Device == vivid::controller::DEVICE_ID::PLAYER1 ||
        vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::K) && m_Device == vivid::controller::DEVICE_ID::PLAYER2;

    m_Input.SkillX = controller::Trigger(m_Device, controller::BUTTON_ID::X) ||
        vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::Y) && m_Device == vivid::controller::DEVICE_ID::PLAYER1 ||
        vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::P) && m_Device == vivid::controller::DEVICE_ID::PLAYER2;;

    m_Input.SkillA = controller::Trigger(m_Device, controller::BUTTON_ID::A) ||
        vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::U) && m_Device == vivid::controller::DEVICE_ID::PLAYER1 ||
        vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::O) && m_Device == vivid::controller::DEVICE_ID::PLAYER2;;

    m_Input.Guard = controller::Button(m_Device, controller::BUTTON_ID::LEFT_SHOULDER) ||
        vivid::keyboard::Button(vivid::keyboard::KEY_ID::H) && m_Device == vivid::controller::DEVICE_ID::PLAYER1 ||
        vivid::keyboard::Button(vivid::keyboard::KEY_ID::L) && m_Device == vivid::controller::DEVICE_ID::PLAYER2;

    m_Input.Throw = controller::Trigger(m_Device, controller::BUTTON_ID::RIGHT_SHOULDER) ||
        vivid::keyboard::Button(vivid::keyboard::KEY_ID::N) && m_Device == vivid::controller::DEVICE_ID::PLAYER1 ||
        vivid::keyboard::Button(vivid::keyboard::KEY_ID::M) && m_Device == vivid::controller::DEVICE_ID::PLAYER2;

    m_Input.Plummet |= controller::Button(m_Device, controller::BUTTON_ID::DOWN);

    m_Input.PlummetReleased |= controller::Released(m_Device, controller::BUTTON_ID::DOWN);
}
