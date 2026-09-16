
/*!
 *  @file       guide.h
 *  @brief      ガイドベースクラス
 *  @author     Ryusei Shimizu
 *  @date       2026/02/24
 */

#pragma once

#include "vivid.h"

/*!
 *  @class      IGuide
 *
 *  @brief      ガイドベースクラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2026/02/24
 */
class IGuide
{
public:

	/*!
	 *  @brief      コンストラクタ
	 * 
	 *	@param[in]  width   幅
	 *	@param[in]  height  高さ
	 *	@param[in]  path    テクスチャパス
	 */
	IGuide(int width, int height, std::string path);

	/*!
	 *  @brief      デストラクタ
	 */
	virtual ~IGuide(void);

	/*!
	 *  @brief      初期化
	  *
	  *  @param[in]  position    位置
	 */
	virtual void        Initialize(const vivid::Vector2& position);

	/*!
	 *  @brief      更新
	 */
	virtual void        Update(void);

	/*!
	 *  @brief      描画
	 */
	virtual void        Draw(void);

	/*!
	 *  @brief      解放
	 */
	virtual void        Finalize(void);

	/*!
	 *  @brief      位置の取得
	 *
	 *  @return     位置
	 */
	vivid::Vector2      GetPosition(void)const;

	/*!
	 *  @brief      位置の設定
	 *
	 *  @param[in]  position    位置
	 */
	void                SetPosition(const vivid::Vector2& position);

	/*!
	 *  @brief      アクティブフラグの取得
	 *
	 *  @return     アクティブフラグ
	 */
	bool                IsActive(void)const;

	/*!
	 *  @brief      アクティブフラグの設定
	 *
	 *  @param[in]  active  アクティブフラグ
	 */
	void                SetActive(bool active);

private:

	static const float  m_default_achive_time;		//!< デフォルトのアクティブ時間
	static const int	m_fade_speed;				//!< フェード速度

	int                 m_Width;        //!< 幅 
	int                 m_Height;       //!< ⾼さ 
	std::string			m_TextureName;  //!< テクスチャ名
	vivid::Vector2      m_Position;     //!< 位置 
	unsigned int        m_Color;        //!< ⾊ 
	vivid::Vector2      m_Anchor;       //!< 基準点 
	vivid::Rect         m_Rect;         //!< 読み込み範囲 
	vivid::Vector2      m_Scale;        //!< 拡⼤率     
	float               m_Rotation;     //!< 回転値 
	float               m_AchiveTime;   //!< アクティブ時間
	bool                m_ActiveFlg;    //!< アクティブフラグ
};