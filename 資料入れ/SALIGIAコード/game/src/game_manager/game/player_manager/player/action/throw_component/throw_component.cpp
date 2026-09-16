/*!
 *  @file		throw_component.cpp
 *  @brief		投げコンポーネント
 *  @author     Ryusei Shimizu
 *  @date       2026/09/12
 */

#include "throw_component.h"
#include "../../player.h"

const int CThrowComponent::m_startup_frame			= 8;		//!< 発生フレーム
const int CThrowComponent::m_active_frame			= 3;		//!< 投げ判定フレーム
const int CThrowComponent::m_success_frame			= 12;		//!< 投げ成功演出フレーム
const int CThrowComponent::m_follow_through_frame	= 4;		//!< 投げ後のフォロースルーフレーム
const int CThrowComponent::m_recovery_frame			= 24;		//!< 空振り硬直フレーム

const float CThrowComponent::m_range_rate			= 0.7f;		//!< プレイヤー横幅に対する投げ射程倍率
const float CThrowComponent::m_height_rate			= 0.7f;		//!< プレイヤー高さに対する投げ判定倍率
const float CThrowComponent::m_grab_offset_rate		= 0.8f;		//!< 掴み位置の横方向倍率
const float CThrowComponent::m_hp_damage			= 8.0f;		//!< 投げダメージ
const int	CThrowComponent::m_hit_stun				= 10;		//!< 投げ後の硬直時間
const vivid::Vector2 CThrowComponent::m_knock_back	= vivid::Vector2(10.0f, -34.0f);	//!< 投げの吹っ飛ばし
const float CThrowComponent::m_guard_damage_rate	= 2.0f;		//!< 投げダメージに対するガード中の倍率

/*
 *	コンストラクタ
 */
CThrowComponent::
CThrowComponent(void)
	: m_PlayerWidth(0)
	, m_PlayerHeight(0)
	, m_CurrentFrame(0)
	, m_State(THROW_STATE::IDLE)
	, m_Target(nullptr)
	, m_WasTargetGuarding(false)
{
}

/*
 *	初期化
 */
void
CThrowComponent::
Initialize(int player_width, int player_height)
{
	m_PlayerWidth = player_width;
	m_PlayerHeight = player_height;
	m_CurrentFrame = 0;
	m_State = THROW_STATE::IDLE;
	m_Target = nullptr;
	m_WasTargetGuarding = false;
}

/*
 *	更新
 */
void
CThrowComponent::
Update(CPlayer& owner)
{
	switch (m_State)
	{
	case THROW_STATE::IDLE:			return;
	case THROW_STATE::STARTUP:
		m_CurrentFrame++;

		// 投げ発生フレームが経過したら投げ判定状態へ移行
		if (m_CurrentFrame >= m_startup_frame)
		{
			m_CurrentFrame = 0;
			m_State = THROW_STATE::ACTIVE;
		}
		return;

	case THROW_STATE::ACTIVE:
		m_CurrentFrame++;

		// 投げ判定フレームが経過したら空振り硬直状態へ移行
		if (m_CurrentFrame >= m_active_frame)
		{
			m_CurrentFrame = 0;
			m_State = THROW_STATE::RECOVERY;
		}
		return;

	case THROW_STATE::SUCCESS:
		// 投げ成功中は対象が有効かどうかを確認
		if (!m_Target || !m_Target->IsActive())
		{
			this->Cancel();
			return;
		}

		// 投げ成功中は対象の位置を更新
		this->UpdateTargetPosition(owner);

		m_CurrentFrame++;

		// 投げ成功演出フレームが経過したらフォロースルー状態へ移行
		if (m_CurrentFrame >= m_success_frame)
		{
			this->FinishThrow(owner);
		}
		return;

	case THROW_STATE::FOLLOW_THROUGH:

		m_CurrentFrame++;

		// フォロースルー状態は4フレームで終了し、待機状態へ移行
		if (m_CurrentFrame >= m_follow_through_frame)
		{
			m_CurrentFrame = 0;
			m_State = THROW_STATE::IDLE;
		}
		return;

	case THROW_STATE::RECOVERY:

		m_CurrentFrame++;

		// 空振り硬直状態は24フレームで終了し、待機状態へ移行
		if (m_CurrentFrame >= m_recovery_frame)
		{
			m_CurrentFrame = 0;
			m_State = THROW_STATE::IDLE;
		}
		return;
	}
}

