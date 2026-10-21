
/*!
 *  @file       csv_helper.cpp
 *  @brief      CSVヘルパー
 *  @author     Ryusei Shimizu
 *  @date       2026/07/07
 */

#include "csv_helper.h"

#include <sstream>
#include <algorithm>
#include <cctype>

/*
 *  CSVを分割
 */
std::vector<std::string>
CSV::
Split(const std::string& line, char delimiter)
{
    std::stringstream ss(line);

    std::string cell;

    std::vector<std::string> values;

	// 区切り文字で分割
    while (std::getline(ss, cell, delimiter))
    {
		// 前後の空白を削除して格納
        values.emplace_back(Trim(cell));
    }

    return values;
}

/*
 *  前後の空白を削除
 */
std::string
CSV::
Trim(const std::string& str)
{
	// 前後の空白を削除
    auto begin = std::find_if_not(str.begin(), str.end(), ::isspace);
    auto end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();

	// 空文字列の場合
    if (begin >= end)
        return "";

	// 前後の空白を削除した文字列を返す
    return std::string(begin, end);
}

/*
 *  列数チェック
 */
bool
CSV::
CheckColumnCount(const std::vector<std::string>& values, size_t count)
{
    return values.size() == count;
}