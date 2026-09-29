#include "Player.h"

vivid::Vector2 Base::Player_Pos;

int Base::Scroll_Cnt;

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(void)
{
	Player_Pos = { Start_Zone_Pos.x + 250, Start_Zone_Pos.y };
	P_Jp = 45;
	Scroll_Cnt = 0;
}

void Player::Update(void)
{
	namespace keyboard = vivid::keyboard;

	if (500.0f < Player_Pos.x && Player_Pos.x < 700.0f)
	{
		if (keyboard::Button(keyboard::KEY_ID::A))//Aキーを押すと左へ移動
			Player_Pos.x -= 3.0f;

		if (keyboard::Button(keyboard::KEY_ID::D))//Dキーを押すと右へ移動
			Player_Pos.x += 3.0f;
	}
	else
	{
		if (keyboard::Button(keyboard::KEY_ID::A))//Aキーを押すと左へ移動
		{
			if( 500.0f < Player_Pos.x)
				Player_Pos.x -= 3.0f;
			else if (Pos_Background_Under_Left.x < 0.0f)
			{
				Pos_Background_Under_Left.x += 3.0f;
				Pos_Background_Under_Right.x += 3.0f;
				Pos_Background_On_Left.x += 3.0f;
				Pos_Background_On_Right.x += 3.0f;

				for (int i = 0; i < Object_1_Cnt; i++)
					Object_1_Pos[i].x += 3.0f;
				for (int i = 0; i < Object_2_Cnt; i++)
					Object_2_Pos[i].x += 3.0f;
				for (int i = 0; i < Object_3_Cnt; i++)
					Object_3_Pos[i].x += 3.0f;
				for (int i = 0; i < Object_4_Cnt; i++)
					Object_4_Pos[i].x += 3.0f;
				for (int i = 0; i < Object_5_Cnt; i++)
					Object_5_Pos[i].x += 3.0f;
				for (int i = 0; i < 1; i++)
					Object_6_Pos[i].x += 3.0f;
				for (int i = 0; i < Object_8_Cnt; i++)
					Object_8_Pos[i].x += 3.0f;
				for (int i = 0; i < Object_9_Cnt; i++)
					Object_9_Pos[i].x += 3.0f;
				for (int i = 0; i < Object_10_Cnt; i++)
					Object_10_Pos[i].x += 3.0f;

				for (int i = 0; i < Star_Cnt; i++)
					Star_Pos[i].x += 3.0f;

				for (int i = 0; i < Spike_Cnt; i++)
					Spike_Pos[i].x += 3.0f;

				for (int i = 0; i < Coin_1_cnt; i++)
					Coin_1_Position[i].x += 3.0f;
				for (int i = 0; i < Coin_5_cnt; i++)
					Coin_5_Position[i].x += 3.0f;
				for (int i = 0; i < Coin_10_cnt; i++)
					Coin_10_Position[i].x += 3.0f;
				for (int i = 0; i < Coin_50_cnt; i++)
					Coin_50_Position[i].x += 3.0f;

				for (int i = 0; i < E_1_Cnt; i++)
					Enemy_1_Position[i].x += 3.0f;
				for (int i = 0; i < E_2_Cnt; i++)
					Enemy_2_Position[i].x += 3.0f;
				for (int i = 0; i < E_3_Cnt; i++)
					Enemy_3_Position[i].x += 3.0f;

				for (int i = 0; i < 30; i++)
					Ruler_Pos[i].x += 3.0f;
				for (int i = 0; i < 30; i++)
					SetSquare_Pos[i].x += 3.0f;
				for (int i = 0; i < 30; i++)
					Compass_Pos[i].x += 3.0f;

				for (int i = 0; i < 5; i++)
				Safe_Zone_Pos[i].x += 3.0f;

				Start_Zone_Pos.x += 3.0f;
				Goal_Zone_Pos.x += 3.0f;
			}
			else
				Player_Pos.x -= 3.0f;
		}
		if (keyboard::Button(keyboard::KEY_ID::D))//Dキーを押すと右へ移動
		{
			if(Player_Pos.x < 700.0f)
				Player_Pos.x += 3.0f;
			else if (Pos_Background_Under_Left.x > -1280.0f)
			{
				Pos_Background_Under_Left.x -= 3.0f;
				Pos_Background_Under_Right.x -= 3.0f;
				Pos_Background_On_Left.x -= 3.0f;
				Pos_Background_On_Right.x -= 3.0f;

				for (int i = 0; i < Object_1_Cnt; i++)
					Object_1_Pos[i].x -= 3.0f;
				for (int i = 0; i < Object_2_Cnt; i++)
					Object_2_Pos[i].x -= 3.0f;
				for (int i = 0; i < Object_3_Cnt; i++)
					Object_3_Pos[i].x -= 3.0f;
				for (int i = 0; i < Object_4_Cnt; i++)
					Object_4_Pos[i].x -= 3.0f;
				for (int i = 0; i < Object_5_Cnt; i++)
					Object_5_Pos[i].x -= 3.0f;
				for (int i = 0; i < 1; i++)
					Object_6_Pos[i].x -= 3.0f;
				for (int i = 0; i < Object_8_Cnt; i++)
					Object_8_Pos[i].x -= 3.0f;
				for (int i = 0; i < Object_9_Cnt; i++)
					Object_9_Pos[i].x -= 3.0f;
				for (int i = 0; i < Object_10_Cnt; i++)
					Object_10_Pos[i].x -= 3.0f;

				for (int i = 0; i < Star_Cnt; i++)
					Star_Pos[i].x -= 3.0f;

				for (int i = 0; i < Spike_Cnt; i++)
					Spike_Pos[i].x -= 3.0f;

				for (int i = 0; i < Coin_1_cnt; i++)
					Coin_1_Position[i].x -= 3.0f;
				for (int i = 0; i < Coin_5_cnt; i++)
					Coin_5_Position[i].x -= 3.0f;
				for (int i = 0; i < Coin_10_cnt; i++)
					Coin_10_Position[i].x -= 3.0f;
				for (int i = 0; i < Coin_50_cnt; i++)
					Coin_50_Position[i].x -= 3.0f;

				for (int i = 0; i < E_1_Cnt; i++)
					Enemy_1_Position[i].x -= 3.0f;
				for (int i = 0; i < E_2_Cnt; i++)
					Enemy_2_Position[i].x -= 3.0f;
				for (int i = 0; i < E_3_Cnt; i++)
					Enemy_3_Position[i].x -= 3.0f;

				for (int i = 0; i < 30; i++)
					Ruler_Pos[i].x -= 3.0f;
				for (int i = 0; i < 30; i++)
					SetSquare_Pos[i].x -= 3.0f;
				for (int i = 0; i < 30; i++)
					Compass_Pos[i].x -= 3.0f;

				for (int i = 0; i < 5; i++)
					Safe_Zone_Pos[i].x -= 3.0f;

				Start_Zone_Pos.x -= 3.0f;
				Goal_Zone_Pos.x -= 3.0f;
			}
			else
				Player_Pos.x += 3.0f;
		}
	}

	if (Player_Pos.y < 150.0f && Change_Flg == false && Scroll_Cnt < 2)
	{
		Scroll_Cnt += 1;

		Player_Pos.y += 300.0f;

		Pos_Background_Under_Left.y += 360.0f;
		Pos_Background_Under_Right.y += 360.0f;
		Pos_Background_On_Left.y += 360.0f;
		Pos_Background_On_Right.y += 360.0f;

		for (int i = 0; i < Object_1_Cnt; i++)
			Object_1_Pos[i].y += 360.0f;
		for (int i = 0; i < Object_2_Cnt; i++)
			Object_2_Pos[i].y += 360.0f;
		for (int i = 0; i < Object_3_Cnt; i++)
			Object_3_Pos[i].y += 360.0f;
		for (int i = 0; i < Object_4_Cnt; i++)
			Object_4_Pos[i].y += 360.0f;
		for (int i = 0; i < Object_5_Cnt; i++)
			Object_5_Pos[i].y += 360.0f;
		for (int i = 0; i < 1; i++)
			Object_6_Pos[i].y += 360.0f;
		for (int i = 0; i < Object_8_Cnt; i++)
			Object_8_Pos[i].y += 360.0f;
		for (int i = 0; i < Object_9_Cnt; i++)
			Object_9_Pos[i].y += 360.0f;
		for (int i = 0; i < Object_10_Cnt; i++)
			Object_10_Pos[i].y += 360.0f;

		for (int i = 0; i < Star_Cnt; i++)
			Star_Pos[i].y += 360.0f;

		for (int i = 0; i < Spike_Cnt; i++)
			Spike_Pos[i].y += 360.0f;

		for (int i = 0; i < Coin_1_cnt; i++)
			Coin_1_Position[i].y += 360.0f;
		for (int i = 0; i < Coin_5_cnt; i++)
			Coin_5_Position[i].y += 360.0f;
		for (int i = 0; i < Coin_10_cnt; i++)
			Coin_10_Position[i].y += 360.0f;
		for (int i = 0; i < Coin_50_cnt; i++)
			Coin_50_Position[i].y += 360.0f;

		for (int i = 0; i < E_1_Cnt; i++)
			Enemy_1_Position[i].y += 360.0f;
		for (int i = 0; i < E_2_Cnt; i++)
			Enemy_2_Position[i].y += 360.0f;
		for (int i = 0; i < E_3_Cnt; i++)
			Enemy_3_Position[i].y += 360.0f;

		for (int i = 0; i < 30; i++)
			Ruler_Pos[i].y += 360.0f;
		for (int i = 0; i < 30; i++)
			SetSquare_Pos[i].y += 360.0f;
		for (int i = 0; i < 30; i++)
			Compass_Pos[i].y += 360.0f;

		for (int i = 0; i < 5; i++)
			Safe_Zone_Pos[i].y += 360.0f;

		Start_Zone_Pos.y += 360.0f;
		Goal_Zone_Pos.y += 360.0f;

	}

	if (keyboard::Trigger(keyboard::KEY_ID::SPACE))//スペースを押すとジャンプ
	{
		//オブジェクトの数だけ判定をする
		for (int i = 0; i < Object_1_Cnt; i++)
		{
			//オブジェクトの上であればジャンプができる
			if (Player_Pos.y == object_1.Get_Object_1_Pos_Y(i) - 64)
				Jump();
		}
		//オブジェクトの数だけ判定をする
		for (int i = 0; i < Object_2_Cnt; i++)
		{
			//オブジェクトの上であればジャンプができる
			if (Player_Pos.y == object_2.Get_Object_2_Pos_Y(i) - 64)
				Jump();
		}
		//オブジェクトの数だけ判定をする
		for (int i = 0; i < Object_3_Cnt; i++)
		{
			//オブジェクトの上であればジャンプができる
			if (Player_Pos.y == object_3.Get_Object_3_Pos_Y(i) - 64)
				Jump();
		}
		//オブジェクトの数だけ判定をする
		for (int i = 0; i < Object_4_Cnt; i++)
		{
			//オブジェクトの上であればジャンプができる
			if (Player_Pos.y == object_4.Get_Object_4_Pos_Y(i) - 64)
				Jump();
		}
		//オブジェクトの数だけ判定をする
		for (int i = 0; i < Object_5_Cnt; i++)
		{
			//オブジェクトの上であればジャンプができる
			if (Player_Pos.y == object_5.Get_Object_5_Pos_Y(i) - 64)
				Jump();
		}
		//オブジェクトの数だけ判定をする
		for (int i = 0; i < Object_6_Cnt; i++)
		{
			//オブジェクトの上であればジャンプができる
			if (Player_Pos.y == object_6.Get_Object_6_Pos_Y(i) - 64)
				Jump();
		}
		//オブジェクトの数だけ判定をする
		for (int i = 0; i < Object_8_Cnt; i++)
		{
			//オブジェクトの上であればジャンプができる
			if (Player_Pos.y == object_8.Get_Object_8_Pos_Y(i) - 64)
				Jump();
		}
		//オブジェクトの数だけ判定をする
		for (int i = 0; i < Object_9_Cnt; i++)
		{
			//オブジェクトの上であればジャンプができる
			if (Player_Pos.y == object_9.Get_Object_9_Pos_Y(i) - 64)
				Jump();
		}
		//オブジェクトの数だけ判定をする
		for (int i = 0; i < Object_10_Cnt; i++)
		{
			//オブジェクトの上であればジャンプができる
			if (Player_Pos.y == object_10.Get_Object_10_Pos_Y(i) - 64)
				Jump();
		}
	}

	//ジャンプのための処理
	if (P_Jp > 45)
	{
		P_Jp -= 2;
	}

	//ジャンプ時の挙動
	Player_Pos.y += sin(P_Jp * 3.14f / 130.0f) * 10.0f;

	//重力
	Player_Pos.y += Gravity;

	//ステージ中のオブジェクトとの当たり判定をする関数
	for (int i = 0; i < Object_1_Cnt; i++)
	{
		Player_Pos = object_1.CheckHit_Object(Player_Pos, Player_Radius, i);
	}
	//ステージ中のオブジェクトとの当たり判定をする関数
	for (int i = 0; i < Object_2_Cnt; i++)
	{
		Player_Pos = object_2.CheckHit_Object(Player_Pos, Player_Radius, i);
	}
	//ステージ中のオブジェクトとの当たり判定をする関数
	for (int i = 0; i < Object_3_Cnt; i++)
	{
		Player_Pos = object_3.CheckHit_Object(Player_Pos, Player_Radius, i);
	}
	//ステージ中のオブジェクトとの当たり判定をする関数
	for (int i = 0; i < Object_4_Cnt; i++)
	{
		Player_Pos = object_4.CheckHit_Object(Player_Pos, Player_Radius, i);
	}
	//ステージ中のオブジェクトとの当たり判定をする関数
	for (int i = 0; i < Object_5_Cnt; i++)
	{
		Player_Pos = object_5.CheckHit_Object(Player_Pos, Player_Radius, i);
	}
	//ステージ中のオブジェクトとの当たり判定をする関数
	for (int i = 0; i < Object_6_Cnt; i++)
	{
		Player_Pos = object_6.CheckHit_Object(Player_Pos, Player_Radius, i);
	}
	//ステージ中のオブジェクトとの当たり判定をする関数
	for (int i = 0; i < Object_8_Cnt; i++)
	{
		Player_Pos = object_8.CheckHit_Object(Player_Pos, Player_Radius, i);
	}
	//ステージ中のオブジェクトとの当たり判定をする関数
	for (int i = 0; i < Object_9_Cnt; i++)
	{
		Player_Pos = object_9.CheckHit_Object(Player_Pos, Player_Radius, i);
	}
	//ステージ中のオブジェクトとの当たり判定をする関数
	for (int i = 0; i < Object_10_Cnt; i++)
	{
		Player_Pos = object_10.CheckHit_Object(Player_Pos, Player_Radius, i);
	}

	if (Player_Pos.y >= Ground_Line)
	{
		Player_Pos.y = Ground_Line - Player_Height;
	}

}

void Player::Draw(void)
{
	vivid::DrawTexture("data\\player.png", Player_Pos);
}

void Player::Finalize(void)
{
}