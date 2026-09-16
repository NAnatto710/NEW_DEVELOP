
/*!
 *  @file       time.h
 *  @brief      時間表示
 *  @author     Ryusei Shimizu
 *  @date       2026/02/23
 */

#pragma once

#include "vivid.h"


 /*!
   *  @class      CTime
   *
   *  @brief      時間表示クラス
   *
   *  @author     Ryusei Shimizu
   *
   *  @date       2026/02/23
   */
class CTime
{
public:

	/*!
	 *  @brief      コンストラクタ
	 */
	CTime(void);

	/*!
	 *  @brief      デストラクタ
	 */
	~CTime(void);

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
	static const int				m_height;           //!< 高さ 
	static const vivid::Vector2		m_position;         //!< 位置 
	static const vivid::Rect		m_frame_rect;       //!< 読み込み範囲
	static const vivid::Rect		m_time_rect;        //!< 時間の読み込み範囲
	static const vivid::Vector2     m_anchor;			//!< 基準点
	static const float              m_max_time;         //!< 最大時間

	float                           m_Rotation;			//!< 回転値
};
