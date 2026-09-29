#include "Shop.h"

Shop::Shop()
{
}

Shop::~Shop()
{
}

void Shop::Initialize(void)
{
	compass.Initialize();
	pencil.Initialize();
	ruler.Initialize();
	setsquare.Initialize();

	HP_level = 3;
}

void Shop::Update(void)
{
	//マウスカーソルの座標取得
	vivid::Point point = vivid::mouse::GetCursorPos();

	//ゲームメイン画面へ移動する処理
	if (vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::W))
		game.ChangeScene(SCENE_ID::GAME_MAIN);

	//画面へ移動する処理
	if ((vivid::mouse::Trigger(vivid::mouse::BUTTON_ID::LEFT))
		&& (point.x > 1105.0f && point.y < 40.0f))
	{
		game.ChangeScene(SCENE_ID::GAME_MAIN);
	}

	if (vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::T))
		game.ChangeScene(SCENE_ID::TITLE);

	Total_Buy_Money = compass.Update() + pencil.Update() + ruler.Update() + setsquare.Update();

	Possession_Money_Text2 = std::to_string(Coin_All);
	Receipt_Total_Buy_Money_Text2 = to_string(Total_Buy_Money);
}

void Shop::Draw(void)
{
	//インベントリ画面出力
	vivid::DrawTexture("data\\Inventory_Scene.png", vivid::Vector2(0.0f, 0.0f));

	compass.Draw();
	pencil.Draw();
	ruler.Draw();
	setsquare.Draw();

	//所持金の表示
	vivid::DrawText(40, Possession_Money_Text1, vivid::Vector2(820.0f, 10.0f), 0xff66ff33);
	vivid::DrawText(40, Possession_Money_Text2, vivid::Vector2(950.0f, 10.0f), 0xff66ff33);
	vivid::DrawText(40, Possession_Money_Text3, vivid::Vector2(1050.0f, 10.0f), 0xff66ff33);

	//買った時の合計金額のテキスト
	vivid::DrawText(40, Receipt_Total_Buy_Money_Text1, vivid::Vector2(550.0f, 570.0f), 0xff66ff33);
	vivid::DrawText(40, Receipt_Total_Buy_Money_Text2, vivid::Vector2(600.0f, 570.0f), 0xff66ff33);
	vivid::DrawText(40, Receipt_Total_Buy_Money_Text3, vivid::Vector2(770.0f, 570.0f), 0xff66ff33);

	vivid::DrawTexture("data\\60×280.png", vivid::Vector2(1105.0f, 0.0f), 0xff3b7258);
	vivid::DrawText(40, "戻る", vivid::Vector2(1120.0f, 10.0f));
}

void Shop::Finalize(void)
{
	/*compass.Finalize();
	pencil.Finalize();
	ruler.Finalize();
	setsquare.Finalize();*/
}
