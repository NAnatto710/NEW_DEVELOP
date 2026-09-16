
/*!
 *  @file       attack_csv_loader.cpp
 *  @brief      攻撃CSVローダー
 *  @author     Ryusei Shimizu
 *  @date       2026/09/01
 */

#include "attack_csv_loader.h"

#include "../../csv_helper/csv_helper.h"
#include "../../csv_convert/csv_convert.h"

const std::unordered_map<std::string, ATTACK_ID> CAttackCSVLoader::m_Table =
{
    { "ATTACK_NEUTRAL", ATTACK_ID::ATTACK_NEUTRAL },
    { "ATTACK_SIDE",    ATTACK_ID::ATTACK_SIDE    },
    { "ATTACK_UP",      ATTACK_ID::ATTACK_UP      },
    { "ATTACK_DOWN",    ATTACK_ID::ATTACK_DOWN    },

    { "SUPERBIA_1", ATTACK_ID::SUPERBIA_1 },
    { "SUPERBIA_2", ATTACK_ID::SUPERBIA_2 },

    { "AVARITIA_1", ATTACK_ID::AVARITIA_1 },
    { "AVARITIA_2", ATTACK_ID::AVARITIA_2 },

    { "LUXURIA_1", ATTACK_ID::LUXURIA_1 },
    { "LUXURIA_2", ATTACK_ID::LUXURIA_2 },

    { "INVIDIA_1", ATTACK_ID::INVIDIA_1 },
    { "INVIDIA_2", ATTACK_ID::INVIDIA_2 },

    { "GULA_1", ATTACK_ID::GULA_1 },
    { "GULA_2", ATTACK_ID::GULA_2 },

    { "IRA_1", ATTACK_ID::IRA_1 },
    { "IRA_2", ATTACK_ID::IRA_2 },

    { "ACEDIA_1", ATTACK_ID::ACEDIA_1 },
    { "ACEDIA_2", ATTACK_ID::ACEDIA_2 },
};

const std::unordered_map<std::string, ATTACK_EASING_TYPE> CAttackCSVLoader::m_EasingTable =
{
    { "LINEAR",      ATTACK_EASING_TYPE::LINEAR },
    { "EASE_IN",     ATTACK_EASING_TYPE::EASE_IN },
    { "EASE_OUT",    ATTACK_EASING_TYPE::EASE_OUT },
    { "EASE_IN_OUT", ATTACK_EASING_TYPE::EASE_IN_OUT },
};

const std::unordered_map<std::string, ARM_ROTATION_TYPE> CAttackCSVLoader::m_RotationTable =
{
    { "PATH",  ARM_ROTATION_TYPE::PATH },
    { "LERP",  ARM_ROTATION_TYPE::LERP_ROTATION },
    { "FIXED", ARM_ROTATION_TYPE::FIXED },
    { "SPIN",  ARM_ROTATION_TYPE::ROTATE_SPIN },
};

/*
 *  攻撃IDをインデックスに変換
 */
namespace
{
    constexpr size_t ToIndex(ATTACK_ID id)
    {
        return static_cast<size_t>(id);
    }
}

/*
 *  コンストラクタ
 */
CAttackCSVLoader::
CAttackCSVLoader()
{}

/*
 *  CSVロード
 */
bool
CAttackCSVLoader::
Load(const std::string& filepath)
{
    // 攻撃データを初期化
    m_AttackDatas = {};

    return LoadFile(
        filepath,
        [&](const std::string& line)
        {
            return LoadLine(line);
        });
}

/*
 *  1行ロード
 */
