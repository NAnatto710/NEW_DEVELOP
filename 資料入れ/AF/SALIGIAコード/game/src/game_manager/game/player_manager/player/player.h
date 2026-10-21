
/*!
 *  @file		player.h
 *  @brief		プレイヤー
 *  @author     Ryusei Shimizu
 *  @date       2026/04/13
 */

#pragma once

#include "vivid.h"
#include "../player_id.h"
#include "arm/arm.h"

#include "action/attack_component/attack_component.h"
#include "build_component/build_component.h"
#include "action/guard_component/guard_component.h"
#include "action/throw_component/throw_component.h"
#include "physics_component/physics_component.h"
#include "player_input/player_input.h"
#include "state_controller/state_controller.h"
#include "status/status_object.h"

/*!
 *	@brief		プレイヤーデータ
 */
struct PlayerData
{
	int								Width;				//!< 幅
	int								Height;				//!< 高さ
	vivid::Rect						Rect;				//!< 読み込み範囲
	vivid::Vector2					Anchor;				//!< 基準点
	vivid::Vector2					Scale;				//!< 拡大率
	unsigned int					Color;				//!< 色
	float							Rotation;			//!< 回転値
	bool 							Direction;			//!< 向き
	PLAYER_ID						PlayerID;			//!< プレイヤー識別子
	CATEGORY_ID						CategoryID;			//!< キャラクター識別子
	vivid::controller::DEVICE_ID	DeviceID;			//!< デバイス識別子
};

/*!
 *	@brief		パスデータ
 */
struct PathData
{
	std::string						HeadPath;			//!< 頭部のテクスチャパス
	std::string						BodyPath;			//!< 体のテクスチャパス
	std::string						HeadLightPath;		//!< 頭部のライトテクスチャパス
	std::string						BodyLightPath;		//!< 体のライトテクスチャパス
	std::string						ShadowPath;			//!< 頭部の影テクスチャパス
	std::string						ResourceCSVPath;	//!< リソースCSVファイルパス
	std::string						AttributeCSVPath;	//!< 属性CSVファイルパス
	std::string						AttackCSVPath;		//!< 攻撃CSVファイルパス
	std::string						PassiveCSVPath;		//!< パッシブCSVファイルパス
};

/*!
 *	@class		CPlayer
 *
 *	@brief		プレイヤークラス
 *
 *	@author     Ryusei Shimizu
 *
 *  @date       2026/04/13
 */
class CPlayer
{
public:
	/*!
	 *	@brief	コンストラクタ
	 *
	 *	@param[in]	width			幅
	 *	@param[in]	height			高さ
	 *	@param[in]	category		キャラクター識別子
	 */
	CPlayer(CATEGORY_ID category, int width = m_default_width, int height = m_default_height);

	/*!
	 *	@brief				デストラクタ
	 */
	~CPlayer(void);

	/*
	 *	@brief				初期化
	 *
	 *	@param[in]			player_id		プレイヤー識別子
	 *	@param[in]			device_id		デバイス識別子
	 *	@param[in]			build			ビルドデータ
	 *	@param[in]			position		位置
	 */
	void					Initialize(const PLAYER_ID player_id, vivid::controller::DEVICE_ID device_id, const BuildData& build, const vivid::Vector2& position);

	/*!
	 *  @brief				更新
	 */
	void					Update(void);

	/*!
	 *  @brief				描画
	 */
	void					DrawLeftArm(void);
	void					DrawBody(void);
	void					DrawRightArm(void);

	/*!
	 *  @brief				解放
	 */
	void					Finalize(void);

	/*!
	 *  @brief				キャラクターデータ取得
	 *
	 *  @return				プレイヤーデータ
	 */
	const PlayerData&		GetPlayerData(void) const { return m_PlayerData; };

	/*!
	 *  @brief				攻撃コンポーネント取得
	 *
	 *  @return				攻撃コンポーネント
	 */
	CAttackComponent&		GetAttackComponent(void) { return m_AttackComponent; };

	/*!
	 *  @brief				物理演算コンポーネント取得
	 *
	 *  @return				物理演算コンポーネント
	 */
	const CAttackComponent& GetAttackComponent(void) const { return m_AttackComponent; };

	/*!
	 *  @brief				ビルドコンポーネント取得
	 *
	 *  @return				ビルドコンポーネント
	 */
	CBuildComponent&		GetBuildComponent(void) { return m_BuildComponent; };

	/*!
	 *  @brief				ビルドコンポーネント取得
	 *
	 *  @return				ビルドコンポーネント
	 */
	const CBuildComponent&	GetBuildComponent(void) const { return m_BuildComponent; };

