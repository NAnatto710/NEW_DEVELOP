
/*!
 *  @file		spawn_point1.h
 *  @brief		スポーンポイント１
 *  @author     Ryusei Shimizu
 *  @date       2026/04/17
 */

#pragma once

#include "../../stage_object.h"

 /*!
  *  @class      CSpawnPoint1
  *
  *  @brief      スポーンポイント１クラス
  *
  *  @author     Ryusei Shimizu
  *
  *  @date       2026/04/17
  */
class CSpawnPoint1
    : public IStageObject
{
public:

    /*!
     *  @brief      コンストラクタ
     */
    CSpawnPoint1(void);

    /*!
     *  @brief      デストラクタ
     */
    ~CSpawnPoint1();

    /*!
     *  @brief      初期化
     */
    void    Initialize(void) override;
};
