
/*!
 *  @file		attack_id.h
 *  @brief		アクションID
 *  @author     Ryusei Shimizu
 *  @date       2026/06/26
 */

#pragma once

#include "vivid.h"
#include <vector>
#include "../../../../game_manager/game/player_manager/player/arm/arm_id.h"

/*!
 *	@brief		攻撃ID
 */
enum class ATTACK_ID
{
	NONE,	//!< 無効

	//!< 通常攻撃
	ATTACK_NEUTRAL,
	ATTACK_SIDE,
	ATTACK_UP,
	ATTACK_DOWN,

	//!< 傲慢
	SUPERBIA_1,
	SUPERBIA_2,

	//!< 強欲
	AVARITIA_1,
	AVARITIA_2,

	//!< 色欲
	LUXURIA_1,
	LUXURIA_2,

	//!< 嫉妬
	INVIDIA_1,
	INVIDIA_2,

	//!< 暴食
	GULA_1,
	GULA_2,

	//!< 憤怒
	IRA_1,
	IRA_2,

	//!< 怠惰
	ACEDIA_1,
	ACEDIA_2,

	MAX		//!< 最大値
};


/*!
 *  @brief      攻撃軌道の時間補間
 */
enum class ATTACK_EASING_TYPE
{
    LINEAR,
    EASE_IN,
    EASE_OUT,
    EASE_IN_OUT,
};

/*!
 *  @brief      攻撃中の腕回転方式
 */
enum class ARM_ROTATION_TYPE
{
    PATH,           //!< 軌道の進行方向を向く
    LERP_ROTATION,  //!< StartRotation -> EndRotation
    FIXED,          //!< StartRotationで固定
    ROTATE_SPIN,    //!< StartRotationからSpinDegreeだけ回転
};

/*!
 *  @brief      攻撃情報CSVの列ID
 */
enum class ATTACK_CSV
{
    ATTACK_ID,
    ARM_ID,

    CONTROL1_X,
    CONTROL1_Y,
    CONTROL2_X,
    CONTROL2_Y,
    END_X,
    END_Y,

    MOVE_FRAME,
    HOLD_FRAME,
    ACTIVE_START_FRAME,
    ACTIVE_FRAME,
    IMPACT_FRAME,
    RETURN_FRAME,

    EASING,
    STEER_SPEED,

    ROTATION_MODE,
    START_ROTATION,
    END_ROTATION,
    SPIN_DEG,
    ROTATION_OFFSET,

    STOP_ON_HIT,

    HP_DAMAGE,
    GUARD_DAMAGE,
    KNOCKBACK_X,
    KNOCKBACK_Y,
    DESIRE_GAIN,
    DESIRE_COST,
    HITSTUN,
    HITSTOP,
    CAMERASHAKE_X,
    CAMERASHAKE_Y,

    MAX
};


/*!
 *  @brief      ダメージ情報構造体
 */
struct DamageInfo
{
	float			HpDamage	= 0.0f;		//!< HPダメージ
	float			GuardDamage = 0.0f;		//!< ガードダメージ

	vivid::Vector2	KnockBack{};			//!< ノックバック

	float			DesireGain	= 0.0f;     //!< 欲望増加量
	float			DesireCost	= 0.0f;     //!< 欲望消費量

	int				HitStun		= 0;		//!< ヒット硬直時間
	int				HitStop		= 0;		//!< ヒットストップ時間

	vivid::Vector2	CameraShake{};			//!< カメラシェイク
};

/*!
 *  @brief      攻撃情報
 */
struct AttackInfo
{
    ATTACK_ID            ID               = ATTACK_ID::NONE;

    // 攻撃開始位置からの相対座標。CSVは右向き基準。
    vivid::Vector2       Control1{};
    vivid::Vector2       Control2{};
    vivid::Vector2       EndPosition{};

    int                  MoveFrame        = 0;
    int                  HoldFrame        = 0;
    int                  ActiveStartFrame = 0;
    int                  ActiveFrame      = 0;
    int                  ImpactFrame      = 0;
    int                  ReturnFrame      = 0;

    ATTACK_EASING_TYPE   Easing           = ATTACK_EASING_TYPE::LINEAR;
    float                SteerSpeed       = 0.0f;

    ARM_ROTATION_TYPE    RotationMode     = ARM_ROTATION_TYPE::PATH;
    float                StartRotation    = 0.0f; // degree
    float                EndRotation      = 0.0f; // degree
    float                SpinDegree       = 0.0f; // degree
    float                RotationOffset   = 0.0f; // degree

    bool                 StopOnHit        = true;

    DamageInfo           Damage;

    int GetAttackFrame() const
    {
        return MoveFrame + HoldFrame;
    }

    int GetTotalFrame() const
    {
        return GetAttackFrame() + ImpactFrame + ReturnFrame;
    }

    bool IsMoveFrame(int frame) const
    {
        return frame < MoveFrame;
    }

    bool IsHoldFrame(int frame) const
    {
        return frame >= MoveFrame && frame < GetAttackFrame();
    }

    bool IsImpactFrame(int frame) const
    {
        return frame >= GetAttackFrame() && frame < GetAttackFrame() + ImpactFrame;
    }

    bool IsReturnFrame(int frame) const
    {
        return frame >= GetAttackFrame() + ImpactFrame && frame < GetTotalFrame();
    }
};

/*!
 *  @brief      攻撃データ
 */
struct AttackData
{
	AttackInfo		RightArm;				//!< 右腕の攻撃情報
	AttackInfo		LeftArm;				//!< 左腕の攻撃情報
};