/*
 *	投げ中の腕ポーズ更新
 */
void
CThrowComponent::
ApplyArmPose(CPlayer& owner)
{
	CArm& left_arm = owner.GetAttackComponent().GetLeftArm();
	CArm& right_arm = owner.GetAttackComponent().GetRightArm();

	// 投げ中でない場合は腕ポーズを解除
	if (m_State == THROW_STATE::IDLE)
	{
		left_arm.EndThrowPose();
		right_arm.EndThrowPose();

		return;
	}

	// 投げ空振り後、8フレーム経過したら腕ポーズを解除
	const int recovery_pose_frame = 8;

	if (m_State == THROW_STATE::RECOVERY &&
		m_CurrentFrame >= recovery_pose_frame)
	{
		left_arm.EndThrowPose();
		right_arm.EndThrowPose();

		return;
	}

	const vivid::Vector2 owner_position = owner.GetPhysicsComponent().GetPosition();

	const float direction = owner.GetPlayerData().Direction ? 1.0f : -1.0f;

	const float player_width = static_cast<float>(m_PlayerWidth);
	const float player_height = static_cast<float>(m_PlayerHeight);

	// 投げ中の腕ポーズを計算
	float scoop_rate = 0.0f;

	// 投げ発生中
	if (m_State == THROW_STATE::STARTUP)
	{
		float rate = 0.0f;

		// 投げ発生フレームが1フレーム以上ある場合のみ計算
		if (m_startup_frame > 1)
			rate = static_cast<float>(m_CurrentFrame) / static_cast<float>(m_startup_frame - 1);

		rate = CLAMP(rate, 0.0f, 1.0f);

		scoop_rate = 0.55f * rate;
	}

	// 投げ判定中
	if (m_State == THROW_STATE::ACTIVE)
	{
		float rate = 0.0f;

		// 投げ判定フレームが1フレーム以上ある場合のみ計算
		if (m_active_frame > 1)
			rate = static_cast<float>(m_CurrentFrame) / static_cast<float>(m_active_frame - 1);

		rate = CLAMP(rate, 0.0f, 1.0f);

		scoop_rate = 0.55f + 0.13f * rate;
	}

	// 投げ成功中
	if (m_State == THROW_STATE::SUCCESS)
	{
		float rate = 0.0f;

		if (m_success_frame > 1)
			rate = static_cast<float>(m_CurrentFrame) / static_cast<float>(m_success_frame - 1);

		rate = CLAMP(rate, 0.0f, 1.0f);

		scoop_rate = 0.68f + (0.32f * rate);
	}

	// 投げ後のフォロースルー中
	if (m_State == THROW_STATE::RECOVERY)
	{
		float rate = 0.0f;

		// 投げ空振り硬直フレームが1フレーム以上ある場合のみ計算
		if (recovery_pose_frame > 1)
			rate = static_cast<float>(m_CurrentFrame) / static_cast<float>(recovery_pose_frame - 1);

		rate = CLAMP(rate, 0.0f, 1.0f);

		scoop_rate = 0.68f + (0.32f * rate);
	}

	scoop_rate = CLAMP(scoop_rate, 0.0f, 1.0f);

	// 腕の位置と回転角度を計算
	float path_x = 0.0f;
	float path_y = 0.0f;
	float rotation_degree = 0.0f;

	// 前半
	if (scoop_rate < 0.68f)
	{
		float rate = scoop_rate / 0.68f;

		rate = CLAMP(rate, 0.0f, 1.0f);

		const float move_rate = rate * rate;

		path_x			=  0.18f + 0.42f * move_rate;
		path_y			=  0.30f - 0.25f * move_rate;
		rotation_degree = -40.0f - 50.0f * move_rate;
	}

	// 後半
	if (scoop_rate >= 0.68f)
	{
		float rate = (scoop_rate - 0.68f) / 0.32f;

		rate = CLAMP(rate, 0.0f, 1.0f);

		const float inverse_rate = 1.0f - rate;

		const float snap_rate = 1.0f - inverse_rate * inverse_rate * inverse_rate;

		path_x			=  0.60f + 0.30f * snap_rate;
		path_y			=  0.05f - 0.58f * snap_rate;
		rotation_degree = -90.0f - 65.0f * snap_rate;
	}

	// 投げ後のフォロースルー中は腕を少し戻す
	if (m_State == THROW_STATE::FOLLOW_THROUGH)
	{
		float rate = static_cast<float>(m_CurrentFrame + 1) / 4.0f;

		rate = CLAMP(rate, 0.0f, 1.0f);

		const float inverse_rate = 1.0f - rate;

		const float snap_rate = 1.0f - inverse_rate * inverse_rate;

		path_x			=  0.90f + 0.12f * snap_rate;
		path_y			= -0.53f - 0.22f * snap_rate;
		rotation_degree = -155.0f - 20.0f * snap_rate;
	}

	// 腕の位置を少し内側に寄せる
	const float arm_space_x = player_width * 0.04f;
	const float arm_space_y = player_height * 0.10f;

	// 腕の目的位置を計算
	vivid::Vector2 left_target  = owner_position + vivid::Vector2(direction * player_width * path_x, player_height * path_y - arm_space_y);
	vivid::Vector2 right_target = owner_position + vivid::Vector2(direction * (player_width * path_x - arm_space_x), player_height * path_y + arm_space_y);

	// 腕の回転角度を計算
	const float left_rotation = DEG_TO_RAD((rotation_degree - 6.0f) * direction);
	const float right_rotation = DEG_TO_RAD((rotation_degree + 6.0f) * direction);

	// 腕の投げポーズを設定
	left_arm.SetThrowPose(left_target, left_rotation);
	right_arm.SetThrowPose(right_target, right_rotation);
}

