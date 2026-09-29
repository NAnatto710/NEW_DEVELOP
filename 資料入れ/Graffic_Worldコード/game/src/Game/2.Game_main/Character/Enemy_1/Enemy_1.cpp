#include "Enemy_1.h"

vivid::Vector2 Base::Enemy_1_Position[];

Enemy_1::Enemy_1()
{
}

Enemy_1::~Enemy_1()
{
}

void Enemy_1::Initialize(void)
{
	if (Change_Flg == false)
	{
		Enemy_1_Position[0] = { 1600.0f,150.0f };
	}
	else
	{
		Enemy_1_Position[0] = { -1600.0f,150.0f };
	}

	for (int i = 0; i < E_1_Cnt; i++)
	{
		E_1_Jp[i] = 45;

		Time[i] = 0;
		Time_cnt[i] = 0;
	}
}

void Enemy_1::Update(int i)
{
	//左右に移動する処理
	Time_cnt[i] += 1;
	if (Time_cnt[i] % 180 == 0)
		Time[i] += 1;
	switch (Time[i] % 2)
	{
	case 0:
		Enemy_1_Position[i].x -= 1;
		break;
	case 1:
		Enemy_1_Position[i].x += 1;
		break;
	default:
		break;
	}

	//地面につくとジャンプをする
	if (Enemy_1_Position[i].y == object_1.Get_Object_1_Pos_Y(i) - 64)
		Jump(i);
	if (Enemy_1_Position[i].y == object_2.Get_Object_2_Pos_Y(i) - 64)
		Jump(i);
	if (Enemy_1_Position[i].y == object_3.Get_Object_3_Pos_Y(i) - 64)
		Jump(i);
	if (Enemy_1_Position[i].y == object_4.Get_Object_4_Pos_Y(i) - 64)
		Jump(i);
	if (Enemy_1_Position[i].y == object_5.Get_Object_5_Pos_Y(i) - 64)
		Jump(i);
	if (Enemy_1_Position[i].y == object_6.Get_Object_6_Pos_Y(i) - 64)
		Jump(i);
	if (Enemy_1_Position[i].y == object_8.Get_Object_8_Pos_Y(i) - 64)
		Jump(i);
	if (Enemy_1_Position[i].y == object_9.Get_Object_9_Pos_Y(i) - 64)
		Jump(i);
	if (Enemy_1_Position[i].y == object_10.Get_Object_10_Pos_Y(i) - 64)
		Jump(i);
	


	//ジャンプのための処理
	if (45 < E_1_Jp[i])
		E_1_Jp[i] -= 1;

	Enemy_1_Position[i].y += sin(E_1_Jp[i] * 3.14f / 130.0f) * 10.0f;


	//ステージ中のオブジェクトとの当たり判定をする関数
	for (int j = 0; j < Object_1_Cnt; j++)
	{
		Enemy_1_Position[i] = object_1.CheckHit_Object(Enemy_1_Position[i], Enemy_1_Radius, j);
	}
	//ステージ中のオブジェクトとの当たり判定をする関数
	for (int j = 0; j < Object_2_Cnt; j++)
	{
		Enemy_1_Position[i] = object_2.CheckHit_Object(Enemy_1_Position[i], Enemy_1_Radius, j);
	}
	//ステージ中のオブジェクトとの当たり判定をする関数
	for (int j = 0; j < Object_3_Cnt; j++)
	{
		Enemy_1_Position[i] = object_3.CheckHit_Object(Enemy_1_Position[i], Enemy_1_Radius, j);
	}
	//ステージ中のオブジェクトとの当たり判定をする関数
	for (int j = 0; j < Object_4_Cnt; j++)
	{
		Enemy_1_Position[i] = object_4.CheckHit_Object(Enemy_1_Position[i], Enemy_1_Radius, j);
	}
	//ステージ中のオブジェクトとの当たり判定をする関数
	for (int j = 0; j < Object_5_Cnt; j++)
	{
		Enemy_1_Position[i] = object_5.CheckHit_Object(Enemy_1_Position[i], Enemy_1_Radius, j);
	}
	//ステージ中のオブジェクトとの当たり判定をする関数
	for (int j = 0; j < Object_6_Cnt; j++)
	{
		Enemy_1_Position[i] = object_6.CheckHit_Object(Enemy_1_Position[i], Enemy_1_Radius, j);
	}
	//ステージ中のオブジェクトとの当たり判定をする関数
	for (int j = 0; j < Object_8_Cnt; j++)
	{
		Enemy_1_Position[i] = object_8.CheckHit_Object(Enemy_1_Position[i], Enemy_1_Radius, j);
	}
	//ステージ中のオブジェクトとの当たり判定をする関数
	for (int j = 0; j < Object_9_Cnt; j++)
	{
		Enemy_1_Position[i] = object_9.CheckHit_Object(Enemy_1_Position[i], Enemy_1_Radius, j);
	}
	//ステージ中のオブジェクトとの当たり判定をする関数
	for (int j = 0; j < Object_10_Cnt; j++)
	{
		Enemy_1_Position[i] = object_10.CheckHit_Object(Enemy_1_Position[i], Enemy_1_Radius, j);
	}
}

void Enemy_1::Draw(int i)
{
	vivid::DrawTexture("data\\enemy.png",Enemy_1_Position[i]);
}

void Enemy_1::Finalize(void)
{
}