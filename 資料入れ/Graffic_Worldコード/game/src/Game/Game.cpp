#include "Game.h"
#include "1.Title/Title.h"
#include "2.Game_main/Game_main.h"
#include "3.Shop/Shop.h"
#include "4.Game_clear/Game.clear.h"
#include "5.Game_over/Game_over.h"

SCENE_ID  scene_id = SCENE_ID::TITLE;

Title		title;
Game_main	game_main;
Game_main	game_main2;
Shop		shop;
Game_clear  game_clear;
Game_over   game_over;


Game::Game()
{
}

Game::~Game()
{
}

void Game::Initialize(void)
{
	ChangeScene(SCENE_ID::TITLE);
}

void Game::Update(void)
{
	vivid::Point mpos = vivid::mouse::GetCursorPos();
	POS = { (float)mpos.x - 10.0f,(float)mpos.y - 40.0f };

	switch (scene_id)
	{
	case SCENE_ID::TITLE:
		title.Update();
		break;

	case SCENE_ID::GAME_MAIN:
		if (Change_Flg == true)
			game_main.Update();
		else
			game_main2.Update();
		break;

	case SCENE_ID::SHOP:
		shop.Update();
		break;

	case SCENE_ID::GAME_CLEAR:
		game_clear.Update();
		break;

	case SCENE_ID::GAME_OVER:
		game_over.Update();
		break;
	}
}

void Game::Draw(void)
{
	switch (scene_id)
	{
	case SCENE_ID::TITLE:
		title.Draw();
		break;

	case SCENE_ID::GAME_MAIN:
		if (Change_Flg == true)
			game_main.Draw();
		else
			game_main2.Draw();
		break;

	case SCENE_ID::SHOP:
		shop.Draw();
		break;

	case SCENE_ID::GAME_CLEAR:
		game_clear.Draw();
		break;

	case SCENE_ID::GAME_OVER:
		game_over.Draw();
		break;
	}

	vivid::DrawTexture("data\\mouse_pencil.png", POS);
}

void Game::Finalize(void)
{
	switch (scene_id)
	{
	case SCENE_ID::TITLE:
		title.Finalize();
		break;

	case SCENE_ID::GAME_MAIN:
		if (Change_Flg == true)
			game_main.Finalize();
		else
			game_main2.Finalize();
		break;

	case SCENE_ID::SHOP:
		shop.Finalize();
		break;

	case SCENE_ID::GAME_CLEAR:
		game_clear.Finalize();
		break;

	case SCENE_ID::GAME_OVER:
		game_over.Finalize();
		break;
	}
}

void Game::ChangeScene(SCENE_ID next_scene)
{
	//現在のシーンの解放
	//各シーンのFinalizeを呼ぶ
	switch (scene_id)
	{
	case SCENE_ID::TITLE:
		title.Finalize();
		break;

	case SCENE_ID::GAME_MAIN:
		if (Change_Flg == true)
			game_main.Finalize();
		else
			game_main2.Finalize();
		break;

	case SCENE_ID::SHOP:
		shop.Finalize();
		break;

	case SCENE_ID::GAME_CLEAR:
		game_clear.Finalize();
		break;

	case SCENE_ID::GAME_OVER:
		game_over.Finalize();
		break;
	}

	//シーンIDの更新
	scene_id = next_scene;

	//新しいシーンの初期化
	//各シーンのInitializeを呼ぶ
	switch (scene_id)
	{
	case SCENE_ID::TITLE:
		title.Initialize();
		break;

	case SCENE_ID::GAME_MAIN:
		if (Scene_Flg == false)
			if (Change_Flg == true)
				game_main.Initialize();
			else
				game_main2.Initialize();

	case SCENE_ID::SHOP:
		shop.Initialize();
		break;

	case SCENE_ID::GAME_CLEAR:
		game_clear.Initialize();
		break;

	case SCENE_ID::GAME_OVER:
		game_over.Initialize();
		break;
	}
}
