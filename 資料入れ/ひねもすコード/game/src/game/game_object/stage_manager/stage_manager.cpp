
/*!
 *  @file       stage_manager.cpp
 *  @brief      ステージ管理
 *  @author     Ryusei Shimizu
 *  @date       2025/10/09
 */

#include "stage_manager.h"
#include "stage_object/stage_object_id.h"
#include "stage_object/object/object.h"
#include "../camera_manager/camera_manager.h"
#include "../parameter_manager/parameter_manager.h"
#include "../../../utility/utility.h"

const int CStageManager::m_block_size				= 90;															//!< ブロックのサイズ
const int CStageManager::m_map_chip_count_width		= 63;															//!< マップの横幅に何ブロック存在するか
const int CStageManager::m_map_chip_count_height	= 63;															//!< マップの縦幅に何ブロック存在するか
const int CStageManager::m_map_chip_draw_width		= (vivid::WINDOW_WIDTH / CStageManager::m_block_size) + 3;		//!< 画面に表示するブロック数の横幅
const int CStageManager::m_map_chip_draw_height		= (vivid::WINDOW_HEIGHT / CStageManager::m_block_size) + 3;		//!< 画面に表示するブロック数の縦幅
const int CStageManager::m_playerspawn_block_count	= 5;															//!< プレイヤースポーンブロック数
const int CStageManager::m_feed_object_count		= 20;															//!< マップのエサオブジェクト数
const int CStageManager::m_enemyspawn_object_count	= 4;															//!< マップの敵オブジェクト数
const int CStageManager::m_object_interval			= 3;															//!< オブジェクトの配置間隔

const float			CStageManager::m_enemy_spawn_interval		= 3.0f;												//!< 敵の出現間隔

const std::string	CStageManager::m_map_file_name	= "data\\map\\map.csv";											//!< マップファイル名
const std::string	CStageManager::m_block_data_name_1= "data\\map\\season_floor_1.png";							//!< ブロックデータ名
const std::string	CStageManager::m_block_data_name_2= "data\\map\\season_floor_2.png";							//!< ブロックデータ名


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
Initialize(void)
{
	m_MapBlock.clear();
	m_MapBlockRect.clear();
	m_MapBlockFlg.clear();
	m_StageObjectList.clear();

	m_EnemySpawnTimer = m_enemy_spawn_interval;

	m_FeedObjectCount = 0;
	m_EnemySpawnObjectCount = 0;

	srand((unsigned int)time(nullptr));

	/*
	 * ファイル操作
	 */
	m_CSVLoader.Load(m_map_file_name);

	// マップデータの2次元配列を確保
	m_MapBlock.resize(m_CSVLoader.GetMapChipCols(), std::vector<unsigned char>(m_CSVLoader.GetMapChipRows()));
	m_MapBlockRect.resize(m_CSVLoader.GetMapChipCols(), std::vector<vivid::Rect>(m_CSVLoader.GetMapChipRows()));
	m_MapBlockFlg.resize(m_CSVLoader.GetMapChipCols(), std::vector<bool>(m_CSVLoader.GetMapChipRows()));

	// マップブロックデータを2次元配列に格納する
	m_MapBlock = m_CSVLoader.GetData();

	// オブジェクトの配置
	this->ArrangementObject();
}

/*
 *  更新
 */
