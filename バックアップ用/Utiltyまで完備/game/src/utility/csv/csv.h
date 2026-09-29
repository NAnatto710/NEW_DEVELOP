
/*!
 *  @file       csv.h
 *  @brief      CSV読み込み・分割・型変換Utility
 *  @author     Ryusei Shimizu
 *  @date       2026/09/16
 */

#pragma once

#include <cstddef>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace Utility
{
    namespace CSV
    {
        /*!
		 *  @brief CSV1行を表す型
         */
        using Row = std::vector<std::string>;

		/*!
		 *  @brief CSV全行を表す型
         */
        using Rows = std::vector<Row>;

        /*!
         *  @brief CSVファイルをすべて読み込む
         *
         *  @param[in]  path        CSVファイルパス
         *  @param[out] rows        読み込んだ行
         *  @param[in]  skipHeader  trueなら先頭の有効行をヘッダーとして読み飛ばす
         *
         *  @return 読み込みに成功したらtrue
         */
        bool Load(const std::string& path, Rows& rows, bool skipHeader = false);

        /*!
         *  @brief CSVを1行ずつ処理する
         *
		 *  @param[in]  path        CSVファイルパス
		 *  @param[in]  callback    各行を処理するコールバック関数
		 *  @param[in]  skipHeader  trueなら先頭の有効行をヘッダーとして読み飛ばす
         * 
		 *  @return 読み込みに成功したらtrue
         */
        bool Each(const std::string& path, const std::function<bool(const Row&, std::size_t)>& callback, bool skipHeader = false);

        /*!
         *  @brief CSVの1行をセル単位へ分割
         *
		 *  @param[in]  line       CSV1行文字列
		 *  @param[in]  delimiter  区切り文字
         * 
		 *  @return 分割後のセル文字列配列
         */
        Row Split(const std::string& line, char delimiter = ',');

		/*!
		 *  @brief 文字列の前後空白を削除
         * 
		 *  @param[in]  value  対象文字列
         *  
		 *  @return 前後空白を削除した文字列
		 */
        std::string Trim(const std::string& value);

		/*!
		 *  @brief 行の列数が指定数と一致するか確認
		 *
		 *  @param row   対象行
		 *  @param count 列数
		 *
		 *  @return 一致する場合はtrue
		 */
        bool CheckColumns(const Row& row, std::size_t count);

        /*!
         *  @brief セルを指定型へ変換して取得
         *
		 *  @param row   対象行
		 *  @param index 対象列インデックス
         *  
		 *  @return 指定型へ変換したセル値
         */
        template <class T>
        T Get(const Row& row, std::size_t index);

        /*!
         *  @brief 文字列からenumへ変換
         *
         *  @param[in]   table   文字列 -> enum の変換表
         *  @param[in]   value   CSV上の文字列
         *  @param[out]  out     変換結果
         *
         *  @return 変換表に値が存在した場合はtrue、存在しない場合はfalse
         */
        template <class TEnum>
        bool ParseEnum(const std::unordered_map<std::string, TEnum>& table, const std::string& value, TEnum& out)
        {
            const std::string key = Trim(value);
            const auto it = table.find(key);

            if (it == table.end()) return false;

            out = it->second;
            return true;
        }

        /*!
         *  @brief  よく使用する基本型のGet特殊化
         *
         *  @note   実際の変換処理はcsv.cpp側で実装する
         */
        template <> float Get<float>(const Row& row, std::size_t index);
        template <> int Get<int>(const Row& row, std::size_t index);
        template <> bool Get<bool>(const Row& row, std::size_t index);
        template <> std::string Get<std::string>(const Row& row, std::size_t index);
    }
}
