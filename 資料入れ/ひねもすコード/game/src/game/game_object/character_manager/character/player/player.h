
/*!
 *  @file       player.h
 *  @brief      プレイヤークラス
 *  @author     Ryusei Shimizu
 *  @date       2025/10/15
 */

#pragma once

#include "vivid.h"
#include "../character.h"
#include "../../parts/parts.h"	
#include <list>

/*!
  *  @class      CPlayer
  *
  *  @brief      プレイヤークラス
  *
  *  @author     Ryusei Shimizu
  *
  *  @date       2025/10/15
  */
class CPlayer
	:public ICharacter
{
public:

	/*!
	 *  @brief      コンストラクタ
	 */
	CPlayer(void);

	/*!
	 *  @brief      デストラクタ
	 */
	~CPlayer(void);

	/*!
	 *  @brief      初期化
	 *
	 *  @param[in]	position	位置
	 */
	void			Initialize(const vivid::Vector2& position);

	/*!
	 *  @brief      更新
	 */
	void			Update(void);

	/*!
	 *  @brief      描画
	 */
	void			Draw(void);

	/*!
	 *  @brief      解放
	 */
	void			Finalize(void);

	/*
	 *  @brief		自キャラの操作フラグ
	 *
	 *	@return		RushFlag
	 */
	bool			IsControlFlg(void)const;

	/*
	 *	@brief		満腹度上昇
	 */
	void            IncreaseFullness(void);

	/*!
	 *  @brief      体の数を設定
	 *
	 *  @param[in]  quantity        体の数
	 */
	void            SetTargetBodyQuantity(int quantity) { m_TargetBodyQuantity = quantity; }

	/*!
	 *	@brief		弾との当たり判定
	 *
	 *	@param[in]	bullet	弾
	 *
	 *	@return		当たったかどうか
	 */
	bool            CheckHitBullet(IBullet* bullet);

	/*
	 *  @brief		敵との当たり判定
	 *
	 *	@param[in]	enemy	敵キャラクター
	 */
	bool            Hit(ICharacter* enemy);

	/*
	 *  @brief  ステータスの取得
	 *
	 *  @param[in]  id              ステータスID
	 *
	 *  @return     ステータス値
	 */
	float           GetPlayerUpgradeStatus(UPGRADE_STATUS_ID id)const;

	/*
	 *  @brief  無敵更新
	 */
	void            Invincible(void);

	/*
	 *  @brief  強化ステータス更新
	 */
	void			UpgradeStatusUpdate(void);

private:

	/*
	 *  @brief  生存
	 */
	void			Alive(void) override;

	/*
	 *  @brief  ブロックとの当たり判定
	 */
	void			CheckHitBlock(void)override;

	/*
	 *	@brief	攻撃
	 */
	void			Attack(void)override;

	/*
	 *  @brief  発射
	 */
	void            Fire(void)override;

	/*!
	 *  @brief  操作
	 */
	void			Control(void);

	/*
	 *  @brief  満腹度減少更新
	 */
	void            FullnessDecreaseUpdate(void);

	/*
	 *  @brief  体の数更新
	 */
	void            BodyQuantityUpdate(void);

	/*
	 *	@brief		体のパーツ生成
	 */
	void			CreateBodyParts(void);

	/*
	 *	@brief		体のパーツ削除
	 */
	void			DeleteBodyParts(void);

	/*
	 *	@brief		体のパーツ停止
	 */
	void            StopBodyParts(void);


	static const float              m_status[(int)STATUS_ID::MAX][(int)STATUS_DEFINITION::MAX];		//!< ステータス配列
	static const float              m_upgrade_status[(int)UPGRADE_STATUS_ID::MAX];					//!< 強化ステータス配列

	static const int				m_size;							//!< サイズ
	static const std::string		m_head_path_name;               //!< 画像名
	static const std::string		m_body_path_name;               //!< 画像名

	static const int				m_default_body_quantity;        //!< 体の数の初期値
	static const int				m_max_body_quantity;            //!< 体の数の最大値
	static const int				m_min_body_quantity;            //!< 体の数の最小値

	static const float				m_fullness_increase;			//!< 満腹度増加量
	static const float				m_fullness_decrease;			//!< 満腹度減少量
	static const float				m_fullness_decrease_interval;	//!< 満腹度減少間隔
	static const float				m_rush_interval;				//!< 突進のチャージ間隔

	int								m_BodyQuantity;					//!< 体の数
	int                             m_TargetBodyQuantity;           //!< 目標体の数

	bool							m_ForrowMoveFlg;                //!< 追従フラグ
	bool							m_UpdirectionFlg;               //!< 上方向フラグ
	bool                            m_ControlFlg;					//!< 操作フラグ

	bool                            m_RushFlg;						//!< 突進フラグ
	bool                            m_RushChargeFlg;				//!< 突進チャージフラグ
	float                           m_RushIntervalTimer;			//!< 突進のチャージ間隔タイマー

	bool 							m_FullnessDecreaseFlg;			//!< 満腹度減少フラグ
	float							m_FullnessDecreaseTimer;		//!< 満腹度減少タイマー

	float                           m_BulletFireTimer;              //!< 弾発射タイマー

	float                           m_RushAddDurationValue;			//!< 突進の持続時間加算値

	/*!
	 *  @brief      パーツリスト型
	 */
	using BodyPartsList = std::list<CParts*>;

	BodyPartsList					m_BodyPartsList;				//!< 体のパーツリスト
};