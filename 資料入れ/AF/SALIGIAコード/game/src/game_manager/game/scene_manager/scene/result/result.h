
/*!
 *  @file		result.h
 *  @brief		リザルト
 *  @author     Hiroto Maniwa
 *  @date       2026/04/22
 */

#pragma once

#include "vivid.h"
#include "../scene.h"
#include <vector>

/*!
 *	@class		CResult
 *
 *	@brief		リザルトシーン
 *
 *	@author     Hiroto Maniwa
 *
 *  @date       2026/04/22
 */
class CResult
    : public IScene
{
public:

    /*!
     *  @brief      コンストラクタ
     */
    CResult(void);

    /*!
     *  @brief      デストラクタ
     */
    ~CResult(void);

    /*!
     *  @brief      初期化
     */
    void            Initialize(void);

    /*!
     *  @brief      更新
     */
    void            Update(void);

    /*!
     *  @brief      描画
     */
    void            Draw(void);

    /*!
     *  @brief      解放
     */
    void            Finalize(void);

    /*!
     *  @brief      表示
     */
    void            DisPlay(void);

private:

    /*!
     *  @brief      表示フラグ
     */
    bool            IsDisplayFlag(void);

    /*!
     *  @brief      引き分けフラグ
     */
    bool            DrawFlag(void);

    static const std::string m_ranking_text[4];
    static const std::string m_player_text[4];
    static const int    m_text_size;
    static const int    m_result_size;
    static const unsigned int m_player_text_color[4];
    static const unsigned int m_ranking_color[4];
    static const vivid::Vector2 m_text_position[4];
    static const vivid::Vector2 m_box_position[4];
    static const float          m_scroll_speed;
    static const float          m_start_x[3];
    static const float          m_interval[3];
    static const float          m_particle_interval;
    static const int            m_display_background_width;
    static const int            m_display_background_height;
    static const int            m_player_display_width;
    static const int            m_player_display_height;
    static const int            m_crown_width;
    static const int            m_crown_height;
    static const float          m_fade_speed;
    static const float          m_drop_speed;
    static const float          m_swich_change_time;
    static const unsigned int   m_default_color;
    static const float          m_enlarge_speed;
    static const float          m_rotation_speed;



    //std::vector<vivid::Vector2> m_BoxPosition;
    //vivid::Vector2      m_BoxPosition[4];
    int                         m_JoinCount;
    float                       m_SwitchSceneTimer;
    float                       m_ScrollTimer;
    float                       m_ParticleTimer;


    vivid::Rect*                m_PlayerDisplayRect;
    unsigned int                m_CrownColor;
    unsigned int*               m_RankingColor;
    unsigned int*               m_PlayerColor;
    bool                        m_DropFlag;
    bool*                       m_DisplayFlag;
    bool*                       m_RankingFlag;
    vivid::Rect*                m_DisplayBackGroundRect;
    vivid::Vector2*             m_DisplayBackGroundAnchor;
    vivid::Vector2*             m_DisplayBackGroundScale;
    float*                      m_DisplayBackGroundRotation;



    vivid::Rect                 m_IconRect[2];
    vivid::Vector2              m_IconPosition[2];

    vivid::Vector2              m_CrownPosition;
};