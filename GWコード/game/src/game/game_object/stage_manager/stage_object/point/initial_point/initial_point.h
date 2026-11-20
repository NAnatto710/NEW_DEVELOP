
/*!
 *  @file		initial_point.h
 *  @brief		初期位置
 *  @author     Ryusei Shimizu
 *  @date       2026/11/20
 */

#pragma once

#include "..\..\stage_object.h"

class CInitialPoint
    : public IStageObject
{
public:

    /*!
     *  @brief      コンストラクタ
     */
    CInitialPoint (void);

    /*!
     *  @brief      デストラクタ
     */
    ~CInitialPoint (void) = default;

    /*!
     *  @brief      初期化
     */
    void    Initialize (void) override;
};
