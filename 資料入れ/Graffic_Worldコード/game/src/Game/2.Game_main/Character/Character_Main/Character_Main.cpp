#include "Character_Main.h"
namespace keyboard = vivid::keyboard;

bool Base::Jamp_Flg;

Character_Main::Character_Main()
{
}

Character_Main::~Character_Main()
{
}

void Character_Main::Initialize(void)
{
	//ゲームオーバーのためのフラグ最初はFalseにしておく
	Game_Over_Flg = false;

	Jamp_Flg = false;

	//プレイヤーの位置を設定する
	player.Initialize();

	//敵1の位置を設定する
	enemy_1.Initialize();
	enemy_2.Initialize();
	enemy_3.Initialize();

	//敵とプレイヤーとの当たり判定のための変数の初期化
	for (int i = 0; i < E_1_Cnt; i++)
	{
		Length[i] = 0;
	}

	inventory.Initialize();
}

void Character_Main::Update(void)
{
	//まずプレイヤーのUpdateを呼び出し座標を取得する
	player.Update();

	//インベントリーのUpdateを呼び出し座標を取得する
	inventory.Update();

	//定規をおいている数を取得
	Put_Block_Cnt[0] = Block_Manager::GetInstance().Get_Put_Block_Cnt(BLOCK_ID::RULER);
	Put_Block_Cnt[1] = Block_Manager::GetInstance().Get_Put_Block_Cnt(BLOCK_ID::SETSQUARE);
	Put_Block_Cnt[2] = Block_Manager::GetInstance().Get_Put_Block_Cnt(BLOCK_ID::COMPASS);

	//定規の当たり判定の処理
	for (int i = 0; i < Put_Block_Cnt[0]; i++)
	{
		if (Block_1_Check[i])
		{
			//プレイヤーと定規の当たり判定をして当たっていればプレイヤーの座標を変化させる
			Player_Pos = Check_Hit_Block(Player_Pos, Ruler_Pos[i], Player_Radius, Ruler_Width, Ruler_Height);
			inventory.Get_Move_Flg_1(Move_Flg, i);

			//定規の上でスペースを押すとジャンプできる
			if (keyboard::Trigger(keyboard::KEY_ID::SPACE))//ジャンプ
			{
				if (Jamp_Flg)
				{
					//呼び出すことでプレイヤーがジャンプする関数
					player.Jump();
				}
			}
		}
	}

	//三角定規の当たり判定の処理
	for (int i = 0; i < Put_Block_Cnt[1]; i++)
	{
		if (Block_2_Check[i])
		{
			//プレイヤーと三角定規の当たり判定をして当たっていればプレイヤーの座標を変化させる
			Player_Pos = Check_Hit_SetSquare(Player_Pos, Player_Radius, i);
			inventory.Get_Move_Flg_2(Move_Flg2, i);

			//三角定規の上でスペースを押すとジャンプできる
			if (keyboard::Trigger(keyboard::KEY_ID::SPACE))//ジャンプ
			{
				if (Jamp_Flg)
				{
					//呼び出すことでプレイヤーがジャンプする関数
					player.Jump();
				}
			}
		}
	}

	//コンパスブロックの当たり判定の処理
	for (int i = 0; i < Put_Block_Cnt[2]; i++)
	{
		//プレイヤーとコンパスの当たり判定をして当たっていればプレイヤーの座標を変化させる
		Player_Pos = Check_Hit_Block(Player_Pos, Compass_Pos[i], Player_Radius, Compass_Width, Compass_Height);
		
		//コンパスブロックの上でスペースを押すとジャンプできる
		if (keyboard::Trigger(keyboard::KEY_ID::SPACE))//ジャンプ
		{
			if (Jamp_Flg)
			{
				//呼び出すことでプレイヤーがジャンプする関数
				player.Jump();
			}
		}
	}

	//プレイヤーと画鋲の当たり判定
	for (int i = 0; i < Spike_Cnt; i++)
	{
		Length[i] = Get_Character_length(Player_Pos, Spike_Pos[i], Player_Radius, Spike_Radius);

		if (Length[i] <= Player_Radius + Spike_Radius)
		{
			//当たってHPが1以上だったらプレイヤーの位置をリセットする
			if (HP_level > 1)
			{
				if (Scroll_Cnt == 1)
				{
					Player_Pos.x += 300.0f;
				}
				else
				{
					Player_Pos.x -= 300.0f;
				}

				HP_level -= 1;
			}
			//当たっていてHPが０になったらゲームオーバーフラグをTrueにする
			else
			{
				HP_level -= 1;
				Game_Over_Flg = true;
				//ゲームオーバーへ
			}
		}
	}
	
	//敵１の数の分だけ処理をする
	for (int i = 0; i < E_1_Cnt; i++)
	{
		//敵1のUpdateを呼び出し座標を取得する
		enemy_1.Update(i);
		
		for (int j = 0; j < Put_Block_Cnt[0]; j++)
		{
			vivid::Vector2 pos = Enemy_1_Position[i];
			//敵1と定規の当たり判定をして当たっていれば敵1の座標を変化させる
			pos = Check_Hit_Block(Enemy_1_Position[i], Ruler_Pos[j], Enemy_1_Radius, Ruler_Width, Ruler_Height);
			Enemy_1_Position[i].x = pos.x;


			//敵1が定規の上に乗っていればジャンプできる
			if (Enemy_1_Position[i].y == Ruler_Pos[j].y - 64)
				enemy_1.Jump(i);
		}
		//三角定規の当たり判定の処理
		for (int i = 0; i < Put_Block_Cnt[1]; i++)
		{
			//敵1と三角定規の当たり判定をして当たっていればプレイヤーの座標を変化させる
			Enemy_1_Position[i] = Check_Hit_SetSquare(Enemy_1_Position[i], Enemy_1_Radius, i);
		}
		//プレイヤーと敵1との当たり判定をする
		Length[i] = Get_Character_length(Player_Pos, Enemy_1_Position[i], Player_Radius, Enemy_1_Radius);
		if (Length[i] <= Player_Radius + Enemy_1_Radius)
		{
			//当たっていてHPが１以上だったらプレイヤーの位置をリセットする
			if (HP_level > 1)
			{
				if (Scroll_Cnt == 1)
				{
					Player_Pos.x += 300.0f;
				}
				else
				{
					Player_Pos.x -= 300.0f;
				}
				HP_level -= 1;
			}
			//当たっていてHPが０になったらゲームオーバーフラグをTrueにする
			else
			{
				HP_level -= 1;
				Game_Over_Flg = true;
				//ゲームオーバーへ
			}
		}
	}
	//敵2の数の分だけ処理をする
	for (int i = 0; i < E_2_Cnt; i++)
	{
		//敵2のUpdateを呼び出し座標を取得する
		enemy_2.Update(i);

		for (int j = 0; j < Put_Block_Cnt[0]; j++)
		{
			//敵2と定規の当たり判定をして当たっていれば敵2の座標を変化させる
			Enemy_2_Position[i] = Check_Hit_Block(Enemy_2_Position[i], Ruler_Pos[j], Enemy_2_Radius, Ruler_Width, Ruler_Height);
		}
		//三角定規の当たり判定の処理
		for (int i = 0; i < Put_Block_Cnt[1]; i++)
		{
			//敵1と三角定規の当たり判定をして当たっていればプレイヤーの座標を変化させる
			Enemy_2_Position[i] = Check_Hit_SetSquare(Enemy_2_Position[i], Enemy_2_Radius, i);
		}

		//プレイヤーと敵2との当たり判定をする
		Length[i] = Get_Character_length(Player_Pos, Enemy_2_Position[i], Player_Radius, Enemy_2_Radius);
		if (Length[i] <= Player_Radius + Enemy_2_Radius)
		{
			//当たっていてHPが１以上だったらプレイヤーの位置をリセットする
			if (HP_level > 1)
			{
				if (Scroll_Cnt == 1)
				{
					Player_Pos.x += 300.0f;
				}
				else
				{
					Player_Pos.x -= 300.0f;
				}
				HP_level -= 1;
			}
			//当たっていてHPが０になったらゲームオーバーフラグをTrueにする
			else
			{
				HP_level -= 1;
				Game_Over_Flg = true;
				//ゲームオーバーへ
			}
		}
	}

	//敵3の数の分だけ処理をする
	for (int i = 0; i < E_3_Cnt; i++)
	{
		//敵3のUpdateを呼び出し座標を取得する
		enemy_3.Update(i);

		for (int j = 0; j < Put_Block_Cnt[0]; j++)
		{
			vivid::Vector2 pos = Enemy_3_Position[i];
			//敵3と定規の当たり判定をして当たっていれば敵3の座標を変化させる
			pos = Check_Hit_Block(Enemy_3_Position[i], Ruler_Pos[j], Enemy_3_Radius, Ruler_Width, Ruler_Height);

			Enemy_3_Position[i].x = pos.x;
		}
		//三角定規の当たり判定の処理
		for (int i = 0; i < Put_Block_Cnt[1]; i++)
		{
			//敵1と三角定規の当たり判定をして当たっていればプレイヤーの座標を変化させる
			Enemy_3_Position[i] = Check_Hit_SetSquare(Enemy_3_Position[i], Enemy_3_Radius, i);
		}

		//プレイヤーと敵3との当たり判定をする
		Length[i] = Get_Character_length(Player_Pos, Enemy_3_Position[i], Player_Radius, Enemy_3_Radius);
		if (Length[i] <= Player_Radius + Enemy_3_Radius)
		{
			//当たっていてHPが１以上だったらプレイヤーの位置をリセットする
			if (HP_level > 1)
			{
				if (Scroll_Cnt == 1)
				{
					Player_Pos.x += 300.0f;
				}
				else
				{
					Player_Pos.x -= 300.0f;
				}
				HP_level -= 1;
			}
			//当たっていてHPが０になったらゲームオーバーフラグをTrueにする
			else
			{
				HP_level -= 1;
				Game_Over_Flg = true;
				//ゲームオーバーへ
			}
		}
	}
	Game_Over_Check();
}

