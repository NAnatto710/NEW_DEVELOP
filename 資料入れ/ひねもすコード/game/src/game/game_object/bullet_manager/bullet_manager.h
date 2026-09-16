
/*!
 *  @file       bullet_manager.h
 *  @brief      弾丸管理
 *  @author     Ryusei Shimizu
 *  @date       2025/12/18
 */

#pragma once

#include "vivid.h"
#include <list>
#include "bullet_object/bullet_object.h"
#include "bullet_object/bullet.h"
#include "bullet_id.h"
#include "../character_manager/character_id.h"

/*!
 *  @class      CBulletManager
 *
 *  @brief      弾丸管理クラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2025/12/18
 */
class CBulletManager
{
public:

	/*!
	 *  @brief      インスタンスの取得
	 *
	 *  @return     インスタンス
	 */
	static CBulletManager& GetInstance(void);

	/*!
	 *  @brief      初期化
	 */
	void        Initialize(void);

	/*!
	 *  @brief      更新
	 */
	void        Update(void);

	/*!
	 *  @brief      描画
	 */
	void        Draw(void);

	/*!
	 *  @brief      解放
	 */
	void        Finalize(void);

	/*!
	 *  @brief      弾生成
	 *
	 *	@param[in]	category		ユニット識別子
	 *	@param[in]	id				弾ID
	 *	@param[in]	pos				位置
	 *	@param[in]	dir				向き
	 *	@param[in]	damage			ダメージ
	 *	@param[in]	speed			スピード
	 *	@param[in]	duration		持続時間
	 */
	void        Create(CHARACTER_CATEGORY category, BULLET_ID id, const vivid::Vector2& pos, float dir, float damage, float speed, float duration);

private:

	/*!
	 *  @brief      コンストラクタ
	 */
	CBulletManager(void);

	/*!
	 *  @brief      コピーコンストラクタ
	 *
	 *  @param[in]  rhs     オブジェクト
	 */
	CBulletManager(const CBulletManager& rhs);

	/*!
	 *  @brief      ムーブコンストラクタ
	 *
	 *  @param[in]  rhs     オブジェクト
	 */
	CBulletManager(CBulletManager&& rhs);

	/*!
	 *  @brief      デストラクタ
	 */
	~CBulletManager(void);

	/*!
	 *  @brief      代入演算子
	 *
	 *  @param[in]  rhs 代入オブジェクト
	 *
	 *  @return     自身のオブジェクト
	 */
	CBulletManager& operator=(const CBulletManager& rhs);

	/*!
	 *  @brief      弾リスト型
	 */
	using BULLET_LIST = std::list<IBullet*>;

	BULLET_LIST     m_BulletList;   //!< 弾リスト
};