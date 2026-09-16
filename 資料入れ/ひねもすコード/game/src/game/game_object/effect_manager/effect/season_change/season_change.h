
/*!
 *  @file       season_change.h
 *  @brief      シーン変更エフェクト
 *  @author     Ryusei Shimizu
 *  @date       2026/02/19
 */

#pragma once

#include "vivid.h"
#include "../effect.h"
#include "../../../parameter_manager/season_id.h"

/*!
 *	@brief	シーズンチェンジエフェクトの左右
 */
enum class SEASON_CHANGE_SIDE
{
	LEFT,	//!< 左
	RIGHT,	//!< 右
};

/*!
 *  @class      CSeasonChange
 *
 *  @brief      シーン変更エフェクトクラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2026/02/19
 */
class CSeasonChange 
	: public IEffect
{
public:

	/*!
	 *  @brief      コンストラクタ
	 */
	CSeasonChange(void);

	/*!
	 *	@brief		デストラクタ
	 */
	~CSeasonChange(void);

	/*!
	 *  @brief      初期化
	 *
	 *  @param[in]  position  位置
	 *  @param[in]  color     色
	 *  @param[in]  rotation  回転
	 */
	void			Initialize(const vivid::Vector2& position, unsigned int color, float rotation) override;

	/*!
	 *  @brief      更新
	 */
	void			Update(void) override;

	/*!
	 *  @brief      描画
	 */
	void			Draw(void) override;

private:

	static const int			m_width;								//!< 幅
	static const int			m_height;								//!< 高さ
	static const int			m_fade_speed;							//!< フェード速度
	static const vivid::Rect    m_left_rect;							//!< 描画矩形
	static const vivid::Rect    m_right_rect;							//!< 描画矩形
	static const std::string	m_texture_path[(int)SEASON_ID::MAX];	//!< テクスチャ名

	SEASON_ID					m_Season;			//!< シーズン
	SEASON_CHANGE_SIDE          m_Side;				//!< 左右
};