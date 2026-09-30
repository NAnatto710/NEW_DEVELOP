
/*!
 *  @file			result.h
 *  @brief			リザルトシーン
 *  @author			Ryusei Shimizu
 *  @date			2026/09/30
 */

#pragma once

#include "../../scene.h"

class CResult
	: public IScene
{
public:

	/*!
	 *  @brief		コンストラクタ
	 */
	CResult(void);

	/*!
	 *  @brief		デストラクタ
	 */
	~CResult(void) = default;

	/*!
	 *  @brief		初期化
	 */
	void			Initialize (void);

	/*!
	 *	@brief		更新
	 */
	void			Update (void);

	/*!
	 *	@brief		描画
	 */
	void			Draw (void);

	/*!
	 *	@brief		解放
	 */
	void			Finalize (void);

private:

};