
/*!
 *  @file		state_id.h
 *  @brief		キャラクターの状態ID
 *  @author     Ryusei Shimizu
 *  @date       2026/06/29
 */

#pragma once

/*!
 *	@brief	キャラクターの生存状態ID
 */
enum class ALIVE_STATE
{
	ALIVE,				//!< 生存
	DEAD,				//!< 死亡
};

/*!
 *	キャラクターの移動状態
 */
enum class MOVE_STATE
{
	NONE,				//!< 空

	IDLE,				//!< 待機
	DASH,				//!< ダッシュ
	JUMP,				//!< ジャンプ
	FALL,				//!< 落下
	ATTACK,				//!< 攻撃中
	SKILL,				//!< スキル中
	GUARD,				//!< ガード中
	STIFFNESS,			//!< 硬直中

	MAX,				//!< 最大数
};

/*!
 *	欲望帯の状態
 */
enum class DESIRE_STATE
{
	MUYOKU,		//!< 無欲状態
	NORMAL,		//!< 通常欲望帯
};

/*!
 *	キャラクターの動作状態
 */
enum class ACTION_STATE
{
	IDLE,				//!< 待機

	ATTACK_NEUTRAL,		//!< 通常攻撃
	ATTACK_SIDE,		//!< 横攻撃
	ATTACK_UP,			//!< 上攻撃
	ATTACK_DOWN,		//!< 下攻撃

	SKILLX,				//!< スキルX
	SKILLA,				//!< スキルA

	GUARD,				//!< ガード

	THROW,				//!< 投げ
	THROWN,				//!< 投げられ

	STIFFNESS,			//!< 硬直
	RESPAWN,			//!< リスポーン

	MAX,				//!< 最大数
};