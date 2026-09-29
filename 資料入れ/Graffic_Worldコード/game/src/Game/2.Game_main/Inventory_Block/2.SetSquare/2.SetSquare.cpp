#include "2.SetSquare.h"

vivid::Vector2 Base::SetSquare_Pos[];

SetSquare_Block::SetSquare_Block()
{
}

SetSquare_Block::~SetSquare_Block()
{
}

void SetSquare_Block::Initialize(void)
{
	for (int i = 0; i < 30; i++)
	{
		SetSquare_Pos[i] = vivid::Vector2::ZERO;
		SetSquare_Pos[i].x = { -10000.0f };
		ActiveFlag[i] = false;
	}
}

void SetSquare_Block::Update(int i)
{
	//重力
	SetSquare_Pos[i].y += 10.0f;

	//オブジェクトとの当たり判定
	for (int j = 0; j < Object_1_Cnt; j++)
	{
		SetSquare_Pos[i] = object_1.CheckHit_Block(SetSquare_Pos[i], SetSquare_Width, SetSquare_Height, j);
	}
	//オブジェクトとの当たり判定
	for (int j = 0; j < Object_2_Cnt; j++)
	{
		SetSquare_Pos[i] = object_2.CheckHit_Block(SetSquare_Pos[i], SetSquare_Width, SetSquare_Height, j);
	}
	//オブジェクトとの当たり判定
	for (int j = 0; j < Object_3_Cnt; j++)
	{
		SetSquare_Pos[i] = object_3.CheckHit_Block(SetSquare_Pos[i], SetSquare_Width, SetSquare_Height, j);
	}
	//オブジェクトとの当たり判定
	for (int j = 0; j < Object_4_Cnt; j++)
	{
		SetSquare_Pos[i] = object_4.CheckHit_Block(SetSquare_Pos[i], SetSquare_Width, SetSquare_Height, j);
	}
	//オブジェクトとの当たり判定
	for (int j = 0; j < Object_5_Cnt; j++)
	{
		SetSquare_Pos[i] = object_5.CheckHit_Block(SetSquare_Pos[i], SetSquare_Width, SetSquare_Height, j);
	}//オブジェクトとの当たり判定
	for (int j = 0; j < Object_6_Cnt; j++)
	{
		SetSquare_Pos[i] = object_6.CheckHit_Block(SetSquare_Pos[i], SetSquare_Width, SetSquare_Height);
	}
	//オブジェクトとの当たり判定
	for (int j = 0; j < Object_8_Cnt; j++)
	{
		SetSquare_Pos[i] = object_8.CheckHit_Block(SetSquare_Pos[i], SetSquare_Width, SetSquare_Height, j);
	}
	//オブジェクトとの当たり判定
	for (int j = 0; j < Object_9_Cnt; j++)
	{
		SetSquare_Pos[i] = object_9.CheckHit_Block(SetSquare_Pos[i], SetSquare_Width, SetSquare_Height, j);
	}
	//オブジェクトとの当たり判定
	for (int j = 0; j < Object_10_Cnt; j++)
	{
		SetSquare_Pos[i] = object_10.CheckHit_Block(SetSquare_Pos[i], SetSquare_Width, SetSquare_Height, j);
	}
	//オブジェクトとの当たり判定
	for (int j = 0; j < Spike_Cnt; j++)
	{
		SetSquare_Pos[i] = spike.CheckHit_Block(SetSquare_Pos[i], SetSquare_Width, SetSquare_Height, j);
	}
	
	//コンパスブロックとの当たり判定
	for (int j = 0; j < 30; j++)
	{
		SetSquare_Pos[i] = CheckHit_SetSquare2(i, Compass_Pos[j], Compass_Width, Compass_Height);
	}

	//画鋲の当たり判定
	for (int j = 0; j < Spike_Cnt; j++)
	{
		SetSquare_Pos[i] = spike.CheckHit_Block(SetSquare_Pos[i], Spike_Radius, j, i);
	}

	for (int j = 0; j < 30; j++)
	{
		SetSquare_Pos[i] = CheckHit_SetSquare(j, SetSquare_Pos[i], SetSquare_Width, SetSquare_Height);
	}
}

void SetSquare_Block::Draw(int i)
{
	if (ActiveFlag)
		vivid::DrawTexture("data\\Set square (2).png", SetSquare_Pos[i]);
}

void SetSquare_Block::Finalize(void)
{
}
bool SetSquare_Block::CheckHit_Block(const vivid::Vector2& pos,int i)
{
	if (pos.x > SetSquare_Pos[i].x &&
		pos.x < SetSquare_Pos[i].x + SetSquare_Width &&
		pos.y > SetSquare_Pos[i].y &&
		pos.y < SetSquare_Pos[i].y + SetSquare_Height)
	{
		if (!(pos.x > SetSquare_Pos[i].x &&
			pos.x < SetSquare_Pos[i].x + SetSquare_Width / 2 &&
			pos.y > SetSquare_Pos[i].y &&
			pos.y < SetSquare_Pos[i].y + SetSquare_Height / 2))
		{
			return true;
		}
	}
	return false;
}

void SetSquare_Block::Hit(int i)
{
	vivid::Point mpos = vivid::mouse::GetCursorPos();

	SetSquare_Pos[i].x = (mpos.x - SetSquare_Width / 2) - 50;
	SetSquare_Pos[i].y = (mpos.y - SetSquare_Height / 2) - 10;

	ActiveFlag[i] = true;
}

