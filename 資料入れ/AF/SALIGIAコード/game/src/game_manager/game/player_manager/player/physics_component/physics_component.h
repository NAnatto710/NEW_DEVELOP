
/*!
 *  @file		physics_component.h
 *  @brief		物理演算コンポーネント
 *  @author     Ryusei Shimizu
 *  @date       2026/05/21
 */

#pragma once
#include "vivid.h"
#include "velocity.h"
#include "../../../../../utility/collision_check/collision_check.h"

class CAttributeComponent;

/*!
 *	@class		CPhysicsComponent
 *
 *	@brief		物理演算コンポーネントクラス
 *
 *	@author     Ryusei Shimizu
 *
 *  @date       2026/05/21
 */
class CPhysicsComponent
{
public:
    
    /*!
     *	@brief	    コンストラクタ
     */
    CPhysicsComponent();

    /*
     *	@brief	    デストラクタ
	 */
	~CPhysicsComponent(void) = default;

    /*!
     *	@brief	    初期化
     * 
	 *  @param[in]  width   キャラクターの幅
	 *  @param[in]  height  キャラクターの高さ
	 *  @param[in]  position  キャラクターの初期位置
	 */
	void            Initialize(int width, int height, const vivid::Vector2& position);

    /*!
     *	@brief	    更新
	 */
    void            Update(void);

    /*!
	 *	@brief	    解放
     */ 
	void            Finalize(void);

    /*!
	 *  @brief      重力加速度の適用
     */
    void            ApplyGravity(void);

    /*!
	 *  @brief      衝突チェック
     */
	void            CheckCollision(void);

    /*!
     *  @brief      速度リセット
     */
	void            ResetVelocity(VelocityID id);

    /*!
	 *  @brief      速度の減衰
     */
    void            VelocityDecay(void);

    /*!
	 *  @brief      当たり判定の更新
     */
	void            HitCapsuleUpdate(void);

    /*!
	 *  @brief      速度の加算
     * 
	 *  @param[in]  id          加算する速度の種類
     *  @param[in]  velocity    加算する速度
	 */
	void            AddVelocity(VelocityID id, vivid::Vector2 velocity);

    /*!
	 *  @brief      ジャンプ処理
     * 
     *  @param[in]  direction   ジャンプの方向（右：正、左：負）
	 *  @param[in]  power       ジャンプの強さ
     */
    void            Jump(float direction, float power);

	/*! 
     *  @brief      移動処理
     * 
	 *  @param[in]  direction               移動方向
	 *  @param[in]  attribute_component     属性コンポーネント
     */
	void            Move(vivid::Vector2 direction, const CAttributeComponent& attribute_component);

    /*!
	 *  @brief      位置の設定
     * 
	 *  @param[in]  position    設定する位置
     */
    void            SetPosition(vivid::Vector2 position) { m_Position = position; }

    /*!
	 *  @brief      X速度の設定
     *
     *  @param[in]  velocity    設定する速度
     */
	void            SetVelocityX(VelocityID id, float velocity);

    /*!
     *  @brief      Y速度の設定
     *
     *  @param[in]  velocity    設定する速度
	 */
	void 		    SetVelocityY(VelocityID id, float velocity);

    /*!
	 *  @brief      当たり判定の取得
     * 
	 *  @return     長円形の当たり判定
     */
    Capsule		    GetHitCapsule(void) const { return m_Capsule; }

    /*!
	 *  @brief      位置の取得
     * 
	 *  @return     現在位置
     */
    vivid::Vector2  GetPosition(void) const { return m_Position; }

    /*!
	 *  @brief      中心位置の取得
     * 
	 *  @return     現在の中心位置
     */
	vivid::Vector2  GetCenterPosition(void) const { return m_Position + vivid::Vector2(m_Width / 2.0f, m_Height / 2.0f); }

    /*!
     *  @brief      初期位置の取得
     *
     *  @return     初期位置
     */
	vivid::Vector2  GetInitialPosition(void) const { return m_InitialPosition; }

    /*!
	 *  @brief      速度の取得
     * 
	 *  @return     現在の速度（移動速度 + 重力加速度 + ノックバック速度）
     */
    vivid::Vector2  GetVelocity(void) const { return m_Velocity.GetFinalVelocity(); }

    /*!
	 *  @brief      地面接地判定取得
     * 
	 *  @return     地面に接地しているかどうか
     */
    bool            IsLanding(void) const { return m_LandingFlg; }

    /*!
	 *  @brief      壁接触判定取得（未実装）
     * 
	 *  @return     壁に接触しているかどうか
     */
    bool            IsWall(void) const { return m_WallFlg; }

	/*!
     *  @brief      地面接地時の硬直判定取得
     * 
	 *  @return     地面接地時の硬直中かどうか
     */
	bool            IsLandingStiffness(void) const { return m_LandingStiffnessFlg; }

    /*!
     *  @brief      地面接地時の硬直フラグの設定
     * 
     *  @param[in]  flg 地面接地時の硬直中かどうかのフラグ
	 */
	void			SetLandingStiffnessFlg(bool flg) { m_LandingStiffnessFlg = flg; }

    /*!
	 *  @brief      地面接地フラグの設定
     * 
	 *  @param[in]  flg 地面に接地しているかどうかのフラグ
     */
    void            SetLandingFlg(bool flg);

    /*!
	 *  @brief      急落下フラグの設定
     * 
	 *  @param[in]  flg 急落下しているかどうかのフラグ
     */
    void            SetPlummetFlg(bool flg) { m_PlummetFlg = flg; }

private:

    static const float      m_friction;			//!< 移動時の摩擦力
    static const float      m_gravity;			//!< 重力
    static const float		m_gravity_increase;	//!< 重力による速度の増加量
	static const float      m_max_fall_speed;	//!< 落下速度の上限

	int 				    m_Width;            //!< キャラクターの幅
	int 				    m_Height;           //!< キャラクターの高さ
    vivid::Vector2          m_Position;         //!< 位置
    vivid::Vector2          m_InitialPosition;	//!< 初期位置
    VelocityInfo            m_Velocity;         //!< 各種速度成分
	float                   m_MoveAccelerator;  //!< 移動加速度
    bool                    m_LandingFlg;       //!< 地面接地フラグ
    bool                    m_WallFlg;          //!< 壁接触フラグ（未実装：必要に応じて利用）
	bool                    m_PlummetFlg;       //!< 急落下フラグ
	bool					m_LandingStiffnessFlg; //!< 地面接地時の硬直フラグ
    float                   m_GravityPower;     //!< 重力加速度

	Capsule                 m_Capsule;          //!< 当たり判定用の長円形
};