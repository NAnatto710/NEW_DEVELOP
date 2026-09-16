
/*!
 *  @file		player_manager.cpp
 *  @brief		プレイヤー管理
 *  @author     Ryusei Shimizu
 *  @date       2026/04/13
 */

#include "player_manager.h"
#include "player/player.h"
#include "../camera_manager/camera_manager.h"
#include "../stage_manager/stage_manager.h"
#include "../scene_manager/scene_manager.h"
#include "../effect_manager/effect_manager.h"
#include "../../../utility/collision_check/collision_check.h"
#include "../../../utility/utility.h"
#include "../../../utility/sound_manager/sound_manager.h"
#include <cfloat>
#include <cmath>

namespace
{
    vivid::Vector2 LerpVector2(const vivid::Vector2& a, const vivid::Vector2& b, float t)
    {
        return a + (b - a) * t;
    }

    bool IsSweptCapsuleHit(const CArm& arm, const Capsule& target_capsule)
    {
        const Capsule& previous = arm.GetPreviousCapsule();
        const Capsule& current = arm.GetCapsule();

        const float start_distance = vivid::Vector2::Length(current.Start - previous.Start);
        const float end_distance = vivid::Vector2::Length(current.End - previous.End);
        const float max_distance = start_distance > end_distance ? start_distance : end_distance;

        float sample_step = current.Radius * 0.5f;
        if (sample_step < 1.0f) sample_step = 1.0f;

        int sample_count = static_cast<int>(std::ceil(max_distance / sample_step));
        if (sample_count < 1) sample_count = 1;
        if (sample_count > 64) sample_count = 64;

        for (int i = 0; i <= sample_count; ++i)
        {
            const float t = static_cast<float>(i) / static_cast<float>(sample_count);

            Capsule sample;
            sample.Start = LerpVector2(previous.Start, current.Start, t);
            sample.End = LerpVector2(previous.End, current.End, t);
            sample.Radius = previous.Radius + (current.Radius - previous.Radius) * t;

            if (IsCollision::IsHit(sample, target_capsule))
                return true;
        }

        return false;
    }
}

/*
 *  インスタンス取得
 */
CPlayerManager&
CPlayerManager::
GetInstance()
{
    static CPlayerManager instance;

    return instance;
}

/*
 *  初期化
 */
void
CPlayerManager::
Initialize()
{
    // 前回のPlayerが残っていても必ず解放
    this->Finalize();

	CSceneManager& sm = CSceneManager::GetInstance();

	// 参加人数分のプレイヤーを生成
    for (int i = 0; i < sm.GetJoinCount(); ++i)
    {
        BuildData build = sm.GetBuildData(static_cast<PLAYER_ID>(i));

        auto controller = (vivid::controller::DEVICE_ID)i;

        CPlayer* player = Create(static_cast<PLAYER_ID>(i), controller, build, CStageManager::GetInstance().GetSpawnPosition(static_cast<PLAYER_ID>(i)));

        CCameraManager::GetInstance().SetPlayer(static_cast<PLAYER_ID>(i), player);
    }
}

/*
 *  更新
 */
void
CPlayerManager::
Update()
{
    // リストが空なら終了
    if (m_PlayerList.empty()) return;

	// プレイヤーの更新
    for (PLAYER_LIST::iterator it = m_PlayerList.begin(); it != m_PlayerList.end(); ++it)
    {
        (*it)->Update();
    }

	// プレイヤー同士の当たり判定
    for (PLAYER_LIST::iterator it = m_PlayerList.begin(); it != m_PlayerList.end(); ++it)
    {
        for (PLAYER_LIST::iterator it2 = std::next(it); it2 != m_PlayerList.end(); ++it2)
        {
            this->CheckPlayerHit(**it, **it2);
            this->CheckPlayerHit(**it2, **it);
        }
    }

    // プレイヤーの投げ判定
    for (PLAYER_LIST::iterator it = m_PlayerList.begin(); it != m_PlayerList.end(); ++it)
    {
        this->CheckPlayerThrow(**it);
    }
}

/*
 *  描画
 */
