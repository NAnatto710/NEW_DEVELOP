
/*!
 *  @file       result.h
 *  @brief      リザルトシーン
 *  @author     Ryusei Shimizu
 *  @date       2025/10/08
 */

#pragma once

#include "vivid.h"
#include "../scene.h"

/*!
  *  @class      CResult
  *
  *  @brief      リザルトシーンクラス
  *
  *  @author     Ryusei Shimizu
  *
  *  @date       2025/10/08
  */
class CResult
    :public IScene
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

	static const float              m_scene_change_wait_time;      //!< シーン切り替え待機時間
    static const std::string        m_result_logo_path;            //!< リザルト背景のパス
    static const std::string        m_result_background_path;      //!< リザルト背景のパス
    
	std::string			  m_ResultPath;                             //!< リザルトのパス

	float		          m_WaitTime;                               //!< 待機時間
};