
/*!
 *  @file       feed.cpp
 *  @brief      エサクラス
 *  @author     Ryusei Shimizu
 *  @date       2026/01/28
 */

#include "enemy_spawn.h"
#include "../../../stage_manager.h"
#include "../../../../character_manager/character_manager.h"
#include "../../../../../../utility/utility.h"

const float CEnemySpawn::m_enemy_spawn_range = 90.0f;    //!< 敵出現範囲

/*
 *  コンストラクタ
 */
CEnemySpawn::
CEnemySpawn(void)
{
}

/*
 *  デストラクタ
 */
CEnemySpawn::
~CEnemySpawn(void)
{
}

/*
 *  初期化
 */
void
CEnemySpawn::
Initialize(void)
{
    // 読み込み範囲指定
    m_Rect.left = m_default_size * ((int)STAGE_OBJECT_ID::ENEMY_SPAWN_OBJECT);
    m_Rect.top = 0;
    m_Rect.right = m_Rect.left + m_default_size;
    m_Rect.bottom = m_default_size;

    // ID設定
    m_StageObjectID = STAGE_OBJECT_ID::ENEMY_SPAWN_OBJECT;

    // アクティブフラグをON
    m_ActiveFlg = true;

    // コリジョンフラグOFF
    m_CollisionFlg = true;
}

/*
 *  更新
 */
void
CEnemySpawn::
Update(void)
{
    // ベースの更新処理
    IStageObject::Update();
}

/*
 *  敵の出現位置の取得
 */
vivid::Vector2
CEnemySpawn::
GetSpawnPosition(void) const
{
    srand((unsigned int)time(nullptr));

    vivid::Vector2 spawn_position = this->GetCenterPosition();

    // 敵の出現位置をランダムに決定する
    float angle = u_RandomInt(0.0f, 360.0f); // ランダムな角度を生成

	// 敵の出現位置を円形の範囲内で計算する
	// 角度をラジアンに変換して、cosとsinを使ってxとyの座標を計算する
    spawn_position.x += m_enemy_spawn_range * cosf(DEG_TO_RAD(angle)); // x座標を計算
    spawn_position.y += m_enemy_spawn_range * sinf(DEG_TO_RAD(angle)); // y座標を計算

    // 出現位置がキャラクターの中心になるように調整
    float radius = CCharacterManager::GetInstance().GetPlayer()->GetWidth() / 2.0f;

    spawn_position.x -= radius;
    spawn_position.y -= radius;

    return spawn_position;
}
