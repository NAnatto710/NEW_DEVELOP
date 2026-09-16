
/*!
 *  @file		game_main.h
 *  @brief		ゲームメイン
 *  @author     Ryusei Shimizu
 *  @date       2026/04/10
 */

#pragma once

#include "vivid.h"
#include "../scene.h"
#include "game_state_id.h"

/*!
 *	@class		CGameMain
 *
 *	@brief		ゲームメインシーン
 *
 *	@author     Ryusei Shimizu
 *
 *  @date       2026/04/10
 */
class CGameMain
    : public IScene
{
public:

    /*!
     *  @brief      コンストラクタ
     */
    CGameMain(void);

    /*!
     *  @brief      デストラクタ
     */
    ~CGameMain(void);

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

    static const float m_no_operation_time;     // 無操作状態でリザルト移る時間
    static const int    m_text_width;
    static const int    m_text_height;
    static const int    m_center_line_width;
    static const int    m_center_line_height;
    static const vivid::Vector2 m_text_position;
    static const vivid::Vector2 m_text_anchor;
    static const vivid::Vector2 m_text_scale;
    static const vivid::Vector2 m_center_line_anchor;
    static const float  m_fade_speed;
    static const int    m_load_speed;


    float m_NoOperationTimer;
    bool  m_IsNoOperationFinish;

    void UpdateNoOperation(void);

    float m_Timer;                          // タイマー
    int m_text_count;                       // テキストの切り替えカウント
    unsigned int                m_TextColor;
    unsigned int                m_CenterLineColor;
    vivid::Rect                 m_TextRect;
    vivid::Rect                 m_CenterLineRect;

    GAME_STATE_ID m_GameState;              // ゲームの状態
    int           m_JoinCount;              // 参加人数

    int           m_Count;
};