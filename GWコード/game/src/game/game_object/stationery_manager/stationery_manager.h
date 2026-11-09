/*!
 *  @file           stationery_manager.h
 *  @brief          文房具管理
 *  @date           2026/11/05
 */

#pragma once

#include "vivid.h"

#include <memory>
#include <vector>

class IStationery;
class CRuler;

/*!
 *  @brief          文房具の生成・更新・描画・解放を管理する
 *
 *  生成した文房具の所有権は本クラスが持つ。
 *  外部へ返すポインタはFinalize後には使用できない。
 */
class CStationeryManager
{
public:

    /*!
     *  @brief      インスタンス取得
     *
     *  @return     インスタンス
     */
    static CStationeryManager& GetInstance(void);

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
     *  @brief      定規生成
     *
     *  @param[in]  position    定規の中心座標
     *  @return     生成した定規（管理クラスが所有）
     */
    CRuler*         CreateRuler(const vivid::Vector2& position);

    /*!
     *  @brief      文房具の個数取得
     *
     *  @return     管理中の文房具数
     */
    std::size_t     GetCount(void) const;

    /*!
     *  @brief      定規をカーソルで掴んでいるか
     */
    bool            IsDragging(void) const;

private:

    /*!
     *  @brief      マウスによる文房具操作
     */
    void            UpdateMouse(void);

    /*!
     *  @brief      指定座標にある文房具を調べる
     *
     *  @param[in]  position    判定位置（画面座標・暫定）
     *  @return     操作可能な文房具（見つからなければnullptr）
     */
    IStationery*    FindStationery(const vivid::Vector2& position) const;

    /*!
     *  @brief      文房具の回転矩形に点が含まれるか
     */
    bool            ContainsPoint(const IStationery& stationery,
                                 const vivid::Vector2& position) const;

    /*!
     *  @brief      マウス操作を中止して操作前の値へ戻す
     */
    void            CancelDrag(void);

private:

    IStationery*            m_Dragging = nullptr;     //!< 操作中の文房具（所有しない）
    vivid::Vector2         m_DragOffset;             //!< 掴んだ位置と中心の差
    vivid::Vector2         m_StartPosition;          //!< 操作開始前の位置
    float                  m_StartRotation = 0.0f;   //!< 操作開始前の回転角

    std::vector<std::unique_ptr<IStationery>> m_Stationeries; //!< 文房具一覧

    // 以下コンストラクタ類
                    CStationeryManager(void) = default;
                    ~CStationeryManager(void);
                    CStationeryManager(const CStationeryManager& rhs) = delete;
    CStationeryManager& operator=(const CStationeryManager& rhs) = delete;
};
