
/*!
 *  @file       passive_id.h
 *  @brief      パッシブID定義
 *  @author     Ryusei Shimizu
 *  @date       2026/07/21
 */

#pragma once

#include "../../../../game_manager/game/player_manager/player_id.h"
#include "../../../../game_manager/game/player_manager/player/status/attribute_component/attribute_id.h"

/*!
 *  @brief      パッシブCSV列定義
 */
enum class PASSIVE_CSV
{
    SALIGIA,		//!< 選択した欲望
	CONDITION,		//!< 条件
	ATTRIBUTE,		//!< 属性
	TYPE,			//!< タイプ
	VALUE,			//!< 値

    MAX         //!< 最大値	
};

/*!
 *  @brief      パッシブ条件定義
 */
enum class PASSIVE_CONDITION
{
	NORMAL,			//!< 通常
	SAME,			//!< ビルドが重複
    MUYOKU,			//!< 無欲

	MAX			//!< 最大値
};

/*!
 *  @brief      パッシブ条件定義
 */
struct PassiveInfo
{
	SALIGIA_ID			Saligia;     //!< 選択した欲望
    PASSIVE_CONDITION	Condition;   //!< 条件

	ATTRIBUTE_ID		Attribute;   //!< どのステータスにかかるか
    AttributeModifier	Modifier;    //!< 修正値
};