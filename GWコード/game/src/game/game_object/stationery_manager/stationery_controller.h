/*!
 *  @file           stationery_controller.h
 *  @brief          文房具のマウス操作
 *  @date           2026/11/10
 */

#pragma once

#include "vivid.h"

#include <memory>
#include <vector>

class IStationery;

/*!
 *  @brief          文房具の選択・ドラッグ・回転を担当する
 *
 *  文房具の所有権は持たない。文房具の削除前にResetを呼ぶ。
 *  既存の画面座標操作を維持し、カメラ座標変換は後で対応する。
 */
class CStationeryController
{
public:

    /*!
     *  @brief      マウス入力と文房具の操作を更新する
     */
    void            Update(const std::vector<std::unique_ptr<IStationery>>& stationeries);

    /*!
     *  @brief      操作状態だけを破棄する
     *
     *  文房具の位置・角度は変更しない。
     */
    void            Reset(void);

    /*!
     *  @brief      指定した文房具をドラッグ中か
     */
    bool            IsDragging(const IStationery* stationery) const;

    /*!
     *  @brief      いずれかの文房具をドラッグ中か
     */
    bool            IsDragging(void) const;

private:

    /*!
     *  @brief      指定位置にある操作可能な文房具を探す
     */
    IStationery*    FindStationery(const std::vector<std::unique_ptr<IStationery>>& stationeries,
                                  const vivid::Vector2& position) const;

    /*!
     *  @brief      文房具の回転矩形に点が含まれるか
     */
    bool            ContainsPoint(const IStationery& stationery,
                                  const vivid::Vector2& position) const;

    /*!
     *  @brief      操作前の位置・角度に戻す
     */
    void            CancelDrag(void);

private:

    IStationery*    m_Dragging = nullptr;      //!< 操作中の文房具（所有しない）
    vivid::Vector2  m_DragOffset{};             //!< マウスと中心座標の差
    vivid::Vector2  m_StartPosition{};         //!< ドラッグ開始前の位置
    float           m_StartRotation = 0.0f;   //!< ドラッグ開始前の角度
};
