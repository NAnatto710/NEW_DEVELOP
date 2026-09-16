
/*!
 *  @file		throw_component.h
 *  @brief		投げコンポーネント
 *  @author     Ryusei Shimizu
 *  @date       2026/09/12
 */

#pragma once

#include "vivid.h"
#include "../../../../../../utility/collision_check/collision_check.h"

class CPlayer;

/*!
 *	@brief		投げ状態
 */
enum class THROW_STATE
{
	IDLE,				//!< 待機状態
	STARTUP,			//!< 投げ発生状態
	ACTIVE,				//!< 投げ判定状態
	SUCCESS,			//!< 投げ成功状態
	FOLLOW_THROUGH,		//!< 投げ後のフォロースルー状態
	RECOVERY,			//!< 投げ後の空振り硬直状態
};

/*!
 *	@class		CThrowComponent
 *
 *	@brief		投げコンポーネントクラス
 *
 *	@author     Ryusei Shimizu
 *
 *  @date       2026/09/12
 */
class CThrowComponent
{
public:

	/*!
	 *	@brief		コンストラクタ
	 */
	CThrowComponent(void);

	/*!
	 *	@brief		デストラクタ
	 */
	~CThrowComponent(void) = default;

	/*!
	 *	@brief		初期化
	 *
	 *	@param[in]	player_width	プレイヤーの横幅
	 *	@param[in]	player_height	プレイヤーの高さ
	 */
	void			Initialize(int player_width, int player_height);

	/*!
	 *	@brief		更新
	 *
	 *	@param[in]	owner	投げを行うプレイヤー
	 */
	void			Update(CPlayer& owner);

	/*!
	 *	@brief		投げ中の腕ポーズ更新
	 *
	 *	@param[in]	owner	投げを行うプレイヤー
	 */
	void			ApplyArmPose(CPlayer& owner);

	/*!
	 *	@brief		投げ開始
	 */
	void			Start(void);

	/*!
	 *	@brief		投げ成功
	 *
	 *	@param[in]	target	投げられるプレイヤー
	 *
	 *	@return		投げが成立した場合true
	 */
	bool			Catch(CPlayer& target);

	/*!
	 *	@brief		投げキャンセル
	 */
	void			Cancel(void);

	/*!
	 *	@brief		投げ判定が有効かどうか
	 *
	 *	@return		投げ判定が有効であればtrue
	 */
	bool			IsActiveFrame(void) const { return m_State == THROW_STATE::ACTIVE; }

	/*!
	 *	@brief		投げ成功中かどうか
	 *
	 *	@return		投げ成功中であればtrue
	 */
	bool			IsSuccess(void) const { return m_State == THROW_STATE::SUCCESS; }

	/*!
	 *	@brief		投げ動作中かどうか
	 *
	 *	@return		投げ動作中であればtrue
	 */
	bool			IsThrowing(void) const { return m_State != THROW_STATE::IDLE; }

	/*!
	 *	@brief		投げ判定取得
	 *
	 *	@param[in]	player_position	プレイヤー位置
	 *	@param[in]	facing_right		右向きかどうか
	 *
	 *	@return		投げ判定
	 */
	AABB			GetThrowArea(const vivid::Vector2& player_position, bool facing_right) const;

	/*!
	 *	@brief		投げ状態取得
	 *
	 *	@return		投げ状態
	 */
	THROW_STATE		GetState(void) const { return m_State; }

private:

	/*!
	 *	@brief		投げ成功中の対象位置更新
	 *
	 *	@param[in]	owner	投げを行うプレイヤー
	 */
	void			UpdateTargetPosition(CPlayer& owner);
	/*!
	 *	@brief		投げ終了処理
	 *
	 *	@param[in]	owner	投げを行うプレイヤー
	 */
	void			FinishThrow(CPlayer& owner);


	static const int			m_startup_frame;		//!< 発生フレーム
	static const int			m_active_frame;			//!< 投げ判定フレーム
	static const int			m_success_frame;		//!< 投げ成功演出フレーム
	static const int			m_follow_through_frame;	//!< 投げ後のフォロースルーフレーム
	static const int			m_recovery_frame;		//!< 空振り硬直フレーム

	static const float			m_range_rate;			//!< プレイヤー横幅に対する投げ射程倍率
	static const float			m_height_rate;			//!< プレイヤー高さに対する投げ判定倍率
	static const float			m_grab_offset_rate;		//!< 掴み位置の横方向倍率
	static const float			m_hp_damage;			//!< 投げダメージ
	static const int			m_hit_stun;				//!< 投げ後の硬直時間
	static const vivid::Vector2 m_knock_back;			//!< 投げの吹っ飛ばし
	static const float			m_guard_damage_rate;	//!< 投げダメージに対するガード中の倍率

	int							m_PlayerWidth;			//!< プレイヤー横幅
	int							m_PlayerHeight;			//!< プレイヤー高さ
	int							m_CurrentFrame;			//!< 現在フレーム
	bool						m_WasTargetGuarding;	//!< 対象がガード中だったかどうか

	THROW_STATE					m_State;				//!< 投げ状態
	CPlayer*					m_Target;				//!< 投げ対象
};
