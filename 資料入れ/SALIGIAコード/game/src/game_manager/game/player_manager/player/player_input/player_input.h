
/*!
 *  @file		player_input.h
 *  @brief		キャラクターの入力情報
 *  @author     Ryusei Shimizu
 *  @date       2026/06/29
 */

#pragma once

#include "vivid.h"

namespace controller = vivid::controller;

/*!
 *	@brief		入力情報構造体
 */
struct Input
{
    float MoveX = 0.0f;
    float MoveY = 0.0f;

    bool Jump = false;

    bool Attack = false;
    bool SkillA = false;
    bool SkillX = false;

    bool Guard = false;
	bool Throw = false;

    bool Plummet = false;
    bool PlummetReleased = false;
};

/*!
 *  @class		CPlayerInput
 *
 *  @brief		プレイヤーの入力情報クラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2026/06/29
 */
class CPlayerInput
{
public:

    /*!
     *  @brief				コンストラクタ
     */
    CPlayerInput(void);

    /*!
     *	@brief              デストラクタ
     */
    ~CPlayerInput(void) = default;

    /*!
     *	@brief				初期化
     *
     *  @param[in]          device  デバイスID
     */
    void Initialize(vivid::controller::DEVICE_ID device);

    /*!
     *	@brief				更新
     */
    void Update(void);

    /*!
     *	@brief				入力情報の取得
     *
     *  @return             入力情報
     */
    const Input& GetInput(void) const;

    /*!
     *  @brief      操作入力があるか
     *
     *  @return     操作入力があればtrue
     */
    bool IsOperating(void) const;

private:

    /*!
     *	@brief				移動入力の更新
     */
    void UpdateMove(void);

    /*!
     *	@brief				アクション入力の更新
     */
    void UpdateAction(void);

    controller::DEVICE_ID   m_Device;       //!< デバイスID

    Input                   m_Input;        //!< 入力情報

    bool                    m_IsJump;       //!< ジャンプ入力の状態を保持するフラグ
	bool                    m_IsPlummet;    //!< 急落下入力の状態を保持するフラグ
};