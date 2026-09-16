
/*!
 *  @file       bee_bullet.h
 *  @brief      ハチ弾クラス
 *  @author     Ryusei Shimizu
 *  @date       2026/02/24
 */

#pragma once

#include "vivid.h"
#include "../bullet.h"

 /*!
   *  @class      CBeeBullet
   *
   *  @brief      ハチ弾クラス
   *
   *  @author     Ryusei Shimizu
   *
   *  @date       2026/02/24
   */
class CBeeBullet
	: public IBullet
{
public:

	/*!
	 *  @brief      コンストラクタ
	 */
	CBeeBullet(void);

	/*!
	 *  @brief      デストラクタ
	 */
	~CBeeBullet(void);

	/*!
	 *  @brief      初期化
	 *
	 *  @param[in]  category  ユニット識別子
	 *  @param[in]  position    位置
	 *  @param[in]  direction   向き
	 *  @param[in]  damage      ダメージ
	 *  @param[in]  speed       速さ
	 *  @param[in]  duration    持続時間
	 */
	void			Initialize(CHARACTER_CATEGORY category, const vivid::Vector2& position, float direction, float damage, float speed, float duration) override;

private:

	static const std::string	m_texture_name;					//!< テクスチャパス
	static const int			m_width;						//!< 幅
	static const int			m_height;						//!< 高さ
};