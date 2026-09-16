
/*!
 *  @file       stage_object.h
 *  @brief      ステージオブジェクトベースクラス
 *  @author     Ryusei Shimizu
 *  @date       2026/01/28
 */

#pragma once

#include "vivid.h"
#include "stage_object_id.h"

 /*!
   *  @class      IStageObject
   *
   *  @brief      ステージオブジェクトベースクラス
   *
   *  @author     Ryusei Shimizu
   *
   *  @date       2026/01/28
   */
class IStageObject
{
public:

    /*!
      *  @brief      コンストラクタ
      */
    IStageObject(void);

    /*!
      *  @brief      デストラクタ
      */
    virtual ~IStageObject(void) = default;

    /*!
      *  @brief      初期化
      */
    virtual void Initialize(void);

    /*!
      *  @brief      更新
      */
    virtual void Update(void);

    /*!
      *  @brief      描画
      */
    virtual void Draw(void);

    /*!
      *  @brief      解放
      */
    virtual void Finalize(void);

    /*!
     *  @brief      ステージオブジェクトID取得
     *
     *  @return     ステージオブジェクトID
     */
    STAGE_OBJECT_ID     GetStageObjectID(void) const;

    /*!
     *  @brief      ステージオブジェクトID設定
     *
     *  @param[in]  stage_object_id     ステージオブジェクトID
     */
    void                SetStageObjectID(STAGE_OBJECT_ID stage_object_id);

    /*!
     *  @brief      位置取得
     *
     *  @return     位置
     */
    vivid::Vector2      GetPosition(void) const;

    /*!
     *  @brief      位置設定
     *
     *  @param[in]  position    位置
     */
    void                SetPosition(const vivid::Vector2& position);

    /*!
     *  @brief      中心位置取得
     *
     *  @return     中心位置
     */
    vivid::Vector2	    GetCenterPosition(void) const;

    /*!
     *  @brief      コリジョンチェック
     *
     *  @param[in]  x   x座標
     *  @param[in]  y   y座標
     *
     *  @retval     true    衝突している
     *  @retval     false   衝突していない
     */
    bool                IsCheckCollision(int x, int y) const;

    /*!
     *  @brief      アクティブ状態チェック
     *
     *  @retval     true    アクティブ
     *  @retval     false   非アクティブ
     */
    bool                IsActive(void) const;

    /*!
     *  @brief      アクティブフラグ設定
     *
     *  @param[in]  active  アクティブ状態
     */
    void                SetActive(bool active);

    /*!
     *  @brief      コリジョンフラグ取得
     *
     *  @return     コリジョンフラグ
     */
    bool                IsCollision(void) const;

    /*!
     *  @brief      コリジョンフラグ設定
     *
     *  @param[in]  collision   コリジョンフラグ
     */
    void                SetCollision(bool collision);

    /*!
     *  @brief      大きさ取得
     *
     *  @return     大きさ
     */
    static int          GetSize(void);

protected:

    static const int            m_default_size;         //!< 標準サイズ
    static const std::string    m_map_chip_file_name;   //!< マップチップファイル名

    vivid::Vector2              m_Position;             //!< 位置
    vivid::Vector2              m_CenterPosition;       //!< 中心位置
    vivid::Vector2              m_ScreenPosition;       //!< 画面上での描画位置
    vivid::Rect                 m_Rect;                 //!< 読み込み範囲
    bool                        m_ActiveFlg;            //!< アクティブフラグ
    bool                        m_CollisionFlg;         //!< 衝突フラグ

    STAGE_OBJECT_ID             m_StageObjectID;        //!< ステージオブジェクトID
};