void Character_Main::Draw(void)
{
	//各種描画
	player.Draw();
	for (int i = 0; i < E_1_Cnt; i++)
	{
		enemy_1.Draw(i);
	}
	for (int i = 0; i < E_2_Cnt; i++)
	{
		enemy_2.Draw(i);
	}
	for (int i = 0; i < E_3_Cnt; i++)
	{
		enemy_3.Draw(i);
	}
	inventory.Draw();
}

void Character_Main::Finalize(void)
{
	inventory.Finalize();
	player.Finalize();
	for (int i = 0; i < E_1_Cnt; i++)
	{
		enemy_1.Finalize();
	}
	for (int i = 0; i < E_2_Cnt; i++)
	{
		enemy_2.Finalize();
	}
	for (int i = 0; i < E_3_Cnt; i++)
	{
		enemy_3.Finalize();
	}
}

void Character_Main::Game_Over_Check(void)
{
	if (HP_level <= 0)
	{
		Game_Over_Flg = true;
	}
}

int Character_Main::Get_Character_length(vivid::Vector2 pos, vivid::Vector2 pos2, const float radius, const float radius2)
{
	vivid::Vector2 Center_A = pos + vivid::Vector2(radius, radius);
	vivid::Vector2 Center_B = pos2 + vivid::Vector2(radius2, radius2);

	float x = Center_A.x - Center_B.x;
	float y = Center_A.y - Center_B.y;
	float length = sqrt(x * x + y * y);
	return length;
}

