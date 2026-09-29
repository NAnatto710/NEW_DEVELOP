#include "Safe_Zone.h"

vivid::Vector2 Base::Safe_Zone_Pos[];

Safe_Zone::Safe_Zone()
{
}

Safe_Zone::~Safe_Zone()
{
}

void Safe_Zone::Initialize(void)
{
	if (Change_Flg == false)
	{
		Safe_Zone_Pos[0] = { 1180.0f,-60.0f };
		Safe_Zone_Pos[1] = { 2154.0f,-58.0f };
		Safe_Zone_Pos[2] = { 0.0f,-5380.0f};
	}
	else
	{
		Safe_Zone_Pos[0] = {1080.0f, Ground_Line - Safe_Zone_Height};
	}
}

void Safe_Zone::Update(void)
{
}

void Safe_Zone::Draw(void)
{
	vivid::DrawTexture("data\\Safezone.png", Safe_Zone_Pos[0]);
	if (Change_Flg == false)
	{
		vivid::DrawTexture("data\\Safezone.png", Safe_Zone_Pos[1]);
		vivid::DrawTexture("data\\Safezone.png", Safe_Zone_Pos[2]);

	}
}

void Safe_Zone::Finalize(void)
{
}
