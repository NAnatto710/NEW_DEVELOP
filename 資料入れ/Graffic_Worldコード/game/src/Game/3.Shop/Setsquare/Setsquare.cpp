#include "Setsquare.h"

Setsquare::Setsquare()
{
}

Setsquare::~Setsquare()
{
}

void Setsquare::Initialize(void)
{
	//すでに持っている三角定規の個数
	Setsquare_Cnt = Block_Manager::GetInstance().Get_Block_Cnt(BLOCK_ID::SETSQUARE);

	//買った時の三角定規の合計の値段
	Total_Setsquare_Money = 0;

	//三角定規を買った個数
	Buy_Setsquare_Cnt = 0;

	//三角定規の値段を表示するテキスト
	Setsquare_Money_Text1 = std::to_string(Setsquare_Price);
	Setsquare_Money_Text3 = Setsquare_Money_Text1 + Setsquare_Money_Text2;
}

int Setsquare::Update(void)
{
	//マウスカーソルの座標取得
	vivid::Point point = vivid::mouse::GetCursorPos();

	//三角定規を買う処理
	if ((vivid::mouse::Trigger(vivid::mouse::BUTTON_ID::LEFT)) &&
		((915.0f < point.x) && (251.0f < point.y && point.y < 400.0f)) &&
		(Coin_All >= Setsquare_Price))
	{
		//三角定規の金額分引かれる処理
		Coin_All -= Setsquare_Price;

		//三角定規の個数を一つ追加処理
		Buy_Setsquare_Cnt += 1;

		//元々持っている三角定規の個数を一個追加
		Setsquare_Cnt += 1;
	}

	//三角定規を売る処理
	if ((vivid::mouse::Trigger(vivid::mouse::BUTTON_ID::RIGHT))
		&& ((915.0f < point.x) && (251.0f < point.y && point.y < 400.0f))
		&& (1 <= Buy_Setsquare_Cnt || 1 <= Setsquare_Cnt))
	{
		//所持金に三角定規の値段を追加する処理
		Coin_All += Setsquare_Price;

		if (1 <= Buy_Setsquare_Cnt) 
		{
			//三角定規の個数を一つ減らす処理
			Buy_Setsquare_Cnt -= 1;
			Setsquare_Cnt -= 1;
		}
		else 
		{
			//元々持っている三角定規の個数を一つ減らす処理
			Setsquare_Cnt -= 1;
		}
	}
	Total_Setsquare_Money = Setsquare_Price * Buy_Setsquare_Cnt;

	//左側の三角定規の個数表示テキスト
	Yet_Setsquare_Count_Text2 = std::to_string(Setsquare_Cnt);
	Yet_Setsquare_Count_Text3 = Yet_Setsquare_Count_Text1 + Yet_Setsquare_Count_Text2;

	//三角定規のレシートテキスト
	Receipt_Setsquare_Text2 = std::to_string(Buy_Setsquare_Cnt);
	Receipt_Setsquare_Text3 = std::to_string(Total_Setsquare_Money);
	Receipt_Setsquare_Text5 = Receipt_Setsquare_Text1 + Receipt_Setsquare_Text2 + "   "
							+ Receipt_Setsquare_Text3 + Receipt_Setsquare_Text4;

	Block_Manager::GetInstance().Set_Block_Cnt(BLOCK_ID::SETSQUARE, Setsquare_Cnt);

	return Total_Setsquare_Money;
}

void Setsquare::Draw(void)
{
	vivid::DrawTexture("data\\w_box.png", vivid::Vector2(1145.0f, 350.0f),0xff3b7258);
	//三角定規の値段表示
	vivid::DrawText(40, Setsquare_Money_Text3, vivid::Vector2(1150.0f, 360.0f), 0xff66ff33);

	//左側の三角定規の個数表示
	vivid::DrawText(40, Yet_Setsquare_Count_Text3, vivid::Vector2(310.0f, 300.0f), 0xff66ff33);

	//レシートの三角定規表示
	vivid::DrawText(40, Receipt_Setsquare_Text5, vivid::Vector2(500.0f, 280.0f), 0xff66ff33);
}

void Setsquare::Finalize(void)
{
}
