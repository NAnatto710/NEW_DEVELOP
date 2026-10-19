
/*!
 *  @file			stage_manager.h
 *  @brief			ステージ管理
 *  @author			Ryusei Shimizu
 *  @date			2026/10/13
 */

#pragma once

#include "vivid.h"
#include "stage_object/stage_object_id.h"
#include <string>
#include <vector>

class IStageObject;

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
	// x, yはマスの番号ではなくピクセル座標。範囲外・空マスならnullptr。
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
	// 現状は専用の足場IDが存在しないため、falseを返す暫定実装。
	bool                IsHitScaffolding(int x, int y) const;

	/*!
	 *  @brief			ステージオブジェクトの生成
	 *
	 *  @param[in]		stage_object_id     ステージオブジェクトID
	 * 
	 *  @return			生成されたステージオブジェクト
	 */
	IStageObject*		CreateStageObject(STAGE_OBJECT_ID stage_object_id);

private:

	int                         m_map_chip_count_width = 0;         //!< マップの横幅に何ブロック存在するか
	int                         m_map_chip_count_height = 0;        //!< マップの縦幅に何ブロック存在するか

	static const std::string    m_map_file_name;                //!< マップファイル名

	int 						m_BlockSize = 0;                    //!< ブロックの大きさ

	// [行][列] でIDを取り出す。値はSTAGE_OBJECT_IDに対応する。
	std::vector<std::vector<unsigned char>>   m_MapObject;      //!< マップオブジェクトデータ
	// [行][列] に生成済みオブジェクトを保持。所有権はCStageManagerが持つ。
	std::vector<std::vector<IStageObject*>>   m_StageObject;    //!< ステージオブジェクトデータ

	// 以下コンストラクタ類
	CStageManager() = default;
	~CStageManager() = default;
	CStageManager(const CStageManager& rhs) = delete;
	CStageManager& operator=(const CStageManager& rhs) = delete;
};