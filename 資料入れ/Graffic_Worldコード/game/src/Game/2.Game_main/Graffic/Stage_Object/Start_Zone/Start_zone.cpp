#include "Start_Zone.h"

vivid::Vector2 Base::Start_Zone_Pos;

Start_Zone::Start_Zone()
{
}

Start_Zone::~Start_Zone()
{
}

void Start_Zone::Initialize(void)
{
	if (Change_Flg == false)
	{
		Start_Zone_Pos = { 0.0f,Ground_Line - Start_Zone_Height };
	}
	else
	{
		Start_Zone_Pos = { 0.0f, Ground_Line - Start_Zone_Height };
	}
}

void Start_Zone::Update(void)
{
}

void Start_Zone::Draw(void)
{
	vivid::DrawTexture("data\\Startzone.png", Start_Zone_Pos);
}

void Start_Zone::Finalize(void)
{
}
