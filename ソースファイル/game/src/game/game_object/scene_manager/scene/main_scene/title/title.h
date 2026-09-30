
/*!
 *  @file			title.h
 *  @brief			タイトルシーン
 *  @author			Ryusei Shimizu
 *  @date			2026/09/30
 */

#pragma once

#include "../../scene.h"

class CTitle
	: public IScene
{
public:

	/*!
	 *  @brief		コンストラクタ
	 */
	CTitle(void);

	/*!
	 *  @brief		デストラクタ
	 */
	~CTitle(void) = default;

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