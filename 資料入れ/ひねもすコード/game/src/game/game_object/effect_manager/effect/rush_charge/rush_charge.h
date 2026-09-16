
/*!
 *  @file       rush_charge.h
 *  @brief      突進チャージエフェクトクラス
 *  @author     Ryusei Shimizu
 *  @date       2026/02/26
 */

#pragma once

#include "vivid.h"
#include "../effect.h"

/*!
 *  @class      CRushCharget
 *
 *  @brief      突進チャージエフェクトクラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2026/02/26
 */
class CRushCharge
    : public IEffect
{
public:
    
    /*!
     *  @brief      コンストラクタ
     */
    CRushCharge(void);

    /*!
     *  @brief      デストラクタ
     */
    ~CRushCharge(void);

    /*!
	 *  @brief      初期化
     * 
	 *  @param[in]  position    位置
	 *  @param[in]  color       色
	 *  @param[in]  rotation    回転
     */
    void Initialize(const vivid::Vector2& position, unsigned int color, float rotation);

	/*!
	 *  @brief      更新
     */
    void Update(void);

	/*!
	 *  @brief      描画
	 */
    void Draw(void);

private:

    static const int            m_width;            //!< 幅 
    static const int            m_height;           //!< ⾼さ 
    static const int            m_move_speed;       //!< エフェクトの移動速度
	static const int            m_fade_speed;       //!< フェード速度
	static const std::string    m_texture_path;     //!< テクスチャパス
	static const int            m_spawn_range;      //!< エフェクトの出現範囲

	vivid::Vector2              m_TargetPos;        //!< 目標位置
};