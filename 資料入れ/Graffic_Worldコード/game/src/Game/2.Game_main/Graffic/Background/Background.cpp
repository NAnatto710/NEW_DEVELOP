#include "Background.h"

vivid::Vector2 Base::Pos_Background_Under_Left ;
vivid::Vector2 Base::Pos_Background_Under_Right;
vivid::Vector2 Base::Pos_Background_On_Left;
vivid::Vector2 Base::Pos_Background_On_Right;

float Base::Ground_Line;


Background::Background()
{
}

Background::~Background()
{
}

void Background::Initialize(void)
{
	Ground_Line = 690.0f;

	object_1.Initialize();
	object_2.Initialize();
	object_3.Initialize();
	object_4.Initialize();
	object_5.Initialize();
	object_6.Initialize();
	object_8.Initialize();
	object_9.Initialize();
	object_10.Initialize();

	star.Initialize();
	start.Initialize();
	safe.Initialize();
	goal.Initialize();
	spike.Initialize();

	Pos_Background_Under_Left = { 0.0f,0.0f };
	Pos_Background_Under_Right = { 1280.0f, 0.0f };
	Pos_Background_On_Left = { 0.0f, -720.0f };
	Pos_Background_On_Right = { 1280.0f, -720.0f };

	Game_Clear_Flg = false;
}

void Background::Update(void)
{
	if (Scroll_Cnt == 1)
		Ground_Line = 570.0f;

	if (Scroll_Cnt == 2)
		Ground_Line = 450.0f;

	Goal_Check();
}

void Background::Draw(void)
{
	vivid::DrawTexture("data\\note1.png", Pos_Background_Under_Left);
	vivid::DrawTexture("data\\note4.png", Pos_Background_Under_Right);

	vivid::DrawTexture("data\\note2.png", Pos_Background_On_Left);
	vivid::DrawTexture("data\\note3.png", Pos_Background_On_Right);

	object_1.Draw();
	object_2.Draw();
	object_3.Draw();
	object_4.Draw();
	object_5.Draw();
	object_6.Draw();
	object_8.Draw();
	object_9.Draw();
	object_10.Draw();

	star.Draw();
	spike.Draw();
	start.Draw();
	safe.Draw();
	goal.Draw();
}

void Background::Finalize(void)
{
}

void Background::Goal_Check(void)
{
	vivid::Vector2 P_Center = { Player_Pos.x + Player_Radius,Player_Pos.y + Player_Radius };

	bool result_h = P_Center.x > Goal_Zone_Pos.x - Player_Radius
		&& P_Center.x < Goal_Zone_Pos.x + Goal_Zone_Width + Player_Radius
		&& P_Center.y > Goal_Zone_Pos.y
		&& P_Center.y < Goal_Zone_Pos.y + Goal_Zone_Height;

	//点と短形の判定その２
	bool result_v = P_Center.x > Goal_Zone_Pos.x
		&& P_Center.x < Goal_Zone_Pos.x + Goal_Zone_Width
		&& P_Center.y > Goal_Zone_Pos.y - Player_Radius
		&& P_Center.y < Goal_Zone_Pos.y + Goal_Zone_Height + Player_Radius;

	//オブジェクトの中にブロックが入ったとき
	if (result_h || result_v)
	{
		Game_Clear_Flg = true;
	}
}

bool Background::CheckHit_Start(void)
{
	return	Player_Pos.x > Start_Zone_Pos.x - Player_Width &&
			Player_Pos.x < Start_Zone_Pos.x + Start_Zone_Width &&
			Player_Pos.y > Start_Zone_Pos.y - Player_Height &&
			Player_Pos.y < Start_Zone_Pos.y + Start_Zone_Height;
}

bool Background::CheckHit_Safe(int i)
{
	return	Player_Pos.x > Safe_Zone_Pos[i].x - Player_Width &&
			Player_Pos.x < Safe_Zone_Pos[i].x + Safe_Zone_Width &&
			Player_Pos.y > Safe_Zone_Pos[i].y - Player_Height &&
			Player_Pos.y < Safe_Zone_Pos[i].y + Safe_Zone_Height;
}