vivid::Vector2 Character_Main::Check_Hit_Block
(vivid::Vector2 pos, vivid::Vector2 block_pos, float radius, int block_width, int block_height)
{
	//CIRCLEの中心点を求める
	vivid::Vector2 P_center = { pos.x + radius,pos.y + radius };

	//点と短形の判定その１
	bool result_h = P_center.x > block_pos.x - radius
		&& P_center.x < block_pos.x + block_width + radius
		&& P_center.y > block_pos.y
		&& P_center.y < block_pos.y + block_height;

	//点と短形の判定その２
	bool result_v = P_center.x > block_pos.x
		&& P_center.x < block_pos.x + block_width
		&& P_center.y > block_pos.y - radius
		&& P_center.y < block_pos.y + block_height + radius;

	//点と円の判定その１
	//BOXの左上
	vivid::Vector2 v = P_center - block_pos;
	bool result_lu = v.Length() <= radius;//Length = sqrt
	//BOXの右上
	v = P_center - vivid::Vector2(block_pos.x + block_width, block_pos.y);
	bool result_ru = v.Length() <= radius;
	//BOXの左下
	v = P_center - vivid::Vector2(block_pos.x, block_pos.y + block_height);
	bool result_Id = v.Length() <= radius;
	//BOXの右下
	v = P_center - vivid::Vector2(block_pos.x + block_width, block_pos.y + block_height);
	bool result_rd = v.Length() <= radius;

	//どこかしらで真であれば当たったとみなされる
	if (result_h || result_v || result_lu || result_Id || result_ru || result_rd)
	{
		//ブロックに乗っていない間はFalseにする
		Move_Flg = false;
		vivid::Vector2 B_Center = { block_pos.x + block_width / 2,block_pos.y + block_height / 2 };

		if (P_center.y <= block_pos.y || P_center.y >= block_pos.y + block_height)
		{
			//地面判定
			if (P_center.y < B_Center.y)
			{
				Move_Flg = true;
				Jamp_Flg = true;
				if (P_center.x < B_Center.x)
				{
					if (P_center.x + 22.0f < block_pos.x)
					{
						pos.x = block_pos.x - 64;
						return pos;
					}
					else
					{
						pos.y = block_pos.y - 64;
						return pos;
					}
				}
				else
				{
					if (block_pos.x + block_width < P_center.x - 22.0f)
					{
						pos.x = block_pos.x + block_width;
						return pos;
					}
					else
					{
						pos.y = block_pos.y - 64;
						return pos;
					}
				}
			}
			//天井判定
			if (P_center.y > block_pos.y)
			{
				Jamp_Flg = false;
				pos.y = block_pos.y + block_height;
				return pos;
			}
		}
		if (P_center.x <= block_pos.x || P_center.x >= block_pos.x + block_width)
		{
			//左の壁判定
			if (P_center.x < block_pos.x)
			{
				Jamp_Flg = false;
				pos.x = block_pos.x - 64;
				return pos;
			}
			//右の壁判定
			if (P_center.x > block_pos.x)
			{
				Jamp_Flg = false;
				pos.x = block_pos.x + block_width;
				return pos;
			}
		}
		//オブジェクトの中にブロックが入ったとき
		if (block_pos.x < P_center.x < block_pos.x + block_width ||
			block_pos.y < P_center.y < block_pos.y + block_height)
		{
			Jamp_Flg = false;
			pos.y = block_pos.y - block_height;
			return pos;
		}
	}
	else
	{
		//ブロックに乗っていない間はFalseにする
		Move_Flg = false;
		Jamp_Flg = false;
		return pos;
	}
}

