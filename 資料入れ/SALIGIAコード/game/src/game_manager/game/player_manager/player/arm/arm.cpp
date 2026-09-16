
/*!
 *  @file		arm.cpp
 *  @brief		プレイヤーの腕
 *  @author     Ryusei Shimizu
 *  @date       2026/08/25
 */

#include "arm.h"
#include "../../../camera_manager/camera_manager.h"
#include "../../../../../utility/utility.h"
#include <cmath>

const float CArm::m_follow_rate = 0.6f;		//!< 腕の追従率
const int   CArm::m_width = 70;		//!< 腕の幅
const int   CArm::m_height = 140;		//!< 腕の高さ

/*
 *  コンストラクタ
 */
CArm::
CArm(ARM_ID id, const vivid::Vector2& local_position)
	: m_LocalPosition(local_position)
	, m_Position(vivid::Vector2::ZERO)
	, m_PreviousPlayerPosition(vivid::Vector2::ZERO)
	, m_Rotation(0.0f)
	, m_FacingRight(true)
	, m_GuardFlg(false)
	, m_GuardBreakFlg(false)
	, m_ThrowPoseFlg(false)
	, m_GuardBreakFrame(0)
	, m_GuardBreakDuration(0)
	, m_GuardBreakStartPosition(vivid::Vector2::ZERO)
	, m_GuardBreakStartRotation(0.0f)
	, m_AttackStartPosition(vivid::Vector2::ZERO)
	, m_ControlPoint1(vivid::Vector2::ZERO)
	, m_ControlPoint2(vivid::Vector2::ZERO)
	, m_AttackEndPosition(vivid::Vector2::ZERO)
	, m_SteerOffset(vivid::Vector2::ZERO)
	, m_ReturnStartPosition(vivid::Vector2::ZERO)
	, m_ReturnStartRotation(0.0f)
	, m_CurrentFrame(0)
	, m_IdleTimer(0.0f)
	, m_SaligiaID(SALIGIA_ID::NONE)
	, m_Id(id)
	, m_State(ARM_STATE::IDLE)
	, m_AttackInfo()
{
}

/*
 *  初期化
 */
void
CArm::
Initialize(const vivid::Vector2& player_position, bool facing_right, SALIGIA_ID saligia_id)
{
	m_State = ARM_STATE::IDLE;
	m_FacingRight = facing_right;

	m_GuardFlg = false;
	m_GuardBreakFlg = false;
	m_ThrowPoseFlg = false;

	m_GuardBreakFrame = 0;
	m_GuardBreakDuration = 0;
	m_GuardBreakStartPosition = vivid::Vector2::ZERO;
	m_GuardBreakStartRotation = 0.0f;

	m_SaligiaID = saligia_id;

	std::string file_path = "data\\player\\arm\\";

	// 腕の画像パスを大罪IDに応じて設定
	switch (saligia_id)
	{
	case SALIGIA_ID::NONE:										break;
	case SALIGIA_ID::SUPERBIA:	file_path += "superbia.png";	break;
	case SALIGIA_ID::AVARITIA:	file_path += "avaritia.png";	break;
	case SALIGIA_ID::LUXURIA:	file_path += "luxuria.png";		break;
	case SALIGIA_ID::INVIDIA:	file_path += "invidia.png";		break;
	case SALIGIA_ID::GULA:		file_path += "gula.png";		break;
	case SALIGIA_ID::IRA:		file_path += "ira.png";			break;
	case SALIGIA_ID::ACEDIA:	file_path += "acedia.png";		break;
	}

	m_FilePath = file_path;

	m_AttackStartPosition = vivid::Vector2::ZERO;
	m_ControlPoint1 = vivid::Vector2::ZERO;
	m_ControlPoint2 = vivid::Vector2::ZERO;
	m_AttackEndPosition = vivid::Vector2::ZERO;
	m_SteerOffset = vivid::Vector2::ZERO;
	m_ReturnStartPosition = vivid::Vector2::ZERO;
	m_ReturnStartRotation = 0.0f;

	m_CurrentFrame = 0;
	m_IdleTimer = 0;

	m_PreviousPlayerPosition = player_position;
	m_Position = GetTargetPosition(player_position, facing_right);
	m_Rotation = 0.0f;

	CapsuleUpdate();
	m_PreviousCapsule = m_Capsule;
}

/*
 *  更新
 */
