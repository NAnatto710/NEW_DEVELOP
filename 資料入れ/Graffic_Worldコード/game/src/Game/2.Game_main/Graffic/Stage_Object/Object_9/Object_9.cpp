#include "Object_9.h"

vivid::Vector2 Base::Object_9_Pos[];

Object_9::Object_9()
{
}

Object_9::~Object_9()
{
}

void Object_9::Initialize(void)
{
	if (Change_Flg == false)
	{
		for (int i = 0; i <= Object_9_Cnt; i++)
		{
			Object_9_Pos[i] = { -1000.0f, 0.0f };
		}
	}
	else
	{
		for (int i = 0; i <= Object_9_Cnt; i++)
		{
			Object_9_Pos[i] = { -1000.0f, 0.0f };
		}
	}
}

void Object_9::Update(void)
{
}

void Object_9::Draw(void)
{
	for (int i = 0; i < Object_9_Cnt; i++)
	{
		vivid::DrawTexture("data\\ground1 30~880.png", Object_9_Pos[i], 0xff8080ff);
	}
}

void Object_9::Finalize(void)
{
}

vivid::Vector2 Object_9::CheckHit_Block(vivid::Vector2 block_pos, int block_width, int block_height, int num)
{
	bool result_A = block_pos.y > Object_9_Pos[num].y - block_height;
	bool result_B = block_pos.y < Object_9_Pos[num].y + Object_9_Height;
	bool result_C = block_pos.x > Object_9_Pos[num].x - block_width;
	bool result_D = block_pos.x < Object_9_Pos[num].x + Object_9_Width;

	if (result_A && result_B && result_C && result_D)
	{
		vivid::Vector2 POS = { block_pos.x , block_pos.y };
		vivid::Vector2 B_Center = { block_pos.x + block_width / 2 , block_pos.y + block_height / 2 };
		O_Center[num] = { Object_9_Pos[num].x + Object_9_Width / 2 , Object_9_Pos[num].y + Object_9_Height / 2 };

		POS.y = Object_9_Pos[num].y + Object_9_Height;
		return POS;
	}
	else
	{
		return block_pos;
	}
}

vivid::Vector2 Object_9::CheckHit_Object(vivid::Vector2 player_pos, float player_radius, int num)
{
	//CIRCLE‚Ì’†S“_‚ð‹‚ß‚é
	vivid::Vector2 circle_center = { player_pos.x + player_radius,player_pos.y + player_radius };

	//“_‚Æ’ZŒ`‚Ì”»’è‚»‚Ì‚P
	bool result_h = circle_center.x > Object_9_Pos[num].x - player_radius
		&& circle_center.x < Object_9_Pos[num].x + Object_9_Width + player_radius
		&& circle_center.y > Object_9_Pos[num].y
		&& circle_center.y < Object_9_Pos[num].y + Object_9_Height;

	//“_‚Æ’ZŒ`‚Ì”»’è‚»‚Ì‚Q
	bool result_v = circle_center.x > Object_9_Pos[num].x
		&& circle_center.x < Object_9_Pos[num].x + Object_9_Width
		&& circle_center.y > Object_9_Pos[num].y - player_radius
		&& circle_center.y < Object_9_Pos[num].y + Object_9_Height + player_radius;

	//“_‚Æ‰~‚Ì”»’è‚»‚Ì‚P
	//BOX‚Ì¶ã
	vivid::Vector2 v = circle_center - Object_9_Pos[num];
	bool result_lu = v.Length() <= player_radius;//Length = sqrt
	//BOX‚Ì‰Eã
	v = circle_center - vivid::Vector2(Object_9_Pos[num].x + Object_9_Width, Object_9_Pos[num].y);
	bool result_ru = v.Length() <= player_radius;
	//BOX‚Ì¶‰º
	v = circle_center - vivid::Vector2(Object_9_Pos[num].x, Object_9_Pos[num].y + Object_9_Height);
	bool result_Id = v.Length() <= player_radius;
	//BOX‚Ì‰E‰º
	v = circle_center - vivid::Vector2(Object_9_Pos[num].x + Object_9_Width, Object_9_Pos[num].y + Object_9_Height);
	bool result_rd = v.Length() <= player_radius;

	//‚Ç‚±‚©‚µ‚ç‚Å^‚Å‚ ‚ê‚Î“–‚½‚Á‚½‚Æ‚Ý‚È‚³‚ê‚é
	if (result_h || result_v || result_lu || result_Id || result_ru || result_rd)
	{
		vivid::Vector2 POS = { player_pos.x , player_pos.y };
		vivid::Vector2 P_Center = { player_pos.x + player_radius , player_pos.y + player_radius };
		vivid::Vector2 O_Center = { Object_9_Pos[num].x + Object_9_Width / 2 , Object_9_Pos[num].y + Object_9_Height / 2 };

		//“Vˆä”»’è
		POS.y = Object_9_Pos[num].y + Object_9_Height;
		return POS;
	}
	else
	{
		return player_pos;
	}
}
