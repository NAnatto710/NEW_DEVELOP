#include "Goal_Zone.h"

vivid::Vector2 Base::Goal_Zone_Pos;

Goal_Zone::Goal_Zone()
{
}

Goal_Zone::~Goal_Zone()
{
}

void Goal_Zone::Initialize(void)
{
	if (Change_Flg == false)
	{
		Goal_Zone_Pos = { 2560.0f - Goal_Zone_Width - 80.0f, -370.0f };
	}
	else
	{
		Goal_Zone_Pos = { 2560.0f - Goal_Zone_Width - 80.0f ,Ground_Line - Goal_Zone_Height };
	}
}

void Goal_Zone::Update(void)
{
}

void Goal_Zone::Draw(void)
{
	vivid::DrawTexture("data\\Goal.png", Goal_Zone_Pos, 0xffffffff);
}

void Goal_Zone::Finalize(void)
{
}