void
CStageManager::
Update(void)
{
	// ステージオブジェクトの更新
	for (StageObjectList::iterator it = m_StageObjectList.begin(); it != m_StageObjectList.end(); ++it)
	{
		(*it)->Update();

		if ((*it)->IsActive() == false)
		{
			if ((*it)->GetStageObjectID() == STAGE_OBJECT_ID::FEED_OBJECT)
				m_FeedObjectCount--;

			if ((*it)->GetStageObjectID() == STAGE_OBJECT_ID::ENEMY_SPAWN_OBJECT)
				m_EnemySpawnObjectCount--;

			(*it)->Finalize();
			delete (*it);
			it = m_StageObjectList.erase(it);

			continue;
		}
	}

	CGameParameterManager& gpm = CGameParameterManager::GetInstance();
	CCharacterManager& cm = CCharacterManager::GetInstance();

	SEASON_ID season = gpm.GetSeasonId();
	int select = 0;;

	if (m_EnemySpawnTimer > 0)
		m_EnemySpawnTimer -= vivid::GetDeltaTime();

	// 敵の出現タイマーを減らす
	if (m_EnemySpawnTimer <= 0)
	{
		m_EnemySpawnTimer = m_enemy_spawn_interval;

		// 敵の出現オブジェクトから敵の出現位置を取得する
		CEnemySpawn enemy_spawn_object = this->GetEnemySpawnObject();

		// 敵の出現位置に敵を出現させる
		switch (season)
		{
			// 季節に応じた敵の出現IDをランダムに選択する
			// 冬は毛虫のみを出現させる
		case SEASON_ID::WINTER:

			cm.Create(CHARACTER_ID::CATERPILLAR, enemy_spawn_object.GetSpawnPosition());

			break;

			// 春は毛虫とムカデのいずれかを出現させる
		case SEASON_ID::SPRING:

			select = u_RandomInt(1, 2);

			if (select == 1) 
				cm.Create(CHARACTER_ID::CATERPILLAR, enemy_spawn_object.GetSpawnPosition());
			else		 
				cm.Create(CHARACTER_ID::CENTIPEDE, enemy_spawn_object.GetSpawnPosition());

			break;

			// 夏は毛虫と蜂のいずれかを出現させる
		case SEASON_ID::SUMMER:

			select = u_RandomInt(1, 2);

			if (select == 1)
				cm.Create(CHARACTER_ID::CATERPILLAR, enemy_spawn_object.GetSpawnPosition());
			else
				cm.Create(CHARACTER_ID::BEE, enemy_spawn_object.GetSpawnPosition());

			break;

			// 秋は毛虫、ムカデ、蜂のいずれかを出現させる
		case SEASON_ID::AUTUMN:

			select = u_RandomInt(1, 3);

			cm.Create((CHARACTER_ID)select, enemy_spawn_object.GetSpawnPosition());

			break;
		}
	}
}

/*
 *  描画
 */
void
CStageManager::
Draw(void)
{
	vivid::Vector2 position;

	// カメラの位置から描画開始位置を決定する
	int camera_x = (int)(CCameraManager::GetInstance().GetPosition().x) / m_block_size;
	int camera_y = (int)(CCameraManager::GetInstance().GetPosition().y) / m_block_size;

	// 要素数分繰り返す
	// 画面描画範囲内のマップチップを描画する
	for (int i = camera_y; i <= m_map_chip_draw_height + camera_y && i < m_map_chip_count_height; ++i)
	{
		for (int k = camera_x; k <= m_map_chip_draw_width + camera_x && k < m_map_chip_count_width; ++k)
		{
			// 表示位置を決める
			position.x = (int)(k * m_block_size);
			position.y = (int)(i * m_block_size);

			// カメラの位置反映
			position -= CCameraManager::GetInstance().GetPosition();

			// 季節に応じた床オブジェクトを配置するかどうかをランダムに決定する
			bool      is_season_floor = u_RandomInt(0, 1);

			// 季節に応じた床オブジェクトを配置する
			if (m_MapBlockFlg[i][k])	vivid::DrawTexture(m_block_data_name_1, position, 0xffffffff, m_MapBlockRect[k][i]);
			else						vivid::DrawTexture(m_block_data_name_2, position, 0xffffffff, m_MapBlockRect[k][i]);
		}
	}

	// ステージオブジェクトの描画
	StageObjectList::iterator it = m_StageObjectList.begin();
	while (it != m_StageObjectList.end())
	{
		(*it)->Draw();

		++it;
	}
}

/*
 *  解放
 */
void
CStageManager::
Finalize(void)
{
	m_CSVLoader.UnLoad();
	m_MapBlock.clear();

	// ステージオブジェクトの解放
	for (StageObjectList::iterator it = m_StageObjectList.begin(); it != m_StageObjectList.end(); ++it)
	{
		(*it)->Finalize();
		(*it)->SetActive(false);
		delete (*it);
	}

	m_StageObjectList.clear();
}

/*
 *  ブロックの大きさ取得
 */
int
CStageManager::
GetBlockSize(void)const
{
	return m_block_size;
}

/*
 *  マップの横幅に何ブロック存在するか取得
 */
int
CStageManager::
GetMapChipWidth(void)const
{
	return m_map_chip_count_width;
}

/*
 *  マップの縦幅に何ブロック存在するか取得
 */
int
CStageManager::
GetMapChipHeight(void)const
{
	return m_map_chip_count_height;
}

/*
 *  画面に表示するブロック数の横幅取得
 */
int
CStageManager::
GetMapDrawWidth(void)const
{
	return m_map_chip_draw_width;
}

/*
 *  画面に表示するブロック数の縦幅取得
 */
int
CStageManager::
GetMapDrawHeight(void)const
{
	return m_map_chip_draw_height;
}

/*
 *  スタート位置を返す
 */
