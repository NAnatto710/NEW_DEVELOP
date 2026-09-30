
/*!
 *  @file			scene.h
 *  @brief			シーン基底
 *  @author			Ryusei Shimizu
 *  @date			2026/09/30
 */

#pragma once

#include "vivid.h"
#include <string>

class IScene
{
public:

	/*!
	 *	@brief		コンストラクタ
	 * 
	 *	@param[in]	scene_id
	 */
	IScene(std::string scene_id = "null");

	/*!
	 *	@brief		デストラクタ
	 */
	virtual			~IScene(void) = default;

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

	static const int			m_scene_text_size;		//!< デバックモードで表示されるテキストの文字サイズ
	static const unsigned int	m_scene_text_color;		//!< デバックモードで表示されるテキストの文字の色

protected:

	std::string					m_SceneText;			//!< デバックモードで表示されるテキスト
	vivid::Vector2				m_SceneTextPosition;	//!< デバックモードで表示されるテキストの表示位置
};