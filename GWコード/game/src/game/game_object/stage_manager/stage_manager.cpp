
/*!
 *  @file			stage_manager.cpp
 *  @brief			ステージ管理
 *  @author			Ryusei Shimizu
 *  @date			2026/10/14
 */

#include "stage_manager.h"
#include "stage_object/block/block_object.h"
#include "../../../utility/csv/csv.h"

#include <exception>
#include <limits>
#include <utility>

const std::string	CStageManager::m_map_file_name			= "data\\map\\map.csv";				//!< マップファイル名

/*
 *  インスタンスの取得
 */
CStageManager&
CStageManager::
GetInstance(void)
{
	static CStageManager instance;

	return instance;
}

/*
 *  初期化
 */
void
CStageManager::
Initialize()
{
	this->Finalize();

	m_BlockSize = IStageObject::GetSize();

	// CSVを読み込み、数値をステージオブジェクトIDとして保持する
    Utility::CSV::Rows rows;
    if (!Utility::CSV::Load(m_map_file_name, rows, false) || rows.empty() || rows.front().empty()) return;

    // 先頭行の列数を横幅とし、行数を縦幅とする
    const std::size_t width = rows.front().size();

	m_MapObject.reserve(rows.size());

    for (const auto& row : rows)
    {
        // 行ごとの列数が異なるCSVは中断する
        if (!Utility::CSV::CheckColumns(row, width)) return;

        std::vector<unsigned char> line;
        line.reserve(width);
        for (std::size_t x = 0; x < width; ++x)
        {
			// CSVの数値がSTAGE_OBJECT_IDの範囲外なら中断する
            const int id = Utility::CSV::Get<int>(row, x);
            if (id < 0 || id >= static_cast<int>(STAGE_OBJECT_ID::MAX)) return;

            line.push_back(static_cast<unsigned char>(id));
        }
		m_MapObject.push_back(std::move(line));
    }

    m_map_chip_count_width = static_cast<int>(width);
    m_map_chip_count_height = static_cast<int>(m_MapObject.size());

    // 各マスに生成したオブジェクトへのポインタを保存する
    m_StageObject.assign(m_map_chip_count_height, std::vector<IStageObject*>(m_map_chip_count_width, nullptr));

    for (int y = 0; y < m_map_chip_count_height; ++y)
    {
        for (int x = 0; x < m_map_chip_count_width; ++x)
        {
            IStageObject* stage_object = CreateStageObject(static_cast<STAGE_OBJECT_ID>(m_MapObject[y][x]));

            if (stage_object)
            {
                const float size = static_cast<float>(stage_object->GetSize());

                stage_object->SetPosition(vivid::Vector2(x * size, y * size));

                m_StageObject[y][x] = stage_object;
            }
        }
    }
}

/*
 *  更新
 */
void
CStageManager::
Update()
{
	for (int i = 0; i < m_map_chip_count_height; i++)
	{
		for (int k = 0; k < m_map_chip_count_width; k++)
		{
			// オブジェクトが存在するマスだけ更新する
			if (m_StageObject[i][k])
				m_StageObject[i][k]->Update();
		}
	}
}

/*
 *  描画
 */
void
CStageManager::
Draw()
{
	for (int i = 0; i < m_map_chip_count_height; i++)
	{
		for (int k = 0; k < m_map_chip_count_width; k++)
		{
			// オブジェクトが存在するマスだけ描画する
			if (m_StageObject[i][k])
				m_StageObject[i][k]->Draw();
		}
	}
}

/*
 *  解放
 */
void
CStageManager::
Finalize()
{
	for (auto& row : m_StageObject)
	{
		for (auto& obj : row)
		{
			if (obj)
			{
				obj->Finalize();
				delete obj;
				obj = nullptr;
			}
		}
	}

	m_StageObject.clear();
	m_MapObject.clear();
	m_map_chip_count_width = 0;
	m_map_chip_count_height = 0;
}

/*
 *  ブロックの大きさ取得
 */
int
CStageManager::
GetBlockSize(void) const
{
	return m_BlockSize;
}

/*
 *  マップの横幅に何ブロック存在するか取得
 */
int
CStageManager::
GetMapChipWidth(void) const
{
	return m_map_chip_count_width;
}

/*
 *  マップの縦幅に何ブロック存在するか取得
 */
int
CStageManager::
GetMapChipHeight(void) const
{
	return m_map_chip_count_height;
}

/*
 *  ステージデータ取得
 */
IStageObject*
CStageManager::
GetStageObject(int x, int y) const
{
	if (x < 0 || y < 0) return nullptr;

	// 引数のx, yはタイル番号ではなくワールド上のピクセル座標
	int ix = x / IStageObject::GetSize();
	int iy = y / IStageObject::GetSize();

	// マップ外参照を防止
	if (ix < 0 || ix >= m_map_chip_count_width || iy < 0 || iy >= m_map_chip_count_height) return nullptr;

	return m_StageObject[iy][ix];
}

/*
 *  当たり判定
 */
bool
CStageManager::
IsHit(int x, int y) const
{
	IStageObject* object = GetStageObject(x, y);

	// 空マスや未使用のオブジェクトは当たり判定なし。
	if (!object)                  return false;
	// 非アクティブのオブジェクトも判定対象外。
	if (!object->IsActive())      return false;
	// 当たり判定フラグが有効なオブジェクトのみヒット扱い。
	if (!object->IsCollision())   return false;

	return true;
}

/*
 *  足場の当たり判定
 */
bool
CStageManager::
IsHitScaffolding(int x, int y) const
{
	IStageObject* object = GetStageObject(x, y);

	// 現在のSTAGE_OBJECT_IDに足場の種類は定義されていない。
	// 未実装の足場判定が常にtrueになるのを防ぐ。
	(void)object;
	return false;
}

/*
 *  ステージオブジェクトの生成
 */
IStageObject*
CStageManager::
CreateStageObject(STAGE_OBJECT_ID stage_object_id)
{
	IStageObject* stage_object = nullptr;

	switch (stage_object_id)
	{
	case STAGE_OBJECT_ID::NORMAL_BLOCK:		stage_object = new CNormalBlock();	break;
	case STAGE_OBJECT_ID::NONE:
	case STAGE_OBJECT_ID::MAX:
	default:		break;
	}

	// 生成されたステージオブジェクトが存在する場合は初期化
	if (stage_object)
		stage_object->Initialize();

	return stage_object;
}