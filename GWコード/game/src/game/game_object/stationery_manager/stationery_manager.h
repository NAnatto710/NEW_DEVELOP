
/*!
 *  @file           stationery_manager.h
 *  @brief          文房具管理
 *  @author         Ryusei Shimizu
 *  @date           2026/11/20
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
 *  @brief  文房具と地形・文房具同士の接触情報
 *
 *  反復計算で必要な衝突力だけ同じサブステップ中に保持する。
 */
struct Contact
{
    IStationery*        A               = nullptr;          //!< 衝突している文房具A
    IStationery*        B               = nullptr;          //!< 衝突相手の文房具B 地形ならnullptr
    vivid::Vector2      Normal          = {0.0f, 0.0f};     //!< 衝突方向を表す法線ベクトル
    vivid::Vector2      Point           = {0.0f, 0.0f};     //!< 衝突した位置
    float               Depth           = 0.0f;             //!< 物体同士がめり込んでいる深さ
    float               NormalImpulse   = 0.0f;             //!< 法線方向に適用した力積の累積値
    float               TangentImpulse  = 0.0f;             //!< 接線方向に適用した摩擦力積の累積値
};

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
     * 
     *  @return     詳細な衝突が得られた場合true
     */
    bool            CheckCollision(const Utility::AABB& player, Utility::CollisionResult& result, IStationery*& stationery) const;

    /*!
     *  @brief      定規をカーソルで掴んでいるか
     */
    bool            IsDragging(void) const;

private:


    /*!
     *  @brief      文房具の回転矩形を取得する
     *
     *  文房具の中心座標・サイズ・回転角から、
     *  当たり判定に使用するOBBを生成する。
     *
     *  @param[in]  item        当たり判定を取得する文房具
     *
     *  @return     文房具の回転矩形（OBB）
     */
    static Utility::OBB     GetCollider (const IStationery& item);

    /*!
     *  @brief      2つの回転矩形の接触点を求める
     *
     *  2つのOBBの重なり領域を基に、
     *  衝突応答に使用する代表接触点を計算する。
     *
     *  @param[in]  a           1つ目の回転矩形
     *  @param[in]  b           2つ目の回転矩形
     *
     *  @return     計算された接触点の座標
     */
    static vivid::Vector2   GetContactPoint (const Utility::OBB& a, const Utility::OBB& b);

    /*!
     *  @brief      文房具同士の接触情報を追加する
     *
     *  衝突判定結果から接触法線・接触点・めり込み量を取得し、
     *  接触情報をContactとして一覧に追加する。
     *
     *  @param[in]      a           接触している文房具A
     *  @param[in]      b           接触している文房具B
     *  @param[in]      box_a       文房具Aの回転矩形
     *  @param[in]      box_b       文房具Bの回転矩形
     *  @param[in]      hit         2つの物体の衝突判定結果
     *  @param[in,out]  contacts    接触情報の格納先
     */
    static void             AddContact (IStationery* a, IStationery* b, const Utility::OBB& box_a, const Utility::OBB& box_b, const Utility::CollisionResult& hit, std::vector<Contact>& contacts);

    /*!
     *  @brief      地形と文房具同士の接触情報を収集する
     *
     *  物理演算対象の文房具について、
     *  地形および他の文房具との衝突を判定する。
     *  検出した接触情報を共通の一覧に格納する。
     *
     *  @param[in]      active      物理演算対象となる文房具の一覧
     *  @param[in,out]  contacts    接触情報の格納先
     */
    void                    CollectContacts (const std::vector<IStationery*>& active, std::vector<Contact>& contacts) const;

    /*!
     *  @brief      接触による速度と角速度を文房具同士や地形との衝突によって、移動速度と回転速度を変化させる
     *
     *  @param[in]  contact     速度補正に使用する接触情報
     */
    static void             ResolveVelocity (Contact& contact);

    /*!
     *  @brief      文房具同士や地形とのめり込みを解消しながら、文房具の位置と角度を補正する
     *
     *  @param[in]  contact     位置補正に使用する接触情報
     */
    static void             ResolvePosition (Contact& contact);


    // 必要最小限の物理設定値。
    static const int   m_sub_steps;           //!< 1フレームの分割数
    static const int   m_velocity_iterations; //!< 速度の補正回数
    static const int   m_position_iterations; //!< めり込みの補正回数
    static const float m_friction;            //!< 接触面の摩擦係数
    static const float m_contact_slop;        //!< 許容めり込み(px)
    static const float m_position_rate;       //!< めり込みの補正割合
    
    CStationeryController                       m_Controller;       //!< 文房具のマウス操作

    std::vector<std::unique_ptr<IStationery>>   m_Stationeries;     //!< 文房具一覧

    // 以下コンストラクタ類
    CStationeryManager(void) = default;
    ~CStationeryManager(void) = default;
    CStationeryManager(const CStationeryManager& rhs) = delete;
    CStationeryManager& operator=(const CStationeryManager& rhs) = delete;
};
