
/*!
 *  @file       icon.h
 *  @brief      アイコン
 *  @author     Misaki Kawada
 *  @date       2026/02/12
 */

#pragma once

#include "vivid.h"
#include "../../../hud_manager.h"
#include "../../../../game_object.h"
#include "../../../../../../utility/utility.h"
#include "../../../../character_manager/character_id.h"
#include <list>


 /*!
  *  @class      CIcon
  *
  *  @brief      アイコンクラス
  *
  *  @author     Misaki Kawada
  *
  *  @date       2026/02/12
  */
class CIcon
{
public:

	/*!
	 *  @brief      コンストラクタ
	 */
	CIcon(void);

	/*!
	 *  @brief      デストラクタ
	 */
	~CIcon(void) = default;

	/*!
	 *  @brief      初期化
	 */
	void Initialize(void);

	/*!
	 *  @brief      更新
	 */
	void Update(void);

	/*!
	 *  @brief      描画
	 */
	void Draw(void);

	/*!
	 *  @brief      解放
	 */
	void Finalize(void);

private:

	/*!
	 *  @brief      プレイヤーアイコン動作
	 */
	void PlayerIconMove(void);

	/*!
	 *	@breif		敵描画
	 */
	void EnemyDraw(void);


	/*!
	 *	@breif		餌描画
	 */
	void FeedDraw(void);


	static const int				m_player_size;				//!< プレイヤーサイズ
	static const float				m_enemy_size;				//!< 敵サイズ
	static const float				m_between;					//!< 間

	static const float				m_player_size_radius;		//!< プレイヤーサイズの半径
	static const float				m_enemy_size_radius;		//!< エネミーサイズの半径

	static const std::string		m_player_file_path;			//!< プレイヤーのファイルパス
	static const std::string		m_enemy_file_path;			//!< 敵のファイルパス
	static const std::string		m_feed_file_path;			//!< 餌のファイルパス
	static const float				m_size_range;				//!< ミニマップの描画範囲

	vivid::Vector2					m_PlayerPosition;			//!< プレイヤー位置
	vivid::Vector2					m_PlayerCenterPosition;		//!< プレイヤー中心位置
	float							m_PlayerRotation;			//!< プレイヤー回転値
	vivid::Rect						m_PlayerRect;				//!< プレイヤーの読み込み範囲
	vivid::Vector2					m_PlayerAnchor;				//!< プレイヤーの基準点
	vivid::Vector2					m_PlayerScale;				//!< プレイヤーの拡大率

	vivid::Vector2					m_EnemyPosition;			//!< 敵の位置
	vivid::Vector2					m_EnemyCenterPosition;		//!< 敵中心位置
	float							m_EnemyRotation;			//!< 敵回転値
	vivid::Rect						m_EnemyRect;				//!< 敵の読み込み範囲
	vivid::Vector2					m_EnemyAnchor;				//!< 敵の基準点
	vivid::Vector2					m_EnemyScale;				//!< 敵の拡大率


	vivid::Vector2					m_FeedPosition;				//!< 餌の位置
	float							m_FeedRotation;				//!< 餌回転値
	vivid::Rect						m_FeedRect;					//!< 餌の読み込み範囲
	vivid::Vector2					m_FeedAnchor;				//!< 餌の基準点
	vivid::Vector2					m_FeedScale;				//!< 餌の拡大率





};