void
CArm::
Update(const vivid::Vector2& player_position, bool facing_right, const vivid::Vector2& attack_input)
{
	m_FacingRight = facing_right;

	// フレーム間のすり抜け判定用
	m_PreviousCapsule = m_Capsule;

	// Playerの移動量
	const vivid::Vector2 player_delta =
		player_position - m_PreviousPlayerPosition;

	m_PreviousPlayerPosition = player_position;

	// 攻撃軌道をPlayerへ追従
	if (m_State == ARM_STATE::ATTACK)
	{
		m_AttackStartPosition += player_delta;
		m_ControlPoint1 += player_delta;
		m_ControlPoint2 += player_delta;
		m_AttackEndPosition += player_delta;
	}
	else if (m_State == ARM_STATE::IMPACT)
	{
		m_Position += player_delta;
	}
	else if (m_State == ARM_STATE::RETURN)
	{
		m_ReturnStartPosition += player_delta;
	}

	// ガードブレイク開始位置もPlayerへ追従
	if (m_GuardBreakFlg)
	{
		m_GuardBreakStartPosition += player_delta;
	}

	// ガードブレイク演出を最優先
	if (m_GuardBreakFlg && m_State == ARM_STATE::IDLE)
	{
		UpdateGuardBreak(player_position, facing_right);
		CapsuleUpdate();
		return;
	}

	// 投げポーズ中
	if (m_ThrowPoseFlg && m_State == ARM_STATE::IDLE)
	{
		CapsuleUpdate();
		return;
	}

	switch (m_State)
	{
	case ARM_STATE::IDLE:		UpdateIdle(player_position, facing_right);		break;
	case ARM_STATE::ATTACK:		UpdateAttack(attack_input);						break;
	case ARM_STATE::IMPACT:		UpdateImpact();									break;
	case ARM_STATE::RETURN:		UpdateReturn(player_position, facing_right);	break;
	}

	CapsuleUpdate();
}

/*
 *  描画
 */
void
CArm::
Draw(void)
{
	vivid::Vector2 scroll = CCameraManager::GetInstance().GetScroll();
	float camera_scale = CCameraManager::GetInstance().GetCameraScale();

	// 画像の回転の中心を設定
	const vivid::Vector2 rotation_pivot =
	{
		m_width * 0.5f,
		m_height * 0.5f
	};

	vivid::Vector2 pivot = rotation_pivot;

	// 左向きの場合はPivotのX座標を反転
	if (!m_FacingRight)
		pivot.x = -pivot.x;

	const float cos_rotation = cosf(m_Rotation);
	const float sin_rotation = sinf(m_Rotation);

	// 回転後のPivot座標を計算
	const vivid::Vector2 rotated_pivot =
	{
		pivot.x * cos_rotation - pivot.y * sin_rotation,
		pivot.x * sin_rotation + pivot.y * cos_rotation
	};

	// 描画位置を計算
	vivid::Vector2 draw_position = m_Position + pivot - rotated_pivot;

	// カメラの反映
	draw_position *= camera_scale;
	draw_position -= scroll;

	vivid::Vector2 scale = { camera_scale, camera_scale };
	vivid::Rect draw_rect = { 0, 0, m_width, m_height };

	// 左向きの場合は描画位置を反転
	if (!m_FacingRight)
	{
		scale.x *= -1.0f;

		draw_position.x += m_width * camera_scale;
	}

	// 待機状態のときは腕を揺らす
	if (m_State == ARM_STATE::IDLE && !m_GuardFlg && !m_GuardBreakFlg && !m_ThrowPoseFlg)
	{
		m_IdleTimer++;

		float IdleOffset;

		// 腕によって揺れの周期を変える
		if (m_Id == ARM_ID::LEFT)
			IdleOffset = std::sin(m_IdleTimer * 0.15f) * 1.8f;
		else
			IdleOffset = std::sin(m_IdleTimer * 0.14f) * 1.8f;

		draw_position.y += IdleOffset;
	}
	else
	{
		m_IdleTimer = 0.0f;
	}

	// ガード中は両腕とも表側を描画
	if (m_GuardFlg || m_GuardBreakFlg)
	{
		draw_rect = { 0, 0, m_width, m_height };

		// 大罪IDが色欲の場合ガードの見た目がハートになるように調整
		if (m_SaligiaID == SALIGIA_ID::LUXURIA)
		{
			if (m_Id == ARM_ID::LEFT)
			{
				if (m_FacingRight)
				{
					scale.x *= -1.0f;
					draw_position.x += m_width * camera_scale;
					draw_position.x += 10;
				}
				else
				{
					scale.x *= -1.0f;
					draw_position.x -= m_width * camera_scale;
					draw_position.x -= 10;
				}
			}
			else
			{
				if (m_FacingRight)
				{
					draw_position.x -= 10;
				}
				else
				{
					draw_position.x += 10;
				}
			}
		}

		// 大罪IDが怠惰の時の見た目調整
		if (m_SaligiaID == SALIGIA_ID::ACEDIA)
		{
			if (m_Id != ARM_ID::LEFT)
			{
				if (m_FacingRight)
				{
					scale.x *= -1.0f;
					draw_position.x += m_width * camera_scale;
					draw_position.x -= 5;
					draw_position.y += 4;
				}
				else
				{
					scale.x *= -1.0f;
					draw_position.x -= m_width * camera_scale;
					draw_position.x += 5;
					draw_position.y += 4;
				}
			}
		}
	}
	else
	{
		// 腕によって描画する画像の範囲を変更
		if (m_Id == ARM_ID::LEFT)
			draw_rect = { m_width, 0, m_width * 2, m_height };
	}

	const vivid::Vector2 anchor = { 0, 0 };

	vivid::DrawTexture(m_FilePath, draw_position, 0xffffffff, draw_rect, anchor, scale, m_Rotation);
}

