#include "3.Compass.h"

vivid::Vector2 Base::Compass_Pos[];


Compass_Block::Compass_Block()
{
}

Compass_Block::~Compass_Block()
{
}

void Compass_Block::Initialize(void)
{
	for (int i = 0; i < 30; i++)
	{
		Compass_Pos[i] = vivid::Vector2::ZERO;
		Compass_Pos[i].x = { -1000.0f };
		ActiveFlag[i] = false;
	}
}

void Compass_Block::Update(void)
{
}

void Compass_Block::Draw(int i)
{
	if (ActiveFlag[i])
		vivid::DrawTexture("data\\compass_circle3.png", Compass_Pos[i]);
}

void Compass_Block::Finalize(void)
{
}

bool Compass_Block::CheckHit_Block(const vivid::Vector2& pos, int i)
{
	if (pos.x > Compass_Pos[i].x && pos.x < Compass_Pos[i].x + Compass_Width
		&& pos.y > Compass_Pos[i].y && pos.y < Compass_Pos[i].y + Compass_Height)
	{
		return true;
	}
	return false;
}

void Compass_Block::Hit(int i, vivid::Vector2 pos)
{
	Compass_Pos[i].x = pos.x - 100;
	Compass_Pos[i].y = pos.y + 200;

	ActiveFlag[i] = true;
}
