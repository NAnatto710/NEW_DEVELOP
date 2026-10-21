
/*!
 *  @file		scene.h
 *  @brief		シーン基底
 *  @author     Ryusei Shimizu
 *  @date       2026/04/10
 */

#pragma once

#include "vivid.h"
#include <string>

/*!
 *	@class		IScene
 *
 *	@brief		シーン基底クラス
 *
 *	@author     Ryusei Shimizu
 *
 *  @date       2026/04/10
 */
class IScene
{
public:

	/*!
	 *	@brief		コンストラクタ
	 */
	IScene(std::string SceneName);

	/*!
	 *	@brief		デストラクタ
	 */
	virtual			~IScene(void);

	/*!
	 *	@brief		初期化
	 */
	virtual void	Initialize(void);

	/*!
	 *	@brief		更新
	 */
	virtual void	Update(void);

	/*!
	 *	@brief		描画
	 */
	virtual void	Draw(void);

	/*!
	 *	@brief		解放
	 */
	virtual void	Finalize(void);

private:

	static const int			m_text_size;		//!< デバックモードで表示されるテキストの文字サイズ
	static const unsigned int	m_text_color;		//!< デバックモードで表示されるテキストの文字の色

	vivid::Vector2				m_Position;			//!< デバックモードで表示されるテキストの表示位置
	std::string					m_SceneName;		//!< デバックモードで表示されるテキスト
};