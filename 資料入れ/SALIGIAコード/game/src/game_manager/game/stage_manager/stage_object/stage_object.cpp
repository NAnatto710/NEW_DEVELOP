
/*!
 *  @file		stage_object.cpp
 *  @brief		ステージオブジェクト
 *  @author     Ryusei Shimizu
 *  @date       2026/04/15
 */

#include "stage_object.h"
#include "../stage_manager.h"
#include "../../camera_manager/camera_manager.h"

const int           IStageObject::m_default_size        = 70;
const std::string   IStageObject::m_map_chip_file_name  = "data\\map\\block0.png";


/*
 *  コンストラクタ
 */
IStageObject::
IStageObject(void)
    : m_StageObjectID(STAGE_OBJECT_ID::NONE)
	, m_Position(vivid::Vector2::ZERO)
	, m_Rect({ 0, 0, m_default_size, m_default_size })
    , m_ActiveFlg(true)
    , m_CollisionFlg(false)
{
}

/*
 *  デストラクタ
 */
IStageObject::
~IStageObject(void)
{
}

/*
 *  初期化
 */
void
IStageObject::
Initialize(void)
{
    // アクティブフラグをON
    m_ActiveFlg = true;

    // コリジョンフラグをOFF
    m_CollisionFlg = false;
}

/*
 *  更新
 */
void
IStageObject::
Update(void)
{
    // アクティブフラグOFFなら終了
    if (!m_ActiveFlg) return;
}

/*
 *  描画
 */
void
IStageObject::
Draw(void)
{
    // アクティブフラグOFFなら終了
    if (!m_ActiveFlg) return;

	vivid::Vector2 scroll = CCameraManager::GetInstance().GetScroll();
	float    camera_scale = CCameraManager::GetInstance().GetCameraScale();
    vivid::Vector2 draw_position = m_Position;

	draw_position *= camera_scale;
	draw_position -= scroll;

    // 描画
    vivid::DrawTexture(m_map_chip_file_name, draw_position, 0xffffffff, m_Rect, vivid::Vector2::ZERO, vivid::Vector2(camera_scale, camera_scale));
}

/*
 *  解放
 */
void
IStageObject::
Finalize(void)
{
    // アクティブフラグをOFF
    m_ActiveFlg = false;
}