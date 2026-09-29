
/*!
 *  @file       csv.cpp
 *  @brief      CSV読み込み・分割・型変換Utility
 *  @author     Ryusei Shimizu
 *  @date       2026/09/16
 */

#include "csv.h"

#include <algorithm>
#include <cctype>
#include <fstream>

namespace
{
    /*!
     *  @brief  読み込み対象として使用できる行か判定
     *
     *  @param[in]  line    判定するCSV1行
     *
     *  @return 空行または#から始まるコメント行ならfalse、それ以外はtrue
     */
    bool
    IsUsableLine(const std::string& line)
    {
        const std::string trimmed = Utility::CSV::Trim(line);
        return !trimmed.empty() && trimmed[0] != '#';
    }
}

namespace Utility
{
    namespace CSV
    {
        /*
         *  CSVファイル全体をRowsへ読み込む
         *  1行ずつの読み込み処理はEach()へ任せ、取得したRowを順番に追加する
         */
        bool
        Load(const std::string& path, Rows& rows, bool skipHeader)
        {
            // 読み込み失敗時に前回データが残らないよう、結果格納先を先に空にする
            rows.clear();

            return Each(path, [&rows](const Row& row, std::size_t)
                {
                    rows.push_back(row);
                    return true;
                },
                skipHeader);
        }

        /*
         *  CSVファイルを先頭から1行ずつ読み込む
         *  空行・コメント行・必要に応じてヘッダーを除外し、有効行だけcallbackへ渡す
         */
        bool
        Each(const std::string& path, const std::function<bool(const Row&, std::size_t)>& callback, bool skipHeader)
        {
            std::ifstream file(path.c_str());

            if (!file.is_open()) return false;

            std::string line;
            std::size_t rowIndex = 0;
            bool headerSkipped = !skipHeader;

            while (std::getline(file, line))
            {
                // std::getline()ではWindows改行のCRが末尾に残る場合があるため取り除く
                if (!line.empty() && line[line.size() - 1] == '\r')
                {
                    line.erase(line.size() - 1);
                }

                if (!IsUsableLine(line)) continue;

                if (!headerSkipped)
                {
                    headerSkipped = true;
                    continue;
                }

                const Row row = Split(line);

                if (!callback(row, rowIndex)) return false;

                ++rowIndex;
            }

            return true;
        }

        /*
         *  CSV1行をセル単位へ分割する
         *  ダブルクォート内の区切り文字は通常文字として扱い、CSVの引用ルールにも対応する
         */
        Row
        Split(const std::string& line, char delimiter)
        {
            Row row;
            std::string cell;
            bool inQuotes = false;
            bool quotedCell = false;

            for (std::size_t i = 0; i < line.size(); ++i)
            {
                const char ch = line[i];

                if (ch == '"')
                {
                    // クォート内の "" はエスケープされた1文字のダブルクォートとして扱う
                    if (inQuotes && i + 1 < line.size() && line[i + 1] == '"')
                    {
                        cell.push_back('"');
                        ++i;
                        continue;
                    }

                    inQuotes = !inQuotes;
                    quotedCell = true;
                    continue;
                }

                // クォートの外側にある区切り文字だけをセルの境界として扱う
                if (ch == delimiter && !inQuotes)
                {
                    row.push_back(quotedCell ? cell : Trim(cell));
                    cell.clear();
                    quotedCell = false;
                    continue;
                }

                cell.push_back(ch);
            }

            // ループ終了後に最後のセルを追加する。末尾が区切り文字なら空セルも保持する
            row.push_back(quotedCell ? cell : Trim(cell));
            return row;
        }

        /*
         *  文字列の前後にある空白文字を取り除く
         *  CSVの数値変換前や未クォートセルの整形に使用する
         */
        std::string
        Trim(const std::string& value)
        {
            std::string::const_iterator begin = std::find_if_not(value.begin(), value.end(), [](unsigned char ch) { return std::isspace(ch) != 0; });
            std::string::const_reverse_iterator reverseEnd = std::find_if_not(value.rbegin(), value.rend(), [](unsigned char ch) { return std::isspace(ch) != 0; });

            std::string::const_iterator end = reverseEnd.base();

            if (begin >= end) return std::string();

            return std::string(begin, end);
        }

        /*
         *  CSV1行の列数が想定した数と一致しているか確認する
         */
        bool
        CheckColumns(const Row& row, std::size_t count)
        {
            return row.size() == count;
        }

        /*
         *  指定セルをfloatへ変換して取得する
         */
        template <>
        float
        Get<float>(const Row& row, std::size_t index)
        {
            return std::stof(Trim(row.at(index)));
        }

        /*
         *  指定セルをintへ変換して取得する
         */
        template <>
        int
        Get<int>(const Row& row, std::size_t index)
        {
            return std::stoi(Trim(row.at(index)));
        }

        /*
         *  指定セルをboolへ変換して取得する
         *  true / 1 / yes / on をtrueとして扱い、大文字小文字は区別しない
         */
        template <>
        bool
        Get<bool>(const Row& row, std::size_t index)
        {
            std::string value = Trim(row.at(index));

            std::transform(
                value.begin(), value.end(), value.begin(),
                [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });

            return value == "true" || value == "1" || value == "yes" || value == "on";
        }

        /*
         *  指定セルを文字列のまま取得する
         */
        template <>
        std::string
        Get<std::string>(const Row& row, std::size_t index)
        {
            return row.at(index);
        }
    }
}
