
/*!
 *  @file		under_block.h
 *  @brief		地中のブロック
 *  @author     Ryusei Shimizu
 *  @date       2026/04/14
 */

#pragma once

#include "..\..\stage_object.h"

 /*!
  *  @class      CUnderBlock
  *
  *  @brief      地中のブロッククラス
  *
  *  @author     Ryusei Shimizu
  *
  *  @date       2026/04/14
  */
class CUnderBlock
    : public IStageObject
{
public:

    /*!
     *  @brief      コンストラクタ
     */
    CUnderBlock(void);

    /*!
     *  @brief      デストラクタ
     */
    ~CUnderBlock(void);

    /*!
     *  @brief      初期化
     */
    void    Initialize(void) override;
};
