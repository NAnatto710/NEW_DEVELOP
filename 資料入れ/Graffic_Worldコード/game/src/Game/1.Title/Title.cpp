#include "Title.h"

bool Base::Scene_Flg;
bool Base::Change_Flg;

Title::Title()
{
}

Title::~Title()
{
}

void Title::Initialize(void)
{
	Scene_Flg = false;
	Time_Cnt = 0;
	Change_Flg = true;
}

void Title::Update(void)
{
}

void Title::Draw(void)
{
	//マウスカーソルの座標取得
	vivid::Point point = vivid::mouse::GetCursorPos();

	vivid::DrawTexture("data\\title.png", vivid::Vector2(0.0f, 0.0f));

	if (Time_Cnt < 10)
		Time_Cnt += 1;

	if (!(Time_Cnt > 2))
		return;

	//消しゴムの処理
	if ((220.0f < point.x && point.x < 535.0f) && (520.0f < point.y && point.y < 685.0f))
	{
		//消しゴムの上にカーソルが来たら少し大きく表示
		vivid::DrawTexture("data\\bigeraser.png", EraserPos);

		//クリックしたらゲームメイン画面に移動する処理
		if ((vivid::mouse::Trigger(vivid::mouse::BUTTON_ID::LEFT)))
		{
			Change_Flg = true;
			game.ChangeScene(SCENE_ID::GAME_MAIN);
		}
	}

	//鉛筆の処理
	if ((800.0f < point.x && point.x < 1105.0f) && (525.0f < point.y && point.y < 680.0f))
	{
		//鉛筆の上にカーソルが来たら少し大きく表示
		vivid::DrawTexture("data\\bigpencil.png", PencilPos);

		//クリックしたらゲームメイン画面に移動する処理
		if ((vivid::mouse::Trigger(vivid::mouse::BUTTON_ID::LEFT)))
		{
			Change_Flg = false;
			game.ChangeScene(SCENE_ID::GAME_MAIN);							
		}
	}
}

void Title::Finalize(void)
{
}