bool
CAttackCSVLoader::
LoadLine(const std::string& line)
{
    // CSVを分割
    auto values = CSV::Split(line);

    // 列数が不正な場合は失敗
    if (!CSV::CheckColumnCount(values, ToIndex(ATTACK_CSV::MAX))) return false;

    ATTACK_ID id;

    // 攻撃IDを変換
    if (!CSV::ParseEnum(m_Table, values[ToIndex(ATTACK_CSV::ATTACK_ID)], id)) return false;

    // 攻撃データを取得
    auto& attack_data = m_AttackDatas[ToIndex(id)];

    // 腕IDを取得
    const auto& arm_id = values[ToIndex(ATTACK_CSV::ARM_ID)];


    // 右腕
    if (arm_id == "RIGHT")
    {
        attack_data.RightArm.ID = id;

        ReadAttack(values, attack_data.RightArm);
    }
    // 左腕
    else if (arm_id == "LEFT")
    {
        attack_data.LeftArm.ID = id;

        ReadAttack(values, attack_data.LeftArm);
    }
    else
    {
        return false;
    }

    return true;
}

/*
 *  攻撃情報読み込み
 */
void
CAttackCSVLoader::
ReadAttack(const std::vector<std::string>& values, AttackInfo& attack)
{
    // ベジェ軌道
    attack.Control1.x = CSV::Get<float>(values, ToIndex(ATTACK_CSV::CONTROL1_X));
    attack.Control1.y = CSV::Get<float>(values, ToIndex(ATTACK_CSV::CONTROL1_Y));
    attack.Control2.x = CSV::Get<float>(values, ToIndex(ATTACK_CSV::CONTROL2_X));
    attack.Control2.y = CSV::Get<float>(values, ToIndex(ATTACK_CSV::CONTROL2_Y));
    attack.EndPosition.x = CSV::Get<float>(values, ToIndex(ATTACK_CSV::END_X));
    attack.EndPosition.y = CSV::Get<float>(values, ToIndex(ATTACK_CSV::END_Y));

    attack.MoveFrame = CSV::Get<int>(values, ToIndex(ATTACK_CSV::MOVE_FRAME));
    attack.HoldFrame = CSV::Get<int>(values, ToIndex(ATTACK_CSV::HOLD_FRAME));
    attack.ActiveStartFrame = CSV::Get<int>(values, ToIndex(ATTACK_CSV::ACTIVE_START_FRAME));
    attack.ActiveFrame = CSV::Get<int>(values, ToIndex(ATTACK_CSV::ACTIVE_FRAME));
    attack.ImpactFrame = CSV::Get<int>(values, ToIndex(ATTACK_CSV::IMPACT_FRAME));
    attack.ReturnFrame = CSV::Get<int>(values, ToIndex(ATTACK_CSV::RETURN_FRAME));

    ATTACK_EASING_TYPE easing = ATTACK_EASING_TYPE::LINEAR;
    if (CSV::ParseEnum(m_EasingTable, values[ToIndex(ATTACK_CSV::EASING)], easing))
        attack.Easing = easing;

    attack.SteerSpeed = CSV::Get<float>(values, ToIndex(ATTACK_CSV::STEER_SPEED));

    ARM_ROTATION_TYPE rotation_mode = ARM_ROTATION_TYPE::PATH;
    if (CSV::ParseEnum(m_RotationTable, values[ToIndex(ATTACK_CSV::ROTATION_MODE)], rotation_mode))
        attack.RotationMode = rotation_mode;

    attack.StartRotation = CSV::Get<float>(values, ToIndex(ATTACK_CSV::START_ROTATION));
    attack.EndRotation = CSV::Get<float>(values, ToIndex(ATTACK_CSV::END_ROTATION));
    attack.SpinDegree = CSV::Get<float>(values, ToIndex(ATTACK_CSV::SPIN_DEG));
    attack.RotationOffset = CSV::Get<float>(values, ToIndex(ATTACK_CSV::ROTATION_OFFSET));
    attack.StopOnHit = CSV::Get<int>(values, ToIndex(ATTACK_CSV::STOP_ON_HIT)) != 0;

    // ダメージ情報
    attack.Damage.HpDamage = CSV::Get<float>(values, ToIndex(ATTACK_CSV::HP_DAMAGE));
    attack.Damage.GuardDamage = CSV::Get<float>(values, ToIndex(ATTACK_CSV::GUARD_DAMAGE));
    attack.Damage.KnockBack.x = CSV::Get<float>(values, ToIndex(ATTACK_CSV::KNOCKBACK_X));
    attack.Damage.KnockBack.y = CSV::Get<float>(values, ToIndex(ATTACK_CSV::KNOCKBACK_Y));
    attack.Damage.DesireGain = CSV::Get<float>(values, ToIndex(ATTACK_CSV::DESIRE_GAIN));
    attack.Damage.DesireCost = CSV::Get<float>(values, ToIndex(ATTACK_CSV::DESIRE_COST));
    attack.Damage.HitStun = CSV::Get<int>(values, ToIndex(ATTACK_CSV::HITSTUN));
    attack.Damage.HitStop = CSV::Get<int>(values, ToIndex(ATTACK_CSV::HITSTOP));
    attack.Damage.CameraShake.x = CSV::Get<float>(values, ToIndex(ATTACK_CSV::CAMERASHAKE_X));
    attack.Damage.CameraShake.y = CSV::Get<float>(values, ToIndex(ATTACK_CSV::CAMERASHAKE_Y));
}

