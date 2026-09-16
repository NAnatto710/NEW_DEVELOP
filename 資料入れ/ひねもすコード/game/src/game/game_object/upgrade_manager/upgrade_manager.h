
/*!
 *  @file       upgrade_manager.h
 *  @brief      アップグレードステータス管理
 *  @author     Ryusei Shimizu
 *  @date       2026/02/23
 */

#pragma once

#include "vivid.h"
#include "upgrade_status/upgrade_status.h"

 /*!
  *  @class      CUpgraeStatusManager
  *
  *  @brief      アップグレードステータス管理クラス
  *
  *  @author     Ryusei Shimizu
  *
  *  @date       2026/02/23
  */
class CUpgraeStatusManager
{
public:

	/*!
	 *  @brief      インスタンスの取得
	 *
	 *  @return     インスタンス
	 */
	static CUpgraeStatusManager&	GetInstance(void);

	/*!
	 *  @brief      初期化
	 */
	void							Initialize(void);

	/*!
	 *  @brief      更新
	 */
	void                            Update(void);

	/*!
	 *  @brief      描画
	 */
	void							Draw(void);

	/*!
	 *  @brief      解放
	 */
	void							Finalize(void);

	/*!
	 *  @brief      強化ステータス選択
	 */
	void                            SelectUpgradeStatus(void);

	/*!
	 *  @brief      強化ステータスグラフ描画
	 */
	void                            DrawUpgradeStatusGraph(bool flg);

	/*!
	 *  @brief      強化ステータスID取得
	 *
	 *  @return     強化ステータスID
	 */
	UPGRADE_STATUS_ID				GetUpgradeStatusID(UPGRADE_STATUS_ID id) const;

	/*!
	 *  @brief      強化ステータスカウント取得
	 *
	 *  @return     強化ステータスカウント
	 */
	int								GetUpgradeStatusCount(UPGRADE_STATUS_ID id) const;

	/*!
	 *  @brief      強化ステータスID設定
	 *
	 *  @param[in]  id  強化ステータスID
	 */
	void							SetUpgradeStatusID(UPGRADE_STATUS_ID id);

	/*!
	 *  @brief      強化取得
	 *
	 *	@param[in]  id  強化ステータスID
	 * 
	 *  @return     倍率
	 */
	float							GetUpgrade(UPGRADE_STATUS_ID id) const;

	/*!
	 *  @brief      強化ステータスID取得
	 *
	 *  @return     強化ステータスID
	 */
	UPGRADE_STATUS_ID				GetUpgradeStatusID() const;

	/*!
	 *  @brief      強化ステータスカウント取得
	 *
	 *  @return     強化ステータスカウント
	 */
	void                            SetFullness(float value);

	/*!
	 *  @brief      強化ステータスの名前取得
	 *
	 *  @return     強化ステータスの名前
	 */
	unsigned int                    GetUpgradeStatusNameColor(void) const;

private:

	/*!
	 *  @brief      コンストラクタ
	 */
	CUpgraeStatusManager(void);

	/*!
	 *  @brief      コピーコンストラクタ
	 *
	 *  @param[in]  rhs     オブジェクト
	 */
	CUpgraeStatusManager(const CUpgraeStatusManager& rhs);

	/*!
	 *  @brief      デストラクタ
	 */
	~CUpgraeStatusManager(void);

	/*!
	 *  @brief      代入演算子
	 *
	 *  @param[in]  rhs 代入オブジェクト
	 *
	 *  @return     自身のオブジェクト
	 */
	CUpgraeStatusManager& operator=(const CUpgraeStatusManager& rhs);

	static const std::string		    m_background_path;              //!< 背景のパス
	static const std::string		    m_select_scene_logo_path;		//!< 選択シーンロゴのパス
	static const std::string		    m_select_button_path;           //!< 選択ボタンのパス
	static const std::string		    m_select_button_bar_path;		//!< 選択ボタンのバーのパス
	static const std::string		    m_select_button_hilight_path;   //!< 選択ボタンのハイライトパス
	static const std::string            m_small_tab_path;               //!< 小タブのパス
	static const std::string            m_large_tab_path;               //!< 大タブのパス
	static const std::string            m_head_graph_path;              //!< ヘッドグラフのパス
	static const std::string            m_body_graph_path;              //!< ボディグラフのパス
	static const std::string		    m_upgrade_status_path[4];       //!< アップグレードステータスのパス

	static const int                    m_select_scene_logo_width;      //!< 選択シーンロゴの幅
	static const int                    m_select_button_width;          //!< 選択ボタンの幅
	static const int                    m_select_button_height;         //!< 選択ボタンの高さ

	static const float                  m_select_time;                  //!< 選択時間
	static const float                  m_select_cooltime;              //!< 選択クールタイム
	static const int                    m_upgrade_status_num;           //!< アップグレードステータスの数

	static const vivid::Vector2         m_select_scene_logo_pos;        //!< 選択シーンロゴの位置
	static const vivid::Vector2		    m_small_tab_pos;		        //!< 小タブの位置
	static const vivid::Vector2		    m_large_tab_pos;		        //!< 大タブの位置
	static const vivid::Vector2         m_select_button_pos[4];         //!< 選択ボタンの位置
	static const vivid::Vector2         m_garph_pos[4][5];              //!< グラフの位置
	static const UPGRADE_STATUS_ID		m_upgrade_status[4];            //!< アップグレードステータス
	static const unsigned int           m_upgrade_status_name_color[4]; //!< アップグレードステータスの名前の色
	static const unsigned int           m_select_button_bar_color;      //!< 選択ボタンの色
	static const vivid::Rect			m_select_button_bar_rect;       //!< 選択ボタンのバーの矩形

	static const unsigned int           m_background_color;				//!< 背景の色

	float 								m_SelectTimer;                  //!< 選択時間のカウンター
	float                               m_SelectCoolTimer;              //!< 選択クールタイムのカウンター
	int                                 m_Select;                       //!< 選択されたステータスID
	vivid::Rect							m_SelectButtonBarRect;          //!< 選択ボタンの矩形

	CUpgradeStatus						m_UpgradeStatus;				//!< アップグレードステータス
};