
/*!
 *  @file       stage_object.cpp
 *  @brief      ステージオブジェクトベースクラス
 *  @author     Ryusei Shimizu
 *  @date       2026/01/28
 */

#include "stage_object.h"
#include "../stage_manager.h"
#include "../../camera_manager/camera_manager.h"

const int           IStageObject::m_default_size        = 180;                                  //!< 標準サイズ
const std::string   IStageObject::m_map_chip_file_name  = "data\\map\\map_object_data4.png";    //!< マップチップファイル名

/*
 *  コンストラクタ
 */
IStageObject::
IStageObject(void)
    : m_StageObjectID(STAGE_OBJECT_ID::EMPTY_OBJECT)
    , m_ActiveFlg(true)
    , m_CollisionFlg(false)
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

    // スクロール値取得
    vivid::Vector2 scroll = CCameraManager::GetInstance().GetPosition();

    // スクロールを考慮して描画位置を設定
    m_ScreenPosition = m_Position - scroll;
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

    // 描画
    vivid::DrawTexture(m_map_chip_file_name, m_ScreenPosition, 0xffffffff, m_Rect);
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

/*
 *  ステージオブジェクトID取得
 */
STAGE_OBJECT_ID
IStageObject::
GetStageObjectID(void) const
{
    return m_StageObjectID;
}

/*
 *  ステージオブジェクトID設定
 */
void
IStageObject::
SetStageObjectID(STAGE_OBJECT_ID id)
{
    m_StageObjectID = id;
}

/*
 *  位置取得
 */
vivid::Vector2
IStageObject::
GetPosition(void) const
{
    return m_Position;
}

/*
 *  位置設定
 */
void
IStageObject::
SetPosition(const vivid::Vector2& position)
{
    m_Position = position;
    m_CenterPosition = position + vivid::Vector2((float)m_default_size / 2, (float)m_default_size / 2);
}

/*
 *  中心位置取得
 */
vivid::Vector2
IStageObject::
GetCenterPosition(void) const
{
    return m_CenterPosition;
}

/*
 *  コリジョンチェック
 */
bool
IStageObject::
IsCheckCollision(int x, int y) const
{
    vivid::Vector2 object_pos = this->GetPosition();

    CStageManager& sm = CStageManager::GetInstance();

    int X = (int)(object_pos.x) / sm.GetBlockSize();
    int Y = (int)(object_pos.y) / sm.GetBlockSize();

    // オブジェクトの位置と指定された座標が同じブロック内にあるかどうかを判定する
    if (X <= x && x <= X + 1 && Y <= y && y <= Y + 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
 *  アクティブフラグ取得
 */
bool
IStageObject::
IsActive(void) const
{
    return m_ActiveFlg;
}

/*
 *  アクティブフラグ設定
 */
void
IStageObject::
SetActive(bool active)
{
    m_ActiveFlg = active;
}

/*
 *  コリジョンフラグ取得
 */
bool
IStageObject::
IsCollision(void) const
{
    return m_CollisionFlg;
}

/*
 *  コリジョンフラグ設定
 */
void
IStageObject::
SetCollision(bool collision)
{
    m_CollisionFlg = collision;
}

/*
 *  大きさ取得
 */
int
IStageObject::
GetSize(void)
{
    return m_default_size;
}
