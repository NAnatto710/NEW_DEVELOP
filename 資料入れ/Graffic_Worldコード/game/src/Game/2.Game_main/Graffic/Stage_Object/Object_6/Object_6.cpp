#include "Object_6.h"

vivid::Vector2 Base::Object_6_Pos[];

Object_6::Object_6()
{
}

Object_6::~Object_6()
{
}

void Object_6::Initialize(void)
{
	if (Change_Flg == false)
	{
		Object_6_Pos[0] = { 0.0f,-120.0f };

		Object_6_Pos[10] = { -70.0f,0.0f };
		Object_6_Pos[1] = { -70.0f,210.0f };
		Object_6_Pos[2] = { -70.0f,420.0f };
		Object_6_Pos[3] = { -70.0f,630.0f };
		Object_6_Pos[4] = { -70.0f,820.0f };

		Object_6_Pos[5] = { 1280.0f,0.0f };
		Object_6_Pos[6] = { 1280.0f,210.0f };
		Object_6_Pos[7] = { 1280.0f,420.0f };
		Object_6_Pos[8] = { 1280.0f,630.0f };
		Object_6_Pos[9] = { 1280.0f,820.0f };

	}
	else
	{
		Object_6_Pos[0] = { -1000.0f,0.0f };

		Object_6_Pos[10] = { -70.0f,0.0f };
		Object_6_Pos[1] = { -70.0f,210.0f };
		Object_6_Pos[2] = { -70.0f,420.0f };
		Object_6_Pos[3] = { -70.0f,630.0f };
		Object_6_Pos[4] = { -70.0f,820.0f };

		Object_6_Pos[5] = { 1280.0f,0.0f };
		Object_6_Pos[6] = { 1280.0f,210.0f };
		Object_6_Pos[7] = { 1280.0f,420.0f };
		Object_6_Pos[8] = { 1280.0f,630.0f };
		Object_6_Pos[9] = { 1280.0f,820.0f };
	}
}

void Object_6::Update(void)
{
}

void Object_6::Draw(void)
{
	for (int i = 0; i < Object_6_Cnt; i++)
	{
		vivid::DrawTexture("data\\210×70.png", Object_6_Pos[i], 0xff8080ff);
	}
}

void Object_6::Finalize(void)
{
}

vivid::Vector2 Object_6::CheckHit_Block(vivid::Vector2 block_pos, int block_width, int block_height)
{
	bool result_A = block_pos.y > Object_6_Pos[0].y - block_height;
	bool result_B = block_pos.y < Object_6_Pos[0].y + Object_6_Height;
	bool result_C = block_pos.x > Object_6_Pos[0].x - block_width;
	bool result_D = block_pos.x < Object_6_Pos[0].x + Object_6_Width;

	if (result_A && result_B && result_C && result_D)
	{
		vivid::Vector2 POS = { block_pos.x , block_pos.y };
		vivid::Vector2 B_Center = { block_pos.x + block_width / 2 , block_pos.y + block_height / 2 };
		O_Center[0] = { Object_6_Pos[0].x + Object_6_Width / 2 , Object_6_Pos[0].y + Object_6_Height / 2 };

		if (B_Center.y <= Object_6_Pos[0].y || B_Center.y >= Object_6_Pos[0].y + Object_6_Height)
		{
			//地面判定
			if (B_Center.y < O_Center[0].y)
			{
				POS.y = Object_6_Pos[0].y - block_height;
				return POS;
			}
			//天井判定
			if (B_Center.y > O_Center[0].y)
			{
				POS.y = Object_6_Pos[0].y + Object_6_Height;
				return POS;
			}
		}
		if (B_Center.x <= Object_6_Pos[0].x || B_Center.x >= Object_6_Pos[0].x + Object_6_Width)
		{
			//左の壁判定
			if (B_Center.x < O_Center[0].x)
			{
				POS.x = Object_6_Pos[0].x - block_width;
				return POS;
			}
			//右の壁判定
			if (B_Center.x > O_Center[0].x)
			{
				POS.x = Object_6_Pos[0].x + Object_6_Width;
				return POS;
			}
		}
		//オブジェクトの中にブロックが入ったとき
		if (Object_6_Pos[0].x < B_Center.x < Object_6_Pos[0].x + Object_6_Width ||
			Object_6_Pos[0].y < B_Center.y < Object_6_Pos[0].y + Object_6_Height)
		{
			POS.y = Object_6_Pos[0].y - block_height;
			return POS;
		}
	}
	else
	{
		return block_pos;
	}
}

