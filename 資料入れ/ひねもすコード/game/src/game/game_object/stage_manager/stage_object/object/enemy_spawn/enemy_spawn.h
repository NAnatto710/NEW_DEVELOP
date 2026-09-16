
/*!
 *  @file       enemy_spawn.h
 *  @brief      エネミー出現クラス
 *  @author     Ryusei Shimizu
 *  @date       2026/02/13
 */

#pragma once

#include "..\..\stage_object.h"

 /*!
  *  @class      CEnemySpawn
  *
  *  @brief      エネミー出現クラス
  *
  *  @author     Ryusei Shimizu
  *
  *  @date       2026/02/13
  */
class CEnemySpawn
    : public IStageObject
{
public:

    /*!
     *  @brief      コンストラクタ
     */
    CEnemySpawn(void);

    /*!
     *  @brief      デストラクタ
     */
    ~CEnemySpawn(void);

    /*!
     *  @brief      初期化
     */
    void                Initialize(void);

    /*!
     *  @brief      更新
     */
    void                Update(void);

    /*!
     *  @brief      敵の出現位置の取得
     */
    vivid::Vector2      GetSpawnPosition(void) const;

private:

    static const float m_enemy_spawn_range;    //!< 敵出現範囲
};