void
CPlayerManager::
Draw()
{
    if (m_PlayerList.empty()) return;

    auto draw_group = [this](bool guard)
        {
            // 左腕
            for (PLAYER_LIST::iterator it = m_PlayerList.begin(); it != m_PlayerList.end(); ++it)
            {
                CPlayer* player = *it;

                if (!player) continue;
                if (player->GetGuardComponent().IsGuard() != guard) continue;

                player->DrawLeftArm();
            }

            // 体
            for (PLAYER_LIST::iterator it = m_PlayerList.begin(); it != m_PlayerList.end(); ++it)
            {
                CPlayer* player = *it;

                if (!player) continue;
                if (player->GetGuardComponent().IsGuard() != guard) continue;

                player->DrawBody();
            }

            // 右腕
            for (PLAYER_LIST::iterator it = m_PlayerList.begin(); it != m_PlayerList.end(); ++it)
            {
                CPlayer* player = *it;

                if (!player) continue;
                if (player->GetGuardComponent().IsGuard() != guard) continue;

                player->DrawRightArm();
            }
        };

    // ガード中を先に描く
    draw_group(true);

    // 通常プレイヤーを後に描く
    draw_group(false);
}

/*
 *  解放
 */
void
CPlayerManager::
Finalize()
{
    // リストが空なら終了
    if (m_PlayerList.empty()) return;

    // プレイヤーの解放
    for (PLAYER_LIST::iterator it = m_PlayerList.begin(); it != m_PlayerList.end(); ++it)
    {
        (*it)->Finalize();

        delete (*it);
    }

    m_PlayerList.clear();
}

/*
 *  プレイヤーの生成
 */
CPlayer*
CPlayerManager::
Create(PLAYER_ID id, vivid::controller::DEVICE_ID device_id, BuildData build, vivid::Vector2& position)
{
	// プレイヤーの生成
    CPlayer* player = new CPlayer(CATEGORY_ID::PLAYER);

    if (!player)return nullptr;

	// プレイヤーの初期化
    player->Initialize(id, device_id, build, position);

    // 生成したプレイヤーをリストに追加
    m_PlayerList.push_back(player);

    return player;
}

/*
 *  プレイヤーの取得
 */
CPlayer*
CPlayerManager::
GetPlayer(PLAYER_ID id) const
{
    // リストが空なら終了
    if (m_PlayerList.empty()) return nullptr;

    // プレイヤーの取得
    for (PLAYER_LIST::const_iterator it = m_PlayerList.begin(); it != m_PlayerList.end(); ++it)
    {
		// プレイヤーIDが一致する場合は返す
        if ((*it)->GetPlayerData().PlayerID == id)
        {
            return (*it);
        }
    }
    return nullptr;
}

/*
 *  プレイヤー同士の当たり判定
 */
