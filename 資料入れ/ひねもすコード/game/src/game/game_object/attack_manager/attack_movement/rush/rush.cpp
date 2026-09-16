
/*!
 *  @file       CRush.cpp
 *  @brief      突進クラス
 *  @author     Misaki Kawada
 *  @date       2026/02/17
 */

#include "rush.h"

/*
 *	コンストラクタ
 */
CRush::
CRush()
{
}

/*
 *	突進
 */
vivid::Vector2
CRush::
Rush(vivid::Vector2 velocity, float speed, float direction, float val)
{
	//速度計算
	velocity.x = speed * cos(direction) * val;
	velocity.y = speed * sin(direction) * val;

	return velocity;
}