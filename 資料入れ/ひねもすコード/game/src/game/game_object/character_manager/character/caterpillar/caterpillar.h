
/*!
 *  @file       CCaterpillar.h
 *  @brief      毛虫 クラス
 *  @author     Misaki Kawada
 *  @date       2026/02/04
 */

#pragma once

#include "vivid.h"
#include "../character.h"
#include "../../../../../utility/utility.h"
#include "../../../game_object.h"

 /*!
   *  @class      CCaterpillar
   *
   *  @brief      毛虫クラス
   *
   *  @author     Misaki Kawada
   *
   *  @date       2026/02/04
   */
class CCaterpillar
	:public ICharacter
{
public:

	/*!
	 *  @brief      コンストラクタ
	 */
	CCaterpillar(void);

	/*!
	 *  @brief      デストラクタ
	 */
	~CCaterpillar(void);

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
	*	@breif		体生成
	*/
	void			Createbody(void);

	/*
	*	@breif		弾との当たり判定
	*/
	bool			CheckHitBullet(IBullet* bullet);

	/*
	*	@breif		突進した時の判定
	*/
	bool			RushCheckHit(ICharacter* player);

private:

	/*!
	 *  @brief  動作
	 */
	void			Move(void)override;


	static const float              m_status[(int)STATUS_ID::MAX][(int)STATUS_DEFINITION::MAX];				//!< ステータス配列
	static const float				m_upgrade_status[(int)UPGRADE_STATUS_ID::MAX];

	static const int				m_size;										//!< サイズ
	static const std::string		m_head_name;								//!< 頭の画像名
	static const std::string		m_body_name;								//!< 体の画像名
	static const int				m_default_body_quantity;					//!< 体の数の初期値
	static const int				m_max_body_quantity;						//!< 体の数の最大値
	static const int				m_min_body_quantity;						//!< 体の数の最小値
	static const float				m_body_distance;							//!< 体との間の距離
	static const int				m_usually_val;								//!< 通常の時の数値

	static const float				m_max_hp_add;								//!< 最大HPの加算値


	int								m_BodyQuantity;					//!< 体の数
	int                             m_TargetBodyQuantity;           //!< 目標体の数

	/*!
	 *  @brief      パーツリスト型
	 */
	using	BodyPartsList = std::list<CParts*>;

	BodyPartsList			m_BodyPartsList;
};