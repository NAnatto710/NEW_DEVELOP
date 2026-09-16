
/*!
 *  @file       map.h
 *  @brief      マップ
 *  @author     Misaki Kawada
 *  @date       2026/02/12
 */

#pragma once

#include "vivid.h"
#include "icon/icon.h"
#include <list>

class CIcon;

/*!
 *  @class      CMap
 *
 *  @brief      マップクラス
 *
 *  @author     Misaki Kawada
 *
 *  @date       2026/02/12
 */
class CMap
{
public:

	/*!
	 *  @brief      コンストラクタ
	 */
	CMap(void);

	/*!
	 *  @brief      デストラクタ
	 */
	~CMap(void) = default;

	/*!
	 *  @brief      初期化
	 */
	void			Initialize(void);

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

	/*!
	 *  @brief      アイコン生成
	 */
	void			Create(void);

	/*!
	 *  @brief      ミニマップポジション取得
	 *
	 *  @return		ミニマップポジション
	 */
	vivid::Vector2	GetMapPosition(void);

	/*!
	 *  @brief      ミニマップサイズ取得
	 *
	 *  @return		ミニマップサイズ
	 */
	float			GetSize(void);

	/*!
	 *  @brief      ミニマップサイズ取得
	 *
	 *  @return		ミニマップサイズ
	 */
	float			GetDrawRange(void);

private:

	static const float				m_size;				//!< サイズ
	static const float				m_between;			//!< 間
	static const vivid::Vector2		m_position;         //!< 位置 
	static const std::string		m_file_path;		//!< ファイルパス
	static const float				m_drawing_range;	//!< 描画範囲

	/*!
	 *	@brief アイコンリスト
	 */
	using ICONLIST = std::list<CIcon*>;

	ICONLIST	m_IconList;								//!< アイコンリスト

};


