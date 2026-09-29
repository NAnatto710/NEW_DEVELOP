#include "Coin.h"

vivid::Vector2 Base::Coin_1_Position[];
vivid::Vector2 Base::Coin_5_Position[];
vivid::Vector2 Base::Coin_10_Position[];
vivid::Vector2 Base::Coin_50_Position[];

int Base::Coin_All;

Coin::Coin()
{
}

Coin::~Coin()
{
}

void Coin::Initialize(void)
{
	//最初に所持しているコインの数
	Coin_All = 350;

	//画面上にコインの数を表示するためのText
	text_1 = "Coin:";
	text_2 = "";
	text_3 = "";

	if (Change_Flg == false)
	{
		Coin_1_Position[0] = { 932.0f,506.0f };
		Coin_1_Position[1] = { 1216.0f,630.0f };
		Coin_1_Position[2] = { 352.0f,-104.0f };
		Coin_1_Position[3] = { 2050.0f,150.0f };
		Coin_1_Position[4] = { 0.0f,-184.0f };

		Coin_5_Position[0] = { 658.0f,630.0f };
		Coin_5_Position[1] = { 862.0f,566.0f };
		Coin_5_Position[2] = { 1490.0f,630.0f };
		Coin_5_Position[3] = { 522.0f,150.0f };
		Coin_5_Position[4] = { 700.0f,370.0f };

		Coin_10_Position[0] = { 1696.0f,324.0f};
		Coin_10_Position[1] = { 1680.0f,150.0f};
		Coin_10_Position[2] = { 586.0f,-204.0f};
		Coin_10_Position[3] = { 884.0f,-398.0f};
		Coin_10_Position[4] = { 200.0f,-334.0f};

		Coin_50_Position[0] = { 1766.0f,566.0f };
		Coin_50_Position[1] = { 1680.0f,-104.0f};
		Coin_50_Position[2] = { 1520.0f,-650.0f };
		Coin_50_Position[3] = { 1980.0f,-334.0f };
		Coin_50_Position[4] = { 0.0f,-664.0f };
	}
	else
	{
		Coin_1_Position[0] = { 483.0f, 630.0f - Coin_Height };
		Coin_1_Position[1] = { 1580.0f, Ground_Line - Coin_Height };

		Coin_1_Position[2] = { -1580.0f, Ground_Line - Coin_Height };
		Coin_1_Position[3] = { -1580.0f, Ground_Line - Coin_Height };
		Coin_1_Position[4] = { -1580.0f, Ground_Line - Coin_Height };

		Coin_5_Position[0] = { 618.0f, 630.0f - Coin_Height * 2 };

		Coin_5_Position[1] = { -618.0f, 630.0f - Coin_Height };
		Coin_5_Position[2] = { -618.0f, 630.0f - Coin_Height };
		Coin_5_Position[3] = { -618.0f, 630.0f - Coin_Height };
		Coin_5_Position[4] = { -618.0f, 630.0f - Coin_Height };

		Coin_10_Position[0] = { 1753.0f, 570.0f - Coin_Height };

		Coin_10_Position[1] = { -1753.0f, 570.0f - Coin_Height };
		Coin_10_Position[2] = { -1753.0f, 570.0f - Coin_Height };
		Coin_10_Position[3] = { -1753.0f, 570.0f - Coin_Height };
		Coin_10_Position[4] = { -1753.0f, 570.0f - Coin_Height };

		Coin_50_Position[0] = { 1018.0f, Ground_Line - Coin_Height };
		Coin_50_Position[1] = { 2300.0f - Coin_Width, Ground_Line - Coin_Height };

		Coin_50_Position[2] = { -2300.0f - Coin_Width, Ground_Line - Coin_Height };
		Coin_50_Position[3] = { -2300.0f - Coin_Width, Ground_Line - Coin_Height };
		Coin_50_Position[4] = { -2300.0f - Coin_Width, Ground_Line - Coin_Height };
	}

	for (int i = 0; i < Coin_1_cnt; i++)
	{
		C_1_Length[i] = 0;
		Coin_1_Flg[i] = 0;
	}
	for (int i = 0; i < Coin_5_cnt; i++)
	{
		C_5_Length[i] = 0;
		Coin_5_Flg[i] = 0;
	}
	for (int i = 0; i < Coin_10_cnt; i++)
	{
		C_10_Length[i] = 0;
		Coin_10_Flg[i] = 0;
	}
	for (int i = 0; i < Coin_50_cnt; i++)
	{
		C_50_Length[i] = 0;
		Coin_50_Flg[i] = 0;
	}
}

