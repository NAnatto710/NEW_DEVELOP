
/*!
 *  @file           stationery.h
 *  @brief          文房具共通基底
 *  @author         Ryusei Shimizu
 *  @date           2026/11/05
 */

#pragma once

#include "vivid.h"

/*!
 *  @brief          文房具の種類
 */
enum class STATIONERY_TYPE
{
    RULER,          //!< 定規
    SET_SQUARE,     //!< 三角定規
    PENCIL,         //!< 鉛筆
    ERASER          //!< 消しゴム
};

/*!
 *  @brief          文房具の配置・使用状態
 */
enum class STATIONERY_STATE
{
    HELD,           //!< 購入後、配置確定前
    PLACED,         //!< 配置済み、プレイヤー未接触
    USED            //!< プレイヤー接触済み
};

class IStationery
{
public:

    /*!
     *  @brief      コンストラクタ
     *
     *  @param[in]  type        文房具の種類
     */
    explicit        IStationery(STATIONERY_TYPE type);

    /*!
     *  @brief      デストラクタ
     */
    virtual         ~IStationery(void) = default;

    /*!
     *  @brief      初期化
     *
     *  @param[in]  position    中心座標
     *  @param[in]  size        幅・高さ
     */
    virtual void    Initialize(const vivid::Vector2& position, const vivid::Vector2& size);

    /*!
     *  @brief      更新
     */
    virtual void    Update(void) = 0;

    /*!
     *  @brief      描画
     */
    virtual void    Draw(void) = 0;

    /*!
     *  @brief      配置確定
     *
     *  @return     配置を確定できた場合true
     */
    bool            Place(void);

    /*!
     *  @brief      プレイヤーに接触した文房具を使用済みにする
     *
     *  @return     使用済みに変更できた場合true
     */
    bool            MarkUsed(void);

    /*!
     *  @brief      プレイヤー操作による位置変更
     *
     *  @param[in]  position    中心座標
     * 
     *  @return     変更できた場合true
     */
    bool            SetPosition(const vivid::Vector2& position);

    /*!
     *  @brief      プレイヤー操作による回転変更
     *
     *  @param[in]  rotation    回転角（ラジアン）
     * 
     *  @return     変更できた場合true
     */
    bool            SetRotation(float rotation);

    /*!
     *  @brief      プレイヤーが操作可能か
     * 
     *  @return     可能かどうか
     */
    bool            CanManipulate(void) const;

    STATIONERY_TYPE             GetType(void) const { return m_Type; }
    STATIONERY_STATE            GetState(void) const { return m_State; }
    const vivid::Vector2&       GetPosition(void) const { return m_Position; }
    const vivid::Vector2&       GetSize(void) const { return m_Size; }
    float                       GetRotation(void) const { return m_Rotation; }

protected:

    /*!
     *  @brief      物理演算から位置を更新する
     */
    void            SetPhysicsPosition(const vivid::Vector2& position);

    /*!
     *  @brief      物理演算から回転を更新する
     */
    void            SetPhysicsRotation(float rotation);

    const STATIONERY_TYPE   m_Type;         //!< 文房具の種類
    STATIONERY_STATE        m_State;        //!< 配置・使用状態
    vivid::Vector2          m_Position;     //!< 中心座標
    vivid::Vector2          m_Size;         //!< 幅・高さ
    float                   m_Rotation;     //!< 回転角（ラジアン）
};
