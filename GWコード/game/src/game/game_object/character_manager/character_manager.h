
/*!
 *  @file			character_manager.h
 *  @brief			キャラクター管理
 *	@author			Ryusei Shimizu
 *  @date			2026/10/16
 */

#pragma once

#include "vivid.h"
#include <memory>

class CPlayer;

class CCharacterManager
{
public:

	/*!
	 *  @brief		インスタンス取得
	 *
	 *  @return		インスタンス
	 */
	static CCharacterManager& GetInstance (void);

	/*!
	 *  @brief		初期化
	 *
	 *  @param[in]	position	プレイヤー初期位置
	 *  @param[in]	width		プレイヤーの横幅
	 *  @param[in]	height		プレイヤーの縦幅
	 */
	void			Initialize (const vivid::Vector2& position, int width, int height);

	/*!
	 *  @brief		更新
	 */
	void			Update (void);

	/*!
	 *  @brief		描画
	 */
	void			Draw (void);

	/*!
	 *  @brief		解放
	 */
	void			Finalize (void);

	/*!
	 *  @brief		プレイヤー取得
	 *
	 *  @return		プレイヤー（未生成時はnullptr）
	 */
	CPlayer* GetPlayer (void);

	/*!
	 *  @brief		プレイヤー取得
	 *
	 *  @return		プレイヤー（未生成時はnullptr）
	 */
	const CPlayer* GetPlayer (void) const;

private:

	std::unique_ptr<CPlayer>	m_Player;	//!< プレイヤー

	// 以下コンストラクタ類
	CCharacterManager (void) = default;
	~CCharacterManager (void) = default;
	CCharacterManager (const CCharacterManager& rhs) = delete;
	CCharacterManager& operator=(const CCharacterManager& rhs) = delete;
};
