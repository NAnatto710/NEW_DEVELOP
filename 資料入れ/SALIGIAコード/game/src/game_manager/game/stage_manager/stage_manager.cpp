/*!
 *  @file		stage_manager.cpp
 *  @brief		ステージ管理
 *  @author     Ryusei Shimizu
 *  @date       2026/04/14
 */

#include "stage_manager.h"
#include "stage_object/block/block_object.h"
#include "../camera_manager/camera_manager.h"


namespace
{
	struct StageBackgroundLayer
	{
		const char*		FileName;

		vivid::Vector2	CanvasPosition;	//!< 背景画像のステージ座標

		float			ParallaxX;		//!< カメラ追従Xの割合
		float			ParallaxY;		//!< カメラ追従Yの割合
		
		bool			StageLinked;	//!< あたり判定のあるステージ画像かどうか
	};


	// 4200x2100のステージの外側に
	// 左右上下1000pxずつ背景領域を追加
	constexpr float BACKGROUND_ORIGIN_X = -1000.0f;
	constexpr float BACKGROUND_ORIGIN_Y = -1000.0f;


	const StageBackgroundLayer BACKGROUND_LAYERS[] =
	{
		// 月
		{"data\\map\\background_layer\\02_moon.png", {2250.0f, 1280.0f}, 0.01f, 0.002f, false},

		// ブラックホール
		{"data\\map\\background_layer\\03_portal.png", {3750.0f, 1280.0f}, 0.01f, 0.002f, false},

		// 背景の複数の島
		{"data\\map\\background_layer\\04_far_islands.png", {2250.0f, 1350.0f}, 0.014f, 0.008f, false},

		// 左の背景島
		{"data\\map\\background_layer\\07_left_island.png", {2050.0f, 1950.0f}, 0.03f, 0.012f, false},

		// 右の背景島
		{"data\\map\\background_layer\\08_right_island.png", {3800.0f, 1500.0f}, 0.03f, 0.012f, false},

		// 当たり判定付き雲足場
		{"data\\map\\background_layer\\05_cloud_platforms.png", {2520.0f, 1330.0f}, 1.00f, 1.00f, true},

		// 当たり判定付き島
		{"data\\map\\background_layer\\06_main_platform.png", {2014.0f, 2140.0f}, 1.00f, 1.00f, true},
	};

	/*!
	 *	@brief		Parallax背景レイヤーの描画
	 * 
	 *	@param[in]	layer					背景レイヤー情報
	 *	@param[in]	camera_center			カメラの現在中心座標
	 *	@param[in]	base_camera_center		Parallaxの基準となるカメラ中心座標
	 */
	void DrawParallaxBackgroundLayer(const StageBackgroundLayer& layer, const vivid::Vector2& camera_center, const vivid::Vector2& base_camera_center)
	{
		// 背景画像のステージ座標
		const vivid::Vector2 world_position = vivid::Vector2(BACKGROUND_ORIGIN_X + layer.CanvasPosition.x, BACKGROUND_ORIGIN_Y + layer.CanvasPosition.y);

		// Parallaxの基準となるカメラ中心座標からの移動量
		const vivid::Vector2 camera_delta =	camera_center - base_camera_center;

		// 描画位置の計算
		vivid::Vector2 draw_position;
		draw_position.x = (world_position.x - base_camera_center.x) + vivid::WINDOW_WIDTH * 0.5f;
		draw_position.y = (world_position.y - base_camera_center.y) + vivid::WINDOW_HEIGHT * 0.5f;

		// Parallaxの割合を考慮して描画位置を調整
		draw_position.x -= camera_delta.x * layer.ParallaxX;
		draw_position.y -= camera_delta.y * layer.ParallaxY;

		const int width = vivid::GetTextureWidth(layer.FileName);
		const int height = vivid::GetTextureHeight(layer.FileName);

		vivid::DrawTexture(layer.FileName, draw_position, 0xffffffff, vivid::Rect{0, 0, width, height}, vivid::Vector2::ZERO, vivid::Vector2(1.0f, 1.0f));
	}

	/*!
	 *	@brief		ステージにリンクした背景レイヤーの描画
	 * 
	 *	@param[in]	layer			背景レイヤー情報
	 *	@param[in]	scroll			カメラのスクロール量
	 *	@param[in]	camera_scale	カメラの拡大率
	 */
	void DrawStageLinkedLayer(const StageBackgroundLayer& layer, const vivid::Vector2& scroll, float camera_scale)
	{
		// 背景画像のステージ座標
		const vivid::Vector2 world_position = vivid::Vector2(BACKGROUND_ORIGIN_X + layer.CanvasPosition.x, BACKGROUND_ORIGIN_Y + layer.CanvasPosition.y);

		// 描画位置の計算
		vivid::Vector2 draw_position = world_position;

		// カメラの拡大率を考慮して描画位置を調整
		draw_position *= camera_scale;
		draw_position -= scroll;

		const int width = vivid::GetTextureWidth(layer.FileName);
		const int height = vivid::GetTextureHeight(layer.FileName);

		vivid::DrawTexture(layer.FileName, draw_position, 0xffffffff, vivid::Rect{0, 0, width, height}, vivid::Vector2::ZERO, vivid::Vector2(camera_scale, camera_scale));
	}
}

