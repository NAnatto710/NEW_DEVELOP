
/*!
 *  @file       utility.cpp
 *  @brief      ユーティリティ
 *  @author     Ryusei Shimizu
 *  @date       2026/04/16
 */

#include "utility.h"

/*
 *	色のIDに応じた色の値を返す
 */
unsigned int
Utility::
GetColorById(COLOR_ID id)
{
	// 色のIDに応じた色の値を返す
	switch (id)
	{
	case COLOR_ID::WHITE:	return 0xffFFFFFF;	break;
	case COLOR_ID::BLACK:	return 0xff000000;	break;
	case COLOR_ID::RED:		return 0xffFF0000;	break;
	case COLOR_ID::GREEN:	return 0xff00FF00;	break;
	case COLOR_ID::BLUE:	return 0xff0000FF;	break;
	case COLOR_ID::YELLOW:	return 0xffFFFF00;	break;
	case COLOR_ID::BLOWN:	return 0xff8B4513;	break;
	case COLOR_ID::PURPLE:	return 0xff9932CC;	break;
	case COLOR_ID::PINK:	return 0xffFF1493;	break;
	case COLOR_ID::ORENGE:	return 0xffFF8C00;	break;
	case COLOR_ID::GRAY:	return 0xff808080;	break;
	case COLOR_ID::CYAN:	return 0xff00FFFF;	break;
	default:				return 0xffFFFFFF;	break;	// デフォルトは白色
	}
}

/*
 *	色のIDに応じた色の値を返す
 */
unsigned int
Utility::
GetColorByIdAlpha(COLOR_ID id, unsigned int alpha)
{
	// アルファ値を0-255の範囲にクランプ
	if (alpha > 255) alpha = 255;

	unsigned int color = 0x00ffffff;	// デフォルトの色は白色

	// 色のIDに応じた色の値を返す
	switch (id)
	{
	case COLOR_ID::WHITE:	color = 0x00FFFFFF;	break;
	case COLOR_ID::BLACK:	color = 0x00000000;	break;
	case COLOR_ID::RED:		color = 0x00FF0000;	break;
	case COLOR_ID::GREEN:	color = 0x0000FF00;	break;
	case COLOR_ID::BLUE:	color = 0x000000FF;	break;
	case COLOR_ID::YELLOW:	color = 0x00FFFF00;	break;
	case COLOR_ID::BLOWN:	color = 0x008B4513;	break;
	case COLOR_ID::PURPLE:	color = 0x009932CC;	break;
	case COLOR_ID::PINK:	color = 0x00FF1493;	break;
	case COLOR_ID::ORENGE:	color = 0x00FF8C00;	break;
	case COLOR_ID::GRAY:	color = 0x00808080;	break;
	case COLOR_ID::CYAN:	color = 0x0000FFFF;	break;
	default:				color = 0x00FFFFFF;	break;	// デフォルトは白色
	}

	// アルファ値をカラーに反映させる
	color = (alpha << 24) | (color & 0x00ffffff);
	return color;
}

/*
 *	指定した範囲内でランダムな整数を生成する関数
 */
int
Utility::
GetRandomInt(int min, int max)
{
	if (max == min)
	{
		return min;
	}
	if (max < min)
	{
		float temp = min;
		min = max;
		max = temp;
	}

	return rand() % (int)(max - min + 1) + min;
}
