
/*!
 *  @file       hp_gauge.h
 *  @brief      HPゲージ
 *  @author     Ryusei Shimizu
 *  @date       2026/02/03
 */

#pragma once

#include "vivid.h"

 /*!
  *  @class      CHpGauge
  *
  *  @brief      HPゲージクラス
  *
  *  @author     Ryusei Shimizu
  *
  *  @date       2026/02/03
  */
class CHpGauge
{
public:

	/*!
	 *  @brief      コンストラクタ
	 */
	CHpGauge(void);

	/*!
	 *  @brief      デストラクタ
	 */
	~CHpGauge(void);

	/*!
	 *  @brief      初期化
	 */
	void Initialize(void);

	/*!
	 *  @brief      更新
	 */
	void Update(void);

	/*!
	 *  @brief      描画
	 */
	void Draw(void);

	/*!
	 *  @brief      解放
	 */
	void Finalize(void);

private:

	static const int				m_width;            //!< 幅 
	static const int				m_bar_height;       //!< バーの高さ 
	static const int				m_frame_height;     //!< 枠の高さ 
	static const vivid::Vector2		m_bar_position;     //!< バーの位置
	static const vivid::Vector2		m_frame_position;   //!< 枠の位置
	static const vivid::Rect		m_frame_rect;       //!< 枠の読み込み範囲 
	static const vivid::Rect		m_bar_rect;         //!< バーの読み込み範囲 
	static const int				m_bar_speed;        //!< バーの移動速度 

	vivid::Rect						m_Rect;             //!< バーの読み込み範囲 
};
