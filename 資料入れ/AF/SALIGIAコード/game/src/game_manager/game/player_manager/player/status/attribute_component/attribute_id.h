
/*!
 *  @file		attribute_id.h
 *  @brief		キャラクターの属性ID
 *  @author     Ryusei Shimizu
 *  @date       2026/07/07
 */

#pragma once

/*!
 *	@brief	ステータス属性補正タイプ
 */
enum class ATTRIBUTE_MODIFIER_TYPE
{
	ADD,		//!< 加算
	RATE		//!< 倍率
};

/*!
 *	@brief	属性構造体
 */
struct AttributeModifier
{
	ATTRIBUTE_MODIFIER_TYPE Type;	//!< 属性補正タイプ

	float	Value = 0.0f;			//!< 属性値
	int     Time = 0;				//!< 属性値の持続時間

	bool	Enable = true;			//!< 有効フラグ
	bool    IsPermanent = false;	//!< 永続フラグ

	/*!
	 *	@brief	属性値の有効期限を判定
	 * 
	 *	@return	有効期限切れかどうか
	 */
	bool    IsExpired() const { return Time <= 0; }
};

/*!
 *	@brief	属性構造体
 */
struct AttributeValue
{
	float Base	= 0.0f;		//!< 基本値
};

/*!
 *	@brief	属性がかかるステータスID
 */
enum class ATTRIBUTE_ID
{
	SPEED,              //!< 移動速度
	JUMP_POWER,         //!< ジャンプ力
	ATTACK_POWER,       //!< 攻撃力
	DESIRE_GAIN,        //!< 欲望獲得量
	COST_DESIRE,		//!< 欲望消費量
	KNOCKBACK_POWER,	//!< ノックバック力
	KNOCKBACK_RESIST,	//!< ノックバック耐性
	GUARD_POWER,        //!< ガード性能

	MAX				//!< 最大値
};