
/*!
 *  @file		skill_resolver.cpp
 *  @brief		スキル解決クラス
 *  @author     Ryusei Shimizu
 *  @date       2026/06/26
 */

#include "skill_resolver.h"

/*
 *  スキル解決ネームスペース
 */
namespace
{
	const std::unordered_map< SALIGIA_ID, SkillSet> SkillTable =
	{
		{
			SALIGIA_ID::SUPERBIA,
			{
				ATTACK_ID::SUPERBIA_1,
				ATTACK_ID::SUPERBIA_2
			}
		},

		{
			SALIGIA_ID::AVARITIA,
			{
				ATTACK_ID::AVARITIA_1,
				ATTACK_ID::AVARITIA_2
			}
		},

		{
			SALIGIA_ID::LUXURIA,
			{
				ATTACK_ID::LUXURIA_1,
				ATTACK_ID::LUXURIA_2
			}
		},

		{
			SALIGIA_ID::INVIDIA,
			{
				ATTACK_ID::INVIDIA_1,
				ATTACK_ID::INVIDIA_2
			}
		},

		{
			SALIGIA_ID::GULA,
			{
				ATTACK_ID::GULA_1,
				ATTACK_ID::GULA_2
			}
		},

		{
			SALIGIA_ID::IRA,
			{
				ATTACK_ID::IRA_1,
				ATTACK_ID::IRA_2
			}
		},

		{
			SALIGIA_ID::ACEDIA,
			{
				ATTACK_ID::ACEDIA_1,
				ATTACK_ID::ACEDIA_2
			}
		}
	};
}

/*
 *  スキルX取得
 */
ATTACK_ID
SkillResolver::
GetSkillX(const BuildData& build)
{
	auto it = SkillTable.find(build.Primary);

	// もし見つからなければ、NONEを返す
	if (it == SkillTable.end())
		return ATTACK_ID::NONE;

	return it->second.SkillX;
}

/*
 *  スキルA取得
 */
ATTACK_ID
SkillResolver::
GetSkillA(const BuildData& build)
{
	auto it = SkillTable.find(build.Secondary);

	// もし見つからなければ、NONEを返す
	if (it == SkillTable.end())
		return ATTACK_ID::NONE;

	// もし同じ欲であれば、スキルAを返す
	bool same = build.Primary == build.Secondary;

	return same ? it->second.SkillA : it->second.SkillX;
}