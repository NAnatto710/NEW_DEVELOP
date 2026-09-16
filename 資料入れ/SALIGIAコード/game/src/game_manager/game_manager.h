
/*!
 *  @file		game_manager.h
 *  @brief		ゲーム管理
 *  @author     Ryusei Shimizu
 *  @date       2026/03/20
 */

#pragma once

#include "vivid.h"

/*!
 *	@class		CGameManager
 *
 *	@brief		ゲーム管理クラス
 *
 *	@author     Ryusei Shimizu
 *
 *  @date       2026/03/20
 */
class CGameManager
{
public:

	/*!
	 * @brief			   インスタンス取得
	 *
	 * @return			   インスタンス
	 */
	static CGameManager& GetInstance();

	/*!
	 * @brief			   初期化
	 */
	void Initialize();

	/*!
	 * @brief			   更新
	 */
	void Update();

	/*!
	 * @brief			   描画
	 */
	void Draw();

	/*!
	 * @brief			   解放
	 */
	void Finalize();

private:

	// 以下コンストラクタ類
	CGameManager() = default;
	~CGameManager() = default;
	CGameManager(const CGameManager& rhs) = delete;
	CGameManager& operator=(const CGameManager& rhs) = delete;
};