
/*!
 *  @file       feed.h
 *  @brief      エサクラス
 *  @author     Ryusei Shimizu
 *  @date       2026/01/28
 */

#pragma once

#include "..\..\stage_object.h"

 /*!
  *  @class      CFeed
  *
  *  @brief      エサクラス
  *
  *  @author     Ryusei Shimizu
  *
  *  @date       2026/01/28
  */
class CFeed
    : public IStageObject
{
public:

    /*!
     *  @brief      コンストラクタ
     */
    CFeed(void);

    /*!
     *  @brief      デストラクタ
     */
    ~CFeed(void);

    /*!
     *  @brief      初期化
     */
    void    Initialize(void);

    /*!
     *  @brief      更新
     */
    void    Update(void);
};
