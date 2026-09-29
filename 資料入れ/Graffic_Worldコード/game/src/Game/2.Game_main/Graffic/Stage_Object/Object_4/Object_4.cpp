#include "Object_4.h"

vivid::Vector2 Base::Object_4_Pos[];


Object_4::Object_4()
{
}

Object_4::~Object_4()
{
}

void Object_4::Initialize(void)
{
	if (Change_Flg == false)
	{
		Object_4_Pos[0] = { 1696.0f,390.0f };

		Object_4_Pos[1] = { 1580.0f,-40.0f };

		Object_4_Pos[2] = { 252.0f,-40.0f };

		Object_4_Pos[3] = { 1420.0f,-570.0f };
	}
	else
	{
		for (int i = 0; i < Object_4_Cnt; i++)
		{
			Object_4_Pos[i] = { -1000.0f, 0.0f };
		}
	}
}

void Object_4::Update(void)
{
}

void Object_4::Draw(void)
{
	for (int i = 0; i < Object_4_Cnt; i++)
	{
		vivid::DrawTexture("data\\60×210.png", Object_4_Pos[i], 0xff8080ff);
	}
}

void Object_4::Finalize(void)
{
}

vivid::Vector2 Object_4::CheckHit_Block(vivid::Vector2 block_pos, int block_width, int block_height, int num)
{
	bool result_A = block_pos.y > Object_4_Pos[num].y - block_height;
	bool result_B = block_pos.y < Object_4_Pos[num].y + Object_4_Height;
	bool result_C = block_pos.x > Object_4_Pos[num].x - block_width;
	bool result_D = block_pos.x < Object_4_Pos[num].x + Object_4_Width;

	if (result_A && result_B && result_C && result_D)
	{
		vivid::Vector2 POS = { block_pos.x , block_pos.y };
		vivid::Vector2 B_Center = { block_pos.x + block_width / 2 , block_pos.y + block_height / 2 };
		O_Center[num] = { Object_4_Pos[num].x + Object_4_Width / 2 , Object_4_Pos[num].y + Object_4_Height / 2 };

		if (B_Center.y <= Object_4_Pos[num].y || B_Center.y >= Object_4_Pos[num].y + Object_4_Height)
		{
			//地面判定
			if (B_Center.y < O_Center[num].y)
			{
				POS.y = Object_4_Pos[num].y - block_height;
				return POS;
			}
			//天井判定
			if (B_Center.y > O_Center[num].y)
			{
				POS.y = Object_4_Pos[num].y + Object_4_Height;
				return POS;
			}
		}
		if (B_Center.x <= Object_4_Pos[num].x || B_Center.x >= Object_4_Pos[num].x + Object_4_Width)
		{
			//左の壁判定
			if (B_Center.x < O_Center[num].x)
			{
				POS.x = Object_4_Pos[num].x - block_width;
				return POS;
			}
			//右の壁判定
			if (B_Center.x > O_Center[num].x)
			{
				POS.x = Object_4_Pos[num].x + Object_4_Width;
				return POS;
			}
		}
		//オブジェクトの中にブロックが入ったとき
		if (Object_4_Pos[num].x < B_Center.x < Object_4_Pos[num].x + Object_4_Width ||
			Object_4_Pos[num].y < B_Center.y < Object_4_Pos[num].y + Object_4_Height)
		{
			POS.y = Object_4_Pos[num].y - block_height;
			return POS;
		}
	}
	else
	{
		return block_pos;
	}
}

vivid::Vector2 Object_4::CheckHit_Object(vivid::Vector2 player_pos, float player_radius, int num)
{
	//CIRCLEの中心点を求める
	vivid::Vector2 circle_center = { player_pos.x + player_radius,player_pos.y + player_radius };

	//点と短形の判定その１
	bool result_h = circle_center.x > Object_4_Pos[num].x - player_radius
		&& circle_center.x < Object_4_Pos[num].x + Object_4_Width + player_radius
		&& circle_center.y > Object_4_Pos[num].y
		&& circle_center.y < Object_4_Pos[num].y + Object_4_Height;

	//点と短形の判定その２
	bool result_v = circle_center.x > Object_4_Pos[num].x
		&& circle_center.x < Object_4_Pos[num].x + Object_4_Width
		&& circle_center.y > Object_4_Pos[num].y - player_radius
		&& circle_center.y < Object_4_Pos[num].y + Object_4_Height + player_radius;

	//点と円の判定その１
	//BOXの左上
	vivid::Vector2 v = circle_center - Object_4_Pos[num];
	bool result_lu = v.Length() <= player_radius;//Length = sqrt
	//BOXの右上
	v = circle_center - vivid::Vector2(Object_4_Pos[num].x + Object_4_Width, Object_4_Pos[num].y);
	bool result_ru = v.Length() <= player_radius;
	//BOXの左下
	v = circle_center - vivid::Vector2(Object_4_Pos[num].x, Object_4_Pos[num].y + Object_4_Height);
	bool result_Id = v.Length() <= player_radius;
	//BOXの右下
	v = circle_center - vivid::Vector2(Object_4_Pos[num].x + Object_4_Width, Object_4_Pos[num].y + Object_4_Height);
	bool result_rd = v.Length() <= player_radius;

	//どこかしらで真であれば当たったとみなされる
	if (result_h || result_v || result_lu || result_Id || result_ru || result_rd)
	{
		vivid::Vector2 POS = { player_pos.x , player_pos.y };
		vivid::Vector2 P_Center = { player_pos.x + player_radius , player_pos.y + player_radius };
		vivid::Vector2 O_Center = { Object_4_Pos[num].x + Object_4_Width / 2 , Object_4_Pos[num].y + Object_4_Height / 2 };

		if (P_Center.y <= Object_4_Pos[num].y || P_Center.y >= Object_4_Pos[num].y + Object_4_Height)
		{
			//地面判定
			if (P_Center.y < O_Center.y)
			{
				POS.y = Object_4_Pos[num].y - 64;
				return POS;
			}
			//天井判定
			if (P_Center.y > O_Center.y)
			{
				POS.y = Object_4_Pos[num].y + Object_4_Height;
				return POS;
			}
		}
		if (P_Center.x <= Object_4_Pos[num].x || P_Center.x >= Object_4_Pos[num].x + Object_4_Width)
		{
			//左の壁判定
			if (P_Center.x < O_Center.x)
			{
				POS.x = Object_4_Pos[num].x - 64;
				return POS;
			}
			//右の壁判定
			if (P_Center.x > O_Center.x)
			{
				POS.x = Object_4_Pos[num].x + Object_4_Width;
				return POS;
			}
		}
		//オブジェクトの中にブロックが入ったとき
		if (Object_4_Pos[num].x < P_Center.x < Object_4_Pos[num].x + Object_4_Width ||
			Object_4_Pos[num].y < P_Center.y < Object_4_Pos[num].y + Object_4_Height)
		{
			POS.y = Object_4_Pos[num].y - 64;
			return POS;
		}
	}
	else
	{
		return player_pos;
	}
}
