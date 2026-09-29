#include "Game_over.h"

Game_over::Game_over()
	: pos(vivid::Vector2(0.0f, 0.0f))
	, box_pos2(vivid::Vector2(130.0f, 540.0f))
	, p_pos(vivid::Vector2::ZERO)
{
}

Game_over::~Game_over()
{
}

void Game_over::Initialize(void)
{
}

void Game_over::Update(void)
{
	vivid::Point mpos = vivid::mouse::GetCursorPos();
	p_pos.x = (float)mpos.x;
	p_pos.y = (float)mpos.y;

	if (vivid::mouse::Trigger(vivid::mouse::BUTTON_ID::LEFT))
		if ((p_pos.x > box_pos2.x) && (p_pos.x < box_pos2.x + 400.0f) &&
			(p_pos.y > box_pos2.y) && (p_pos.y < box_pos2.y + 150.0f))
		{
			// タイトルに遷移
			game.ChangeScene(SCENE_ID::TITLE);
		}
}

void Game_over::Draw(void)
{
	vivid::DrawTexture("data\\GAME-OVER.png", pos);//背景
	//vivid::DrawTexture("data\\w_box.png", box_pos2, 0xff00ff00);//タイトル戻るボタン
}

void Game_over::Finalize(void)
{
}
