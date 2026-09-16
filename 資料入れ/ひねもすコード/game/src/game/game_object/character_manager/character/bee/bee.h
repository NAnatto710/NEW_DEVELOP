
/*!
 *  @file       far_enemy.h
 *  @brief      蜂 クラス
 *  @author     Misaki Kawada
 *  @date       2026/01/27
 */

#pragma once

#include "vivid.h"
#include "../character.h"
#include "../../character_manager.h"
#include "../../../../../utility/utility.h"
#include "../../../bullet_manager/bullet_manager.h"
#include "../../../camera_manager/camera_manager.h"


 /*!
   *  @class      CBee
   *
   *  @brief      蜂クラス
   *
   *  @author      Misaki Kawada
   *
   *  @date        2026/01/27
   */
class  CBee
	:public ICharacter
{
public:

	/*!
	 *  @brief      コンストラクタ
	 */
	CBee(void);

	/*!
	 *  @brief      デストラクタ
	 */
	~CBee(void);

	/*!
	 *  @brief      初期化
	 *
	 *  @param[in]	position	位置
	 */
	void			Initialize(const vivid::Vector2& position);

private:

	/*!
	 *  @brief  動作
	 */
	void			Move(void)override;

	/*
	 *	@brief 攻撃
	 */
	void			Attack(void)override;

	/*
	 *	@breif アニメーション
	 */

	void			Animation(void);

	/*
	 *	@breif アニメーションに使う実数値の更新
	 */
	void			ChangeAnimation(void);

	static const float              m_status[(int)STATUS_ID::MAX][(int)STATUS_DEFINITION::MAX];				//!< ステータス配列
	static const float              m_upgrade_status[(int)UPGRADE_STATUS_ID::MAX];							//!< 強化ステータス配列

	static const int				m_size;							//!< サイズ
	static const std::string		m_path_name;					//!< 画像名
	static const float				m_neardistance;					//!< 近づく一定の距離
	static const float				m_stopdistance;					//!< 止まる距離
	static const float				m_moveawaydistance;				//!< 離れる一定の距離
	static const float				m_animation;					//!< Animationの区切り

	static const float				m_max_hp_add;								//!< 最大HPの加算値
	static const float              m_max_attack_power_add;						//!< 最大攻撃力の加算値

	int								m_AnimationCount;				//!< Animationのカウント
	int								m_AnimationTimer;				//!< Animationのタイマー
	int								m_Timer;						//!< 方向タイマー
	float							m_Interval;						//!< 三秒のインターバル
	float							m_Angle;						//!< 進行方向の角度
	float							m_Speed;						//!< 最大速度
	float							m_FirstRotation;				//!< 最初のローテーション
	float							m_AnimationFrame;				//!< Animationのフレーム
	bool							m_HitFlag;						//!< 当たりフラグ
	bool							m_IntervalFlag;					//!< インターバルフラグ
	bool							m_DirectionFlag;				//!< 方向フラグ
	bool							m_NearDistanceFlag;				//!< 近づく一定の距離フラグ
	bool							m_StopDistanceFlag;				//!< 止まる距離のフラグ
	bool							m_MoveawayDistanceFlag;			//!< 離れる一定の距離フラグ
};