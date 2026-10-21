
/*!
 *  @file		state_controller.cpp
 *  @brief		状態管理クラス
 *  @author     Ryusei Shimizu
 *  @date       2026/05/21
 */

#include "state_controller.h"
#include "../physics_component/physics_component.h"
#include "../player.h"

/*
 *  コンストラクタ
 */
CStateController::
CStateController()
{
}

/*
 *  初期化
 */
void
CStateController::
Initialize(void)
{
    m_MoveState = MOVE_STATE::NONE;
	m_DesireState = DESIRE_STATE::NORMAL;
	m_ActionState = ACTION_STATE::IDLE;
	m_ActionPrevState = ACTION_STATE::IDLE;
	m_AliveState = ALIVE_STATE::ALIVE;
}

/*
 *  更新
 */
MOVE_STATE
CStateController::
MoveStateUpdate(CPhysicsComponent* physics)
{
	// ダメージを受けているときはダメージ状態
	if (this->GetActionState() == ACTION_STATE::STIFFNESS)
    {
        this->ChangeMoveState(MOVE_STATE::STIFFNESS);
        return m_MoveState;
    }

	// 攻撃中のときは攻撃状態
    if (this->GetActionState() == ACTION_STATE::ATTACK_NEUTRAL ||
        this->GetActionState() == ACTION_STATE::ATTACK_SIDE ||
        this->GetActionState() == ACTION_STATE::ATTACK_UP ||
        this->GetActionState() == ACTION_STATE::ATTACK_DOWN)
    {
        this->ChangeMoveState(MOVE_STATE::ATTACK);
        return m_MoveState;
	}

	// スキル中のときはスキル状態
    if(this->GetActionState() == ACTION_STATE::SKILLX ||
        this->GetActionState() == ACTION_STATE::SKILLA)
    {
        this->ChangeMoveState(MOVE_STATE::SKILL);
        return m_MoveState;
	}

	//  ガード中のときはガード状態
    if (this->GetActionState() == ACTION_STATE::GUARD)
    {
        this->ChangeMoveState(MOVE_STATE::GUARD);
        return m_MoveState;
    }

    // 着地判定
    if (!physics->IsLanding())
    {
        // Y速度で上昇か落下か判定
        if (physics->GetVelocity().y < 0.0f)
            this->ChangeMoveState(MOVE_STATE::JUMP);
        else
            this->ChangeMoveState(MOVE_STATE::FALL);

		return m_MoveState;
    }

    // 地上にいるとき
    if (physics->GetVelocity().x != 0.0f)
        this->ChangeMoveState(MOVE_STATE::DASH);
    else
        this->ChangeMoveState(MOVE_STATE::IDLE);

    return m_MoveState;
}

/*
 *  更新
 */
DESIRE_STATE
CStateController::
DesireStateUpdate(CPlayer* player)
{
    // 欲望の値によって状態を分ける
    float desire = player->GetResourceComponent().GetCurrent(RESOURCE_ID::DESIRE);
	BuildData build = player->GetBuildComponent().GetBuild();

	// 無欲状態かどうか
    if (m_DesireState == DESIRE_STATE::MUYOKU)
    {
		// 無欲状態に１度入ったら、一定の欲望値まで回復するまでは無欲として扱う
		if (desire >= 20)
        {
            this->ChangeDesireState(DESIRE_STATE::NORMAL);
        }
    }
    else
    {
		// 欲望が0以下になったら無欲状態にする
        if (desire <= 0.0f)
        {
			// 無欲状態
			this->ChangeDesireState(DESIRE_STATE::MUYOKU);
			return m_DesireState;
        }
        else
        {
			// 通常欲望帯
			this->ChangeDesireState(DESIRE_STATE::NORMAL);
        }
    }

	return m_DesireState;
}

/*
 *  状態変更
 */
void
CStateController::
ChangeMoveState(MOVE_STATE state)
{
	m_MoveState = state;
}

/*
 *  状態変更
 */
void
CStateController::
ChangeDesireState(DESIRE_STATE state)
{
	m_DesireState = state;
}

/*
 *  状態変更
 */
void
CStateController::
ChangeActionState(ACTION_STATE state)
{
	// すでに同じ状態の場合は何もしない
    if (m_ActionState == state) return;

    m_ActionPrevState = m_ActionState;

    m_ActionState = state;
}

/*
 *  状態変更
 */
void
CStateController::
ChangeAliveState(ALIVE_STATE state)
{
	m_AliveState = state;
}
