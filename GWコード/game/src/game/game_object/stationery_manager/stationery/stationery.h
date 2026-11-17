/*!
 *  @file  stationery.h
 *  @brief  文房具共通基底
 *  @date  2026/11/05
 */

#pragma once

#include "vivid.h"

#include <array>
#include <vector>

class CStageManager;

/*!
 *  @brief  文房具の種類
 */
enum class STATIONERY_TYPE
{
    RULER,          //!< 定規
    SET_SQUARE,     //!< 三角定規
    PENCIL,         //!< 鉛筆
    ERASER          //!< 消しゴム
};

/*!
 *  @brief  文房具の配置・使用状態
 */
enum class STATIONERY_STATE
{
    HELD,           //!< 購入後、配置確定前
    PLACED,         //!< 配置済み、プレイヤー未接触
    USED            //!< プレイヤー接触済み
};

/*!
 *  @brief  文房具共通基底クラス
 *
 *  座標は中心座標、回転角はラジアンで管理する。
 *  使用済みでも、物理演算による位置・回転の更新は許可する。
 */
class IStationery
{
    friend class CStationeryManager;

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
    virtual void    Initialize(const vivid::Vector2& position,
                               const vivid::Vector2& size);

    /*!
     *  @brief      更新
     */
    virtual void    Update(void) = 0;

    /*!
     *  @brief      描画
     */
    virtual void    Draw(void) = 0;

    /*!
     *  @brief      配置済み文房具の物理更新
     *
     *  ドラッグ中はManager側で呼び出さない。
     *  落下・地形衝突に加え、簡易回転と接地摩擦を扱う。
     */
    void            UpdatePhysics(void);

    /*!
     *  @brief      物理速度の初期化
     *
     *  掴み直した際に落下速度が残らないようにする。
     */
    void            ResetPhysicsVelocity(void);

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
     *  @return     変更できた場合true
     */
    bool            SetPosition(const vivid::Vector2& position);

    /*!
     *  @brief      プレイヤー操作による回転変更
     *
     *  @param[in]  rotation    回転角（ラジアン）
     *  @return     変更できた場合true
     */
    bool            SetRotation(float rotation);

    /*!
     *  @brief      プレイヤーが操作可能か
     */
    bool            CanManipulate(void) const;

    /*!
     *  @brief  文房具の種類を取得
     */
    STATIONERY_TYPE             GetType(void) const { return m_Type; }
    /*!
     *  @brief  配置・使用状態を取得
     */
    STATIONERY_STATE            GetState(void) const { return m_State; }
    /*!
     *  @brief  中心座標を取得
     */
    const vivid::Vector2&       GetPosition(void) const { return m_Position; }
    /*!
     *  @brief  文房具の幅・高さを取得
     */
    const vivid::Vector2&       GetSize(void) const { return m_Size; }
    /*!
     *  @brief  回転角（ラジアン）を取得
     */
    float                       GetRotation(void) const { return m_Rotation; }

protected:

    /*!
     *  @brief      物理演算から位置を更新する
     *
     *  USED状態でも更新可能。プレイヤー操作には使用しないこと。
     */
    void            SetPhysicsPosition(const vivid::Vector2& position);

    /*!
     *  @brief      物理演算から回転を更新する
     *
     *  USED状態でも更新可能。プレイヤー操作には使用しないこと。
     */
    void            SetPhysicsRotation(float rotation);

private:

    // 物理挙動を調整するクラス共通定数
    static const float  m_gravity                  ; //!< 重力加速度（px / frame^2）
    static const float  m_max_fall_speed           ; //!< 最大落下速度（px / frame）
    static const float  m_static_friction          ; //!< 静止摩擦係数
    static const float  m_dynamic_friction         ; //!< 動摩擦係数
    static const float  m_linear_damping           ; //!< 並進速度の減衰率
    static const float  m_angular_damping          ; //!< 角速度の減衰率
    static const float  m_contact_slop             ; //!< 許容めり込み量（px）
    static const float  m_correction_rate          ; //!< 地形とのめり込み補正率
    static const int    m_sub_steps                ; //!< 物理更新の分割数
    static const int    m_solver_iterations        ; //!< 衝突速度の解決回数
    static const int    m_position_iterations      ; //!< 位置補正の反復回数
    static const float  m_sleep_speed_sq           ; //!< Sleep判定の速度二乗しきい値
    static const float  m_sleep_angular_speed      ; //!< Sleep判定の角速度しきい値
    static const int    m_sleep_frames             ; //!< Sleep移行までの静止フレーム数


    /*!
     *  @brief  文房具と地形との接触点情報
     */
    struct SContact
    {
        vivid::Vector2 Point;   //!< 接触座標
        vivid::Vector2 Normal;  //!< 文房具から地形へ向かう単位法線
        float Depth;            //!< めり込み量
    };

    /*! @brief  ベクトルを反時計回りに90度回転する */
    static vivid::Vector2 Perp(const vivid::Vector2& vector);

    /*! @brief  ベクトルの長さの二乗を取得する */
    static float LengthSq(const vivid::Vector2& vector);

    /*! @brief  回転矩形の4頂点を計算する */
    static std::array<vivid::Vector2, 4> GetCorners(
        vivid::Vector2 center, vivid::Vector2 size, float rotation);

    /*! @brief  回転矩形をタイルの境界で切り抜く */
    static std::vector<vivid::Vector2> ClipPolygon(
        const std::array<vivid::Vector2, 4>& corners,
        float left, float top, float right, float bottom);

    /*! @brief  文房具と地形タイルの接触点を求める */
    static std::vector<SContact> FindContacts(
        const CStageManager& stage, vivid::Vector2 position,
        vivid::Vector2 size, float rotation, int block_size);

protected:

    const STATIONERY_TYPE   m_Type;         //!< 文房具の種類
    STATIONERY_STATE        m_State;        //!< 配置・使用状態
    vivid::Vector2         m_Position;     //!< 中心座標
    vivid::Vector2         m_Size;         //!< 幅・高さ
    float                  m_Rotation;     //!< 回転角（ラジアン）
    vivid::Vector2         m_PhysicsVelocity; //!< 文房具の物理速度
    float                  m_PhysicsAngularVelocity = 0.0f; //!< 角速度（ラジアン/フレーム）
    int                    m_PhysicsStillFrames = 0; //!< 静止判定の連続フレーム数
    bool                   m_IsPhysicsSleeping = false; //!< 物理更新の休止状態
    bool                   m_IsPhysicsGrounded = false; //!< 地形への接地状態

};