void
CPlayerManager::
CheckPlayerHit(CPlayer& attacker, CPlayer& target)
{
    // 攻撃側と被攻撃側がアクティブでない場合は終了
    if (!attacker.IsActive() || !target.IsActive()) return;

    // 投げられ中は通常攻撃の対象外
    if (attacker.GetStateController().IsActionState(ACTION_STATE::THROWN) ||
        target.GetStateController().IsActionState(ACTION_STATE::THROWN)) return;

    // 投げ成功演出中は通常攻撃の対象外
    if (attacker.GetThrowComponent().IsSuccess() ||
        target.GetThrowComponent().IsSuccess()) return;

    CAttackComponent& attack_component = attacker.GetAttackComponent();

    // 相手の体Capsuleを取得
    const Capsule target_capsule = target.GetPhysicsComponent().GetHitCapsule();

    // 右腕
    CArm& right_arm = attack_component.GetRightArm();

    // 右腕が攻撃中かどうか
    if (right_arm.IsActiveFrame())
    {
        // 右腕がこの対象にまだ当てていない場合
        if (!right_arm.HasHit(&target))
        {
            // 腕と体のCapsuleが当たった場合
            if (IsSweptCapsuleHit(right_arm, target_capsule))
            {
                // ヒット対象として右腕に記録
                right_arm.AddHitTarget(&target);

                right_arm.OnHit();

                DamageInfo damage_info = right_arm.GetDamageInfo();

                // 攻撃側の向きに応じてノックバックの方向を反転
                if (!attacker.GetPlayerData().Direction)
                {
                    damage_info.KnockBack.x *= -1.0f;
                }

                // 攻撃力補正
                damage_info.HpDamage *= attacker.GetAttributeComponent().GetValue(ATTRIBUTE_ID::ATTACK_POWER);

                float knock_back = 1 + (attacker.GetAttributeComponent().GetValue(ATTRIBUTE_ID::KNOCKBACK_POWER) -
                    target.GetAttributeComponent().GetValue(ATTRIBUTE_ID::KNOCKBACK_RESIST));

                // ノックバック補正
                damage_info.KnockBack.x *= knock_back;
                damage_info.KnockBack.y *= knock_back;

                // 欲望獲得量補正
                damage_info.DesireGain *= attacker.GetAttributeComponent().GetValue(ATTRIBUTE_ID::DESIRE_GAIN);

                // ガードレジスト補正
                damage_info.GuardDamage *= target.GetAttributeComponent().GetValue(ATTRIBUTE_ID::GUARD_POWER);

                // ガード判定
                if (target.GetGuardComponent().IsGuard())
                {
                    target.GetGuardComponent().Guard(damage_info, target);
                    CSoundManager::GetInstance().PlaySE(SOUND_ID::GUARD);
                    vivid::Vector2 pos = vivid::Vector2(35, 35);
                    if (!target.GetPlayerData().Direction)
                    {
                        pos.x -= 70;
                    }
                    CEffectManager::GetInstance().Create(EFFECT_ID::GUARD, target.GetPlayerData().PlayerID,
                        target.GetPhysicsComponent().GetPosition() + pos
                        , DIRECTION::RIGHT, vivid::Vector2(3.0f, 3.0f), target.GetPlayerData().Color, 0.0f);
                }

                // ダメージ処理
                else if (target.Damage(damage_info))
                {
                    vivid::Vector2 effect_pos = target.GetPhysicsComponent().GetPosition() + vivid::Vector2(Utility::GetRandomInt(0, 70), Utility::GetRandomInt(0, 70));

                    CSoundManager::GetInstance().PlaySE(SOUND_ID::HIT);

                    CEffectManager::GetInstance().Create(EFFECT_ID::HIT, target.GetPlayerData().PlayerID, effect_pos
                        , DIRECTION::RIGHT, vivid::Vector2(3.0f, 3.0f), 0xffffffff, 0.0f);
                    CEffectManager::GetInstance().Create(EFFECT_ID::IMPACT, target.GetPlayerData().PlayerID, effect_pos
                        , DIRECTION::RIGHT, vivid::Vector2(3.0f, 3.0f), 0xffffffff, 0.0f);

                    if (damage_info.DesireCost > 0)
                    {
                        CEffectManager::GetInstance().Create(EFFECT_ID::IMPACT_FLASH, target.GetPlayerData().PlayerID, effect_pos
                            , DIRECTION::RIGHT, vivid::Vector2(1.0f, 1.0f), 0xffffffff, 0.0f);
                    }

                    // カメラ揺れ
                    CCameraManager::GetInstance().SetPlayerShake(target.GetPlayerData().PlayerID, damage_info.HitStun, damage_info.CameraShake);

                    // ヒットストップ
                    attacker.SetHitStop(damage_info.HitStop);

                    // 欲望獲得
                    attacker.GetResourceComponent().AddCurrent(RESOURCE_ID::DESIRE, damage_info.DesireGain);
                }
            }
        }
    }

    // 左腕
    CArm& left_arm = attack_component.GetLeftArm();

    // 左腕が攻撃中かどうか
    if (left_arm.IsActiveFrame())
    {
        // 左腕がこの対象にまだ当てていない場合
        if (!left_arm.HasHit(&target))
        {
            // 腕と体のCapsuleが当たった場合
            if (IsSweptCapsuleHit(left_arm, target_capsule))
            {
                // ヒット対象として左腕に記録
                left_arm.AddHitTarget(&target);

                left_arm.OnHit();

                DamageInfo damage_info = left_arm.GetDamageInfo();

                // 攻撃側の向きに応じてノックバックの方向を反転
                if (!attacker.GetPlayerData().Direction)
                {
                    damage_info.KnockBack.x *= -1.0f;

                }

                // 攻撃力補正
                damage_info.HpDamage *= attacker.GetAttributeComponent().GetValue(ATTRIBUTE_ID::ATTACK_POWER);

                float knock_back = 1 + (attacker.GetAttributeComponent().GetValue(ATTRIBUTE_ID::KNOCKBACK_POWER) -
                    target.GetAttributeComponent().GetValue(ATTRIBUTE_ID::KNOCKBACK_RESIST));

                // ノックバック補正
                damage_info.KnockBack.x *= knock_back;
                damage_info.KnockBack.y *= knock_back;

                // 欲望獲得量補正
                damage_info.DesireGain *= attacker.GetAttributeComponent().GetValue(ATTRIBUTE_ID::DESIRE_GAIN);

                // ガードレジスト補正
                damage_info.GuardDamage *= target.GetAttributeComponent().GetValue(ATTRIBUTE_ID::GUARD_POWER);

                // ガード判定
                if (target.GetGuardComponent().IsGuard())
                {
                    target.GetGuardComponent().Guard(damage_info, target);
                    CSoundManager::GetInstance().PlaySE(SOUND_ID::GUARD);
                    CEffectManager::GetInstance().Create(EFFECT_ID::GUARD, target.GetPlayerData().PlayerID,
                        target.GetPhysicsComponent().GetPosition() + vivid::Vector2(70, 70)
                        , DIRECTION::RIGHT, vivid::Vector2(3.0f, 3.0f), target.GetPlayerData().Color, 0.0f);
                }

                // ダメージ処理
                else if (target.Damage(damage_info))
                {
                    CSoundManager::GetInstance().PlaySE(SOUND_ID::HIT);

                    vivid::Vector2 effect_pos = target.GetPhysicsComponent().GetPosition() + vivid::Vector2(Utility::GetRandomInt(0, 70), Utility::GetRandomInt(0, 70));

                    CEffectManager::GetInstance().Create(EFFECT_ID::HIT, target.GetPlayerData().PlayerID, effect_pos
                        , DIRECTION::RIGHT, vivid::Vector2(5.0f, 5.0f), 0xffffffff, 0.0f);
                    CEffectManager::GetInstance().Create(EFFECT_ID::IMPACT, target.GetPlayerData().PlayerID, effect_pos
                        , DIRECTION::RIGHT, vivid::Vector2(5.0f, 5.0f), 0xffffffff, 0.0f);

                    if (damage_info.DesireCost > 0)
                    {
                        CEffectManager::GetInstance().Create(EFFECT_ID::IMPACT_FLASH, target.GetPlayerData().PlayerID, effect_pos
                            , DIRECTION::RIGHT, vivid::Vector2(1.0f, 1.0f), 0xffffffff, 0.0f);
                    }

                    // カメラ揺れ
                    CCameraManager::GetInstance().SetPlayerShake(target.GetPlayerData().PlayerID, damage_info.HitStun, damage_info.CameraShake);

                    // ヒットストップ
                    attacker.SetHitStop(damage_info.HitStop);

                    // 欲望獲得
                    attacker.GetResourceComponent().AddCurrent(RESOURCE_ID::DESIRE, damage_info.DesireGain);
                }
            }
        }
    }
}

