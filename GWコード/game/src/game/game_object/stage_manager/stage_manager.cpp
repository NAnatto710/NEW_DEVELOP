
/*!
 *  @file		stage_manager.cpp
 *  @brief		ステージ管理
 *  @author     Ryusei Shimizu
 *  @date       2026/10/14
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

	// CSVを読み込み、数値をステージオブジェクトIDとして保持する。
    // CSVはヘッダーなし。1セルを1マスのステージオブジェクトIDとして扱う。
    Utility::CSV::Rows rows;
    if (!Utility::CSV::Load(m_map_file_name, rows, false) || rows.empty() || rows.front().empty())
    {
        return;
    }

    // 先頭行の列数を横幅とし、行数を縦幅とする。
    const std::size_t width = rows.front().size();
    if (width > static_cast<std::size_t>(std::numeric_limits<int>::max()) ||
        rows.size() > static_cast<std::size_t>(std::numeric_limits<int>::max()))
    {
        return;
    }

    // 途中で不正データが見つかった場合に既存の管理配列を汚さないよう、一時配列へ読み込む。
    std::vector<std::vector<unsigned char>> loadedMap;
    loadedMap.reserve(rows.size());

    for (const auto& row : rows)
    {
        // 行ごとの列数が異なるCSVは不正なマップとして中断する。
        if (!Utility::CSV::CheckColumns(row, width)) return;

        std::vector<unsigned char> line;
        line.reserve(width);
        for (std::size_t x = 0; x < width; ++x)
        {
            // 数値以外のセルは例外になるため、下のcatchで初期化を中断する。
            const int id = Utility::CSV::Get<int>(row, x);
            if (id < 0 || id >= static_cast<int>(STAGE_OBJECT_ID::MAX))
            {
                return; // 未定義のIDを含むCSVは読み込まない
            }
            line.push_back(static_cast<unsigned char>(id));
        }
        loadedMap.push_back(std::move(line));
    }

    // すべての値が有効だと確認できた時点で正式なマップデータに反映する。
    m_MapObject = std::move(loadedMap);
    m_map_chip_count_height = static_cast<int>(m_MapObject.size());
    m_map_chip_count_width = static_cast<int>(width);

    // 各マスに生成したオブジェクトへのポインタを保存する。空マスはnullptr。
    m_StageObject.assign(m_map_chip_count_height,
                         std::vector<IStageObject*>(m_map_chip_count_width, nullptr));

    for (int y = 0; y < m_map_chip_count_height; ++y)
    {
        for (int x = 0; x < m_map_chip_count_width; ++x)
        {
            // IDがNONEならnullptr、NORMAL_BLOCKならCNormalBlockなどの実体を生成する。
            IStageObject* stage_object = CreateStageObject(
                static_cast<STAGE_OBJECT_ID>(m_MapObject[y][x]));

            if (stage_object)
            {
                const float size = static_cast<float>(stage_object->GetSize());

                // CSVの列・行インデックスをワールド座標へ変換する。
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
			// オブジェクトが存在するマスだけ更新する。
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
			// オブジェクトが存在するマスだけ描画する。
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
	// 各オブジェクトに終了処理を呼んでからdeleteし、メモリリークを防ぐ。
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

	// 次回Initialize時に古いマップが残らないよう、配列とサイズをリセットする。
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

	// 引数のx, yはタイル番号ではなくワールド上のピクセル座標。
	// 1マスの大きさで割ってCSV上の列・行インデックスを求める。
	int ix = x / IStageObject::GetSize();
	int iy = y / IStageObject::GetSize();

	// マップ外参照を防止。
	if (ix < 0 || ix >= m_map_chip_count_width || iy < 0 || iy >= m_map_chip_count_height)
		return nullptr;

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

	// CSVの数値と列挙型を対応させ、必要な派生クラスを生成する。
	// 新しい地形IDを追加する場合はここにもcaseを追加する。
	switch (stage_object_id)
	{
	case STAGE_OBJECT_ID::NORMAL_BLOCK:		stage_object = new CNormalBlock();	break;
	case STAGE_OBJECT_ID::NONE:
	case STAGE_OBJECT_ID::MAX:
	default:
		break;
	}

	// 生成されたステージオブジェクトが存在する場合は初期化
	if (stage_object)
		stage_object->Initialize();

	return stage_object;
}