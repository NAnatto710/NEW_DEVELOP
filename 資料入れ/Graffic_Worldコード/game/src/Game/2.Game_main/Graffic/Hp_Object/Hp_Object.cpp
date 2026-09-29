#include "Hp_Object.h"

int Base::HP_level;

Hp_Object::Hp_Object()
	: pos_HP_1(0.0f, 0.0f)
	, pos_HP_2(0.0f, 0.0f)
	, pos_HP_3(0.0f, 0.0f)
{
}

Hp_Object::~Hp_Object()
{
}

void Hp_Object::Initialize(void)
{
	pos_HP_1.x = { 1000.0f };
	pos_HP_1.y = { 0.0f };
	pos_HP_2.x = { 1064.0f };
	pos_HP_2.y = { 0.0f };
	pos_HP_3.x = { 1128.0f };
	pos_HP_3.y = { 0.0f };

	//HP‚Ì‰Šú’l
	HP_level = 3;
}

void Hp_Object::Update(void)
{
}

void Hp_Object::Draw()
{
	//ƒRƒCƒ“‚Ì”‚É‚æ‚Á‚Ä•`‰æ‚·‚éHp‚Ì”‚ğ•Ï‰»‚³‚¹‚é
	switch (HP_level)
	{
	case 0:
		break;
	case 1:
		vivid::DrawTexture("data\\HP.png", pos_HP_1);
		break;
	case 2:
		vivid::DrawTexture("data\\HP.png", pos_HP_1);
		vivid::DrawTexture("data\\HP.png", pos_HP_2);
		break;
	case 3:
		vivid::DrawTexture("data\\HP.png", pos_HP_1);
		vivid::DrawTexture("data\\HP.png", pos_HP_2);
		vivid::DrawTexture("data\\HP.png", pos_HP_3);
		break;
	}
}

void Hp_Object::Finalize(void)
{
}
