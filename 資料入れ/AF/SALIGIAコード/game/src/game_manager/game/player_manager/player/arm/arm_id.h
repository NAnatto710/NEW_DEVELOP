
/*!
 *  @file		arm_id.h
 *  @brief		腕の識別子
 *  @author     Ryusei Shimizu
 *  @date       2026/09/02
 */

#pragma once

/*!
 *  @brief		腕の識別子
 */
enum class ARM_ID
{
	LEFT,		//!< 左腕
	RIGHT,		//!< 右腕

	BOTH		//!< 両腕
};

/*!
 *  @brief		腕の状態
 */
enum class ARM_STATE
{
	IDLE,		//!< 待機状態

	ATTACK,		//!< 攻撃状態

	IMPACT,		//!< 命中時の停止状態

	RETURN		//!< 退避状態
};
