
/*!
 *  @file			character_manager.cpp
 *  @brief			キャラクター管理
 *	@author			Ryusei Shimizu
 *  @date			2026/10/16
 */

#include "character_manager.h"
#include "character/player/player.h"

#include <memory>

/*
 *  インスタンスの取得
 */
CCharacterManager&
CCharacterManager::
GetInstance (void)
{
	static CCharacterManager instance;

	return instance;
}

/*
 *  初期化
 */
void
CCharacterManager::
Initialize (const vivid::Vector2& position, int width, int height)
{
	this->Finalize ();

	m_Player = std::make_unique<CPlayer> ();
	m_Player->Initialize (width, height, position);
}

/*
 *  更新
 */
void
CCharacterManager::
Update (void)
{
	if(m_Player)
		m_Player->Update ();
}

/*
 *  描画
 */
void
CCharacterManager::
Draw (void)
{
	if(m_Player)
		m_Player->Draw ();
}

/*
 *  解放
 */
void
CCharacterManager::
Finalize (void)
{
	m_Player.reset ();
}

/*
 *  プレイヤー取得
 */
CPlayer*
CCharacterManager::
GetPlayer (void)
{
	return m_Player.get ();
}

/*
 *  プレイヤー取得
 */
const CPlayer*
CCharacterManager::
GetPlayer (void) const
{
	return m_Player.get ();
}
