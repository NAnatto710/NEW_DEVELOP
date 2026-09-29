#include "Star_Object.h"

vivid::Vector2 Base::Star_Pos[];

Star_Object::Star_Object()
{
}

Star_Object::~Star_Object()
{
}

void Star_Object::Initialize(void)
{
	if (Change_Flg == false)
	{
		Star_Pos[0] = { 874.0f,-650.0f };
	}
	else
	{
		Star_Pos[0] = { 1950.0f,350.0f };
	}
}

void Star_Object::Update(void)
{
}

void Star_Object::Draw(void)
{
	for (int i = 0; i < Star_Cnt; i++)
	{
		vivid::DrawTexture("data\\star.png", Star_Pos[i], 0xffffff00);
	}
}

void Star_Object::Finalize(void)
{
}