vivid::Vector2
CStageManager::
GetStartBlockPosition(void)const
{
	vivid::Vector2 pos[m_playerspawn_block_count] = {};
	int count = 0;

	// すべてのオブジェクトからスタート位置を探す
	for (int i = 0; i < m_map_chip_count_height; ++i)
	{
		for (int k = 0; k < m_map_chip_count_width; ++k)
		{
			//スタートフラグが見つかった
			if (m_MapBlock[i][k] == (unsigned char)STAGE_BLOCK_ID::PLAYER_SPAWN_BLOCK)
			{
				// オブジェクト座標から実際の座標に変換して返す
				pos[count].x = (float)(k * m_block_size);
				pos[count].y = (float)(i * m_block_size);

				count++;
			}
		}
	}

	// ランダムにスタート位置を決定する
	int rand = u_RandomInt(0, m_playerspawn_block_count - 1);

	return pos[rand];
}

/*
 *  敵の出現オブジェクトを返す
 */
CEnemySpawn
CStageManager::
GetEnemySpawnObject(void) const
{
	CEnemySpawn object[m_enemyspawn_object_count] = {};
	int count = 0;

	// すべてのオブジェクトから敵の出現位置を探す
	for (StageObjectList::const_iterator it = m_StageObjectList.cbegin(); it != m_StageObjectList.cend(); ++it)
	{
		if ((*it)->GetStageObjectID() == STAGE_OBJECT_ID::ENEMY_SPAWN_OBJECT)
		{
			// オブジェクトを敵の出現オブジェクトに変換して配列に格納する
			object[count] = *dynamic_cast<CEnemySpawn*>(*it);
			count++;
		}
	}

	// ランダムに敵の出現位置を決定する
	int rand = u_RandomInt(0, m_enemyspawn_object_count - 1);

	return object[rand];
}

/*
 *  壁の当たり判定
 */
bool
CStageManager::
IsWall(int x, int y)
{
	// IDが壁ならtrueを返す
	if ((STAGE_BLOCK_ID)m_MapBlock[y][x] == STAGE_BLOCK_ID::WALL_BLOCK)return true;

	return false;
}

/*
 *  プレイヤースポーン地点の当たり判定
 */
bool
CStageManager::
IsPlayerSpawnBlock(int x, int y)
{
	//IDがプレイヤースポーン地点ならtrueを返す
	if ((STAGE_BLOCK_ID)m_MapBlock[y][x] == STAGE_BLOCK_ID::PLAYER_SPAWN_BLOCK)return true;

	return false;
}

/*
 *  オブジェクトの配置
 */
void
CStageManager::
ArrangementObject(void)
{
	// エサオブジェクトの配置
	this->FeedObjectArrangement();

	// 敵出現オブジェクトの配置
	this->EnemySpawnObjectArrangement();

	// レクトの更新
	this->RectUpdate();
}

/*
 *  オブジェクトの再配置
 */
void
CStageManager::
ReinstallationObject(void)
{
	// ステージオブジェクトの解放
	for (StageObjectList::iterator it = m_StageObjectList.begin(); it != m_StageObjectList.end(); ++it)
	{
		(*it)->Finalize();
		(*it)->SetActive(false);
		delete (*it);
	}

	m_StageObjectList.clear();

	this->ArrangementObject();
}

/*
 *	ステージオブジェクトリスト取得
 */
std::list<IStageObject*>
CStageManager::
GetFeedObject(void) const
{
	return m_StageObjectList;
}

/*
 *  コンストラクタ
 */
CStageManager::
CStageManager(void)
{
}

/*
 *  コピーコンストラクタ
 */
CStageManager::
CStageManager(const CStageManager& rhs)
{
	(void)rhs;
}

/*
 *  デストラクタ
 */
CStageManager::
~CStageManager(void)
{
}

/*
 *  代入演算子
 */
CStageManager&
CStageManager::
operator=(const CStageManager& rhs)
{
	(void)rhs;

	return *this;
}

/*
 *  エサオブジェクトの配置
 */
