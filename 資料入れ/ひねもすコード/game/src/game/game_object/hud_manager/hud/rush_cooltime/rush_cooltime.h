
/*!
 *  @file       rush_cooltime.h
 *  @brief      突進クールタイム表示
 *  @author     Ryusei Shimizu
 *  @date       2026/02/27
 */

#pragma once

#include "vivid.h"

/*!
  *  @class      CRushCoolTime
  *
  *  @brief      突進クールタイム表示クラス
  *
  *  @author     Ryusei Shimizu
  *
  *  @date       2026/02/27
  */
class CRushCoolTime
{
public:

	/*!
	 *  @brief      コンストラクタ
	 */
	CRushCoolTime(void);

	/*!
	 *  @brief      デストラクタ
	 */
	~CRushCoolTime(void);

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
	static const vivid::Rect		m_cool_time_rect;   //!< クールタイムの読み込み範囲
	static const std::string		m_texture_path;     //!< テクスチャ名

	vivid::Rect						m_Rect;				//!< 描画範囲
	float                           m_CoolTime;			//!< クールタイム
	float							m_CoolTimer;		//!< クールタイム
};