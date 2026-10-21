
/*!
 *  @file		scaffolding.h
 *  @brief		足場ブロック
 *  @author     Ryusei Shimizu
 *  @date       2026/04/14
 */

#pragma once

#include "../../stage_object.h"

 /*!
  *  @class      CScaffolding
  *
  *  @brief      足場ブロッククラス
  *
  *  @author     Ryusei Shimizu
  *
  *  @date       2026/04/14
  */
class CScaffolding
    : public IStageObject
{
public:

    /*!
     *  @brief      コンストラクタ
     */
    CScaffolding(void);

    /*!
     *  @brief      デストラクタ
     */
    ~CScaffolding(void);

    /*!
     *  @brief      初期化
     */
    void    Initialize(void) override;
};
