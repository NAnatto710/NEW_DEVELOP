
/*!
 *  @file       csv_convert.cpp
 *  @brief      CSV変換ユーティリティ
 *  @author     Ryusei Shimizu
 *  @date       2026/07/07
 */

#include "csv_convert.h"

#include <cstdlib>

/*
 *  値取得(float)
 */
template<> float
CSV::
Get<float>(const std::vector<std::string>& values, size_t index)
{
    return std::stof(values.at(index));
}

/*
 *  値取得(int)
 */
template<> int
CSV::
Get<int>( const std::vector<std::string>& values, size_t index)
{
    return std::stoi(values.at(index));
}

/*
 *  値取得(bool)
 */
template<> bool
CSV::
Get<bool>( const std::vector<std::string>& values, size_t index)
{
    return values.at(index) == "TRUE" ||
        values.at(index) == "true" ||
        values.at(index) == "1";
}

/*
 *  値取得(std::string)
 */
template<> std::string
CSV::
Get<std::string>( const std::vector<std::string>& values, size_t index)
{
    return values.at(index);
}