
/*!
 *  @file		spawn_point3.h
 *  @brief		スポーンポイント３
 *  @author     Ryusei Shimizu
 *  @date       2026/04/17
 */

#pragma once

#include "../../stage_object.h"

 /*!
  *  @class      CSpawnPoint3
  *
  *  @brief      スポーンポイント３クラス
  *
  *  @author     Ryusei Shimizu
  *
  *  @date       2026/04/17
  */
class CSpawnPoint3
    : public IStageObject
{
public:

    /*!
     *  @brief      コンストラクタ
     */
    CSpawnPoint3(void);

    /*!
     *  @brief      デストラクタ
     */
    ~CSpawnPoint3();

    /*!
     *  @brief      初期化
     */
    void    Initialize(void) override;
};
