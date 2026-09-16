
/*!
 *  @file       camera_manager.h
 *  @brief      カメラ管理
 *  @author     Ryusei Shimizu
 *  @date       2025/10/27
 */

#pragma once

#include "vivid.h"
#include "../character_manager/character_id.h"
#include "../character_manager/character/player/player.h"

class CPlayer;

/*!
 *  @class      CCameraManager
 *
 *  @brief      カメラ管理クラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2025/10/27
 */
class CCameraManager
{
public:

    /*!
      *  @brief      インスタンスの取得
      *
      *  @return     インスタンス
      */
    static CCameraManager& GetInstance(void);

    /*!
     *  @brief      初期化
     */
    void                Initialize(void);

    /*!
     *  @brief      更新
     */
    void                Update(void);

    /*!
     *  @brief      解放
     */
    void                Finalize(void);

    /*!
     *  @brief      カメラ位置の取得
     *
     *  @return     カメラの位置
     */
    vivid::Vector2      GetPosition(void)const;

    /*!
     *  @brief      スクロール可能か判断
     */
    void                MoveCheck(void);

    /*!
     *  @brief      プレイヤーのセット
     *
     *  @param[in]  player  プレイヤーオブジェクト
     */
    void                SetPlayer(CPlayer* player);

private:

    /*!
     *  @brief      コンストラクタ
     */
    CCameraManager(void);

    /*!
     *  @brief      コピーコンストラクタ
     *
     *  @param[in]  rhs     オブジェクト
     */
    CCameraManager(const CCameraManager& rhs);

    /*!
     *  @brief      ムーブコンストラクタ
     *
     *  @param[in]  rhs     オブジェクト
     */
    CCameraManager(CCameraManager&& rhs);

    /*!
     *  @brief      デストラクタ
     */
    ~CCameraManager(void);

    /*!
     *  @brief      代入演算子
     *
     *  @param[in]  rhs 代入オブジェクト
     *
     *  @return     自身のオブジェクト
     */
    CCameraManager& operator=(const CCameraManager& rhs);

    /*
     *  @brief  動作
     */
    void			Move(void);

    static const int                m_camera_up_limit;                  //!< カメラの上移動限界値
    static const int                m_camera_down_limit;                //!< カメラの下移動限界値
    static const int                m_camera_left_limit;                //!< カメラの左移動限界値
    static const int                m_camera_right_limit;               //!< カメラの右移動限界値

    vivid::Vector2                  m_Position;                         //!< 位置
    vivid::Vector2                  m_CameraStartPosition;              //!< 初期位置

    vivid::Vector2                  m_MovePosition;                     //!< カメラ移動用位置
    vivid::Vector2                  m_Velocity;                         //!< 加速
    bool							m_MoveFlg;						    //!< 移動したかどうかの判定
    bool                            m_MoveCheck;                        //!< スクロールの可否判断
    bool                            m_CheckHit[4];                      //!< 画面端までカメラがいっているか判断フラグ

    CPlayer*                        m_Player;                           //!< プレイヤーのポインタ
};