/*
 *  解放
 */
void
CArm::
Finalize(void)
{
}

/*
 *  当たり判定の更新
 */
void
CArm::
CapsuleUpdate(void)
{
	// 腕画像の中心
	const vivid::Vector2 center = m_Position + vivid::Vector2(m_width * 0.5f, m_height * 0.5f);

	// カプセルの半径
	const float radius = m_width * 0.5f;

	// 腕の長さ方向
	const float cos_rotation = cosf(m_Rotation);
	const float sin_rotation = sinf(m_Rotation);

	const vivid::Vector2 direction =
	{
		-sin_rotation,
		 cos_rotation
	};

	// カプセルの直線部分の半分
	const float half_length = (m_height - m_width) * 0.5f;

	m_Capsule.Start = center - direction * half_length;
	m_Capsule.End = center + direction * half_length;
	m_Capsule.Radius = radius;
}

/*
 *  攻撃開始
 */
void
CArm::
StartAttack(const AttackInfo& attack, bool facing_right)
{
	if (m_State != ARM_STATE::IDLE) return;
	if (m_GuardBreakFlg) return;

	m_AttackInfo = attack;
	m_FacingRight = facing_right;

	m_AttackStartPosition = m_Position;
	m_SteerOffset = vivid::Vector2::ZERO;

	vivid::Vector2 control1 = m_AttackInfo.Control1;
	vivid::Vector2 control2 = m_AttackInfo.Control2;
	vivid::Vector2 end_position = m_AttackInfo.EndPosition;

	// CSVは右向き基準。左向き時はXだけ反転する。
	if (!facing_right)
	{
		control1.x *= -1.0f;
		control2.x *= -1.0f;
		end_position.x *= -1.0f;
	}

	m_ControlPoint1 = m_AttackStartPosition + control1;
	m_ControlPoint2 = m_AttackStartPosition + control2;
	m_AttackEndPosition = m_AttackStartPosition + end_position;

	m_Rotation = DEG_TO_RAD(m_AttackInfo.StartRotation + m_AttackInfo.RotationOffset);
	if (!facing_right)
		m_Rotation *= -1.0f;

	m_CurrentFrame = 0;
	ClearHitTargets();
	m_State = ARM_STATE::ATTACK;
}

/*
 *  ヒット時の処理
 */
void
CArm::
OnHit(void)
{
	if (m_State != ARM_STATE::ATTACK) return;

	// 薙ぎ払い等は命中後も軌道を継続する。
	if (!m_AttackInfo.StopOnHit) return;

	m_CurrentFrame = 0;
	m_State = ARM_STATE::IMPACT;
}

/*
 *  攻撃キャンセル
 */
void
CArm::
CancelAttack(void)
{
	if (m_State == ARM_STATE::IDLE) return;

	m_ReturnStartPosition = m_Position;
	m_ReturnStartRotation = m_Rotation;
	m_CurrentFrame = 0;
	m_State = ARM_STATE::RETURN;

	this->ClearHitTargets();
}

/*
 *  ヒット対象の取得
 */
bool
CArm::
HasHit(CPlayer* target) const
{
	for (CPlayer* hit_target : m_HitTargets)
	{
		if (hit_target == target)
		{
			return true;
		}
	}

	return false;
}

