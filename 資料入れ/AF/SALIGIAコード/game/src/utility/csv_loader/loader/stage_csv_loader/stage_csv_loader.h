
/*!
 *  @file       stage_csv_loader.h
 *  @brief      ステージCSVローダー
 *  @author     Ryusei Shimizu
 *  @date       2026/07/07
 */

#pragma once

#include "../../csv_loader.h"

#include <vector>

/*!
 *	@class		CStageCSVLoader
 *
 *	@brief		ステージCSVローダー
 *
 *	@author     Ryusei Shimizu
 *
 *  @date       2026/07/07
 */
class CStageCSVLoader :
    public CCSVLoaderBase
{
public:

    /*!
     *  @brief コンストラクタ
	 */
    CStageCSVLoader();

    /*!
     *  @brief デストラクタ
	 */
	~CStageCSVLoader() = default;

    /*!
     *  @brief CSVをロード
     *
     *  @param[in] filepath CSVパス
     *
     *  @return 読み込み成功
	 */
    bool Load(const std::string& filepath);

    /*!
	 *  @brief CSVをアンロード
     */
    void UnLoad();

    /*!
     *  @brief データ取得
     *
     *  @param[in] x X座標
     *  @param[in] y Y座標
     *
	 *  @return データ
     */
    unsigned char GetData(int x, int y) const;

    /*!
     *  @brief データ取得
     * 
	 *  @return データ
	 */
    const std::vector<std::vector<unsigned char>>& GetData() const;

    /*!
	 *  @brief マップチップの行数取得
     */
    int GetMapChipRows() const;

	/*!
	 *  @brief マップチップの列数取得
     */
    int GetMapChipCols() const;

private:

    /*!
     *  @brief 1行ロード
     *
     *  @param[in] line CSVの1行
     *
	 *  @return 読み込み成功
     */
    bool LoadLine(const std::string& line);

    /*!
     *  @brief 1行読み込み
     *
     *  @param[in] values CSVの1行の値
	 */
    void ReadRow(const std::vector<std::string>& values);

	int m_MapChipRows = 0;      //!< マップチップの行数
	int m_MapChipCols = 0;      //!< マップチップの列数

	std::vector<std::vector<unsigned char>> m_MapData;  //!< マップデータ 
};