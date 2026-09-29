#include "Game.clear.h"

Game_clear::Game_clear()
	: pos(vivid::Vector2(0.0f, 0.0f))
	, box_pos2(vivid::Vector2(130.0f, 540.0f))
	, box_pos3(vivid::Vector2(740.0f, 540.0f))
	, p_pos(vivid::Vector2::ZERO)
{
}

Game_clear::~Game_clear()
{
}

void Game_clear::Initialize(void)
{
	Scene_Flg = false;
}

void Game_clear::Update(void)
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

	//if (vivid::mouse::Trigger(vivid::mouse::BUTTON_ID::LEFT))
	//	if ((p_pos.x > box_pos3.x) && (p_pos.x < box_pos3.x + 400.0f) &&
	//		(p_pos.y > box_pos3.y) && (p_pos.y < box_pos3.y + 150.0f))
	//	{
	//		// 次のステージに進む
	//		game.ChangeScene(SCENE_ID::GAME_MAIN);
	//	}
}

void Game_clear::Draw(void)
{
	vivid::DrawTexture("data\\GAME-CLEAR.png", pos);//背景
}

void Game_clear::Finalize(void)
{
}
