
/*!
 *  @file		stage_manager.h
 *  @brief		ステージ管理
 *  @author     Ryusei Shimizu
 *  @date       2026/04/14
 */

#pragma once

#include "vivid.h"
#include "stage_object/stage_object_id.h"
#include "../player_manager/player_id.h"
#include "../../../utility/csv_loader/loader/stage_csv_loader/stage_csv_loader.h"

class IStageObject;

/*!
 *	@class		CStageManager
 *
 *	@brief		ステージ管理クラス
 *
 *	@author     Ryusei Shimizu
 *
 *  @date       2026/04/14
 */
class CStageManager
{
	public:

	/*!
	 * @brief			   インスタンス取得
	 *
	 * @return			   インスタンス
	 */
	static CStageManager& GetInstance();

	/*!
	 * @brief			初期化
	 */
	void				Initialize();

	/*!
	 * @brief			更新
	 */
	void				Update();

	/*!
	 * @brief			描画
	 */
	void				Draw();

	/*!
	 * @brief			解放
	 */
	void				Finalize();

	/*!
	 *  @brief			ブロックの大きさ取得
	 *
	 *  @return			ブロックサイズ
	 */
	int					GetBlockSize(void)const;

	/*!
	 *  @brief			マップの横幅に何ブロック存在するか取得
	 *
	 *  @return			横幅に何ブロック存在するか
	 */
	int					GetMapChipWidth(void)const;

	/*!
	 *  @brief			マップの縦幅に何ブロック存在するか取得
	 *
	 *  @return			縦幅に何ブロック存在するか
	 */
	int					GetMapChipHeight(void)const;

	/*!
	 *  @brief			ステージデータ取得
	 *
	 *  @param[in]		x   横
	 *  @param[in]		y   縦
	 */
	IStageObject*		GetStageObject(int x, int y) const;

	/*!
	 *  @brief			当たり判定
	 *
	 *  @param[in]		x   横
	 *  @param[in]		y   縦
	 *
	 *  @retval			true    当たっている
	 *  @retval			false   当たっていない
	 */
	bool                IsHit(int x, int y) const;

	/*!
	 *  @brief			足場の当たり判定
	 *
	 *  @param[in]		x   横
	 *  @param[in]		y   縦
	 *
	 *  @retval			true    当たっている
	 *  @retval			false   当たっていない
	 */
	bool                IsHitScaffolding(int x, int y) const;

	/*!
	 *  @brief			ステージオブジェクトの生成
	 *
	 *  @param[in]		stage_object_id     ステージオブジェクトID
	 * 
	 *  @return			生成されたステージオブジェクト
	 */
	IStageObject*		CreateStageObject(STAGE_OBJECT_ID stage_object_id);

	/*!
	 *	@brief		スポーン位置取得
	 * 
	 *	@param[in]	player_id		プレイヤーID
	 * 
	 *	@return		スポーン位置
	 */
	vivid::Vector2		GetSpawnPosition(PLAYER_ID player_id) const;

private:

	static const int            m_map_chip_count_width;         //!< マップの横幅に何ブロック存在するか
	static const int            m_map_chip_count_height;        //!< マップの縦幅に何ブロック存在するか

	static const std::string    m_map_file_name;                //!< マップファイル名

	int 						m_BlockSize;                    //!< ブロックの大きさ

	CStageCSVLoader	            m_CSVLoader;                    //!< CSVローダー

	// 背景Parallax開始時のカメラ中心
	vivid::Vector2 m_BackgroundBaseCameraCenter;

	// 背景カメラ基準位置を取得済みか
	bool m_IsBackgroundCameraInitialized = false;

	std::vector<std::vector<unsigned char>>   m_MapObject;      //!< マップオブジェクトデータ
	std::vector<std::vector<IStageObject*>>   m_StageObject;    //!< ステージオブジェクトデータ

	// 以下コンストラクタ類
	CStageManager() = default;
	~CStageManager() = default;
	CStageManager(const CStageManager& rhs) = delete;
	CStageManager& operator=(const CStageManager& rhs) = delete;
};