
/*!
 *  @file       csv_loader.cpp
 *  @brief      csvローダー
 *  @author     Ryusei Shimizu
 *  @date       2025/12/09
 */

#include "csv_loader.h"

/*
 *  コンストラクタ
 */
CCSVLoader::
CCSVLoader()
    : m_LoadedData()
    , m_MapChipCol(0)
    , m_MapChipRow(0)
{
}

/*
 *  ロード
 */
bool
CCSVLoader::
Load(const std::string& filepath)
{
    std::ifstream file(filepath);

    // ファイルが開けなかった場合は失敗
    if (!file.is_open())
    {
        return false;
    }

    std::string line;

    // 既存データをクリア
    m_LoadedData.clear();

    // 行数・列数を初期化
    m_MapChipCol = 0;
    m_MapChipRow = 0;

    // ファイルを1行ずつ読み込む
    while (std::getline(file, line))
    {
        std::vector<unsigned char> row;
        std::stringstream ss(line);
        std::string cell;

        int colCount = 0;

        // 行をカンマ区切りで分割
        while (std::getline(ss, cell, ','))
        {
            // セルが空でなく、'0'～'3'の範囲ならデータとして追加
            if (!cell.empty() && cell[0] >= '0' && cell[0] <= '3')
            {
                row.push_back(static_cast<unsigned char>(std::stoi(cell)));
            }
            ++colCount;
        }

        // 最初の行で列数を記録
        if (m_MapChipRow == 0)
        {
            m_MapChipRow = colCount;
        }
        // 行データを保存
        m_LoadedData.push_back(row);

        // 行数をカウント
        ++m_MapChipCol;
    }
    return true;
}

/*
 *  行数取得
 */
int
CCSVLoader::
GetMapChipCols()
{
    return m_MapChipCol;
}

/*
 *  列数取得
 */
int
CCSVLoader::
GetMapChipRows()
{
    return m_MapChipRow;
}

/*
 *  指定した座標のデータ取得
 */
std::vector<std::vector<unsigned char>>
CCSVLoader::
GetData()
{
    return m_LoadedData;
}

/*
 *  指定した座標のデータ取得
 */
int
CCSVLoader::
GetData(int x, int y)
{
    if (y >= 0 && y < static_cast<int>(m_LoadedData.size()) &&
        x >= 0 && x < static_cast<int>(m_LoadedData[y].size()))
    {
        return static_cast<int>(m_LoadedData[y][x]);
    }

    // 範囲外なら -1
    return -1;
}

/*
 *  データ数取得
 */
int
CCSVLoader::
GetDataSize()
{
    return (int)m_LoadedData.size();
}

/*
 *  データ解放
 */
void CCSVLoader::UnLoad()
{
    m_LoadedData.clear();
    m_MapChipCol = 0;
    m_MapChipRow = 0;
}