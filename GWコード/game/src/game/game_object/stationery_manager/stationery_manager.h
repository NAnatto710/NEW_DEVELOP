/*!
 *  @file  stationery_manager.h
 *  @brief  文房具管理
 *  @date  2026/11/05
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
 *  @brief  文房具の生成・更新・描画・解放を管理する
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
     *  @brief  文房具と地形・文房具同士の接触情報
     *
     *  法線はAからBへ向かう。地形の場合はBをnullptrとする。
     *  反復計算で必要な衝突力だけ同じサブステップ中に保持する。
     */
    struct SContact
    {
        IStationery*    A = nullptr;
        IStationery*    B = nullptr;
        vivid::Vector2 Normal;
        vivid::Vector2 Point;
        float           Depth = 0.0f;
        float           NormalImpulse = 0.0f;
        float           TangentImpulse = 0.0f;
    };

    // 必要最小限の物理設定値。
    static const int   m_sub_steps;           //!< 1フレームの分割数
    static const int   m_velocity_iterations; //!< 速度の補正回数
    static const int   m_position_iterations; //!< めり込みの補正回数
    static const float m_friction;            //!< 接触面の摩擦係数
    static const float m_contact_slop;        //!< 許容めり込み(px)
    static const float m_position_rate;       //!< めり込みの補正割合

    /*! @brief  物体の回転矩形を取得する */
    static Utility::OBB GetCollider(const IStationery& item);

    /*! @brief  2つの矩形の重なった領域から接触点を求める */
    static vivid::Vector2 GetContactPoint(
        const Utility::OBB& a, const Utility::OBB& b);

    /*! @brief  接触があれば情報を追加する */
    static void AddContact(
        IStationery* a, IStationery* b, const Utility::OBB& box_a,
        const Utility::OBB& box_b, const Utility::CollisionResult& hit,
        std::vector<SContact>& contacts);

    /*! @brief  地形と文房具同士の接触を同じ一覧に集める */
    void CollectContacts(const std::vector<IStationery*>& active,
                         std::vector<SContact>& contacts) const;

    /*! @brief  接触点の速度を補正し、摩擦と回転を計算する */
    static void ResolveVelocity(SContact& contact);

    /*! @brief  衝突によるめり込みを少しずつ解消する */
    static void ResolvePosition(SContact& contact);

    CStationeryController   m_Controller;             //!< 文房具のマウス操作

    std::vector<std::unique_ptr<IStationery>> m_Stationeries; //!< 文房具一覧

    // 以下コンストラクタ類
                    CStationeryManager(void) = default;
                    ~CStationeryManager(void);
                    CStationeryManager(const CStationeryManager& rhs) = delete;
    CStationeryManager& operator=(const CStationeryManager& rhs) = delete;
};
