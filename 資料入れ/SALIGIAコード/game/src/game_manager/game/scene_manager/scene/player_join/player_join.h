
/*!
 *  @file		player_join.h
 *  @brief		プレイヤー参加
 *  @author     Hiroto Maniwa
 *  @date       2026/06/30
 */

#pragma once

#include "vivid.h"
#include "../scene.h"
#include "../../../player_manager/player_id.h"
#include <vector>


 /*!
  *	@class		CPlayerJoin
  *
  *	@brief		プレイヤー参加クラス
  *
  *	@author     Hiroto Maniwa
  *
  *  @date      2026/06/30
  */

class CPlayerJoin
    : public IScene
{
public:

    /*!
     *  @brief      コンストラクタ
     */
    CPlayerJoin(void);

    /*!
     *  @brief      デストラクタ
     */
    ~CPlayerJoin(void) = default;

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
     *  @brief      参加
     */
    void            Join(void);

    void            Ready(void);

private:

    bool            IsAllReady(void);

    static const int m_text_size;                       // テキストのサイズ
    static const int m_max_count;
    static const int m_width;         //  
    static const int m_height;        //
    static const int m_frame_size;
    static const int m_player_display_width;
    static const int m_player_display_height;
    static const float m_change_speed;
    static const float m_frame_distance;
    static const float m_max_scale;
    static const float m_min_scale;
    static const float m_enlarge_speed;
    static const float m_interval;
    static const std::string m_text[4];                 // テキスト
    static const vivid::Vector2  m_text_position[4];     // テキストの位置
    static const vivid::Vector2  m_position;
    static const vivid::Vector2  m_anchor;
    static const vivid::Vector2  m_scale;
    static const vivid::Vector2  m_frame_position;
    static const vivid::Vector2  m_player_display_position;
    static const vivid::Rect     m_player_display_rect[4];
    static const int m_ready_width;
    static const int m_ready_height;
    

    int             m_JoinCount;           // 参加人数
    int             m_JoinController[4];   // 
    bool            m_JoinFlag[4];      // 参加しているフラグ
    bool            m_ReadyFlag[4];
    unsigned int    m_JoinColor[4];     // 色
    std::string     m_JoinText[4];      // テキスト
    vivid::Rect     m_Rect;             //
    vivid::Rect     m_ReadyRect[4];
    vivid::Vector2  m_Scale;
    float           m_Timer;
};