	/*!
	 *  @brief				ガードコンポーネント取得
	 *
	 *  @return				ガードコンポーネント
	 */
	CGuardComponent&		GetGuardComponent(void) { return m_GuardComponent; };

	/*!
	 *  @brief				ガードコンポーネント取得
	 *
	 *  @return				ガードコンポーネント
	 */
	const CGuardComponent&	GetGuardComponent(void) const { return m_GuardComponent; };

	/*!
	 *  @brief				投げコンポーネント取得
	 *
	 *  @return				投げコンポーネント
	 */
	CThrowComponent&		GetThrowComponent(void) { return m_ThrowComponent; };

	/*!
	 *  @brief				投げコンポーネント取得
	 *
	 *  @return				投げコンポーネント
	 */
	const CThrowComponent&	GetThrowComponent(void) const { return m_ThrowComponent; };

	/*!
	 *  @brief				物理演算コンポーネント取得
	 *
	 *  @return				物理演算コンポーネント
	 */
	CPhysicsComponent&		GetPhysicsComponent(void) { return m_PhysicsComponent; };

	/*!
	 *  @brief				物理演算コンポーネント取得
	 *
	 *  @return				物理演算コンポーネント
	 */
	const CPhysicsComponent& GetPhysicsComponent(void) const { return m_PhysicsComponent; };

	/*!
	 *  @brief				状態管理クラス取得
	 *
	 *  @return				状態管理クラス
	 */
	CStateController&       GetStateController(void) { return m_StateController; };

	/*!
	 *  @brief				状態管理クラス取得
	 *
	 *  @return				状態管理クラス
	 */
	const CStateController& GetStateController(void) const { return m_StateController; };

	/*!
	 *  @brief				リソースコンポーネント取得
	 *
	 *  @return				リソースコンポーネント
	 */
	CResourceComponent&		GetResourceComponent(void) { return m_ResourceComponent; };

	/*!
	 *  @brief				リソースコンポーネント取得
	 *
	 *  @return				リソースコンポーネント
	 */
	const CResourceComponent& GetResourceComponent(void) const { return m_ResourceComponent; };	
	
	/*!
	 *  @brief				属性コンポーネント取得
	 *
	 *  @return				属性コンポーネント
	 */
	CAttributeComponent&	GetAttributeComponent(void) { return m_AttributeComponent; };

	/*!
	 *  @brief				属性コンポーネント取得
	 *
	 *  @return				属性コンポーネント
	 */
	const CAttributeComponent& GetAttributeComponent(void) const { return m_AttributeComponent; };

	/*!
	 *  @brief				リスポーン待機中かどうか
	 *
	 *  @return				リスポーン待機中であればtrue、そうでなければfalse
	 */
	bool					IsRespawnDelay(void) const { return m_RespawnTime > 0; };

	/*!
	 *  @brief				アクティブフラグ取得
	 *
	 *  @return				アクティブフラグ
	 */
	bool					IsActive(void) const { return m_ActiveFlg; };

	/*!
	 *	@brief				プレイヤーが操作状態にあるかどうか
	 * 
	 *	@return				操作状態
	 */
	bool					IsOperating(void) const;

	/*!
	 *  @brief				アクティブフラグ設定
	 *
	 *  @param[in]			active  アクティブフラグ
	 */
	void					SetActive(bool active) { m_ActiveFlg = active; };

	/*!
	 *  @brief				投げられる状態かどうか
	 *
	 *  @return				投げられる状態であればtrue
	 */
	bool					CanBeThrown(void) const;

	/*!
	 *  @brief				ダメージ
	 *
	 *  @param[in]			damage_info ダメージ情報
	 * 
	 *	@return				ダメージを受けたかどうか
	 */
	bool					Damage(const DamageInfo& damage_info);

	/*!
	 *  @brief				硬直
	 *
	 *  @param[in]			time    時間
	 */
	void					Stiffness(const int time);

	/*!
	 *  @brief				ヒットストップの設定
	 *
	 *  @param[in]			time    時間
	 */
	void                    SetHitStop(const int time);

	/*!
	 *  @brief				無敵
	 *
	 *  @param[in]			time    時間
	 */
	void					Invincible(const int time);

	/*!
	 *	@brief				移動
	 * 
	 *	@param[in]			dir		移動方向（正の値で右、負の値で左）
	 */
	void					Move(vivid::Vector2 dir);

	/*!
	 *  @brief				攻撃
	 */
	void					Attack();

protected:

	/*
	 *  @brief				生存
	 */
	void					Alive(void);

	/*
	 *  @brief				死亡
	 */
	void					Dead(void);

