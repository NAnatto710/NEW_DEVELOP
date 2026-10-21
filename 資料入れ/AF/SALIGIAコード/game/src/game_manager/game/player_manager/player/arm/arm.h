
/*!
 *  @file		arm.h
 *  @brief		プレイヤーの腕
 *  @author     Ryusei Shimizu
 *  @date       2026/08/25
 */

#pragma once

#include "vivid.h"
#include "../../../../../utility/utility.h"
#include "../../../../../utility/csv_loader/loader/attack_csv_loader/attack_id.h"
#include "../../../../../utility/collision_check/collision_check.h"
#include "../../player_id.h"
#include "arm_id.h"
#include <vector>

class CPlayer;

/*!
 *  @class		CArm
 *
 *  @brief		プレイヤーの腕クラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2026/08/25
 */
class CArm
{
public:
	/*!
	 *  @brief				コンストラクタ
	 *
	 *  @param[in]			id				腕の識別子
	 *  @param[in]			local_position	Playerから見た腕の固定相対位置
	 */
	CArm(ARM_ID id, const vivid::Vector2& local_position);

	/*!
	 *  @brief		デストラクタ
	 */
	~CArm() = default;

	/*!
	 *  @brief				初期化
	 *
	 *  @param[in]			player_position		Playerの位置
	 *  @param[in]			facing_right		Playerが右を向いているかどうか
	 *	@param[in]			saligia_id			腕の識別子
	 */
	void					Initialize(const vivid::Vector2& player_position, bool facing_right, SALIGIA_ID saligia_id);

	/*!
	 *  @brief				更新
	 *
	 *  @param[in]			player_position		Playerの位置
	 *  @param[in]			facing_right		Playerが右を向いているかどうか
	 *	@param[in]			attack_input		攻撃入力
	 */
	void					Update(const vivid::Vector2& player_position, bool facing_right, const vivid::Vector2& attack_input);

	/*!
	 *  @brief				描画
	 */
	void					Draw(void);

	/*!
	 *  @brief				解放
	 */
	void					Finalize(void);

	/*!
	 *  @brief				当たり判定の更新
	 */
	void					CapsuleUpdate(void);

	/*!
	 *  @brief				攻撃開始
	 *
	 *	@param[in]			attack				攻撃情報
	 *	@param[in]			facing_right		Playerが右を向いているかどうか
	 */
	void					StartAttack(const AttackInfo& attack, bool facing_right);

	/*!
	 *  @brief				ヒット時の処理
	 */
	void					OnHit(void);

	/*!
	 *  @brief				攻撃キャンセル
	 */
	void					CancelAttack(void);

	/*!
	 *  @brief				ヒット対象の取得
	 *
	 *  @return				ヒット対象のプレイヤーリスト
	 */
	bool					HasHit(CPlayer* target) const;

	/*!
	 *  @brief				ヒット対象の追加
	 *
	 *  @param[in]			target	ヒット対象のプレイヤー
	 */
	void					AddHitTarget(CPlayer* target);

	/*!
	 *  @brief				ヒット対象のクリア
	 */
	void					ClearHitTargets();

	/*!
	 *  @brief				腕の識別子取得
	 *
	 *  @return				腕の識別子
	 */
	ARM_ID					GetID(void) const { return m_Id; }

	/*!
	 *  @brief				腕の状態取得
	 *
	 *  @return				腕の状態
	 */
	ARM_STATE				GetState(void) const { return m_State; }

	/*!
	 *  @brief				腕の位置取得
	 *
	 *  @return				腕の位置
	 */
	const vivid::Vector2& GetPosition(void) const { return m_Position; }

	/*!
	 *  @brief				腕の当たり判定取得
	 *
	 *  @return				腕の当たり判定
	 */
	Capsule& GetCapsule(void) { return m_Capsule; }
	const Capsule& GetCapsule(void) const { return m_Capsule; }
	const Capsule& GetPreviousCapsule(void) const { return m_PreviousCapsule; }

	/*!
	 *  @brief				腕のダメージ情報取得
	 *
	 *  @return				腕のダメージ情報
	 */
	DamageInfo				GetDamageInfo(void) const { return m_AttackInfo.Damage; }

	/*!
	 *  @brief				攻撃中かどうかの取得
	 *
	 *  @return				攻撃中かどうか
	 */
	bool					IsActiveFrame(void) const;

	/*!
	 *  @brief				腕の大罪ID取得
	 *
	 *  @return				大罪ID
	 */
	void					SetSaligiaID(SALIGIA_ID saligia_id) { m_SaligiaID = saligia_id; }

	/*!
	 *  @brief				腕の大罪ID取得
	 *
	 *  @return				大罪ID
	 */
	SALIGIA_ID				GetSaligiaID(void) const { return m_SaligiaID; }

	/*!
	 *  @brief				ガード状態設定
	 *
	 *  @param[in]			guarding			ガード中かどうか
	 */
	void					SetGuard(bool guarding) { m_GuardFlg = guarding; }

	/*!
	 *  @brief				ガードブレイク開始
	 *
	 *  @param[in]			duration	ガードブレイク硬直時間
	 */
	void					StartGuardBreak(int duration);

