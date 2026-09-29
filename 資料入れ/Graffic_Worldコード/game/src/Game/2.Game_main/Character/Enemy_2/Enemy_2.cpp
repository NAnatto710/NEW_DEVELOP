#include "Enemy_2.h"

vivid::Vector2 Base::Enemy_2_Position[];

Enemy_2::Enemy_2()
{
}

Enemy_2::~Enemy_2()
{
}

void Enemy_2::Initialize(void)
{
	float angle = 0.0f;

	if (Change_Flg == false)
	{
		Enemy_2_Position[0] = { 1520.0f,-600.0f};
	}
	else
	{
		Enemy_2_Position[0] = { -3000.0f,0.0f};
	}
}

void Enemy_2::Update(int i)
{
	angle += 2.0f; // 1“x‰Á‚¦‚é

	Enemy_2_Position[i].x += (cos(angle * 3.14f / 180.0f) * 5.0f) + Enemy_2_Position[i].x;
	Enemy_2_Position[i].y += (sin(angle * 3.14f / 180.0f) * 5.0f) + Enemy_2_Position[i].y;

	for (int j = 0; j < Object_1_Cnt; j++)
	{
		Enemy_2_Position[i] = object_1.CheckHit_Object(Enemy_2_Position[i], Enemy_2_Radius, j);
	}
	for (int j = 0; j < Object_2_Cnt; j++)
	{
		Enemy_2_Position[i] = object_2.CheckHit_Object(Enemy_2_Position[i], Enemy_2_Radius, j);
	}
	for (int j = 0; j < Object_3_Cnt; j++)
	{
		Enemy_2_Position[i] = object_3.CheckHit_Object(Enemy_2_Position[i], Enemy_2_Radius, j);
	}
	for (int j = 0; j < Object_4_Cnt; j++)
	{
		Enemy_2_Position[i] = object_4.CheckHit_Object(Enemy_2_Position[i], Enemy_2_Radius, j);
	}
	for (int j = 0; j < Object_5_Cnt; j++)
	{
		Enemy_2_Position[i] = object_5.CheckHit_Object(Enemy_2_Position[i], Enemy_2_Radius, j);
	}
	for (int j = 0; j < Object_6_Cnt; j++)
	{
		Enemy_2_Position[i] = object_6.CheckHit_Object(Enemy_2_Position[i], Enemy_2_Radius, j);
	}
	for (int j = 0; j < Object_8_Cnt; j++)
	{
		Enemy_2_Position[i] = object_8.CheckHit_Object(Enemy_2_Position[i], Enemy_2_Radius, j);
	}
	for (int j = 0; j < Object_9_Cnt; j++)
	{
		Enemy_2_Position[i] = object_9.CheckHit_Object(Enemy_2_Position[i], Enemy_2_Radius, j);
	}
	for (int j = 0; j < Object_10_Cnt; j++)
	{
		Enemy_2_Position[i] = object_10.CheckHit_Object(Enemy_2_Position[i], Enemy_2_Radius, j);
	}
}

void Enemy_2::Draw(int i)
{
	vivid::DrawTexture("data\\teki.1 2.png", Enemy_2_Position[i]);
}

void Enemy_2::Finalize(void)
{
}

