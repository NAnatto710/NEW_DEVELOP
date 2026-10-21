
/*!
 *  @file		player_manager.h
 *  @brief		プレイヤー管理
 *  @author     Ryusei Shimizu
 *  @date       2026/04/13
 */

#pragma once

#include "player_id.h"
#include "vivid.h"
#include "player/player.h"
#include <list>

/*!
 *	@class		CPlayerManager
 *
 *	@brief		プレイヤー管理クラス
 *
 *	@author     Ryusei Shimizu
 *
 *  @date       2026/04/13
 */
class CPlayerManager
{
	public:

	/*!
	 *	@brief				インスタンス取得
	 *
	 *	@return				インスタンス
	 */
	static CPlayerManager&	GetInstance();

	/*!
	 *	@brief		初期化
	 */
	void			Initialize();

	/*!
	 *	@brief		更新
	 */
	void			Update();

	/*!
	 *	@brief		描画
	 */
	void			Draw();

	/*!
	 *	@brief		解放
	 */
	void			Finalize();

	/*!
	 *	@brief		プレイヤーの生成
	 *
	 *	@param[in]	id			プレイヤーID
	 *	@param[in]	device_id	デバイスID
	 *	@param[in]	saligia		キャラクター欲ID
	 *	@param[in]	position	生成位置
	 */
	CPlayer*		Create(PLAYER_ID id, vivid::controller::DEVICE_ID device_id, BuildData saligia, vivid::Vector2& position);

	/*!
	 *	@brief		プレイヤーの取得
	 *
	 *	@param[in]	id			プレイヤーID
	 *
	 *	@return				プレイヤー
	 */
	CPlayer*		GetPlayer(PLAYER_ID id)const;

	/*!
	 *	@brief		プレイヤーの数を取得
	 *
	 *	@return		プレイヤーの数
	 */
	int             GetPlayerCount() const { return static_cast<int>(m_PlayerList.size()); }

	/*!
	 *	@brief		プレイヤー同士の当たり判定
	 *
	 *	@param[in]	attacker	攻撃側プレイヤー
	 *	@param[in]	target		被攻撃側プレイヤー
	 */
	void            CheckPlayerHit(CPlayer& attacker, CPlayer& target);

	/*!
	 *	@brief		プレイヤーの投げ判定
	 *
	 *	@param[in]	attacker	投げ側プレイヤー
	 */
	void            CheckPlayerThrow(CPlayer& attacker);

	/*!
	 *  @brief      誰か1人でも操作しているか
	 *
	 *  @return     操作しているプレイヤーがいればtrue
	 */
	bool			IsAnyPlayerOperating(void) const;

private:

	/*!
	 *  @brief      プレイヤーリスト型
	 */
	using PLAYER_LIST = std::list<CPlayer*>;
	PLAYER_LIST			m_PlayerList;	//!< プレイヤーリスト


	// 以下コンストラクタ類
	CPlayerManager() = default;
	~CPlayerManager() = default;
	CPlayerManager(const CPlayerManager& rhs) = delete;
	CPlayerManager& operator=(const CPlayerManager& rhs) = delete;
};