/*
 *  ヒット対象の追加
 */
void
CArm::
AddHitTarget(CPlayer* target)
{
	if (target == nullptr)
	{
		return;
	}

	m_HitTargets.push_back(target);
}

/*
 *  ヒット対象のクリア
 */
void
CArm::
ClearHitTargets()
{
	m_HitTargets.clear();
}

/*
 *  攻撃中かどうかの取得
 */
bool
CArm::
IsActiveFrame(void) const
{
	// 腕そのものを攻撃判定として扱う仕様。
	// ATTACKフェーズ中は見た目の腕と判定を常に一致させる。
	// IMPACT / RETURN / IDLEでは攻撃判定を無効にする。
	return m_State == ARM_STATE::ATTACK;
}

/*
 *  ガードブレイク開始
 */
void
CArm::
StartGuardBreak(int duration)
{
	// 攻撃中には開始しない
	if (m_State != ARM_STATE::IDLE)	return;
	if (m_GuardBreakFlg)	return;

	m_GuardFlg = false;
	m_ThrowPoseFlg = false;

	m_GuardBreakFlg = true;

	m_GuardBreakFrame = 0;
	m_GuardBreakDuration = (duration > 0) ? duration : 1;

	// 現在のガード位置から開始
	m_GuardBreakStartPosition = m_Position;
	m_GuardBreakStartRotation = m_Rotation;

	// 攻撃対象情報は念のためクリア
	ClearHitTargets();
}

/*
 *  投げ用ポーズ設定
 */
void
CArm::
SetThrowPose(const vivid::Vector2& position, float rotation)
{
	if (m_State != ARM_STATE::IDLE) return;

	m_ThrowPoseFlg = true;

	m_Position = position;
	m_Rotation = rotation;

	CapsuleUpdate();
}

/*
 *  投げ用ポーズ解除
 */
void
CArm::
EndThrowPose(void)
{
	m_ThrowPoseFlg = false;
}

/*
 *  Idle状態の更新
 */
void
CArm::
UpdateIdle(const vivid::Vector2& player_position, bool facing_right)
{
	// 追従処理
	vivid::Vector2 target_position = GetTargetPosition(player_position, facing_right);
	float target_rotation = 0.0f;

	// ガード中の場合は腕を体の前に移動
	if (m_GuardFlg)
	{
		vivid::Vector2 guard_position;

		if (m_Id == ARM_ID::LEFT)
		{
			guard_position = vivid::Vector2(45.0f, -10.0f);
			target_rotation = DEG_TO_RAD(-5.0f);
		}
		else
		{
			guard_position = vivid::Vector2(15.0f, -10.0f);
			target_rotation = DEG_TO_RAD(5.0f);
		}

		// 左向きの場合は位置だけ反転
		if (!facing_right)
		{
			guard_position.x *= -1.0f;
			target_rotation *= -1.0f;
		}

		target_position = player_position + guard_position;
	}

	// 位置と回転をターゲットに向かって補間
	const vivid::Vector2 diff = target_position - m_Position;
	m_Position += diff * m_follow_rate;

	const float rotation_diff = target_rotation - m_Rotation;
	m_Rotation += rotation_diff * m_follow_rate;
}

/*
 *  ガードブレイク中の腕更新
 */
