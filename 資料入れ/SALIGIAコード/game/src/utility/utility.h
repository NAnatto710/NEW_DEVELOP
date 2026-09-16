
/*!
 *  @file       utility.h
 *  @brief      ユーティリティ
 *  @author     Ryusei Shimizu
 *  @date       2026/03/20
 */

#pragma once

#include "vivid.h"

/*!
 *  @brief      円周率
 */
#define PI 3.141592653589793f

/*!
 *  @brief      誤差
 */
#define EPSILON 0.00001f

/*!
 *  @brief      角度をラジアン値に変換
 *
 *  @param[in]  d				角度
 *
 *  @return     ラジアン値
 */
#define DEG_TO_RAD(d) (float)((d) * PI) / 180.0f

/*!
 *	@brief      ２つの値の大きい方を取得
 *
 *	@param[in]  a				値1
 *	@param[in]  b				値2
 *
 *	@return     大きい方の値
 */
#define MAX(a,b) ((a)>(b)?(a):(b))

/*!
 *	@brief      ２つの値の小さい方を取得
 *
 *	@param[in]  a				値1
 *	@param[in]  b				値2
 *
 *	@return     小さい方の値
 */
#define MIN(a,b) ((a)<(b)?(a):(b))

 /*!
  *  @brief      指定した範囲内で値を修正
  *
  *  @param[in]  value			値
  *  @param[in]  min			最小値
  *  @param[in]  max			最大値
  *
  *  @return     修正後の値
  */
#define CLAMP(Value, Min, Max) ((Value) < (Min) ? (Min) : ((Value) > (Max) ? (Max) : (Value)))

/*!
 *  @brief      色のID
 */
enum class COLOR_ID
{
	WHITE,		//!< 白色
	BLACK,		//!< 黒色
	RED,		//!< 赤色
	GREEN,		//!< 緑色
	BLUE,		//!< 青色
	YELLOW,		//!< 黄色
	BLOWN,		//!< 茶色
	PURPLE,		//!< 紫色
	PINK,		//!< 桃色
	ORENGE,		//!< 橙色
	GRAY,		//!< 灰色
	CYAN,		//!< 碧色
};


namespace Utility
{
	/*!
	 *  @brief      色をIDの名前で取得できる関数
	 *
	 *  @param[in]  id				色のID
	 *
	 *  @return     色の値
	 */
	unsigned int	GetColorById(COLOR_ID id);

	/*!
	 *  @brief      色をIDの名前で取得できる関数
	 *
	 *  @param[in]  id				色のID
	 *  @param[in]  alpha			アルファ値(0~255)
	 *
	 *  @return     色の値
	 */
	unsigned int	GetColorByIdAlpha(COLOR_ID id, unsigned int alpha = 0xFF);

	/*!
	 *  @brief      指定した範囲内でランダムな整数を生成する関数
	 *
	 *  @param[in]  min				最小値
	 *  @param[in]  max				最大値
	 *
	 *  @return     ランダムな整数
	 */	
	int				GetRandomInt(int min, int max);
};