
/*!
 *  @file		velocity.h
 *  @brief		キャラクターの速度情報
 *  @author     Ryusei Shimizu
 *  @date       2026/05/21
 */

#pragma once
#include "vivid.h"

/*!
 *	@brief	速度情報構造体
 */
struct VelocityInfo
{
	vivid::Vector2 Move			= { 0.0f,0.0f };	//!< 移動速度

	vivid::Vector2 Gravity		= { 0.0f,0.0f };    //!< 重力加速度

	vivid::Vector2 KnockBack	= { 0.0f,0.0f };	//!< ノックバック速度

	/*!
	 *	@brief	最終的な速度を取得する関数
	 *
	 *	@return	移動速度 + 重力加速度 + ノックバック速度
	 */
	vivid::Vector2
	GetFinalVelocity(void) const
	{
		return Move + Gravity + KnockBack;
	}

	/*!
	 *	@brief	速度情報をクリアする関数
	 */
	void
	Clear(void)
	{
		Move = { 0.0f,0.0f };
		Gravity = { 0.0f,0.0f };
		KnockBack = { 0.0f,0.0f };
	}
};

/*!
 *	@brief	速度ID列挙型
 */
enum class VelocityID
{
	Move,		//!< 移動速度
	Gravity,	//!< 重力加速度
	KnockBack,	//!< ノックバック速度
	Final		//!< 最終的な速度（移動速度 + 重力加速度 + ノックバック速度）
};