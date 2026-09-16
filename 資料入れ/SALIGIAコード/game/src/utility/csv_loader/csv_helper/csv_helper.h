
/*!
 *  @file       csv_helper.h
 *  @brief      CSVヘルパー
 *  @author     Ryusei Shimizu
 *  @date       2026/07/07
 */

#pragma once

#include <string>
#include <vector>

/*!
 *  @namespace  CSV
 *  @brief      CSVヘルパー
 */
namespace CSV
{
    /*!
     *  @brief CSVを分割
     * 
	 *  @param[in]  line            CSVの1行
	 *  @param[in]  delimiter       区切り文字（デフォルトはカンマ）
     * 
	 *  @return     分割された値の配列
     */
    std::vector<std::string>    Split(const std::string& line, char delimiter = ',');

    /*!
     *  @brief 前後の空白を削除
     * 
	 *  @param[in]  str             対象の文字列
     *  
	 *  @return     前後の空白を削除した文字列
     */
    std::string                 Trim(const std::string& str);

    /*!
     *  @brief 列数チェック
     * 
	 *  @param[in]  values          CSVの1行を分割した値の配列
	 *  @param[in]  count           チェックする列数
     * 
	 *  @return     列数が一致する場合はtrue、一致しない場合はfalse
     */
    bool                        CheckColumnCount(const std::vector<std::string>& values, size_t count);
}