void
CStageManager::
FeedObjectArrangement(void)
{
	// マップの端から離すブロック数(偶数である必要がある)
	int margin_block_count = 2;

	// 餌オブジェクトデータをランダムに配置する
	for (int i = 0; i < m_feed_object_count; ++i)
	{
		int x = rand() % (m_map_chip_count_width - margin_block_count);
		int y = rand() % (m_map_chip_count_height - margin_block_count);

		x += margin_block_count / 2;
		y += margin_block_count / 2;

		IStageObject* feed_object = new CFeed();

		// 壁やプレイヤースポーン地点に配置しないようにする
		// 他のエサオブジェクトや敵オブジェクトと重ならないようにする
		bool is_overlap = false;
		for (StageObjectList::iterator it = m_StageObjectList.begin(); it != m_StageObjectList.end(); ++it)
		{
			vivid::Vector2 object_pos = (*it)->GetPosition();

			vivid::Vector2 v = vivid::Vector2((float)(x * m_block_size), (float)(y * m_block_size)) - object_pos;

			// オブジェクトの位置と配置しようとしている位置が近い場合は重なっているとみなす
			if (vivid::Vector2::Length(v) < ((*it)->GetSize() * m_object_interval) ||
				this->IsWall(x, y) || this->IsWall(x + 1, y) || this->IsWall(x, y + 1) || this->IsWall(x + 1, y + 1) ||
				this->IsPlayerSpawnBlock(x, y) || this->IsPlayerSpawnBlock(x + 1, y) || this->IsPlayerSpawnBlock(x, y + 1) || this->IsPlayerSpawnBlock(x + 1, y + 1))
			{
				is_overlap = true;
				break;
			}
		}

		if (is_overlap)
		{
			// 重なっている場合は再度ランダムに位置を決定する
			--i;
			continue;
		}
		else
		{
			// 餌オブジェクトの初期化
			feed_object->Initialize();
			feed_object->SetPosition(vivid::Vector2((float)(x * m_block_size), (float)(y * m_block_size)));

			// エサオブジェクトの数を増やす
			m_FeedObjectCount++;

			// ステージオブジェクトリストに追加
			m_StageObjectList.push_back(feed_object);
		}
	}
}

/*
 *  敵出現オブジェクトの配置
 */
void
CStageManager::
EnemySpawnObjectArrangement(void)
{
	// マップの端から離すブロック数(偶数である必要がある)
	int margin_block_count = 6;

	// 敵出現オブジェクトデータをランダムに配置する
	for (int i = 0; i < m_enemyspawn_object_count; ++i)
	{
		int x = rand() % (m_map_chip_count_width - margin_block_count);
		int y = rand() % (m_map_chip_count_height - margin_block_count);

		x += margin_block_count / 2;
		y += margin_block_count / 2;

		IStageObject* enemy_object = new CEnemySpawn();

		// 壁やプレイヤースポーン地点に配置しないようにする
		// エサオブジェクトや他の敵オブジェクトと重ならないようにする
		bool is_overlap = false;
		for (StageObjectList::iterator it = m_StageObjectList.begin(); it != m_StageObjectList.end(); ++it)
		{
			vivid::Vector2 object_pos = (*it)->GetPosition();

			vivid::Vector2 v = vivid::Vector2((float)(x * m_block_size), (float)(y * m_block_size)) - object_pos;

			// オブジェクトの位置と配置しようとしている位置が近い場合は重なっているとみなす
			if (vivid::Vector2::Length(v) < ((*it)->GetSize() * m_object_interval) ||
				this->IsWall(x, y) || this->IsWall(x + 1, y) || this->IsWall(x, y + 1) || this->IsWall(x + 1, y + 1) ||
				this->IsPlayerSpawnBlock(x, y) || this->IsPlayerSpawnBlock(x + 1, y) || this->IsPlayerSpawnBlock(x, y + 1) || this->IsPlayerSpawnBlock(x + 1, y + 1))
			{
				is_overlap = true;
				break;
			}
		}

		if (is_overlap)
		{
			// 重なっている場合は再度ランダムに位置を決定する
			--i;
			continue;
		}
		else
		{
			// 敵出現オブジェクトの初期化
			enemy_object->Initialize();
			enemy_object->SetPosition(vivid::Vector2((float)(x * m_block_size), (float)(y * m_block_size)));

			// ステージオブジェクトリストに追加
			m_StageObjectList.push_back(enemy_object);
		}
	}

}

/*
 *  レクトの更新
 */
void
CStageManager::
RectUpdate(void)
{
	CGameParameterManager& pm = CGameParameterManager::GetInstance();

	SEASON_ID season = pm.GetSeasonId();
	vivid::Rect    rect = {};

	// 季節に応じた床オブジェクトをマップ全体に配置する
	for (int i = 0; i < m_map_chip_count_height; ++i)
	{
		for (int k = 0; k < m_map_chip_count_width; ++k)
		{
			// 読み込み範囲を求める
			rect.left = (int)season * m_block_size;
			rect.right = rect.left + m_block_size;
			rect.top = 0;
			rect.bottom = m_block_size;

			// マップブロックの描画範囲データに格納する
			m_MapBlockRect[i][k] = rect;

			// 季節に応じた床オブジェクトを配置するかどうかをランダムに決定する
			m_MapBlockFlg[i][k] = u_RandomInt(0, 1);
		}
	}
}
