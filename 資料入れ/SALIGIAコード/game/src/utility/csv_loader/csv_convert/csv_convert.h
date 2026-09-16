
/*!
 *  @file       csv_convert.h
 *  @brief      CSV変換ユーティリティ
 *  @author     Ryusei Shimizu
 *  @date       2026/07/07
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace CSV
{
    /*!
     *  @brief 値取得
     * 
	 *  @tparam T 取得する型
	 *  @param values CSVの値
	 *  @param index 取得する列番号
     * 
	 *  @return 取得した値
     */
    template<class T>
    T Get(const std::vector<std::string>& values, size_t index);

    /*!
     *  @brief enum変換
     * 
	 *  @tparam TEnum enum型
	 *  @param table 変換テーブル
	 *  @param value 変換する文字列
	 *  @param out 変換結果
     * 
	 *  @return 変換に成功したか
     */
    template<class TEnum>
    bool ParseEnum(const std::unordered_map<std::string, TEnum>& table, const std::string& value, TEnum& out);
}

/*
 *  enum変換
 */
template<class TEnum> bool
CSV::
ParseEnum(const std::unordered_map<std::string, TEnum>& table, const std::string& value, TEnum& out)
{
    std::string trimValue = Trim(value);

    const auto it = table.find(trimValue);

    if (it == table.end())
        return false;

    out = it->second;
    return true;
}

/*!
 *  @brief 値取得(float)
 */
template<> float
CSV::
Get<float>(const std::vector<std::string>& values, size_t index);

/*!
 *  @brief 値取得(int)
 */
template<> int
CSV::
Get<int>(const std::vector<std::string>& values, size_t index);

/*!
 *  @brief 値取得(bool)
 */
template<> bool
CSV::
Get<bool>(const std::vector<std::string>& values, size_t index);

/*!
 *  @brief 値取得(std::string)
 */
template<> std::string
CSV::
Get<std::string>(const std::vector<std::string>& values, size_t index);