vivid::Vector2 Character_Main::Check_Hit_SetSquare
(vivid::Vector2 pos, float radius, int i)
{
	//プレイヤーの中心点
	vivid::Vector2 P_Center = { pos.x + radius, pos.y + radius };

	//三角定規の基準点(左下の頂点)
	vivid::Vector2 SA_Pos = { SetSquare_Pos[i].x , SetSquare_Pos[i].y + SetSquare_Height };

	//三角定規の右下の頂点
	vivid::Vector2 SC_Pos = { SetSquare_Pos[i].x + SetSquare_Width, SetSquare_Pos[i].y + SetSquare_Height };

	//三角定規の右上の頂点
	vivid::Vector2 SB_Pos = { SetSquare_Pos[i].x + SetSquare_Width, SetSquare_Pos[i].y };


	//三角定規の斜辺の当たり判定
	//三角定規の斜辺ベクトル
	vivid::Vector2 AB = { (float)SetSquare_Width , -(float)SetSquare_Height };

	//基準点からプレイヤーの中心へのベクトル
	vivid::Vector2 A = { P_Center.x - SA_Pos.x, P_Center.y - SA_Pos.y };

	//三角定規の基準点からプレイヤーの直下と斜辺の交点までの長さ
	float X = vivid::Vector2::Dot(AB.Normalize(), A);

	//プレイヤーの直下の点
	vivid::Vector2 P_Under_Pos = AB.Normalize() * X;

	//円の中心から三角定規の斜辺までの距離
	float h = ((P_Under_Pos + SA_Pos) - P_Center).Length();

	//三角定規の左下から三角定規の右上までのベクトルの大きさ
	float AB_Mag = sqrt(AB.x * AB.x + AB.y * AB.y);


	//三角定規の高さの辺の当たり判定
	//三角定規の高さベクトル
	vivid::Vector2 CB = { (float)0,  -(float)SetSquare_Height };

	//高さの始点(右下)からプレイヤーの中心へのベクトル
	vivid::Vector2 B = { P_Center.x - SC_Pos.x, P_Center.y - SC_Pos.y };

	//三角定規の右下の頂点からプレイヤーの直下と斜辺の交点までの長さ
	float Y = vivid::Vector2::Dot(CB.Normalize(), B);

	//プレイヤーの直左の点
	vivid::Vector2 P_Left_Pos = CB.Normalize() * Y;

	//円の中心から三角定規の縦辺までの距離
	float l = ((P_Left_Pos + SC_Pos) - P_Center).Length();

	//三角定規の右下から三角定規の右上までのベクトルの大きさ
	float CB_Mag = sqrt(CB.y * CB.y);


	//三角定規の底辺の当たり判定
	//三角定規の底辺ベクトル
	vivid::Vector2 CA = { -(float)SetSquare_Width, (float)0 };

	//底辺ベクトルの始点からプレイヤーの中心へのベクトル
	vivid::Vector2 C = { P_Center.x - SC_Pos.x, P_Center.y - SC_Pos.y };

	//三角定規の右下の頂点からプレイヤーの直下と斜辺の交点までの長さ
	float Z = vivid::Vector2::Dot(CA.Normalize(), C);

	//プレイヤーの直上の点
	vivid::Vector2 P_Up_Pos = CA.Normalize() * Z;

	//円の中心から三角定規の底辺までの距離
	float m = ((P_Up_Pos + SC_Pos) - P_Center).Length();

	//三角定規の左下から三角定規の右下までのベクトルの大きさ
	float CA_Mag = sqrt(CA.x * CA.x);


	//底辺との判定
	bool Result_A = (m <= radius) && (0 <= Z && Z <= CA_Mag);
	//三角定規の高さとの判定
	bool Result_B = (l <= radius) && (0 <= Y && Y <= CB_Mag);
	//三角定規の斜線の辺との判定
	bool Result_C = (h <= radius) && (0 <= X && X <= AB_Mag);
	//三角定規の右上の点との当たり判定
	vivid::Vector2 v = P_Center - SB_Pos;
	bool Result_D = v.Length() <= radius;

	if (Result_A || Result_B || Result_C ||Result_D)
	{
		if (Result_D)
		{
			Jamp_Flg = true;
			Move_Flg2 = true;
			pos.y = SB_Pos.y - Player_Height;
			return pos;
		}
		if (Result_A)
		{
			pos.y = (SC_Pos.y - (X / 2) - Player_Height);
			return pos;
		}
		if (Result_B)
		{
			if (P_Center.x > SB_Pos.x)
			{
				pos.x = SC_Pos.x;
			}
			else
				pos.y = (SC_Pos.y - (X / 2) - Player_Height);
			return pos;
		}
		if (Result_C)
		{
			Jamp_Flg = true;
			Move_Flg2 = true;
			pos.y = (SC_Pos.y - (X / 2) - Player_Height);
			return pos;
		}
	}
	else
	{
		Jamp_Flg = false;
		Move_Flg2 = false;
		return pos;
	}
}