vivid::Vector2 Object_6::CheckHit_Object(vivid::Vector2 player_pos, float player_radius, int num)
{
	//CIRCLEの中心点を求める
	vivid::Vector2 circle_center = { player_pos.x + player_radius,player_pos.y + player_radius };

	//点と短形の判定その１
	bool result_h = circle_center.x > Object_6_Pos[num].x - player_radius
		&& circle_center.x < Object_6_Pos[num].x + Object_6_Width + player_radius
		&& circle_center.y > Object_6_Pos[num].y
		&& circle_center.y < Object_6_Pos[num].y + Object_6_Height;

	//点と短形の判定その２
	bool result_v = circle_center.x > Object_6_Pos[num].x
		&& circle_center.x < Object_6_Pos[num].x + Object_6_Width
		&& circle_center.y > Object_6_Pos[num].y - player_radius
		&& circle_center.y < Object_6_Pos[num].y + Object_6_Height + player_radius;

	//点と円の判定その１
	//BOXの左上
	vivid::Vector2 v = circle_center - Object_6_Pos[num];
	bool result_lu = v.Length() <= player_radius;//Length = sqrt
	//BOXの右上
	v = circle_center - vivid::Vector2(Object_6_Pos[num].x + Object_6_Width, Object_6_Pos[num].y);
	bool result_ru = v.Length() <= player_radius;
	//BOXの左下
	v = circle_center - vivid::Vector2(Object_6_Pos[num].x, Object_6_Pos[num].y + Object_6_Height);
	bool result_Id = v.Length() <= player_radius;
	//BOXの右下
	v = circle_center - vivid::Vector2(Object_6_Pos[num].x + Object_6_Width, Object_6_Pos[num].y + Object_6_Height);
	bool result_rd = v.Length() <= player_radius;

	//どこかしらで真であれば当たったとみなされる
	if (result_h || result_v || result_lu || result_Id || result_ru || result_rd)
	{
		vivid::Vector2 POS = { player_pos.x , player_pos.y };
		vivid::Vector2 P_Center = { player_pos.x + player_radius , player_pos.y + player_radius };
		vivid::Vector2 O_Center = { Object_6_Pos[num].x + Object_6_Width / 2 , Object_6_Pos[num].y + Object_6_Height / 2 };

		if (P_Center.y <= Object_6_Pos[num].y || P_Center.y >= Object_6_Pos[num].y + Object_6_Height)
		{
			//地面判定
			if (P_Center.y < O_Center.y)
			{
				POS.y = Object_6_Pos[num].y - 64;
				return POS;
			}
			//天井判定
			if (P_Center.y > O_Center.y)
			{
				POS.y = Object_6_Pos[num].y + Object_6_Height;
				return POS;
			}
		}
		if (P_Center.x <= Object_6_Pos[num].x || P_Center.x >= Object_6_Pos[num].x + Object_6_Width)
		{
			//左の壁判定
			if (P_Center.x < O_Center.x)
			{
				POS.x = Object_6_Pos[num].x - 64;
				return POS;
			}
			//右の壁判定
			if (P_Center.x > O_Center.x)
			{
				POS.x = Object_6_Pos[num].x + Object_6_Width;
				return POS;
			}
		}
		//オブジェクトの中にブロックが入ったとき
		if (Object_6_Pos[num].x < P_Center.x < Object_6_Pos[num].x + Object_6_Width ||
			Object_6_Pos[num].y < P_Center.y < Object_6_Pos[num].y + Object_6_Height)
		{
			POS.y = Object_6_Pos[num].y - 64;
			return POS;
		}
	}
	else
	{
		return player_pos;
	}
}
