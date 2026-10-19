
/*!
 *  @file           player.h
 *  @brief          プレイヤー
 *  @author         Ryusei Shimizu
 *  @date           2026/10/13
 */

#pragma once

#include "vivid.h"
#include "../character.h"
#include "player_input/player_input.h"
#include "../component/component_object.h"

class CPlayer
	: public ICharacter
{
public:

	/*!
	 *	@brief		コンストラクタ
	 */
	CPlayer(void) = default;

	/*!
	 *	@brief		デストラクタ
	 */
	~CPlayer(void) override = default;

	/*!
	 *	@brief		初期化
	 * 
	 *	@param[in]	width		横幅
	 *	@param[in]	height		縦幅
	 *	@param[in]	position	位置
	 */
	void			Initialize (int width, int height, const vivid::Vector2& position);

	/*!
	 *	@brief		更新
	 */
	void			Update(void) override;

	/*!
	 *	@brief		描画
	 */
	void			Draw(void) override;

	const vivid::Vector2& GetPosition (void) const { return m_CachedPosition; }

private:

	vivid::Vector2		m_CachedPosition;

	CPlayerInput		m_Input{};
	CMovementComponent	m_Movement{};
	CPhysicsComponent	m_Physics{};
};