	/*
	 *  @brief				無敵更新
	 */
	void					InvincibleUpdate(void);

	/*
	 *  @brief				状態更新
	 */
	void					StateUpdate(void);

	/*
	 *  @brief				生存状態更新
	 */
	void					CharacterAliveStateUpdate(void);

	/*
	 *  @brief				操作
	 */
	void					Control(void);

	/*
	 *  @brief				動作
	 */
	void					Action(void);

	/*
	 *  @brief				攻撃
	 */
	void					AttackUpdate(void);

	/*
	 *  @brief				投げ更新
	 */
	void					ThrowUpdate(void);

	/*
	 *  @brief				投げられ更新
	 */
	void					Thrown(void);

	/*
	 *  @brief				ガード
	 */
	void					Guard(void);

	/*
	 *  @brief				硬直
	 */
	void					Stiffness(void);

	/*
	 *  @brief				リスポーン
	 */
	void					Respawn(void);

	/*
	 *  @brief				操作可能かどうか
	 * 
	 *	@return				操作可能であればtrue、そうでなければfalse
	 */
	bool					CanControl(void);

	/*
	 *  @brief				攻撃可能かどうか
	 *
	 *	@return				攻撃可能であればtrue、そうでなければfalse
	 */
	bool					CanAttack(void);

	/*
	 *  @brief				ガード可能かどうか
	 *
	 *	@return				ガード可能であればtrue、そうでなければfalse
	 */
	bool					CanGuard(void);

	/*
	 *  @brief				投げ可能かどうか
	 *
	 *	@return				投げ可能であればtrue、そうでなければfalse
	 */
	bool					CanThrow(void);

	/*
	 *  @brief				現在の攻撃ID取得
	 *
	 *  @return				現在の攻撃ID
	 */
	ATTACK_ID				GetCurrentAttackID(void) const;

	/*
	 *  @brief				描画可能かどうか
	 *
	 *  @return				描画可能であればtrue、そうでなければfalse
	 */
	bool					IsDrawVisible(void) const;


	static const float		m_default_width;				//!< デフォルトの横幅
	static const float		m_default_height;				//!< デフォルトの高さ
	static const int		m_max_jump_count;				//!< 空中での最大ジャンプ回数
	static const int		m_max_invincible_time;			//!< 無敵時間
	static const int		m_invincible_visible_interval;	//!< 無敵時間中の点滅間隔
	static const int	    m_plummet_input_time;			//!< 急落下入力受付時間
	static const int		m_respawn_delay;				//!< リスポーンまでの待機時間
	static const int		m_land_stiffness_time;			//!< 着地時の硬直時間
	static const PathData	m_pathdata;						//!< パスデータ

	PlayerData				m_PlayerData;					//!< プレイヤーデータ
	int						m_JumpCount;					//!< ジャンプ回数
	int						m_InvincibleTime;				//!< 無敵時間
	int						m_StiffnessTime;				//!< 硬直時間
	int						m_HitStopTime;					//!< ヒットストップ時間
	int						m_RespawnTime;					//!< リスポーン待機時間
	int                     m_PlummetInputTimer;			//!< 急落下入力タイマー
	float					m_IdleTimer;					//!< 待機モーション用タイマー
	float                   m_LandingSquash;				//!< 着地時の潰れ具合
	bool                    m_PlummetTimerFlg;				//!< 急落下タイマーフラグ
	bool                    m_PlummetFlg;					//!< 急落下フラグ
	bool					m_MoveFlg;						//!< 移動したかどうかの判定
	bool					m_ActiveFlg;					//!< アクティブフラグ
	bool					m_InvincibleFlg;				//!< 無敵フラグ
	bool 					m_JumpFlg;						//!< ジャンプフラグ
	bool                    m_InputJumpFlg;					//!< ジャンプ入力フラグ
	bool                    m_InputPlummetFlg;				//!< 急落下入力フラグ

	CAttackComponent		m_AttackComponent;				//!< 攻撃コンポーネント
	CBuildComponent			m_BuildComponent;				//!< ビルドコンポーネント
	CGuardComponent			m_GuardComponent;				//!< ガードコンポーネント
	CThrowComponent			m_ThrowComponent;				//!< 投げコンポーネント
	CPhysicsComponent		m_PhysicsComponent;				//!< 物理演算コンポーネント
	CPlayerInput			m_PlayerInput;					//!< プレイヤー入力
	CStateController		m_StateController;				//!< 状態管理コントローラー
	CResourceComponent		m_ResourceComponent;			//!< リソースコンポーネント
	CAttributeComponent		m_AttributeComponent;			//!< 属性コンポーネント
};