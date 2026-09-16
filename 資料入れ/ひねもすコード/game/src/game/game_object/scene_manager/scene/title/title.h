
/*!
 *  @file       title.h
 *  @brief      タイトルシーン
 *  @author     Ryusei Shimizu
 *  @date       2025/10/08
 */

#pragma once

#include "vivid.h"
#include "../scene.h"

 /*!
   *  @class      CTitle
   *
   *  @brief      タイトルシーンクラス
   *
   *  @author     Ryusei Shimizu
   *
   *  @date       2025/10/08
   */
class CTitle
    :public IScene
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
    void        Initialize(void);

    /*!
     *  @brief      更新
     */
    void        Update(void);

    /*!
     *  @brief      描画
     */
    void        Draw(void);

    /*!
     *  @brief      解放
     */
    void        Finalize(void);

private:

	static const int                m_title_logo_width;     //!< タイトルロゴの幅
	static const int                m_title_logo_height;	//!< タイトルロゴの高さ
	static const vivid::Vector2     m_title_logo_pos;	    //!< タイトルロゴの位置
	static const std::string	    m_title_logo_path;      //!< タイトルロゴのパス
    static const std::string        m_title_background_path;    //!< タイトル背景のパス
	static const std::string        m_start_button_path;    //!< スタートボタンのパス
    static const float              m_scene_change_wait_time;   //!< シーン切り替え待機時間

    float                           m_WaitTime;                 //!< 待機時間
	bool                            m_ChangeSceneFlg;       //!< シーン切り替えフラグ
};