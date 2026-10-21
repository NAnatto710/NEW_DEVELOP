/*!
 *  @file       csv_loader_base.cpp
 *  @brief      CSVローダー基底クラス
 *  @author     Ryusei Shimizu
 *  @date       2026/07/07
 */

#include "csv_loader.h"

/*
 *  コンストラクタ
 */
CCSVLoaderBase::
CCSVLoaderBase()
{
}

/*
 *  ファイルを開く
 */
bool
CCSVLoaderBase::
Open(const std::string& filepath)
{
    m_File.open(filepath);

    return m_File.is_open();
}

/*
 *  ファイルを閉じる
 */
void
CCSVLoaderBase::
Close()
{
    if (m_File.is_open())
    {
        m_File.close();
    }
}

/*
 *  1行取得
 */
bool
CCSVLoaderBase::
ReadLine(std::string& line)
{
    while (std::getline(m_File, line))
    {
        // 空行
        if (line.empty())
            continue;

        // コメント
        if (line[0] == '#')
            continue;

        return true;
    }

    return false;
}

/*
 *  ファイルが開いているか
 */
bool
CCSVLoaderBase::
IsOpen() const
{
    return m_File.is_open();
}