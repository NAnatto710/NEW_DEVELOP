/*!
 *  @file       attack_manager.h
 *  @brief      攻撃管理クラス
 *  @author     Misaki Kawada
 *  @date       2026/2/2
 */

#pragma once

#include "vivid.h"
#include "attack_movement/attack_movement_id.h"
#include "../character_manager/character_id.h"
#include "attack_movement/rush/rush.h"
#include <list>

 /*!
  *  @class      CAttackManager
  *
  *  @brief      攻撃管理クラス
  *
  *  @author     Misaki Kawada
  *
  *  @date       2026/2/2
  */
class CAttackManager
{
public:

	/*!
	 *  @brief      インスタンスの取得
	 *
	 *  @return     インスタンス
	 */
	static CAttackManager& GetInstance(void);

	/*!
	 *  @brief      初期化
	 */
	void				Initialize(void);

	/*!
	 *  @brief      解放
	 */
	void				Finalize(void);

	/*
	 *	@breif		突進
	 *
	 *	@parm[in]	velocity	速度
	 *  @parm[in]	speed		ステータスの速さ
	 *	@parm[in]	direction	角度
	 *  @parm[in]	val			数値
	 */
	vivid::Vector2		Rush(vivid::Vector2 velocity, float speed, float direction, float val);

private:

	/*!
	 *  @brief      コンストラクタ
	 */
	CAttackManager(void);

	/*!
	 *  @brief      コピーコンストラクタ
	 *
	 *  @param[in]  rhs     オブジェクト
	 */
	CAttackManager(const CAttackManager& rhs);

	/*!
	 *  @brief      ムーブコンストラクタ
	 *
	 *  @param[in]  rhs     オブジェクト
	 */
	CAttackManager(CAttackManager&& rhs);

	/*!
	 *  @brief      デストラクタ
	 */
	~CAttackManager(void);

	/*!
	 *  @brief      代入演算子
	 *
	 *  @param[in]  rhs 代入オブジェクト
	 *
	 *  @return     自身のオブジェクト
	 */
	CAttackManager& operator=(const CAttackManager& rhs);

	CRush			m_Rush;      //!< 突進
};