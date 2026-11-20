
/*!
 *  @file           stationery_controller.h
 *  @brief          文房具のマウス操作
 *  @author         Ryusei Shimizu
 *  @date           2026/11/20
 */

#pragma once

#include "vivid.h"

#include <memory>
#include <vector>

class IStationery;

class CStationeryController
{
public:

    /*!
     *  @brief      マウス入力と文房具の操作を更新
     *
     *  @param[in]  stationeries    操作対象となる文房具の配列
     */
    void            Update (const std::vector<std::unique_ptr<IStationery>>& stationeries);

    /*!
     *  @brief      文房具の操作状態の初期化
     */
    void            Reset (void);

    /*!
     *  @brief      指定した文房具をドラッグ中か判定
     *
     *  @param[in]  stationery  判定対象の文房具
     *
     *  @return     指定した文房具をドラッグ中の場合true
     */
    bool            IsDragging (const IStationery* stationery) const;

    /*!
     *  @brief      文房具をドラッグ中か判定する
     *
     *  @return     いずれかの文房具をドラッグ中の場合true
     */
    bool            IsDragging (void) const;

private:

    /*!
     *  @brief      指定位置にある操作可能な文房具を取得する
     *
     *  @param[in]  stationeries    検索対象となる文房具の配列
     *  @param[in]  position        検索位置
     *
     *  @return     見つかった文房具のポインタ
     *              見つからなかった場合nullptr
     */
    IStationery*    FindStationery (const std::vector<std::unique_ptr<IStationery>>& stationeries, const vivid::Vector2& position) const;

    /*!
     *  @brief      文房具の回転矩形に点が含まれるか判定する
     * 
     *  @param[in]  stationery  判定対象の文房具
     *  @param[in]  position    判定する点の座標
     *
     *  @return     点が回転矩形に含まれる場合true
     */
    bool            ContainsPoint (const IStationery& stationery, const vivid::Vector2& position) const;

    /*!
     *  @brief      ドラッグ操作をキャンセルする
     */
    void            CancelDrag (void);

    static const float  m_rotation_step;    //!< ホイール1目盛りあたりの回転角（5度、ラジアン）

    IStationery*        m_Dragging;         //!< 操作中の文房具（所有しない）
    vivid::Vector2      m_DragOffset;       //!< マウスと中心座標の差

    vivid::Vector2      m_StartPosition;    //!< ドラッグ開始前の位置
    float               m_StartRotation;    //!< ドラッグ開始前の角度
};