/*
 *	投げ開始
 */
void
CThrowComponent::
Start(void)
{
	if (m_State != THROW_STATE::IDLE) return;

	m_CurrentFrame = 0;
	m_Target = nullptr;
	m_State = THROW_STATE::STARTUP;
	m_WasTargetGuarding = false;
}

/*
 *	投げ成功
 */
bool
CThrowComponent::
Catch(CPlayer& target)
{
	if (m_State != THROW_STATE::ACTIVE) return false;
	if (m_Target) return false;

	// 相手が行っている投げをキャンセル
	target.GetThrowComponent().Cancel();

	// 相手の攻撃をキャンセル
	target.GetAttackComponent().Cancel();

	// 投げが当たった瞬間にガード中だったか保存
	m_WasTargetGuarding = target.GetGuardComponent().IsGuard();

	// ガード中の場合はガードを終了
	target.GetGuardComponent().End();

	// 相手の速度をリセット
	target.GetPhysicsComponent().ResetVelocity(VelocityID::Final);

	// 相手を投げられ状態にする
	target.GetStateController().ChangeActionState(ACTION_STATE::THROWN);

	m_Target = &target;
	m_CurrentFrame = 0;
	m_State = THROW_STATE::SUCCESS;

	return true;
}

/*
 *	投げキャンセル
 */
void
CThrowComponent::
Cancel(void)
{
	if (m_Target)
	{
		if (m_Target->GetStateController().IsActionState(ACTION_STATE::THROWN))
		{
			m_Target->GetStateController().ChangeActionState(ACTION_STATE::IDLE);
		}

		m_Target->GetPhysicsComponent().ResetVelocity(VelocityID::Final);
	}

	m_Target = nullptr;
	m_CurrentFrame = 0;
	m_State = THROW_STATE::IDLE;
	m_WasTargetGuarding = false;
}