/*
 *  攻撃データ取得
 */
const AttackData&
CAttackCSVLoader::
GetAttackData(ATTACK_ID id) const
{
    return m_AttackDatas[ToIndex(id)];
}





//
///*!
// *  @file       attack_csv_loader.cpp
// *  @brief      攻撃CSVローダー
// *  @author     Ryusei Shimizu
// *  @date       2026/07/07
// */
//
//#include "attack_csv_loader.h"
//
//#include "../../csv_helper/csv_helper.h"
//#include "../../csv_convert/csv_convert.h"
//
//const std::unordered_map<std::string, ATTACK_ID>
//CAttackCSVLoader::m_Table =
//{
//    { "ATTACK_NEUTRAL",  ATTACK_ID::ATTACK_NEUTRAL },
//    { "ATTACK_SIDE",     ATTACK_ID::ATTACK_SIDE     },
//    { "ATTACK_UP",       ATTACK_ID::ATTACK_UP       },
//    { "ATTACK_DOWN",     ATTACK_ID::ATTACK_DOWN     },
//
//    { "SUPERBIA_1", ATTACK_ID::SUPERBIA_1 },
//    { "SUPERBIA_2", ATTACK_ID::SUPERBIA_2 },
//
//    { "AVARITIA_1", ATTACK_ID::AVARITIA_1 },
//    { "AVARITIA_2", ATTACK_ID::AVARITIA_2 },
//
//    { "LUXURIA_1", ATTACK_ID::LUXURIA_1 },
//    { "LUXURIA_2", ATTACK_ID::LUXURIA_2 },
//
//    { "INVIDIA_1", ATTACK_ID::INVIDIA_1 },
//    { "INVIDIA_2", ATTACK_ID::INVIDIA_2 },
//
//    { "GULA_1", ATTACK_ID::GULA_1 },
//    { "GULA_2", ATTACK_ID::GULA_2 },
//
//    { "IRA_1", ATTACK_ID::IRA_1 },
//    { "IRA_2", ATTACK_ID::IRA_2 },
//
//    { "ACEDIA_1", ATTACK_ID::ACEDIA_1 },
//    { "ACEDIA_2", ATTACK_ID::ACEDIA_2 },
//};
//
///*
// * コンストラクタ
// */
//CAttackCSVLoader::
//CAttackCSVLoader()
//{
//}
//
///*
// * CSVロード
// */
//bool
//CAttackCSVLoader::
//Load(const std::string& filepath)
//{
//	// 攻撃情報を初期化
//	if(m_AttackInfos.empty())
//	{
//		return false;
//	}
//
//    return LoadFile(filepath,
//        [&](const std::string& line)
//        {
//            return LoadLine(line);
//        });
//}
//
///*
// * 1行ロード
// */
//bool
//CAttackCSVLoader::
//LoadLine(const std::string& line)
//{
//    auto values = CSV::Split(line);
//
//	// 列数が不正な場合は失敗
//    if (!CSV::CheckColumnCount(values, ToIndex(ATTACK_CSV::MAX)))
//    {
//        return false;
//    }
//
//    ATTACK_ID id;
//
//	// 攻撃IDを変換
//    if (!CSV::ParseEnum(m_Table, values[ToIndex(ATTACK_CSV::ATTACK_ID)], id))
//    {
//        return false;
//    }
//
//	// 攻撃情報読み込み
//    ReadAttack(values, m_AttackInfos[static_cast<size_t>(id)]);
//
//    return true;
//}
//
///*
// * 攻撃情報読み込み
// */
//void
//CAttackCSVLoader::
//ReadAttack(const std::vector<std::string>& values, AttackInfo& attack)
//{
//    // 攻撃情報を取得
//    attack.StartFrame = CSV::Get<int>(values, ToIndex(ATTACK_CSV::START_FRAME));
//    attack.ActiveFrame = CSV::Get<int>(values, ToIndex(ATTACK_CSV::ACTIVE_FRAME));
//    attack.RecoveryFrame = CSV::Get<int>(values, ToIndex(ATTACK_CSV::RECOVERY_FRAME));
//
//	// 攻撃円を生成して追加
//    attack.AttackCircles.emplace_back(CreateAttackCircle(values));
//}
//
///*
// * 攻撃円生成
// */
//AttackCircleInfo
//CAttackCSVLoader::
//CreateAttackCircle(const std::vector<std::string>& values) const
//{
//    AttackCircleInfo circle;
//
//	// 円情報
//
//    circle.Offset.x = CSV::Get<float>(values, ToIndex(ATTACK_CSV::OFFSET_X));
//    circle.Offset.y = CSV::Get<float>(values, ToIndex(ATTACK_CSV::OFFSET_Y));
//
//    circle.Radius = CSV::Get<float>(values, ToIndex(ATTACK_CSV::RADIUS));
//
//	// ダメージ情報
//
//    circle.Damage.HpDamage = CSV::Get<float>(values, ToIndex(ATTACK_CSV::HP_DAMAGE));
//
//    circle.Damage.GuardDamage = CSV::Get<float>(values, ToIndex(ATTACK_CSV::GUARD_DAMAGE));
//
//    circle.Damage.KnockBack.x = CSV::Get<float>(values, ToIndex(ATTACK_CSV::KNOCKBACK_X));
//    circle.Damage.KnockBack.y = CSV::Get<float>(values, ToIndex(ATTACK_CSV::KNOCKBACK_Y));
//
//    circle.Damage.Desire = CSV::Get<float>(values, ToIndex(ATTACK_CSV::DESIRE));
//    circle.Damage.CostDesire = CSV::Get<float>(values, ToIndex(ATTACK_CSV::COST_DESIRE));
//
//    circle.Damage.HitStun = CSV::Get<int>(values, ToIndex(ATTACK_CSV::HITSTUN));
//    circle.Damage.HitStop = CSV::Get<int>(values, ToIndex(ATTACK_CSV::HITSTOP));
//
//    circle.Damage.CameraShake.x = CSV::Get<float>(values, ToIndex(ATTACK_CSV::CAMERASHAKE_X));
//    circle.Damage.CameraShake.y = CSV::Get<float>(values, ToIndex(ATTACK_CSV::CAMERASHAKE_Y));
//
//    circle.Damage.InvincibleTime = CSV::Get<int>(values, ToIndex(ATTACK_CSV::INVINCIBLE_TIME));
//
//    circle.Damage.IgnoreInvincible = CSV::Get<bool>(values, ToIndex(ATTACK_CSV::IGNOREINVINCIBLE_FLG));
//
//    circle.Damage.CanGuard = CSV::Get<bool>(values, ToIndex(ATTACK_CSV::CAN_GUARD_FLG));
//
//    return circle;
//}
//
///*
// * 攻撃情報取得
// */
//const AttackInfo&
//CAttackCSVLoader::
//GetAttackInfo(ATTACK_ID id) const
//{
//    return m_AttackInfos[static_cast<size_t>(id)];
//}