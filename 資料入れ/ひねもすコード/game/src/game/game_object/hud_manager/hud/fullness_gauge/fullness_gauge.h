
/*!
 *  @file       fullness_gauge.h
 *  @brief      満腹度ゲージ
 *  @author     Ryusei Shimizu
 *  @date       2026/02/03
 */

#pragma once

#include "vivid.h"

/*!
 *  @class      CFullnessGauge
 *
 *  @brief      満腹度ゲージクラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2026/02/03
 */
class CFullnessGauge
{
public:

	/*!
	 *  @brief      コンストラクタ
	 */
	CFullnessGauge(void);

	/*!
	 *  @brief      デストラクタ
	 */
	~CFullnessGauge(void);

	/*!
	 *  @brief      初期化
	 */
	void		Initialize(void);

	/*!
	 *  @brief      更新
	 */
	void		Update(void);

	/*!
	 *  @brief      描画
	 */
	void		Draw(void);

	/*!
	 *  @brief      解放
	 */
	void		Finalize(void);

private:

	static const int				m_width;				//!< 幅 
	static const int				m_height;				//!< 高さ 
	static const vivid::Vector2     m_default_position;		//!< デフォルトの位置
	static const vivid::Vector2     m_offset;				//!< オフセット
	static const vivid::Rect		m_frame_rect;			//!< 枠の読み込み範囲 
	static const vivid::Rect		m_bar_rect;				//!< バーの読み込み範囲 
	static const std::string        m_fullness_head_path;	//!< 満腹度ゲージの頭の画像のパス
	static const std::string        m_fullness_gauge_path;	//!< 満腹度ゲージの画像のパス
	static const std::string        m_fullness_body_path;	//!< 満腹度ゲージの体の画像のパス

	vivid::Vector2                  m_Position[12];			//!< 位置
	vivid::Rect						m_Rect[10];             //!< バーの読み込み範囲 
	vivid::Vector2                  m_Scale[12];			//!< 拡大率
	float							m_Timer;				//!< 経過時間
};