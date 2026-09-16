
/*!
 *  @file		build_select.h
 *  @brief		欲望選択シーン
 *  @author     Hiroto Maniwa
 *  @date       2026/04/22
 */

#pragma once

#include "vivid.h"
#include "../scene.h"
#include "../../../player_manager/player_id.h"
#include <vector>
#include <list>

 /*!
  *	@class		CBuildSelect
  *
  *	@brief		欲望選択シーン
  *
  *	@author     Hiroto Maniwa
  *
  * @date       2026/04/22
  */

class CBuildSelect
    : public IScene
{
public:

    /*!
     *  @brief      コンストラクタ
     */
    CBuildSelect(void);

    /*!
     *  @brief      デストラクタ
     */
    ~CBuildSelect(void);

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
     *  @brief      キャラ選択
     */
    void            Select(void);

private:

    /*!
     *  @brief      キャラ選択フラグ
     */
    bool                        IsAllReady(void);

    static const int			m_text_size;		//!< デバックモードで表示されるテキストの文字サイズ
    static const unsigned int	m_text_color;		//!< デバックモードで表示されるテキストの文字の色
    static const int            m_icon_width;       //!< ボールの幅
    static const int            m_icon_height;      //!< ボールの高さ

    static const int            m_background_width; //!< 背景の幅
    static const int            m_background_height;//!< 背景の高さ
    static const unsigned int   m_ball_color[7];    //!< ボールの色
    static const unsigned int   m_select_frame_color[4];    //!< 枠の色
    static const unsigned int   m_default_color;
    static const float          m_select_frame_scale;       //!< 枠の大きさ 
    static const float          m_up_position;
    static const vivid::Vector2 m_icon_position;    //!< ボールの位置
    static const vivid::Vector2 m_text_position[4];             //!< テキストの位置
    static const vivid::Vector2 m_current_saligia_position[4];  //!< 選択中の感情の位置
    static const int            m_character_text_size;      //!< キャラクターのテキストの文字サイズ
    static const std::string    m_player_name[4];   //!< 表示されるプレイヤーテキスト
    static const std::string    m_character_name[7];//!< 表示されるテキスト
    static const vivid::Rect    m_background_rect;  //!< 背景の読み込み範囲
    static const int            m_max_select;       //!< 選択できる最大数
    static const int            m_width;            //!< 幅
    static const int            m_height;           //!< 高さ
    static const int            m_frame_width;
    static const int            m_frame_height;
    static const int            m_select_hand_width;
    static const int            m_select_hand_height;
    static const int            m_player_display_width;
    static const int            m_player_display_height;
    static const vivid::Rect    m_player_display_rect[4];
    static const float          m_interval;
    static const float          m_sway_speed;
    static const int            m_arm_width;
    static const int            m_arm_height;
    static const vivid::Rect    m_arm_rect[7];
    static const float          m_up_down_interval;
    static const float          m_up_down_position;
    static const int            m_player_width;
    static const int            m_player_height;
    static const vivid::Vector2 m_player_position;

    vivid::Vector2              m_IconPosition[7];  //!< アイコンの位置
    vivid::Rect                 m_IconRect[7];      //!< アイコンの描画範囲
    vivid::Rect                 m_SelectIconRect[4][2];
    vivid::Rect                 m_SelectWardRect[4][2];   //!< 読み込み範囲
    vivid::Rect                 m_SelectArmRect[4][2];
    vivid::Rect                 m_WardRect[7];
    vivid::Vector2				m_Position;			//!< デバックモードで表示されるテキストの表示位置
    std::string					m_GameModeName;		//!< デバックモードで表示されるテキスト

    vivid::Rect                 m_Rect;             //!< 画像の描画範囲
    vivid::Vector2              m_framePosition[4]; //!< 枠の位置

    int                         m_SelectSaligia[4];     //!< 選択している感情
    unsigned int                m_SelectColor[4][2];    //!< 選択中の色
    
    int                         m_SelectCount[4];       //!< 選択順
    SALIGIA_ID                  m_SaligiaId[4][2];      //!< 選択されている感情
    bool*                       m_ReadyFlag;
    bool                        m_SelectFlag[7];

    int                         m_JoinCount;        //!< 参加人数
    bool*                       m_JoinFlag;         // 参加しているフラグ

    vivid::Vector2*             m_Select_Hand_Position;
    vivid::Vector2              m_SaligiaArmPosition[7];
    bool                        m_UpDownFlag;
    bool*                       m_SwayFlag;
    
    float                       m_Timer;
    float                       m_UpDownTimer;
    
};