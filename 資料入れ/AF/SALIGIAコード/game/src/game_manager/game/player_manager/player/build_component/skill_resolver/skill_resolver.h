
/*!
 *  @file		skill_resolver.h
 *  @brief		スキル解決ネームスペース
 *  @author     Ryusei Shimizu
 *  @date       2026/06/26
 */

#pragma once

#include <unordered_map>

#include "../build_data.h"
#include "../../../../../../utility/csv_loader/loader/attack_csv_loader/attack_id.h"

/*!
 *	@brief		スキルセット構造体
 */
struct SkillSet
{
	ATTACK_ID SkillX;	//!< スキルXの攻撃ID
	ATTACK_ID SkillA;	//!< スキルAの攻撃ID
};

/*!
 *	@namespace	SkillResolver
 *
 *	@brief		スキル解決ネームスペース
 */
namespace SkillResolver
{
	/*!
	 *  @brief				スキルX取得
	 *
	 *  @param[in]			build  ビルドデータ
	 *
	 *  @return				スキルXの攻撃ID
	 */
	ATTACK_ID GetSkillX( const BuildData& build);

	/*!
	 *  @brief				スキルY取得
	 *
	 *  @param[in]			build  ビルドデータ
	 *
	 *  @return				スキルYの攻撃ID
	 */
	ATTACK_ID GetSkillA( const BuildData& build);
}