void
CArm::
UpdateGuardBreak(const vivid::Vector2& player_position, bool facing_right)
{
	// ガードブレイクの時間設定
	const int BREAK_FRAME = 8;
	const int FALL_FRAME = 12;
	const int RECOVER_FRAME = 30;

	int recover_start = m_GuardBreakDuration - RECOVER_FRAME;

	// 硬直時間が短い場合の保険
	if (recover_start < BREAK_FRAME + FALL_FRAME)
		recover_start =	BREAK_FRAME + FALL_FRAME;

	++m_GuardBreakFrame;

	// 向き
	const float facing_sign = facing_right ? 1.0f : -1.0f;

	// ブレイク時の位置設定
	vivid::Vector2 break_local;

	float break_rotation;

	if (m_Id == ARM_ID::LEFT)
	{
		break_local = vivid::Vector2(-85.0f, -30.0f);
		break_rotation = DEG_TO_RAD(-60.0f);
	}
	else
	{
		break_local = vivid::Vector2(85.0f, -30.0f);
		break_rotation = DEG_TO_RAD(60.0f);
	}

	break_local.x *= facing_sign;
	break_rotation *= facing_sign;

	const vivid::Vector2 break_position = player_position + break_local;

	// ダウン時の位置設定
	vivid::Vector2 down_local;

	float down_rotation;

	if (m_Id == ARM_ID::LEFT)
	{
		down_local = vivid::Vector2(-60.0f, 55.0f);
		down_rotation = DEG_TO_RAD(-25.0f);
	}
	else
	{
		down_local = vivid::Vector2(60.0f, 55.0f);
		down_rotation = DEG_TO_RAD(25.0f);
	}

	down_local.x *= facing_sign;
	down_rotation *= facing_sign;

	const vivid::Vector2 down_position = player_position + down_local;

	// ブレイク処理
	if (m_GuardBreakFrame <= BREAK_FRAME)
	{
		float t = static_cast<float>(m_GuardBreakFrame) / static_cast<float>(BREAK_FRAME);

		t = CLAMP(t, 0.0f, 1.0f);

		// EaseOut
		const float inv = 1.0f - t;
		t = 1.0f - inv * inv;

		m_Position = m_GuardBreakStartPosition + (break_position - m_GuardBreakStartPosition) * t;
		m_Rotation = m_GuardBreakStartRotation + (break_rotation - m_GuardBreakStartRotation) * t;

		return;
	}

	// 落ちる処理
	if (m_GuardBreakFrame <= BREAK_FRAME + FALL_FRAME)
	{
		float t = static_cast<float>(m_GuardBreakFrame - BREAK_FRAME) / static_cast<float>(FALL_FRAME);

		t = CLAMP(t, 0.0f, 1.0f);

		// SmoothStep
		t = t * t * (3.0f - 2.0f * t);

		m_Position = break_position + (down_position - break_position) * t;
		m_Rotation = break_rotation + (down_rotation - break_rotation) * t;

		return;
	}

	// おろす
	if (m_GuardBreakFrame < recover_start)
	{
		m_Position = down_position;
		m_Rotation = down_rotation;

		return;
	}

	// 戻す処理
	if (m_GuardBreakFrame <= m_GuardBreakDuration)
	{
		const int recover_elapsed = m_GuardBreakFrame - recover_start;
		const int recover_duration = m_GuardBreakDuration - recover_start;

		float t = 1.0f;

		if (recover_duration > 0)
		{
			t = static_cast<float>(recover_elapsed) / static_cast<float>(recover_duration);
		}

		t = CLAMP(t, 0.0f, 1.0f);

		// SmoothStep
		t = t * t * (3.0f - 2.0f * t);

		const vivid::Vector2 idle_position = GetTargetPosition(player_position, facing_right);

		m_Position = down_position + (idle_position - down_position) * t;
		m_Rotation = down_rotation * (1.0f - t);

		return;
	}

	// 終了
	m_GuardBreakFlg = false;
	m_GuardBreakFrame = 0;

	m_Position = GetTargetPosition(player_position, facing_right);

	m_Rotation = 0.0f;
}

/*
 *  攻撃状態の更新
 */
void
CArm::
UpdateAttack(const vivid::Vector2& attack_input)
{
	// 入力追従。軌道そのものは変えず、全体へオフセットを足す。
	m_SteerOffset += attack_input * m_AttackInfo.SteerSpeed;

	const vivid::Vector2 previous_position = m_Position;

	if (m_CurrentFrame < m_AttackInfo.MoveFrame)
	{
		float progress = 1.0f;
		if (m_AttackInfo.MoveFrame > 0)
			progress = static_cast<float>(m_CurrentFrame + 1) / static_cast<float>(m_AttackInfo.MoveFrame);

		progress = CLAMP(progress, 0.0f, 1.0f);
		const float eased = ApplyAttackEasing(progress);

		m_Position = EvaluateBezier(eased) + m_SteerOffset;
		UpdateAttackRotation(eased, previous_position);
	}
	else
	{
		// HOLD中は終点に残る。入力追従分だけ移動可能。
		m_Position = m_AttackEndPosition + m_SteerOffset;
	}

	++m_CurrentFrame;

	if (m_CurrentFrame >= m_AttackInfo.GetAttackFrame())
		BeginReturn();
}

/*
 *  ヒット時の停止状態の更新
 */
void
CArm::
UpdateImpact(void)
{
	++m_CurrentFrame;

	if (m_CurrentFrame >= m_AttackInfo.ImpactFrame)
		BeginReturn();
}

/*
 *  戻り状態の更新
 */
