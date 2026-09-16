
/*!
 *  @file       bullet.h
 *  @brief      弾クラス
 *  @author     Ryusei Shimizu
 *  @date       2025/12/18
 */

#pragma once

#include "vivid.h"
#include "../bullet_id.h"
#include "../../character_manager/character_id.h"
#include "../bullet_status_id.h"

/*!
 *  @class      IBullet
 *
 *  @brief      弾ベースクラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2025/12/18
 */
class IBullet
{
public:

    /*!
     *  @brief      コンストラクタ
     *
     *  @param[in]  width   幅
     *  @param[in]  height  高さ
     */
    IBullet(std::string name, int width, int height);

    /*!
     *  @brief      デストラクタ
     */
    virtual ~IBullet(void);

    /*!
     *  @brief      初期化
     *
     *  @param[in]  category  ユニット識別子
     *  @param[in]  position    位置
     *  @param[in]  direction   向き
     *  @param[in]  damage      ダメージ
     *  @param[in]  speed       速さ
     *  @param[in]  duration    持続時間
     */
    virtual void            Initialize(CHARACTER_CATEGORY category, const vivid::Vector2& position, float direction, float damage, float speed, float duration);

    /*!
     *  @brief      更新
     */
    virtual void            Update(void);

    /*!
     *  @brief      描画
     */
    virtual void            Draw(void);

    /*!
     *  @brief      解放
     */
    virtual void            Finalize(void);

    /*!
     *  @brief      位置取得
     *
     *  @return     位置
     */
    vivid::Vector2          GetPosition(void)const;

    /*!
     *  @brief      位置設定
     *
     *  @param[in]  position    位置
     */
    void                    SetPosition(const vivid::Vector2& position);

    /*!
     *  @brief      中心位置取得
     *
     *  @return     中心位置
     */
    vivid::Vector2          GetCenterPosition(void)const;

    /*!
     *  @brief      横幅取得
     *
     *  @return     横幅
     */
    int                     GetWidth(void)const;

    /*!
     *  @brief      高さ取得
     *
     *  @return     高さ
     */
    int                     GetHeight(void)const;

    /*!
     *  @brief      半径取得
     *
     *  @return     半径
     */
    float                   GetRadius(void)const;

    /*!
     *  @brief      回転値取得
     *
     *  @return     回転値
     */
    float                   GetRotation(void)const;

    /*!
     *  @brief      アクティブフラグ取得
     *
     *  @return     アクティブフラグ
     */
    bool                    IsActive(void)const;

    /*!
     *  @brief      アクティブフラグ設定
     *
     *  @param[in]  active  アクティブフラグ
     */
    void                    SetActive(bool active);

    /*!
     *  @brief      ユニット識別子取得
     *
     *  @return     ユニット識別子
     */
    CHARACTER_CATEGORY      GetBulletCategory(void)const;

    /*!
     *  @brief      弾の色取得
     *
     *  @return     弾の色
     */
    unsigned int            GetBulletColor(void)const;

    /*!
     *  @brief      ステータス取得
     *
     *  @param[in]  status_id       ステータスID
     *
     *  @return     ステータス値
     */
    float				    GetStatus(BULLET_STATUS_ID status_id) const;

    /*!
     *  @brief      ステータス最大値取得
     *
     *  @param[in]  status_id       ステータスID
     *
     *  @return     ステータス最大値
     */
    float				    GetMaxStatus(BULLET_STATUS_ID status_id) const;

protected:

    static const unsigned int   m_player_color;     //!< プレイヤーの弾の色
    static const unsigned int   m_enemy_color;      //!< 敵の弾の色

    std::string	                m_TextureName;      //!< テクスチャパス
    int                         m_Width;            //!< 幅
    int                         m_Height;           //!< 高さ
    float                       m_Radius;           //!< 半径
    vivid::Vector2              m_CenterPosition;   //!< 中心位置
    vivid::Vector2              m_Position;         //!< 位置
    vivid::Vector2              m_Velocity;         //!< 速度
    vivid::Vector2              m_Anchor;           //!< 基準点
    vivid::Rect                 m_Rect;             //!< 読み込み範囲
    vivid::Vector2              m_Scale;            //!< 拡大率
    unsigned int                m_Color;            //!< 色
    float                       m_Rotation;         //!< 回転値
    bool                        m_ActiveFlg;        //!< アクティブフラグ

    CHARACTER_CATEGORY          m_Category;         //!< 弾識別子

    float						m_MaxStatus[(int)BULLET_STATUS_ID::MAX];	//!< ステータスの最大値
    float						m_Status[(int)BULLET_STATUS_ID::MAX];		//!< ステータスの値
};
