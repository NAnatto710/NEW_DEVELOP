#include "Object_2.h"

vivid::Vector2 Base::Object_2_Pos[];


Object_2::Object_2()
{
}

Object_2::~Object_2()
{
}

void Object_2::Initialize(void)
{
	if (Change_Flg == false)
	{
		Object_2_Pos[0] = { 588.0f,630.0f };


		Object_2_Pos[1] = { 862.0f,630.0f };


		Object_2_Pos[2] = { 932.0f,630.0f };
		Object_2_Pos[3] = { 932.0f,570.0f };


		Object_2_Pos[4] = { 1280.0f,630.0f };

		Object_2_Pos[5] = { 1350.0f,630.0f };
		Object_2_Pos[6] = { 1350.0f,570.0f };

		Object_2_Pos[7] = { 1420.0f,630.0f };
		Object_2_Pos[8] = { 1420.0f,570.0f };
		Object_2_Pos[9] = { 1420.0f,510.0f };


		Object_2_Pos[10] = { 1766.0f,630.0f };


		Object_2_Pos[11] = { 2140.0f,630.0f };
		Object_2_Pos[12] = { 2140.0f,570.0f };
		Object_2_Pos[13] = { 2140.0f,510.0f };

		Object_2_Pos[14] = { 2210.0f,630.0f };
		Object_2_Pos[15] = { 2210.0f,570.0f };
		Object_2_Pos[16] = { 2210.0f,510.0f };

		Object_2_Pos[17] = { 2280.0f,630.0f };
		Object_2_Pos[18] = { 2280.0f,570.0f };
		Object_2_Pos[19] = { 2280.0f,510.0f };
		Object_2_Pos[20] = { 2280.0f,450.0f };
		Object_2_Pos[21] = { 2280.0f,390.0f };

		Object_2_Pos[22] = { 2350.0f,630.0f };
		Object_2_Pos[23] = { 2350.0f,570.0f };
		Object_2_Pos[24] = { 2350.0f,510.0f };
		Object_2_Pos[25] = { 2350.0f,450.0f };
		Object_2_Pos[26] = { 2350.0f,390.0f };

		Object_2_Pos[27] = { 2420.0f,630.0f };
		Object_2_Pos[28] = { 2420.0f,570.0f };
		Object_2_Pos[29] = { 2420.0f,510.0f };
		Object_2_Pos[30] = { 2420.0f,450.0f };
		Object_2_Pos[31] = { 2420.0f,390.0f };
		Object_2_Pos[32] = { 2420.0f,330.0f };
		Object_2_Pos[33] = { 2420.0f,270.0f };

		Object_2_Pos[34] = { 2490.0f,630.0f };
		Object_2_Pos[35] = { 2490.0f,570.0f };
		Object_2_Pos[36] = { 2490.0f,510.0f };
		Object_2_Pos[37] = { 2490.0f,450.0f };
		Object_2_Pos[38] = { 2490.0f,390.0f };
		Object_2_Pos[39] = { 2490.0f,330.0f };

		//中段

		Object_2_Pos[41] = { 586.0f ,90.0f };
		Object_2_Pos[42] = { 586.0f ,150.0f };

		Object_2_Pos[43] = { 656.0f,150.0f };

		Object_2_Pos[44] = { 936.0f,150.0f };

		Object_2_Pos[45] = { 1986.0f,150.0f };
		Object_2_Pos[46] = { -1916.0f,150.0f };
		Object_2_Pos[47] = { 1846.0f,150.0f };


		//上段
		Object_2_Pos[48] = { 476.0f,-330.0f };

		Object_2_Pos[49] = { 546.0f,-330.0f };
		Object_2_Pos[50] = { 546.0f,-390.0f };

		Object_2_Pos[51] = { 616.0f,-330.0f };
		Object_2_Pos[52] = { 616.0f,-390.0f };

		Object_2_Pos[53] = { 686.0f,-330.0f };
		Object_2_Pos[54] = { 686.0f,-390.0f };
		Object_2_Pos[55] = { 686.0f,-450.0f };

		Object_2_Pos[56] = { 884.0f,-330.0f };

		Object_2_Pos[57] = { 1082.0f,-330.0f };
		Object_2_Pos[58] = { 1082.0f,-390.0f };
		Object_2_Pos[59] = { 1082.0f,-450.0f };
		Object_2_Pos[40] = { 1082.0f,-510.0f };

		Object_2_Pos[60] = { 1280.0f,-330.0f };

		Object_2_Pos[61] = { 1630.0f,-330.0f };

		Object_2_Pos[62] = { 1700.0f,-330.0f };
		Object_2_Pos[63] = { 1700.0f,-390.0f };

		Object_2_Pos[64] = { 1770.0f,-330.0f };
		Object_2_Pos[65] = { 1770.0f,-390.0f };
		Object_2_Pos[66] = { 1770.0f,-450.0f };

		Object_2_Pos[67] = { 1840.0f,-330.0f };
		Object_2_Pos[68] = { 1840.0f,-390.0f };
		Object_2_Pos[69] = { 1840.0f,-450.0f };
		Object_2_Pos[70] = { 1840.0f,-510.0f };

		Object_2_Pos[71] = { 1910.0f,-330.0f };
		Object_2_Pos[72] = { 1910.0f,-390.0f };
		Object_2_Pos[73] = { 1910.0f,-450.0f };
		Object_2_Pos[74] = { 1910.0f,-510.0f };
		Object_2_Pos[75] = { 1910.0f,-570.0f };
	}
	else
	{
		Object_2_Pos[0] = { 748.0f, Ground_Line - Object_2_Height * 2 };
		Object_2_Pos[1] = { 748.0f, Ground_Line - Object_2_Height };
		Object_2_Pos[3] = { 748.0f, Ground_Line - Object_2_Height * 3 };

		Object_2_Pos[10] = { 688.0f, Ground_Line - Object_2_Height * 2 };
		Object_2_Pos[11] = { 688.0f, Ground_Line - Object_2_Height };
		Object_2_Pos[12] = { 688.0f, Ground_Line - Object_2_Height * 3 };

		Object_2_Pos[2] = { 946.0f, Ground_Line - Object_2_Height };

		Object_2_Pos[4] = { 1680.0f, Ground_Line - Object_2_Height };
		Object_2_Pos[5] = { 1750.0f, Ground_Line - Object_2_Height };
		Object_2_Pos[6] = { 1750.0f, Ground_Line - Object_2_Height - Object_2_Height };

		Object_2_Pos[7] = { 2020.0f, Ground_Line - Object_2_Height };
		Object_2_Pos[8] = { 2090.0f, Ground_Line - Object_2_Height };
		Object_2_Pos[9] = { 2090.0f, Ground_Line - Object_2_Height - Object_2_Height };

		for (int i = 13; i <= Object_2_Cnt; i++)
		{
			Object_2_Pos[i] = { -1000.0f, 0.0f };
		}
	}

}

