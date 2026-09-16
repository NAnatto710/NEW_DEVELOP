
/*!
 *  @file       utility.cpp
 *  @brief      ユーティリティ
 *  @author     Ryusei Shimizu
 *  @date       2025/12/09
 */

#include "utility.h"

/*
 *	指定した四角形型のオブジェクトとマウスポインタの当たり判定
 */
bool
u_CheckHitMouse(vivid::Vector2 pos, int w, int h)
{
	// マウスポインタの位置の取得
	vivid::Point mouse_point = vivid::mouse::GetCursorPos();

	if (mouse_point.x < pos.x + w
		&& mouse_point.x > pos.x
		&& mouse_point.y < pos.y + h
		&& mouse_point.y > pos.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
 *	指定した四角形型のオブジェクト同士の当たり判定
 */
bool
u_CheckHitObject(vivid::Vector2 pos1, int w1, int h1, vivid::Vector2 pos2, int w2, int h2)
{
	if (pos1.x < pos2.x + w2
		&& pos1.x + w1 > pos2.x
		&& pos1.y < pos2.y + h2
		&& pos1.y + h1 > pos2.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
 *	指定した四角形型のオブジェクト同士の当たり判定（回転対応版）
 */
bool
u_CheckHitObject(vivid::Vector2 pos1, int w1, int h1, vivid::Vector2 pos2, int w2, int h2, float rotataion)
{
	//sizeを割る2にして四角形の真ん中を出す
	float wh = w2 / 2;
	float hh = w2 / 2;

	//角度を出す
	float dir = DEG_TO_RAD(rotataion);

	float c = cos(dir);
	float s = sin(dir);
	//四角形の左上、右上、左下、右下を取る
	vivid::Vector2 localpos2[5] = { {-wh,-hh},{wh,-hh},{wh,hh},{ -wh, hh},{-wh,-hh} };
	vivid::Vector2 centerposition = pos2 + vivid::Vector2{ wh,hh };
	vivid::Vector2 v[5] = { {0,0},{0,0},{0,0},{0,0} ,{0,0} };

	for (int i = 0; i < 5; i++)
	{
		vivid::Vector2 p = centerposition + localpos2[i] - centerposition;

		v[i] = { (p.x * c - p.y * s) + centerposition.x,
							(p.x * s + p.y * c) + centerposition.y };
	}
	vivid::Vector2 c_point[4] = { {pos1},{pos1.x + w1,pos1.y},{pos1.x,pos1.y + h1},{pos1.x + w1,pos1.y + h1} };
	vivid::Vector2 A, B, C, D;
	for (int t = 0; t < 4; t++)
	{
		//一辺(AB)
		A = v[t];
		if (t >= 3)	B = v[0];
		else		B = v[t + 1];

		//playerの辺の数
		for (int b = 0; b < 4; b++)
		{
			//playerの一辺(CD)
			C = c_point[b];
			if (b >= 3)	D = c_point[0];
			else		D = c_point[b + 1];

			//外積
			if (u_CheckCross(A, B, C, D))
			{
				return true;
			}
		}

	}
	return false;
}

/*
 *	指定した円形型のオブジェクト同士の当たり判定
 */
bool
u_CheckHitCircle(vivid::Vector2 pos1, float r1, vivid::Vector2 pos2, float r2)
{
	// 円の中心間の距離の二乗を求める
	float dx = pos1.x - pos2.x;
	float dy = pos1.y - pos2.y;

	float distance = dx * dx + dy * dy;

	float radius = r1 + r2;

	return distance <= radius * radius;
}

/*
 *	指定した円形型のオブジェクトと四角形型のオブジェクトの当たり判定
 */
bool
u_CheckHitCircleObject(vivid::Vector2 pos1, float r1, vivid::Vector2 pos2, int w1, int h1)
{
	// 円の中心から四角形の最も近い点を求める
	float closestX = u_Clamp(pos1.x, pos2.x, pos2.x + w1);
	float closestY = u_Clamp(pos1.y, pos2.y, pos2.y + h1);

	// 最も近い点と円の中心との距離を求める
	float dx = (pos1.x + r1) - closestX;
	float dy = (pos1.y + r1) - closestY;

	float distance = dx * dx + dy * dy;

	return distance <= r1 * r1;
}

/*!
 *  指定した範囲内でランダムな整数値を取得
 */
float
u_RandomInt(float min, float max)
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

/*
 *  値を指定した範囲内に修正する
 */
float
u_Clamp(float value, float min, float max)
{
	if (value < min)
	{
		return min;
	}
	else if (value > max)
	{
		return max;
	}
	else
	{
		return value;
	}
}

/*
 * 辺と辺の交わりの判定
 */
bool
u_CheckCross(vivid::Vector2 A, vivid::Vector2 B, vivid::Vector2 C, vivid::Vector2 D)
{
	vivid::Vector2 AB = B - A;
	vivid::Vector2 AC = C - A;
	vivid::Vector2 AD = D - A;

	vivid::Vector2 CD = D - C;
	vivid::Vector2 CA = A - C;
	vivid::Vector2 CB = B - C;

	// 外積を求める
	double cross1 = vivid::Vector2::Cross(AB, AC);
	double cross2 = vivid::Vector2::Cross(AB, AD);
	double cross3 = vivid::Vector2::Cross(CD, CA);
	double cross4 = vivid::Vector2::Cross(CD, CB);

	// 外積の符号が異なる場合、交差していると判断
	if ((cross1 * cross2 <= 0) && (cross3 * cross4 <= 0))
		return true;

	return false;
}