/*
 *  プレイヤーの投げ判定
 */
void
CPlayerManager::
CheckPlayerThrow(CPlayer& attacker)
{
    if (!attacker.IsActive()) return;

    CThrowComponent& throw_component = attacker.GetThrowComponent();

    // 投げ判定が有効でない場合は終了
    if (!throw_component.IsActiveFrame()) return;

    const AABB throw_area = throw_component.GetThrowArea(
        attacker.GetPhysicsComponent().GetPosition(),
        attacker.GetPlayerData().Direction);

    CPlayer* nearest_target = nullptr;
    float nearest_distance = FLT_MAX;

    for (PLAYER_LIST::iterator it = m_PlayerList.begin(); it != m_PlayerList.end(); ++it)
    {
        CPlayer* target = *it;

        if (!target || target == &attacker) continue;
        if (!target->CanBeThrown()) continue;

        // 同じフレームで互いに投げ判定が出ている場合は不成立
        if (target->GetThrowComponent().IsActiveFrame()) continue;

        const Capsule target_capsule = target->GetPhysicsComponent().GetHitCapsule();

        if (!IsCollision::IsHit(target_capsule, throw_area)) continue;

        vivid::Vector2 diff = target->GetPhysicsComponent().GetPosition() - attacker.GetPhysicsComponent().GetPosition();

        const float distance = diff.Length();

        if (distance < nearest_distance)
        {
            nearest_distance = distance;
            nearest_target = target;
        }
    }

    if (nearest_target)
    {
        throw_component.Catch(*nearest_target);
    }
}


/*
 *  誰か一人でも操作しているか
 */
bool
CPlayerManager::
IsAnyPlayerOperating(void) const
{
    for (CPlayer* player : m_PlayerList)
    {
        if (!player)
            continue;

        if (!player->IsActive())
            continue;

        if (player->IsOperating())
            return true;
    }

    return false;
}