void Object_2::Update(void)
{
}

void Object_2::Draw(void)
{
	for (int i = 0; i < Object_2_Cnt; i++)
	{
		vivid::DrawTexture("data\\60×70 1.png", Object_2_Pos[i], 0xff8080ff);
	}
}

void Object_2::Finalize(void)
{
}

vivid::Vector2 Object_2::CheckHit_Block(vivid::Vector2 block_pos, int block_width, int block_height, int num)
{
	bool result_A = block_pos.y > Object_2_Pos[num].y - block_height;
	bool result_B = block_pos.y < Object_2_Pos[num].y + Object_2_Height;
	bool result_C = block_pos.x > Object_2_Pos[num].x - block_width;
	bool result_D = block_pos.x < Object_2_Pos[num].x + Object_2_Width;

	if (result_A && result_B && result_C && result_D)
	{
		vivid::Vector2 POS = { block_pos.x , block_pos.y };
		vivid::Vector2 B_Center = { block_pos.x + block_width / 2 , block_pos.y + block_height / 2 };
		O_Center[num] = { Object_2_Pos[num].x + Object_1_Width / 2 , Object_2_Pos[num].y + Object_2_Height / 2 };

		if (B_Center.y <= Object_2_Pos[num].y || B_Center.y >= Object_2_Pos[num].y + Object_2_Height)
		{
			//地面判定
			if (B_Center.y < O_Center[num].y)
			{
				POS.y = Object_2_Pos[num].y - block_height;
				return POS;
			}
			//天井判定
			if (B_Center.y > O_Center[num].y)
			{
				POS.y = Object_2_Pos[num].y + Object_2_Height;
				return POS;
			}
		}
		if (B_Center.x <= Object_2_Pos[num].x || B_Center.x >= Object_2_Pos[num].x + Object_2_Width)
		{
			//左の壁判定
			if (B_Center.x < O_Center[num].x)
			{
				POS.x = Object_2_Pos[num].x - block_width;
				return POS;
			}
			//右の壁判定
			if (B_Center.x > O_Center[num].x)
			{
				POS.x = Object_2_Pos[num].x + Object_2_Width;
				return POS;
			}
		}
		//オブジェクトの中にブロックが入ったとき
		if (Object_2_Pos[num].x < B_Center.x < Object_2_Pos[num].x + Object_2_Width ||
			Object_2_Pos[num].y < B_Center.y < Object_2_Pos[num].y + Object_2_Height)
		{
			POS.y = Object_2_Pos[num].y - block_height;
			return POS;
		}
	}
	else
	{
		return block_pos;
	}
}

