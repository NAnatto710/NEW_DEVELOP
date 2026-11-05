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

private:

    std::vector<std::unique_ptr<IStationery>> m_Stationeries; //!< 文房具一覧

    // 以下コンストラクタ類
                    CStationeryManager(void) = default;
                    ~CStationeryManager(void);
                    CStationeryManager(const CStationeryManager& rhs) = delete;
    CStationeryManager& operator=(const CStationeryManager& rhs) = delete;
};
