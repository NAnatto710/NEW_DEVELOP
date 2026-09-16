
/*!
 *  @file		spawn_point4.h
 *  @brief		スポーンポイント４
 *  @author     Ryusei Shimizu
 *  @date       2026/04/17
 */

#pragma once

#include "../../stage_object.h"

 /*!
  *  @class      CSpawnPoint4
  *
  *  @brief      スポーンポイント４クラス
  *
  *  @author     Ryusei Shimizu
  *
  *  @date       2026/04/17
  */
class CSpawnPoint4
    : public IStageObject
{
public:

    /*!
     *  @brief      コンストラクタ
     */
    CSpawnPoint4(void);

    /*!
     *  @brief      デストラクタ
     */
    ~CSpawnPoint4();

    /*!
     *  @brief      初期化
     */
    void    Initialize(void) override;
};