vivid::Vector2 Object_2::CheckHit_Object(vivid::Vector2 player_pos, float player_radius, int num)
{
	//CIRCLEの中心点を求める
	vivid::Vector2 circle_center = { player_pos.x + player_radius,player_pos.y + player_radius };

	//点と短形の判定その１
	bool result_h = circle_center.x > Object_2_Pos[num].x - player_radius
		&& circle_center.x < Object_2_Pos[num].x + Object_2_Width + player_radius
		&& circle_center.y > Object_2_Pos[num].y
		&& circle_center.y < Object_2_Pos[num].y + Object_2_Height;

	//点と短形の判定その２
	bool result_v = circle_center.x > Object_2_Pos[num].x
		&& circle_center.x < Object_2_Pos[num].x + Object_2_Width
		&& circle_center.y > Object_2_Pos[num].y - player_radius
		&& circle_center.y < Object_2_Pos[num].y + Object_2_Height + player_radius;

	//点と円の判定その１
	//BOXの左上
	vivid::Vector2 v = circle_center - Object_2_Pos[num];
	bool result_lu = v.Length() <= player_radius;//Length = sqrt
	//BOXの右上
	v = circle_center - vivid::Vector2(Object_2_Pos[num].x + Object_2_Width, Object_2_Pos[num].y);
	bool result_ru = v.Length() <= player_radius;
	//BOXの左下
	v = circle_center - vivid::Vector2(Object_2_Pos[num].x, Object_2_Pos[num].y + Object_2_Height);
	bool result_Id = v.Length() <= player_radius;
	//BOXの右下
	v = circle_center - vivid::Vector2(Object_2_Pos[num].x + Object_2_Width, Object_2_Pos[num].y + Object_2_Height);
	bool result_rd = v.Length() <= player_radius;

	//どこかしらで真であれば当たったとみなされる
	if (result_h || result_v || result_lu || result_Id || result_ru || result_rd)
	{
		vivid::Vector2 POS = { player_pos.x , player_pos.y };
		vivid::Vector2 P_Center = { player_pos.x + player_radius , player_pos.y + player_radius };
		vivid::Vector2 O_Center = { Object_2_Pos[num].x + Object_2_Width / 2 , Object_2_Pos[num].y + Object_2_Height / 2 };

		if (P_Center.y <= Object_2_Pos[num].y || P_Center.y >= Object_2_Pos[num].y + Object_2_Height)
		{
			//地面判定
			if (P_Center.y < O_Center.y)
			{
				if (P_Center.x < O_Center.x)
				{
					if (P_Center.x + 22.0f < Object_2_Pos[num].x)
					{
						POS.x = Object_2_Pos[num].x - 64;
						return POS;
					}
					else
					{
						POS.y = Object_2_Pos[num].y - 64;
						return POS;
					}
				}
				else
				{
					if (Object_2_Pos[num].x + Object_2_Width < P_Center.x - 22.0f)
					{
						POS.x = Object_2_Pos[num].x + Object_2_Width;
						return POS;
					}
					else
					{
						POS.y = Object_2_Pos[num].y - 64;
						return POS;
					}
				}
			}

			//天井判定
			if (P_Center.y > O_Center.y)
			{
				POS.y = Object_2_Pos[num].y + Object_2_Height;
				return POS;
			}
		}
		if (P_Center.x <= Object_2_Pos[num].x || P_Center.x >= Object_2_Pos[num].x + Object_2_Width)
		{
			//左の壁判定
			if (P_Center.x < O_Center.x)
			{
				POS.x = Object_2_Pos[num].x - 64;
				return POS;
			}
			//右の壁判定
			if (P_Center.x > O_Center.x)
			{
				POS.x = Object_2_Pos[num].x + Object_2_Width;
				return POS;
			}
		}
		//オブジェクトの中にブロックが入ったとき
		if (Object_2_Pos[num].x < P_Center.x < Object_2_Pos[num].x + Object_2_Width ||
			Object_2_Pos[num].y < P_Center.y < Object_2_Pos[num].y + Object_2_Height)
		{
			POS.y = Object_2_Pos[num].y - 64;
			return POS;
		}
	}
	else
	{
		return player_pos;
	}
}
