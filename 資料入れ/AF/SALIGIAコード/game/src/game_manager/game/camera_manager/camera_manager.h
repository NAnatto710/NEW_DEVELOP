
/*!
 *  @file		camera_manager.h
 *  @brief		カメラ管理
 *  @author     Ryusei Shimizu
 *  @date       2026/04/16
 */

#pragma once

#include "vivid.h"
#include "../player_manager/player_id.h"

class CPlayer;

 /*!
  *  @class      CCameraManager
  *
  *  @brief      カメラ管理クラス
  *
  *  @author     Ryusei Shimizu
  *
  *  @date       2025/10/27
  */
class CCameraManager
{
public:

	/*!
	 *  @brief			インスタンスの取得
	 *
	 *  @return		インスタンス
	 */
	static CCameraManager& GetInstance(void);

	/*!
	 *  @brief			初期化
	 */
	void                Initialize(void);

	/*!
	 *  @brief			更新
	 */
	void                Update(void);

	/*!
	 *  @brief			解放
	 */
	void                Finalize(void);

	/*!
	 *  @brief			カメラ位置の更新
	 */
	void                CameraPositionUpdate(void);

	/*!
	 *  @brief			カメラ拡大率の更新
	 */
	void                CameraScaleUpdate(void);

	/*!
	 *  @brief			カメラ揺れの更新
	 */
	void                PlayerShakeUpdate(void);

	/*!
	 *  @brief			カメラ揺れの更新
	 */
	void 				CameraShakeUpdate(void);

	/*!
	 *  @brief			カメラ揺れの設定
	 * 
	 *	@param[in]		player_id  プレイヤー識別子
	 *	@param[in]		time        揺れの時間
	 *	@param[in]		intensity   揺れの強さ
	 */
	void                SetPlayerShake(PLAYER_ID player_id, const int time, const vivid::Vector2 intensity);

	/*!
	 *  @brief			プレイヤーの継続微振動設定
	 *
	 *	@param[in]		player_id  プレイヤー識別子
	 *	@param[in]		flg        微振動フラグ
	 *	@param[in]		intensity  微振動の強さ
	 */
	void				SetPlayerMicroShake(PLAYER_ID player_id, const bool flg, const vivid::Vector2 intensity);

	/*!
	 *  @brief			カメラ揺れの設定
	 *
	 *  @param[in]      time        揺れの時間
	 *  @param[in]      intensity   揺れの強さ
	 *	@param[in]      direction   揺れの方向
	 */
	void 				SetCameraShake(const int time, const vivid::Vector2 intensity, const vivid::Vector2 direction);

	/*!
	 *  @brief			カメラ位置の取得
	 *
	 *  @return			カメラの位置
	 */
	vivid::Vector2      GetScroll(void)const { return m_Scroll; }

	/*!
	 *  @brief			カメラ揺れのオフセットの取得
	 * 
	 *	@param[in]		player_id  プレイヤー識別子
	 *
	 *  @return			カメラ揺れのオフセット
	 */
	vivid::Vector2		GetPlayerShakeOffset(PLAYER_ID player_id) const { return m_PlayerShakeOffset[(int)player_id]; }

	/*!
	 *  @brief			プレイヤー取得
	 * 
	 *	@param[in]		player_id  プレイヤー識別子
	 *
	 *  @return			プレイヤークラス
	 */
	CPlayer*			GetPlayer(PLAYER_ID player_id) const { return m_Player[(int)player_id]; }

	/*!
	 *  @brief			プレイヤー設定
	 *
	 *	@param[in]		player_id  プレイヤー識別子
	 *  @param[in]		player  プレイヤークラス
	 */
	void                SetPlayer(PLAYER_ID player_id, CPlayer* player);

	/*!
	 *  @brief          カメラの最小拡大率取得
	 *
	 *  @return         カメラの最小拡大率
	 */
	float               GetMinCameraScale(void) const { return m_min_camera_scale; }

