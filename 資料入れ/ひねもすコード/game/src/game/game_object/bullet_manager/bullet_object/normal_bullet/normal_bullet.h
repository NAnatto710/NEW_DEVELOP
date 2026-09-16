
/*!
 *  @file       nomal_bullet.h
 *  @brief      通常弾クラス
 *  @author     Ryusei Shimizu
 *  @date       2025/12/22
 */

#pragma once

#include "vivid.h"
#include "../bullet.h"

/*!
  *  @class      CNomalBullet
  *
  *  @brief      通常弾クラス
  *
  *  @author     Ryusei Shimizu
  *
  *  @date       2025/12/22
  */
class CNomalBullet
	: public IBullet
{
public:

	/*!
	 *  @brief      コンストラクタ
	 */
	CNomalBullet(void);

	/*!
	 *  @brief      デストラクタ
	 */
	~CNomalBullet(void);

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

	/*!
	 *  @brief      更新
	 */
	void 			Update(void) override;

private:

	/*!
	 *  @brief      アニメーション
	 */
	void            Animation(void);

	static const std::string	m_texture_name;					//!< テクスチャパス
	static const int			m_width;						//!< 幅
	static const int			m_height;						//!< 高さ
	static const int			m_animation;					//!< Animationの区切り

	int							m_AnimationTimer;				//!< Animationのタイマー
	int							m_AnimationFrame;				//!< Animationのフレーム
};