/*
 *	投げ判定取得
 */
AABB
CThrowComponent::
GetThrowArea(const vivid::Vector2& player_position, bool facing_right) const
{
	AABB area;

	area.Width = static_cast<float>(m_PlayerWidth) * m_range_rate;
	area.Height = static_cast<float>(m_PlayerHeight) * m_height_rate;
	area.Position.y = player_position.y + (static_cast<float>(m_PlayerHeight) - area.Height) * 0.5f;

	// 左右の向きに応じて投げ判定の位置を設定
	if (facing_right)
		area.Position.x = player_position.x + static_cast<float>(m_PlayerWidth);
	else
		area.Position.x = player_position.x - area.Width;

	return area;
}

/*
 *	投げ成功中の対象位置更新
 */
void
CThrowComponent::
UpdateTargetPosition(CPlayer& owner)
{
	if (!m_Target) return;

	const vivid::Vector2 owner_position = owner.GetPhysicsComponent().GetPosition();

	const float direction = owner.GetPlayerData().Direction ? 1.0f : -1.0f;

	const float player_width = static_cast<float>(m_PlayerWidth);
	const float player_height = static_cast<float>(m_PlayerHeight);

	// 投げ成功中のフレームを計算
	int motion_frame = m_CurrentFrame + 1;

	// 投げ成功演出フレームを超えた場合は、投げ成功演出フレームの最後のフレームに固定
	if (motion_frame >= m_success_frame)
		motion_frame = m_success_frame - 1;

	float throw_rate = 0.0f;

	// 投げ成功演出フレームが1フレーム以上ある場合のみ計算
	if (m_success_frame > 1)
		throw_rate = static_cast<float>(motion_frame) / static_cast<float>(m_success_frame - 1);

	throw_rate = CLAMP(throw_rate, 0.0f, 1.0f);

	// 最初から勢いよく上昇する
	const float inverse_rate = 1.0f - throw_rate;
	const float snap_rate = 1.0f - inverse_rate * inverse_rate * inverse_rate;

	// 腕の掬い上げ軌道に合わせて相手を前上方向へ運ぶ
	vivid::Vector2 target_position = owner_position;

	// 投げ成功中の対象位置を計算
	target_position.x += direction * player_width * (0.55f + 0.35f * snap_rate);
	target_position.y += player_height * (0.08f - 0.68f * snap_rate);

	m_Target->GetPhysicsComponent().SetPosition(target_position);
	m_Target->GetPhysicsComponent().ResetVelocity(VelocityID::Final);
	m_Target->GetPhysicsComponent().HitCapsuleUpdate();
}

/*
 *	投げ終了処理
 */
void
CThrowComponent::
FinishThrow(CPlayer& owner)
{
	if (!m_Target)
	{
		this->Cancel();
		return;
	}

	CPlayer* target = m_Target;
	DamageInfo damage_info;

	damage_info.HpDamage = m_hp_damage;

	// 対象がガード中だった場合は投げダメージを増加させる
	if (m_WasTargetGuarding)
	{
		damage_info.HpDamage *= m_guard_damage_rate;
	}

	damage_info.KnockBack = m_knock_back;
	damage_info.HitStun = m_hit_stun;

	// 左向きの場合は吹っ飛ばし方向を反転
	if (!owner.GetPlayerData().Direction)
		damage_info.KnockBack.x *= -1.0f;

	// 投げられ状態を解除
	if (target->GetStateController().IsActionState(ACTION_STATE::THROWN))
		target->GetStateController().ChangeActionState(ACTION_STATE::IDLE);

	// 投げで空中へ吹っ飛ばすため着地状態を解除
	target->GetPhysicsComponent().SetLandingFlg(false);

	m_Target = nullptr;
	m_CurrentFrame = 0;
	m_State = THROW_STATE::FOLLOW_THROUGH;

	// 投げダメージ
	target->Damage(damage_info);
}