vivid::Vector2 SetSquare_Block::CheckHit_SetSquare2(int i, vivid::Vector2 block_pos2, int block_width, int block_height)
{
	bool result_A = SetSquare_Pos[i].y > block_pos2.y - SetSquare_Height;
	bool result_B = SetSquare_Pos[i].y < block_pos2.y + block_height;
	bool result_C = SetSquare_Pos[i].x > block_pos2.x - SetSquare_Width;
	bool result_D = SetSquare_Pos[i].x < block_pos2.x + block_width;

	if (result_A && result_B && result_C && result_D)
	{
		vivid::Vector2 POS = { SetSquare_Pos[i].x , SetSquare_Pos[i].y };
		vivid::Vector2 B_Center = { SetSquare_Pos[i].x + SetSquare_Width / 2 , SetSquare_Pos[i].y + SetSquare_Height / 2 };
		vivid::Vector2 O_Center = { block_pos2.x + block_width / 2 , block_pos2.y + block_height / 2 };

		if (B_Center.y <= block_pos2.y || B_Center.y >= block_pos2.y + block_height)
		{
			//地面判定
			if (B_Center.y < O_Center.y)
			{
				POS.y = block_pos2.y - SetSquare_Height;
				return POS;
			}
			//天井判定
			if (B_Center.y > O_Center.y)
			{
				POS.y = block_pos2.y + block_height;
				return POS;
			}
		}
		if (B_Center.x <= block_pos2.x || B_Center.x >= block_pos2.x + block_width)
		{
			//左の壁判定
			if (B_Center.x < O_Center.x)
			{
				POS.x = block_pos2.x - block_width;
				return POS;
			}
			//右の壁判定
			if (B_Center.x > O_Center.x)
			{
				POS.x = block_pos2.x + SetSquare_Width;
				return POS;
			}
		}
		//オブジェクトの中にブロックが入ったとき
		if (block_pos2.x < B_Center.x < block_pos2.x + block_width ||
			block_pos2.y < B_Center.y < block_pos2.y + block_height)
		{
			POS.y = block_pos2.y - SetSquare_Height;
			return POS;
		}
	}
	else
	{
		return SetSquare_Pos[i];
	}
}

vivid::Vector2 SetSquare_Block::CheckHit_SetSquare(int i, vivid::Vector2 block_pos, int block_width, int block_height)
{
	//三角定規の基準点(左下の頂点)
	vivid::Vector2 SA_Pos = { SetSquare_Pos[i].x , SetSquare_Pos[i].y + SetSquare_Height };
	//三角定規の右下の頂点
	vivid::Vector2 SC_Pos = { SetSquare_Pos[i].x + SetSquare_Width, SetSquare_Pos[i].y + SetSquare_Height };
	//三角定規の右上の頂点
	vivid::Vector2 SB_Pos = { SetSquare_Pos[i].x + SetSquare_Width, SetSquare_Pos[i].y };

	//ものさしの右下の基準点
	vivid::Vector2 M_Base_Pos = { block_pos.x + block_width, block_pos.y + block_height };


	//斜辺ベクトル
	vivid::Vector2 AB = { (float)SetSquare_Width, -(float)SetSquare_Height };

	//三角定規の基準点からものさしの基準点までの距離
	vivid::Vector2 A = { M_Base_Pos.x - SA_Pos.x, M_Base_Pos.y - SA_Pos.y };

	//三角定規の基準点からものさしの基準点の直下の点の交点までの距離
	float l = vivid::Vector2::Dot(AB.Normalize(), A);

	//三角定規の基準点からものさしの基準点の直下の点の交点
	vivid::Vector2 M_Under_Pos = AB.Normalize() * l;

	//ものさしの基準点から斜辺までの距離
	float h = ((M_Under_Pos + SA_Pos) - M_Base_Pos).Length();

	//三角定規の左下から三角定規の右上までのベクトルの大きさ
	float AB_Mag = sqrt(AB.x * AB.x + AB.y * AB.y);


	bool Result_A = (l <= 0);
	bool Result_B = ((0 <= l) && (l <= AB_Mag));
	bool Result_C = (AB_Mag < l) && (block_pos.x <= SetSquare_Pos[i].x + SetSquare_Width);
	bool Result_D = ((SetSquare_Pos[i].y <= M_Base_Pos.y) && (M_Base_Pos.y - block_height <= SetSquare_Pos[i].y + SetSquare_Height));
	bool Result_E = ((SetSquare_Pos[i].x <= M_Base_Pos.x) && (M_Base_Pos.x - block_width <= SetSquare_Pos[i].x + SetSquare_Width));


	if (Result_A || Result_B || Result_C || Result_D)
	{
		//三角定規の左側
		if (Result_A && Result_D && Result_E)
		{
			if (M_Base_Pos.y >= SetSquare_Pos[i].y + SetSquare_Height)
			{
				SetSquare_Pos[i].y = block_pos.y - SetSquare_Height;
				return block_pos;
			}
			else
				return block_pos;
		}
		//三角定規の真ん中
		else if (Result_B && Result_D && Result_E)
		{
			if (M_Base_Pos.y >= SetSquare_Pos[i].y + SetSquare_Height)
			{
				SetSquare_Pos[i].y = block_pos.y - SetSquare_Height;
				return block_pos;
			}
			else
			{
				block_pos.y = (SetSquare_Pos[i].y + SetSquare_Height - (l / 2) - block_height);
				return block_pos;
			}

		}
		//三角定規の右側
		else if (Result_C && Result_D && Result_E)
		{
			if (M_Base_Pos.y >= SetSquare_Pos[i].y + SetSquare_Height)
			{
				SetSquare_Pos[i].y = block_pos.y - SetSquare_Height;
				return block_pos;
			}
			else
			{
				//三角定規の右側
				block_pos.y = SetSquare_Pos[i].y - block_height;
				return block_pos;
			}
		}
		else
		{
			return block_pos;
		}
	}
	else
	{
		//三角定規の場所以外
		return block_pos;
	}

}
