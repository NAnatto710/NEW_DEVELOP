
/*!
 *  @file		normal_block.h
 *  @brief		ノーマルブロック
 *  @author     Ryusei Shimizu
 *  @date       2026/10/13
 */

#pragma once

#include "..\..\stage_object.h"

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
