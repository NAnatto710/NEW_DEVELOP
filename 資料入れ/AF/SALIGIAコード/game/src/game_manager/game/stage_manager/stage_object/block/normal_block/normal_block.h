
/*!
 *  @file		normal_block.h
 *  @brief		ノーマルブロック
 *  @author     Ryusei Shimizu
 *  @date       2026/04/14
 */

#pragma once

#include "..\..\stage_object.h"

 /*!
  *  @class      CNormalBlock
  *
  *  @brief      ノーマルブロッククラス
  *
  *  @author     Ryusei Shimizu
  *
  *  @date       2026/04/14
  */
class CNormalBlock
    : public IStageObject
{
public:

    /*!
     *  @brief      コンストラクタ
     */
    CNormalBlock(void);

    /*!
     *  @brief      デストラクタ
     */
    ~CNormalBlock(void);

    /*!
     *  @brief      初期化
     */
    void    Initialize(void) override;
};