	/*!
	 *  @brief				投げ用ポーズ設定
	 *
	 *  @param[in]			position	腕の位置
	 *  @param[in]			rotation	腕の回転
	 */
	void					SetThrowPose(const vivid::Vector2& position, float rotation);

	/*!
	 *  @brief				投げ用ポーズ終了
	 */
	void					EndThrowPose(void);

	/*!
	 *  @brief				腕の回転取得
	 *
	 *  @return				腕の回転
	 */
	float					GetRotation(void) const { return m_Rotation; }

private:

	/*!
	 *  @brief				待機状態の更新
	 *
	 *  @param[in]			player_position		Playerの位置
	 *  @param[in]			facing_right		Playerが右を向いているかどうか
	 */
	void					UpdateIdle(const vivid::Vector2& player_position, bool facing_right);

	/*!
	 *  @brief				ガードブレイク中の腕更新
	 *
	 *  @param[in]			player_position		Playerの位置
	 *  @param[in]			facing_right		Playerが右を向いているか
	 */
	void					UpdateGuardBreak(const vivid::Vector2& player_position, bool facing_right);

	/*!
	 *  @brief				攻撃状態の更新
	 *
	 *	@param[in]			attack_input		攻撃入力
	 */
	void					UpdateAttack(const vivid::Vector2& attack_input);

	/*!
	 *  @brief				ヒット時の停止状態の更新
	 */
	void					UpdateImpact(void);

	/*!
	 *  @brief				戻り状態の更新
	 *
	 *  @param[in]			player_position	Playerの位置
	 *  @param[in]			facing_right		Playerが右を向いているかどうか
	 */
	void					UpdateReturn(const vivid::Vector2& player_position, bool facing_right);

	/*!
	 *  @brief				腕の目標位置を取得
	 *
	 *  @param[in]			player_position		Playerの位置
	 *  @param[in]			facing_right		Playerが右を向いているかどうか
	 *
	 *  @return				腕の目標位置
	 */
	vivid::Vector2			GetTargetPosition(const vivid::Vector2& player_position, bool facing_right) const;

	/*!
	 *  @brief				攻撃の進行度から攻撃速度を取得
	 *
	 *  @param[in]			progress	攻撃の進行度(0.0f～1.0f)
	 *
	 *  @return				攻撃速度
	 */
	float					ApplyAttackEasing(float progress) const;
	vivid::Vector2			EvaluateBezier(float progress) const;
	void					UpdateAttackRotation(float progress, const vivid::Vector2& previous_position);
	void					BeginReturn(void);

	static const float		m_follow_rate;			//!< 腕の追従率
	static const int        m_width;				//!< 腕の幅
	static const int        m_height;				//!< 腕の高さ

	const vivid::Vector2	m_LocalPosition;			//!< Playerから見た腕の固定相対位置

	vivid::Vector2			m_Position;					//!< 腕の現在位置
	vivid::Vector2			m_PreviousPlayerPosition;	//!< 前フレームのPlayer位置（攻撃軌道追従用）
	float 					m_Rotation;					//!< 腕の現在角度
	bool					m_FacingRight;				//!< Playerの向き
	bool					m_GuardFlg;					//!< ガード状態
	bool					m_GuardBreakFlg;			//!< ガードブレイク演出中
	bool					m_ThrowPoseFlg;				//!< 投げ用ポーズ中かどうか

	int						m_GuardBreakFrame;			//!< ガードブレイク経過フレーム
	int						m_GuardBreakDuration;		//!< ガードブレイク総時間

	vivid::Vector2			m_GuardBreakStartPosition;	//!< ガードブレイク開始位置
	float					m_GuardBreakStartRotation;	//!< ガードブレイク開始角度

	vivid::Vector2			m_AttackStartPosition;	//!< 攻撃開始位置
	vivid::Vector2			m_ControlPoint1;		//!< ベジェ制御点1（ワールド座標）
	vivid::Vector2			m_ControlPoint2;		//!< ベジェ制御点2（ワールド座標）
	vivid::Vector2			m_AttackEndPosition;	//!< ベジェ終点（ワールド座標）
	vivid::Vector2			m_SteerOffset;			//!< 攻撃中の入力補正
	vivid::Vector2			m_ReturnStartPosition;	//!< 戻り開始位置
	float					m_ReturnStartRotation;	//!< 戻り開始角度

	int						m_CurrentFrame;			//!< 現在の攻撃フレーム

	float                   m_IdleTimer;			//!< 待機状態のタイマー

	std::string             m_FilePath;				//!< 腕の画像パス

	SALIGIA_ID				m_SaligiaID;			//!< 腕の大罪ID
	ARM_ID					m_Id;					//!< 腕の識別子
	ARM_STATE				m_State;				//!< 腕の状態
	Capsule                 m_Capsule;				//!< 腕の当たり判定
	Capsule                 m_PreviousCapsule;		//!< 1フレーム前の腕判定

	AttackInfo				m_AttackInfo;			//!< 現在の攻撃情報

	std::vector<CPlayer*>	m_HitTargets;			//!< ヒット対象のプレイヤーリスト
};