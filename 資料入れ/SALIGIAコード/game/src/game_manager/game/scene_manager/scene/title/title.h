
/*!
 *  @file		title.h
 *  @brief		タイトル
 *  @author     Hiroto Maniwa
 *  @date       2026/04/22
 */

#pragma once

#include "vivid.h"
#include "../scene.h"

 /*!
  *	@class		CTitle
  *
  *	@brief		タイトルシーン
  *
  *	@author     Hiroto Maniwa
  *
  *  @date       2026/04/22
  */
class CTitle
    : public IScene
{
public:

    /*!
     *  @brief      コンストラクタ
     */
    CTitle(void);

    /*!
     *  @brief      デストラクタ
     */
    ~CTitle(void);

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

private:

    static const int                m_width;
    static const int                m_height;
    static const int                m_b_push_width;
    static const int                m_b_push_height;
    static const float              m_interval;
    static const float              m_flash_time;
    static const float              m_fade_speed;
    static const float              m_b_push_rotation;
    static const float              m_demo_start_time;
    static const char* m_demo_movie_path;
    static const vivid::Vector2     m_title_position;
    static const vivid::Vector2     m_b_push_position;
    static const vivid::Vector2     m_b_push_anchor;
    static const vivid::Vector2     m_b_push_scale;
    static const vivid::Rect        m_b_push_rect;

    float                           m_Timer;
    float                           m_IdleTimer;
    float                           m_RectTimer;
    bool                            m_AlphaFlg;
    bool                            m_IsMovieMode;
    bool                            m_ChangeFlg;
    unsigned int                    m_BPushColor;
    unsigned int                    m_BPushColorFlash;
    vivid::Vector2                  m_BPushPosition;
    vivid::Vector2                  m_BPushScale;
    vivid::Rect                     m_BPushRect;

};