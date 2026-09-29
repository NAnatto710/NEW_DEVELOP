#include "Spike.h"

vivid::Vector2 Base::Spike_Pos[];

Spike::Spike()
{
}

Spike::~Spike()
{
}

void Spike::Initialize(void)
{
	if (Change_Flg == false)
	{
		//上段
		Spike_Pos[0] = { 756.0f,-334.0f };
		Spike_Pos[1] = { 820.0f,-334.0f };

		Spike_Pos[2] = { 954.0f,-334.0f };
		Spike_Pos[3] = { 1018.0f,-334.0f };

		Spike_Pos[4] = { 1152.0f,-334.0f };
		Spike_Pos[5] = { 1216.0f,-334.0f };

		Spike_Pos[6] = { 2044.0f,-334.0f };
		Spike_Pos[7] = { 2108.0f,-334.0f };
		Spike_Pos[8] = { 2172.0f,-334.0f };
		//中段
		Spike_Pos[9] = { 0.0f,146.0f };
		Spike_Pos[10] = { 64.0f,146.0f };
		Spike_Pos[11] = { 128.0f,146.0f };

		Spike_Pos[12] = { 726.0f,146.0f };
		Spike_Pos[13] = { 790.0f,146.0f };
		Spike_Pos[14] = { 854.0f,146.0f };
		
		Spike_Pos[15] = { 1788.0f,146.0f };
		Spike_Pos[16] = { 1916.0f,150.0f };
		//下段
		Spike_Pos[17] = { 1002.0f,626.0f };
		Spike_Pos[18] = { 1066.0f,626.0f };
	}
	else
	{
		Spike_Pos[0] = { 818.0f, Ground_Line - Spike_Height };
		Spike_Pos[1] = { 882.0f, Ground_Line - Spike_Height };

		for (int i = 2; i < Spike_Cnt; i++)
		{
			Spike_Pos[i] = { -1000.0f, 0.0f };
		}
	}
}

void Spike::Update(void)
{
}

void Spike::Draw(void)
{
	for (int i = 0; i < Spike_Cnt; i++) {
		vivid::DrawTexture("data\\Spike.png", Spike_Pos[i], 0xffffffff);
	}
}

void Spike::Finalize(void)
{
}

vivid::Vector2 Spike::CheckHit_Block(vivid::Vector2 block_pos, float spike_radius, int num1, int num2)
{
	//画鋲の中心点を求める
	vivid::Vector2 S_Circle_Center = { Spike_Pos[num1].x + Spike_Radius, Spike_Pos[num1].y + Spike_Radius };

	bool result_h = S_Circle_Center.x > Ruler_Pos[num2].x - Spike_Radius
		&& S_Circle_Center.x < Ruler_Pos[num2].x + Ruler_Width + Spike_Radius
		&& S_Circle_Center.y > Ruler_Pos[num2].y - Spike_Radius
		&& S_Circle_Center.y < Ruler_Pos[num2].y + Ruler_Height + Spike_Radius;

	bool result_v = S_Circle_Center.x > Ruler_Pos[num2].x - Spike_Radius
		&& S_Circle_Center.x < Ruler_Pos[num2].x + Ruler_Pos[num2].x + Spike_Radius
		&& S_Circle_Center.y > Ruler_Pos[num2].y - Spike_Radius
		&& S_Circle_Center.y < Ruler_Pos[num2].y + Ruler_Height + Spike_Radius;

	//左上
	vivid::Vector2 v = S_Circle_Center - Ruler_Pos[num2];
	bool result_lu = v.Length() <= Spike_Radius;
	//右上
	v = S_Circle_Center - vivid::Vector2(Ruler_Pos[num2].x + Ruler_Width, Ruler_Pos[num2].y);
	bool result_ru = v.Length() <= Spike_Radius;
	//左下
	v = S_Circle_Center - vivid::Vector2(Ruler_Pos[num2].x, Ruler_Pos[num2].y + Ruler_Height);
	bool result_Id = v.Length() <= Spike_Radius;
	//右下
	v = S_Circle_Center - vivid::Vector2(Ruler_Pos[num2].x + Ruler_Width, Ruler_Pos[num2].y + Ruler_Height);
	bool result_rd = v.Length() <= Spike_Radius;

	//どこかしらが真ならば当たったとみなされる
	if (result_h || result_v || result_lu || result_ru || result_Id || result_rd)
	{
		vivid::Vector2 B_POS = { block_pos.x, block_pos.y };
		vivid::Vector2 B_Center = { Ruler_Pos[num2].x + Ruler_Width / 2 , Ruler_Pos[num2].y + Ruler_Height / 2 };
		vivid::Vector2 S_Center = { Spike_Pos[num1].x + Spike_Radius, Spike_Pos[num1].y + Spike_Radius };

		if (B_Center.y <= Spike_Pos[num1].y || B_Center.y >= Spike_Pos[num1].y + Spike_Height)
		{
			//地面判定
			if (B_Center.y < S_Center.y)
			{
				B_POS.y = Spike_Pos[num1].y - 40;
				return B_POS;
			}
			//天井判定
			if (B_Center.y > S_Center.y)
			{
				B_POS.y = Spike_Pos[num1].y - Ruler_Height;
				return B_POS;
			}
		}
		if (B_Center.x <= Spike_Pos[num1].x || B_Center.x >= Spike_Pos[num1].x + Spike_Width)
		{
			//右の壁判定
			if (B_Center.x > S_Center.x)
			{
				B_POS.x = Spike_Pos[num1].x + Spike_Width;
				return B_POS;
			}
			//左の壁判定
			if (B_Center.x < S_Center.x)
			{
				B_POS.x = Spike_Pos[num1].x - Ruler_Width;
				return B_POS;
			}
		}
		//画鋲のなかにブロックが入った時
		if (Ruler_Pos[num2].x < S_Center.x < Ruler_Pos[num2].x + Ruler_Width ||
			Ruler_Pos[num2].y < S_Center.y < Ruler_Pos[num2].y + Ruler_Height)
		{
			B_POS.y = Spike_Pos[num1].y - Ruler_Height;
			return B_POS;
		}
	}
	else
	{
		return block_pos;
	}
}