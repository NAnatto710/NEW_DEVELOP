
/*!
 *  @file       stage_csv_loader.cpp
 *  @brief      ステージCSVローダー
 *  @author     Ryusei Shimizu
 *  @date       2026/07/07
 */

#include "stage_csv_loader.h"
#include "../../csv_helper/csv_helper.h"
#include "../../csv_convert/csv_convert.h"

/*
 *  コンストラクタ
 */
CStageCSVLoader::
CStageCSVLoader()
{
}

/*
 *  CSVをロード
 */
bool
CStageCSVLoader::
Load(const std::string& filepath)
{
	UnLoad();

	return LoadFile(filepath,
		[&](const std::string& line)
		{
			return LoadLine(line);
		});
}

/*
 *  CSVをアンロード
 */
void
CStageCSVLoader::
UnLoad()
{
	m_MapData.clear();

	m_MapChipRows = 0;

	m_MapChipCols = 0;
}

/*
 *  データ取得
 */
unsigned char
CStageCSVLoader::
GetData(int x, int y) const
{
	return m_MapData[y][x];
}

/*
 *  データ取得
 */
const std::vector<std::vector<unsigned char>>&
CStageCSVLoader::
GetData() const
{
	return m_MapData;
}

/*
 *  マップチップの行数取得
 */
int
CStageCSVLoader::
GetMapChipRows() const
{
	return m_MapChipRows;
}

/*
 *  マップチップの列数取得
 */
int
CStageCSVLoader::
GetMapChipCols() const
{
	return m_MapChipCols;
}

/*
 *  1行ロード
 */
bool
CStageCSVLoader::
LoadLine(const std::string& line)
{
	// CSVの1行を分割して値を取得
	auto values = CSV::Split(line);

	// 1行読み込み
	ReadRow(values);

	return true;
}

void
CStageCSVLoader::
ReadRow(const std::vector<std::string>& values)
{
    std::vector<unsigned char> row;

	// 事前にサイズを確保
    row.reserve(values.size());

	// CSVの値をunsigned charに変換して格納
    for (size_t i = 0; i < values.size(); ++i)
    {
        row.emplace_back(static_cast<unsigned char>(CSV::Get<int>(values, i)));
    }

	// マップチップの列数を設定（初回のみ）
    if (m_MapChipCols == 0)
    {
        m_MapChipCols = static_cast<int>(row.size());
    }

    ++m_MapChipRows;

	// マップデータに行を追加
    m_MapData.emplace_back(std::move(row));
}