const int			CStageManager::m_map_chip_count_width	= 60;		//!< マップの横幅に何ブロック存在するか
const int			CStageManager::m_map_chip_count_height	= 30;		//!< マップの縦幅に何ブロック存在するか

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

	m_BackgroundBaseCameraCenter = vivid::Vector2::ZERO;
	m_IsBackgroundCameraInitialized = false;
	
	for (const auto& layer : BACKGROUND_LAYERS)
	{
		vivid::LoadTexture(layer.FileName);
	}

	m_BlockSize = IStageObject::GetSize();

	/*
	 * ファイル操作
	 */
	m_CSVLoader.Load(m_map_file_name);

	// マップオブジェクトデータの取得
	m_MapObject = m_CSVLoader.GetData();

	// ステージオブジェクトの初期化
	m_StageObject.assign(m_MapObject.size(), std::vector<IStageObject*>(m_MapObject[0].size(), nullptr));

	for (int i = 0; i < m_map_chip_count_height; i++)
	{
		for (int k = 0; k < m_map_chip_count_width; k++)
		{
			// ステージオブジェクトの生成
			IStageObject* stage_object = nullptr;

			stage_object = this->CreateStageObject((STAGE_OBJECT_ID)m_MapObject[i][k]);

			// オブジェクトが生成されていたら配置
			if (stage_object)
			{
				float x = (float)(k * stage_object->GetSize());
				float y = (float)(i * stage_object->GetSize());

				stage_object->SetPosition(vivid::Vector2(x, y));

				// 生成されたステージオブジェクトをステージオブジェクトデータに格納
				m_StageObject[i][k] = stage_object;
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
			// 更新
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
	// 背景描画
	vivid::DrawTexture("data\\map\\background_layer\\01_sky.png", vivid::Vector2::ZERO);

	CCameraManager& camera = CCameraManager::GetInstance();

	const vivid::Vector2	scroll			= camera.GetScroll();
	const vivid::Vector2	camera_center	= camera.GetCameraCenter();
	const float				camera_scale	= camera.GetCameraScale();

	// 背景カメラ基準位置を取得済みでなければ、現在のカメラ中心を基準位置として保存
	if (!m_IsBackgroundCameraInitialized)
	{
		m_BackgroundBaseCameraCenter = camera_center;
		m_IsBackgroundCameraInitialized = true;
	}

	// 背景レイヤー描画
	for (const auto& layer : BACKGROUND_LAYERS)
	{
		if (layer.StageLinked)
			DrawStageLinkedLayer(layer, scroll, camera_scale);
		else
			DrawParallaxBackgroundLayer(layer, camera_center, m_BackgroundBaseCameraCenter);
	}

	// ステージオブジェクト描画
	for (int i = 0; i < m_map_chip_count_height; i++)
	{
		for (int k = 0; k < m_map_chip_count_width; k++)
		{
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
	m_CSVLoader.UnLoad();
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
	int ix = x / IStageObject::GetSize();
	int iy = y / IStageObject::GetSize();

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

	if (!object)                  return false;
	if (!object->IsActive())      return false;
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

	if (!object)                  return false;
	if (!object->IsActive())      return false;
	if (object->GetStageObjectID() != STAGE_OBJECT_ID::SCAFFOLDING)	return false;

	return true;
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
	case STAGE_OBJECT_ID::UNDER_BLOCK:		stage_object = new CUnderBlock();	break;
	case STAGE_OBJECT_ID::SCAFFOLDING:		stage_object = new CScaffolding();	break;
	case STAGE_OBJECT_ID::SPAWN_POINT_1:	stage_object = new CSpawnPoint1();	break;
	case STAGE_OBJECT_ID::SPAWN_POINT_2:	stage_object = new CSpawnPoint2();	break;
	case STAGE_OBJECT_ID::SPAWN_POINT_3:	stage_object = new CSpawnPoint3();	break;
	case STAGE_OBJECT_ID::SPAWN_POINT_4:	stage_object = new CSpawnPoint4();	break;
	}

	// 生成されたステージオブジェクトが存在する場合は初期化
	if (stage_object)
		stage_object->Initialize();

	return stage_object;
}

/*
 *  スポーン位置取得
 */
vivid::Vector2
CStageManager::
GetSpawnPosition(PLAYER_ID player_id) const
{
	vivid::Vector2 pos;

	// すべてのオブジェクトからスタート位置を探す
	for (int i = 0; i < m_map_chip_count_height; ++i)
	{
		for (int k = 0; k < m_map_chip_count_width; ++k)
		{
			//スタートフラグが見つかった
			if (m_MapObject[i][k] == (unsigned char)player_id + 4)
			{
				// オブジェクト座標から実際の座標に変換して返す
				pos.x = (float)(k * IStageObject::GetSize());
				pos.y = (float)(i * IStageObject::GetSize());

				return pos;
			}
		}
	}
	return pos;
}