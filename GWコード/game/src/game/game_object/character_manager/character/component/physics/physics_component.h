
/*!
 *  @file           physics_component.h
 *  @brief          キャラクター用物理演算コンポーネント
 *  @author         Ryusei Shimizu
 *  @date           2026/10/13
 */

#pragma once

#include "vivid.h"
#include "velocity.h"

class CPhysicsComponent
{
public:

    CPhysicsComponent (void) = default;
    ~CPhysicsComponent (void) = default;

    /*!
     *  @brief      初期化
     *
     *  @param[in]  width       キャラクターの幅
     *  @param[in]  height      キャラクターの高さ
     *  @param[in]  position    初期位置
     */
    void            Initialize (int width, int height, const vivid::Vector2& position);

    /*!
     *  @brief      更新
     */
    void            Update (void);

    /*!
     *  @brief      重力の適用
     */
    void            ApplyGravity (void);

    /*!
     *  @brief      衝突判定と位置補正
     */
    void            CheckCollision (void);

    /*!
     *  @brief      移動速度の減衰
     */
    void            VelocityDecay (void);

    /*!
     *  @brief      速度成分の加算
     * 
     *  @param[in]  velocity    加算する値
     */
    void            AddMoveVelocity (const vivid::Vector2& velocity);

    /*!
     *  @brief      移動速度の設定
     * 
     *  @param[in]  velocity    設定する値
     */
    void            SetMoveVelocity (const vivid::Vector2& velocity);

    /*!
     *  @brief      垂直速度の設定
     * 
     *  @param[in]  velocity    設定する値
     */
    void            SetVerticalVelocity (float velocity);

    /*!
     *  @brief      垂直速度のリセット
     */
    void            ResetVerticalVelocity (void);

    /*!
     *  @brief      すべての速度をリセット
     */
    void            ResetVelocity (void);

    /*!
     *  @brief      位置の設定
     * 
     *  @param[in]  position    設定する位置
     */
    void            SetPosition (const vivid::Vector2& position);

    /*!
     *  @brief      接地状態の設定
     * 
     *  @param[in]  grounded    接地しているかどうか
     */
    void            SetGrounded (bool grounded);

    vivid::Vector2  GetPosition (void) const        { return m_Position; }
    vivid::Vector2  GetCenterPosition (void) const  { return m_Position + vivid::Vector2 (m_Width / 2.0f, m_Height / 2.0f); }
    vivid::Vector2  GetInitialPosition (void) const { return m_InitialPosition; }
    vivid::Vector2  GetVelocity (void) const        { return m_Velocity.GetFinalVelocity (); }
    bool            IsGrounded (void) const         { return m_IsGrounded; }
    int             GetWidth (void) const           { return m_Width; }
    int             GetHeight (void) const          { return m_Height; }

private:

    /*!
     *  @brief      ステージとの当たり判定
     * 
     *  @param[in]  position    位置
     * 
     *  @return     判定
     */
    bool            IsBlocked (const vivid::Vector2& position) const;

    /*!
     *  @brief      文房具に重なっている場合、衝突解消を試みる
     *
     *  @param[in,out] position  補正するプレイヤー左上座標
     *  @param[in]     horizontal X軸移動中か
     *  @return       補正できた場合true（衝突がない場合もtrue）
     */
    bool            ResolveStationery(vivid::Vector2& position,
                                      bool horizontal);

    /*!
     *  @brief      配置された文房具の上に立っているか
     */
    bool            HasStationeryBelow(void) const;

    /*!
     *  @brief      接地中の定規に沿ったY移動量を取得する
     *
     *  @param[in]  horizontalMove  X方向の移動量
     *  @param[out] slopeMoveY      斜面に沿ったY方向の移動量
     *  @return     斜面接地中ならtrue
     */
    bool            GetStationerySlopeMove(float horizontalMove,
                                           float& slopeMoveY) const;

    /*!
     *  @brief      接地判定
     * 
     *  @return     判定
     */
    bool            HasGroundBelow () const;

    /*!
     *  @brief      移動衝突判定
     * 
     *  @param[in]  delta       移動量
     *  @param[in]  horizontal  縦横の区別（trueがX、falseがY）
     */
    void            MoveAxis (float delta, bool horizontal);

    static const float  m_friction;         //!< 摩擦
    static const float  m_gravity_increase; //!< 重力加算値
    static const float  m_max_gravity;      //!< 最大重力値
    static const float  m_max_fallspeed;    //!< 最大落下速度

    int                 m_Width;            //!< 幅
    int                 m_Height;           //!< 高さ
    vivid::Vector2      m_Position;         //!< 位置
    vivid::Vector2      m_InitialPosition;  //!< 初期位置
    VelocityInfo        m_Velocity{};       //!< 速度
    float               m_GravityPower;     //!< 重力値
    bool                m_IsGrounded;       //!< 接地フラグ
};
