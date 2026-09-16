
/*!
 *  @file		spawn_point2.h
 *  @brief		スポーンポイント２
 *  @author     Ryusei Shimizu
 *  @date       2026/04/17
 */

#pragma once

#include "../../stage_object.h"

 /*!
  *  @class      CSpawnPoint2
  *
  *  @brief      スポーンポイント２クラス
  *
  *  @author     Ryusei Shimizu
  *
  *  @date       2026/04/17
  */
class CSpawnPoint2
    : public IStageObject
{
public:

    /*!
     *  @brief      コンストラクタ
     */
    CSpawnPoint2(void);

    /*!
     *  @brief      デストラクタ
     */
    ~CSpawnPoint2();

    /*!
     *  @brief      初期化
     */
    void    Initialize(void) override;
};