	/*!
	 *  @brief          カメラの最大拡大率取得
	 *
	 *  @return         カメラの最大拡大率
	 */
	float 			    GetMaxCameraScale(void) const { return m_max_camera_scale; }

	/*!
	 *  @brief          カメラの拡大率取得
	 *
	 *  @return         カメラの拡大率
	 */
	float			    GetCameraScale(void) const { return m_CameraScale; }

	/*!
	 *  @brief          カメラ中心の取得
	 *
	 *  @return         カメラ中心
	 */
	vivid::Vector2 		GetCameraCenter(void) const { return m_CameraCenter; }

	/*!
	 *  @brief          カメラ揺れのフラグ取得
	 *
	 *  @param[in]      player_id  プレイヤー識別子
	 *
	 *  @return         カメラ揺れのフラグ
	 */
	bool 				IsPlayerShaking(PLAYER_ID player_id) const;

private:

	static const float  m_max_camera_scale;			//!< カメラの最大拡大率
	static const float  m_min_camera_scale;			//!< カメラの最小拡大率
	static const float  m_complement_rate;			//!< 補間率

	int                 m_BlockSize;			//!< ブロックのサイズ
	int                 m_StageWidth;			//!< ステージの幅
	int                 m_StageHeight;			//!< ステージの高さ
	float			    m_CameraScale;			//!< カメラの拡大率
	vivid::Vector2      m_Scroll;				//!< カメラ位置
	vivid::Vector2		m_CameraCenter;			//!< 補間後のカメラ中心
	float				m_TargetScale;			//!< 目標ズーム	
	bool				m_FirstCameraUpdate;	//!< 初回カメラ更新フラグ

	bool                m_CameraShakeFlg;		//!< カメラ揺れのフラグ
	int 				m_CameraShakeTime;		//!< カメラ揺れの時間
	int					m_CameraShakeMaxTime;	//!< カメラ揺れの最大時間
	vivid::Vector2		m_CameraShakeDirection;	//!< カメラ揺れの方向
	vivid::Vector2      m_CameraShakeIntensity;	//!< カメラ揺れの強さ
	vivid::Vector2		m_CameraShakeOffset;	//!< カメラ揺れのオフセット

	bool                m_PlayerShakeFlg[(int)PLAYER_ID::MAX];		//!< カメラ揺れフラグ
	int					m_PlayerShakeTime[(int)PLAYER_ID::MAX];		//!< カメラ揺れの時間
	int 				m_PlayerShakeMaxTime[(int)PLAYER_ID::MAX];	//!< カメラ揺れの最大時間
	vivid::Vector2      m_PlayerShakeIntensity[(int)PLAYER_ID::MAX];//!< カメラ揺れの強さ
	vivid::Vector2		m_PlayerShakeOffset[(int)PLAYER_ID::MAX];	//!< カメラ揺れのオフセット

	bool				m_PlayerMicroShakeFlg[(int)PLAYER_ID::MAX];			//!< プレイヤーの微振動フラグ
	int					m_PlayerMicroShakeFrame[(int)PLAYER_ID::MAX];		//!< プレイヤーの微振動フレーム数
	vivid::Vector2		m_PlayerMicroShakeIntensity[(int)PLAYER_ID::MAX];	//!< プレイヤーの微振動の強さ

	vivid::Vector2      m_PlayerPosition[(int)PLAYER_ID::MAX];		//!< プレイヤーの位置
	vivid::Vector2		m_MaxPlayerPosition;						//!< プレイヤーの最大位置
	vivid::Vector2		m_MinPlayerPosition;						//!< プレイヤーの最小位置

	CPlayer*			m_Player[(int)PLAYER_ID::MAX];				//!< プレイヤークラス

	// 以下コンストラクタ類
	CCameraManager() = default;
	~CCameraManager() = default;
	CCameraManager(const CCameraManager& rhs) = delete;
	CCameraManager& operator=(const CCameraManager& rhs) = delete;
};