#include "Game_main.h"

Game_main::Game_main()
{
}

Game_main::~Game_main()
{
}

void Game_main::Initialize(void)
{
	Scene_Flg = true;
	background.Initialize();
	character.Initialize();
	coin.Initialize();
	Hp.Initialize();
	time.Initialize();

	UI = true;
	check = false;
	cnt = 0;
}

void Game_main::Update(void)
{
	character.Update();
	coin.Update();
	time.Update();
	background.Update();

	if( UI == true && vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::SPACE))
		UI = false;


	if (vivid::keyboard::Button(vivid::keyboard::KEY_ID::T))
	{
		check = true;
		cnt += 1;
	}

	if (check == true && cnt > 50)
	{
		if (vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::T))
			game.ChangeScene(SCENE_ID::TITLE);
		else
			check = false;
	}

	for (int i = 0; i < 5; i++)
	{
		//シーン切り替えの関数呼び出し
		if ((background.CheckHit_Start() || background.CheckHit_Safe(i)) && UI == false)
			if (vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::W))
				game.ChangeScene(SCENE_ID::SHOP);
	}

	//HPが0になると呼び出される
	if (character.Get_Game_Over_Flg() || time.Get_Time_Flg())
		game.ChangeScene(SCENE_ID::GAME_OVER);

	if (background.Get_Game_Clear_Flg())
		game.ChangeScene(SCENE_ID::GAME_CLEAR);
}

void Game_main::Draw(void)
{
	background.Draw();
	time.Draw();
	Hp.Draw();
	coin.Draw();
	character.Draw();
	if (UI)
	{
		vivid::DrawTexture("data\\UI.png", vivid::Vector2(0.0f, 0.0f));
	}

}

void Game_main::Finalize(void)
{
	character.Finalize();
	background.Finalize();
	coin.Finalize();
	Hp.Finalize();
	time.Finalize();
}
