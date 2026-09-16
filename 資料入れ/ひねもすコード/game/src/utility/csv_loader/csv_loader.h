
/*!
 *  @file       csv_loader.h
 *  @brief      csvローダー
 *  @author     Ryusei Shimizu
 *  @date       2025/12/09
 */

#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

/*!
 *  @class      CCSVLoader
 *
 *  @brief      csvローダークラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2025/12/09
 */
class CCSVLoader
{
public:

    /*!
     * @brief               コンストラクタ
     */
    CCSVLoader();

    /*!
     * @brief               デストラクタ
     */
    ~CCSVLoader() = default;

    /*!
     * @brief               ロード
     *
     * @param[in] filepath  ファイルパス
     *
     * @return              読み込みフラ
     */
    bool Load(const std::string& filepath);

    /*!
     * @brief               行数取得
     *
     * @return              行数
     */
    int GetMapChipCols();

    /*!
     * @brief               列数取得
     *
     * @return              列数
     */
    int GetMapChipRows();

    /*!
     * @brief               指定した座標のデータ取得
     *
     * @return              データ
     */
    std::vector<std::vector<unsigned char>> GetData();


    /*!
     * @brief               指定した座標のデータ取得
     *
     * @param[in] x,y       横,縦
     *
     * @return              データ
     */
    int GetData(int x, int y);

    /*!
     * @brief               データ数取得
     *
     * @return              データ数
     */
    int GetDataSize();

    /*!
     * @brief               データ解放
     */
    void UnLoad();
private:

    std::vector<std::vector<unsigned char>> m_LoadedData; //!< 読み込んだデータ
    int                                     m_MapChipCol; //!< 行数
    int                                     m_MapChipRow; //!< 列数
};