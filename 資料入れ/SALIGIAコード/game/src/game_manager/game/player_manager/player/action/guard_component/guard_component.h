
/*!
 *  @file		guard_component.h
 *  @brief		ガードコンポーネント
 *  @author     Ryusei Shimizu
 *  @date       2026/07/01
 */

#pragma once

#include "vivid.h"
#include "../../../../../../utility/csv_loader/loader/attack_csv_loader/attack_id.h"

class CPlayer;
class CResourceComponent;

/*!
 *	@class		CGuardComponent
 *
 *	@brief		ガードコンポーネント
 *
 *	@author     Ryusei Shimizu
 *
 *  @date       2026/07/01
 */
class CGuardComponent
{
public:

	/*!
	 *	@brief	コンストラクタ
	 */
    CGuardComponent(void);

	/*!
	 *	@brief	デストラクタ
	 */
	~CGuardComponent(void) = default;

	/*!
	 *	@brief	初期化
	 */
    void Initialize(void);

	/*!
	 *	@brief	更新
	 */	
    void Update(CResourceComponent& resource_component);

	/*!
	 *	@brief	ガード開始
	 */
    void Start(void);

	/*!
	 *	@brief	ガード終了
	 */
    void End(void);

	/*!
	 *	@brief	ガード処理
	 * 
	 *	@param[in]	damage		ダメージ情報
	 *	@param[in]	character	プレイヤー情報
	 */
    void Guard(const DamageInfo& damage, CPlayer& character);

	/*!
	 *	@brief	ガードブレイクフラグリセット
	 */
	void  ResetBreak(CResourceComponent& resource_component);

	/*!
	 *	@brief	ガード判定
	 */
	bool IsGuard(void) const { return m_Guarding; };

	/*!
	 *	@brief	ガードブレイク判定
	 */
    bool IsBreak(void) const { return m_Break; };

	/*!
	 *	@brief	ジャストガード判定
	 */
    bool IsJustGuard(void) const { return m_JustGuard; };

	/*!
	 *	@brief	ガードブレイク時の硬直時間取得
	 * 
	 *	@return		ガードブレイク時の硬直時間
	 */
	int GetBreakStiffnessTime(void) const { return m_break_stiffness_time; };

private:

	static const int	m_just_guard_time;			//!< ジャストガード時間
	static const int	m_break_stiffness_time;		//!< ガードブレイク時の硬直時間
	static const int	m_guard_reduce_interval;	//!< ガード耐久減少間隔
	static const float	m_guard_reduce_value;		//!< ガード耐久減少量

	bool				m_Guarding;			//!< ガード中かどうか
	bool				m_Break;			//!< ガードブレイク中かどうか
	bool				m_JustGuard;		//!< ジャストガード中かどうか

	int					m_GuardTimer;		//!< ガード耐久減少タイマー
	int					m_JustGuardTimer;	//!< ジャストガードタイマー
};