void Coin::Update(void)
{
	//コインの数だけ繰り返す
	for (int i = 0; i < Coin_1_cnt; i++)
	{
		//コインとプレイヤーの当たり判定の処理
		C_1_Length[i] = Get_Coin_length(Player_Pos, Coin_1_Position[i], Player_Radius, Coin_Radius);
		if (C_1_Length[i] <= Player_Radius + Coin_Radius)
		{
			//今までにそのコインと当たっているかの判定
			if (Coin_1_Flg[i] == 0)
			{
				//コインの総所持数にコインの種類によって足していく
				Coin_All += 1;
				//コインのFlgをTrueにする
				Coin_1_Flg[i] = 1;
				//コインの位置を変える
				Coin_1_Position[i] = vivid::Vector2::ZERO;
			}
		}
	}
	for (int i = 0; i < Coin_5_cnt; i++)
	{
		//コインとプレイヤーの当たり判定の処理
		C_5_Length[i] = Get_Coin_length(Player_Pos, Coin_5_Position[i], Player_Radius, Coin_Radius);
		if (C_5_Length[i] <= Player_Radius + Coin_Radius)
		{
			//今までにそのコインと当たっているかの判定
			if (Coin_5_Flg[i] == 0)
			{
				//コインの総所持数にコインの種類によって足していく
				Coin_All += 5;
				//コインのFlgをTrueにする
				Coin_5_Flg[i] = 1;
				//コインの位置を変える
				Coin_5_Position[i] = vivid::Vector2::ZERO;
			}
		}
	}
	for (int i = 0; i < Coin_10_cnt; i++)
	{
		//コインとプレイヤーの当たり判定の処理
		C_10_Length[i] = Get_Coin_length(Player_Pos, Coin_10_Position[i], Player_Radius, Coin_Radius);
		if (C_10_Length[i] <= Player_Radius + Coin_Radius)
		{
			//今までにそのコインと当たっているかの判定
			if (Coin_10_Flg[i] == 0)
			{
				//コインの総所持数にコインの種類によって足していく
				Coin_All += 10;
				//コインのFlgをTrueにする
				Coin_10_Flg[i] = 1;
				//コインの位置を変える
				Coin_10_Position[i] = vivid::Vector2::ZERO;
			}
		}
	}
	for (int i = 0; i < Coin_50_cnt; i++)
	{
		//コインとプレイヤーの当たり判定の処理
		C_50_Length[i] = Get_Coin_length(Player_Pos, Coin_50_Position[i], Player_Radius, Coin_Radius);
		if (C_50_Length[i] <= Player_Radius + Coin_Radius)
		{
			//今までにそのコインと当たっているかの判定
			if (Coin_50_Flg[i] == 0)
			{
				//コインの総所持数にコインの種類によって足していく
				Coin_All += 50;
				//コインのFlgをTrueにする
				Coin_50_Flg[i] = 1;
				//コインの位置を変える
				Coin_50_Position[i] = vivid::Vector2::ZERO;
			}
		}
	}

	text_2 = std::to_string(Coin_All);
}

void Coin::Draw(void)
{
	//コインの数だけ表示する
	for (int i = 0; i < Coin_1_cnt; i++)
	{
		if (Coin_1_Flg[i] == 0)
		{
			vivid::DrawTexture("data\\coin_1.png", Coin_1_Position[i]);
		}
	}
	for (int i = 0; i < Coin_5_cnt; i++)
	{
		if (Coin_5_Flg[i] == 0)
		{
			vivid::DrawTexture("data\\coin_5.png", Coin_5_Position[i]);
		}
	}
	for (int i = 0; i < Coin_10_cnt; i++)
	{
		if (Coin_10_Flg[i] == 0)
		{
			vivid::DrawTexture("data\\coin_10.png", Coin_10_Position[i]);
		}
	}
	for (int i = 0; i < Coin_50_cnt; i++)
	{
		if (Coin_50_Flg[i] == 0)
		{
			vivid::DrawTexture("data\\coin_50.png", Coin_50_Position[i]);
		}
	}
	text_3 = text_1 + text_2;
	vivid::DrawText(40, text_3, vivid::Vector2(0.0f, 40.0f), 0xff0000ff);
}

void Coin::Finalize(void)
{
}

int Coin::Get_Coin_length(vivid::Vector2 pos, vivid::Vector2 pos2, const float radius, const float radius2)
{
	vivid::Vector2 Center_A = pos + vivid::Vector2(radius, radius);
	vivid::Vector2 Center_B = pos2 + vivid::Vector2(radius2, radius2);

	float x = Center_A.x - Center_B.x;
	float y = Center_A.y - Center_B.y;
	float length = sqrt(x * x + y * y);
	return length;
}