void
CArm::
UpdateReturn(const vivid::Vector2& player_position, bool facing_right)
{
	const vivid::Vector2 target_position = GetTargetPosition(player_position, facing_right);

	if (m_AttackInfo.ReturnFrame <= 0)
	{
		m_Position = target_position;
		m_Rotation = 0.0f;
		m_CurrentFrame = 0;
		m_State = ARM_STATE::IDLE;
		return;
	}

	++m_CurrentFrame;

	float rate = static_cast<float>(m_CurrentFrame) / static_cast<float>(m_AttackInfo.ReturnFrame);
	rate = CLAMP(rate, 0.0f, 1.0f);
	const float return_rate = rate * rate * (3.0f - 2.0f * rate);

	m_Position = m_ReturnStartPosition + (target_position - m_ReturnStartPosition) * return_rate;
	m_Rotation = m_ReturnStartRotation * (1.0f - return_rate);

	if (m_CurrentFrame >= m_AttackInfo.ReturnFrame)
	{
		m_Position = target_position;
		m_Rotation = 0.0f;
		m_CurrentFrame = 0;
		m_State = ARM_STATE::IDLE;
	}
}

/*
 *  腕の目標位置を取得
 */
vivid::Vector2
CArm::
GetTargetPosition(const vivid::Vector2& player_position, bool facing_right) const
{
	vivid::Vector2 local_position = m_LocalPosition;

	// 左向きの場合はローカル位置のX座標を反転
	if (!facing_right)
		local_position.x *= -1.0f;

	return player_position + local_position;
}

/*
 *  攻撃進行度へEasingを適用
 */
float
CArm::
ApplyAttackEasing(float progress) const
{
	progress = CLAMP(progress, 0.0f, 1.0f);

	switch (m_AttackInfo.Easing)
	{
	case ATTACK_EASING_TYPE::EASE_IN:
		return progress * progress;

	case ATTACK_EASING_TYPE::EASE_OUT:
	{
		const float inv = 1.0f - progress;
		return 1.0f - inv * inv;
	}

	case ATTACK_EASING_TYPE::EASE_IN_OUT:
		return progress * progress * (3.0f - 2.0f * progress);

	case ATTACK_EASING_TYPE::LINEAR:
	default:
		return progress;
	}
}

/*
 *  3次ベジェ曲線上の位置を取得
 */
vivid::Vector2
CArm::
EvaluateBezier(float progress) const
{
	const float t = CLAMP(progress, 0.0f, 1.0f);
	const float u = 1.0f - t;

	return m_AttackStartPosition * (u * u * u)
		+ m_ControlPoint1 * (3.0f * u * u * t)
		+ m_ControlPoint2 * (3.0f * u * t * t)
		+ m_AttackEndPosition * (t * t * t);
}

/*
 *  攻撃中の腕回転を更新
 */
void
CArm::
UpdateAttackRotation(float progress, const vivid::Vector2& previous_position)
{
	const float facing_sign = m_FacingRight ? 1.0f : -1.0f;
	const float rotation_offset = DEG_TO_RAD(m_AttackInfo.RotationOffset) * facing_sign;

	switch (m_AttackInfo.RotationMode)
	{
	case ARM_ROTATION_TYPE::LERP_ROTATION:
	{
		const float start = DEG_TO_RAD(m_AttackInfo.StartRotation) * facing_sign;
		const float end = DEG_TO_RAD(m_AttackInfo.EndRotation) * facing_sign;
		m_Rotation = start + (end - start) * progress + rotation_offset;
		break;
	}

	case ARM_ROTATION_TYPE::FIXED:
		m_Rotation = DEG_TO_RAD(m_AttackInfo.StartRotation) * facing_sign + rotation_offset;
		break;

	case ARM_ROTATION_TYPE::ROTATE_SPIN:
		m_Rotation = DEG_TO_RAD(m_AttackInfo.StartRotation + m_AttackInfo.SpinDegree * progress) * facing_sign + rotation_offset;
		break;

	case ARM_ROTATION_TYPE::PATH:
	default:
	{
		const vivid::Vector2 move = m_Position - previous_position;
		if (vivid::Vector2::Length(move) > 0.001f)
			m_Rotation = std::atan2(move.y, move.x) - PI * 0.5f + rotation_offset;
		break;
	}
	}
}

/*
 *  戻り状態へ移行
 */
void
CArm::
BeginReturn(void)
{
	m_ReturnStartPosition = m_Position;
	m_ReturnStartRotation = m_Rotation;
	m_CurrentFrame = 0;
	m_State = ARM_STATE::RETURN;
}
