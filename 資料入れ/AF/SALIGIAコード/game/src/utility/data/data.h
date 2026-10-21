
/*!
 *  @file       data.h
 *  @brief      セーブデータ読み書き
 *  @author     Ryusei Shimizu
 *  @date       2026/03/20
 */

#pragma once
#include <fstream>
#include <string>

/*!
 * @brief  セーブデータ構造体
 */
struct  SAVE_DATA
{
	int Score;	//!< ハイスコア
};

/*!
 *  @class      CData
 *
 *  @brief      セーブデータ読み書き
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2026/03/20
 */
class CData
{
public:

	/*!
	 *	@brief		コンストラクタ
	 */
	CData();

	/*!
	 *	@brief		デストラクタ
	 */
	~CData() = default;

	/*!
	 *	@brief		セーブ
	 */
	void Save() const;

	/*!
	 *	@brief		ロード
	 */
	void Load();

	/*!
	 *	@brief		リセット
	 */
	void Reset();

	/*!
	 *	@brief		セーブデータ取得
	 *
	 *	@return		セーブデータ
	 */
	SAVE_DATA* GetSaveData();

	/*!
	 *	@brief		セーブデータ取得
	 *
	 *	@return		セーブデータ
	 */
	const SAVE_DATA* GetSaveData() const;

private:

	static const std::string	m_save_filepath;	//!< セーブデータのファイルパス

	SAVE_DATA					m_SaveData;			//!< セーブデータ
};