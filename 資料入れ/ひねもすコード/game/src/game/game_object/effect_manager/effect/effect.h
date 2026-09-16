
/*!
 *  @file       effect.h
 *  @brief      エフェクトベースクラス
 *  @author     Ryusei Shimizu
 *  @date       2026/02/19
 */

#pragma once 

#include "vivid.h"
#include "../effect_id.h" 

/*!
 *  @class      IEffect
 *
 *  @brief      エフェクトベースクラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2026/02/19
 */
class IEffect
{
public:

    /*!
     *  @brief      コンストラクタ
     */
    IEffect(int width, int height, EFFECT_ID id);

    /*!
     *  @brief      デストラクタ
     */
    virtual ~IEffect(void);

    /*!
	 *  @brief      初期化
	  *
	  *  @param[in]  position    位置
	  *  @param[in]  color       色
	  *  @param[in]  rotation    回転値
     */
    virtual void        Initialize(const vivid::Vector2& position, unsigned int color, float rotation);

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

	/*!
	 *  @brief      エフェクトIDの取得
	 *
	 *  @return     エフェクトID
	 */
	EFFECT_ID		    GetEffectID(void)const;

protected:

    int                 m_Width;        //!< 幅 
    int                 m_Height;       //!< ⾼さ 
    vivid::Vector2      m_Position;     //!< 位置 
    unsigned int        m_Color;        //!< ⾊ 
    vivid::Vector2      m_Anchor;       //!< 基準点 
    vivid::Rect         m_Rect;         //!< 読み込み範囲 
    vivid::Vector2      m_Scale;        //!< 拡⼤率     
    float               m_Rotation;     //!< 回転値 
    bool                m_ActiveFlg;   //!< アクティブフラグ
	EFFECT_ID 		    m_EffectID;     //!< エフェクトID
};