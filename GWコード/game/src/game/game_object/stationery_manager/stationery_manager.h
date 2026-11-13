/*!
 *  @file           stationery_manager.h
 *  @brief          文房具管理
 *  @date           2026/11/05
 */

#pragma once

#include "vivid.h"
#include "stationery_controller.h"
#include "../../../utility/collision/collision.h"

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
     *  @brief      配置済み文房具との衝突を調べる
     *
     *  @param[in]  player      プレイヤーのAABB（左上座標）
     *  @param[out] result      衝突結果
     *  @param[out] stationery 衝突した文房具（所有権は移動しない）
     *  @return     詳細な衝突が得られた場合true
     */
    bool            CheckCollision(const Utility::AABB& player,
                                   Utility::CollisionResult& result,
                                   IStationery*& stationery) const;


    /*!
     *  @brief      定規をカーソルで掴んでいるか
     */
    bool            IsDragging(void) const;

private:

    /*!
     *  @brief      文房具同士の衝突を解決する
     *
     *  地形衝突後に実行する。ドラッグ中の文房具は対象外。
     */
    void            ResolveStationeryPairs(void);

    CStationeryController   m_Controller;             //!< 文房具のマウス操作

    std::vector<std::unique_ptr<IStationery>> m_Stationeries; //!< 文房具一覧

    // 以下コンストラクタ類
                    CStationeryManager(void) = default;
                    ~CStationeryManager(void);
                    CStationeryManager(const CStationeryManager& rhs) = delete;
    CStationeryManager& operator=(const CStationeryManager& rhs) = delete;
};
