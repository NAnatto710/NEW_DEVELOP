
/*!
 *  @file		state_controller.h
 *  @brief		状態管理クラス
 *  @author     Ryusei Shimizu
 *  @date       2026/05/21
 */

#pragma once
#include "state_id.h"

class CPhysicsComponent;
class CPlayer;

/*!
 *	@class		CStateController
 *
 *	@brief		状態管理クラス
 *
 *	@author     Ryusei Shimizu
 *
 *  @date       2026/05/21
 */
class CStateController
{
public:

    /*!
	 *  @brief          コンストラクタ
     */
    CStateController();

    /*!
	 *  @brief          デストラクタ
     */
	~CStateController(void) = default;

    /*!
     *  @brief          初期化
	 */
    void                Initialize(void);

	/*!
	 *  @brief          更新
     * 
	 *  @param[in]      physics     物理演算コンポーネント
     * 
	 *  @return         現在の状態
     */
    MOVE_STATE          MoveStateUpdate(CPhysicsComponent* physics);

    /*!
     *  @brief          更新
     *
	 *  @param[in]      character   キャラクター
     * 
	 *  @return         現在の状態
     */
    DESIRE_STATE        DesireStateUpdate(CPlayer* player);

	/*!
	 *  @brief          状態変更
     * 
	 *  @param[in]      state   変更する状態
     */
    void                ChangeMoveState(MOVE_STATE state);

    /*!
     *  @brief          状態変更
     *
     *  @param[in]      state   変更する状態
	 */
	void 	            ChangeDesireState(DESIRE_STATE state);

    /*!
     *  @brief          状態変更
     *
     *  @param[in]      state   変更する状態
	 */
	void 			    ChangeActionState(ACTION_STATE state);

    /*!
     *  @brief          状態変更
     *
     *  @param[in]      state   変更する状態
	 */
	void 			    ChangeAliveState(ALIVE_STATE state);

    /*!
	 *  @brief          状態取得
     * 
	 *  @return         現在の状態
     */
    MOVE_STATE          GetMoveState(void) const { return m_MoveState; }

    /*!
     *  @brief          状態取得
     *
     *  @return         現在の状態
	 */
	DESIRE_STATE        GetDesireState(void) const { return m_DesireState; }

    /*!
     *  @brief          状態取得
     *
     *  @return         現在の状態
	 */
	ACTION_STATE        GetActionState(void) const { return m_ActionState; }

	/*!
	 *  @brief          状態取得
	 *
	 *  @return         前の状態
	 */
	ACTION_STATE		GetActionPrevState(void) const { return m_ActionPrevState; }

    /*!
     *  @brief          状態取得
     *
     *  @return         現在の状態
	 */
	ALIVE_STATE		    GetAliveState(void) const { return m_AliveState; }

	/*!
     *  @brief          状態判定
     * 
	 *  @param[in]      state   判定する状態
     * 
	 *  @return         指定した状態かどうか
     */
    bool                IsMoveState(MOVE_STATE state) const { return m_MoveState == state; }

    /*!
     *  @brief          状態判定
     *
     *  @param[in]      state   判定する状態
     *
	 *  @return         指定した状態かどうか
     */
	bool		        IsDesireState(DESIRE_STATE state) const { return m_DesireState == state; }

    /*!
     *  @brief          状態判定
     *
     *  @param[in]      state   判定する状態
     *
     *  @return         指定した状態かどうか
	 */
	bool		        IsActionState(ACTION_STATE state) const { return m_ActionState == state; }

    /*!
     *  @brief          状態判定
     *
     *  @param[in]      state   判定する状態
     *
     *  @return         指定した状態かどうか
	 */
	bool				IsAliveState(ALIVE_STATE state) const { return m_AliveState == state; }

private:

    MOVE_STATE      m_MoveState;        //!< 現在の動作状態
	DESIRE_STATE    m_DesireState;      //!< 現在の欲望帯状態
	ACTION_STATE    m_ActionState;      //!< 現在のキャラクター動作状態
	ACTION_STATE    m_ActionPrevState;  //!< 前のキャラクター動作状態
	ALIVE_STATE     m_AliveState;       //!< 現在の生存状態
};