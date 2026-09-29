#include "Pencil.h"

Pencil::Pencil()
{
}

Pencil::~Pencil()
{
}

void Pencil::Initialize(void)
{
	//すでに持っている鉛筆の個数
	Pencil_Cnt = Block_Manager::GetInstance().Get_Block_Cnt(BLOCK_ID::PENCIL);

	//買った時の鉛筆の合計の値段
	Total_Pencil_Money = 0;

	//鉛筆を買った個数
	Buy_Pencil_Cnt = 0;

	//鉛筆の値段を表示するテキスト
	Pencil_Money_Text1 = std::to_string(Pencil_Price);
	Pencil_Money_Text3 = Pencil_Money_Text1 + Pencil_Money_Text2;
}

int Pencil::Update(void)
{
	//マウスカーソルの座標取得
	vivid::Point point = vivid::mouse::GetCursorPos();

	////鉛筆を買う処理
	//if ((vivid::mouse::Trigger(vivid::mouse::BUTTON_ID::LEFT)) &&
	//   ((915.0f < point.x) && (551.0f < point.y && point.y < 695.0f)) &&
	//   (Coin_All >= Pencil_Price))
	//{
	//	//鉛筆の金額分引かれる処理
	//	Coin_All -= Pencil_Price;

	//	//買う鉛筆の個数を一つ追加処理
	//	Buy_Pencil_Cnt += 1;

	//	//元々持っている鉛筆の個数を一個追加
	//	Pencil_Cnt += 1;
	//}

	////鉛筆を売る処理
	//if ((vivid::mouse::Trigger(vivid::mouse::BUTTON_ID::RIGHT)) &&
	//   ((915.0f < point.x) && (551.0f < point.y && point.y < 695.0f)) &&
	//   (1 <= Buy_Pencil_Cnt || 1 <= Pencil_Cnt))
	//{
	//	//所持金に鉛筆の値段を追加する処理
	//	Coin_All += Pencil_Price;

	//	if (1 <= Buy_Pencil_Cnt)
	//	{
	//		//買う鉛筆の個数を一つ減らす処理
	//		Buy_Pencil_Cnt -= 1;
	//		Pencil_Cnt -= 1;
	//	}
	//	else 
	//	{
	//		//元々持っている鉛筆の個数を一つ減らす処理
	//		Pencil_Cnt -= 1;
	//	}
	//}
	Total_Pencil_Money = Pencil_Price * Buy_Pencil_Cnt;

	//左側の鉛筆の個数表示テキスト
	Yet_Pencil_Count_Text2 = std::to_string(Pencil_Cnt);
	Yet_Pencil_Count_Text3 = Yet_Pencil_Count_Text1 + Yet_Pencil_Count_Text2;

	//鉛筆のレシートテキスト
	Receipt_Pencil_Text2 = std::to_string(Buy_Pencil_Cnt);
	Receipt_Pencil_Text3 = std::to_string(Total_Pencil_Money);
	Receipt_Pencil_Text5 = Receipt_Pencil_Text1 + Receipt_Pencil_Text2 + "   "
						+ Receipt_Pencil_Text3 + Receipt_Pencil_Text4;

	Block_Manager::GetInstance().Set_Block_Cnt(BLOCK_ID::PENCIL,Pencil_Cnt);

	return Total_Pencil_Money;
}

void Pencil::Draw(void)
{
	vivid::DrawTexture("data\\w_box.png", vivid::Vector2(1145.0f, 645.0f), 0xff3b7258);
	//鉛筆の値段表示
	vivid::DrawText(40, Pencil_Money_Text3, vivid::Vector2(1150.0f, 655.0f), 0xff66ff33);

	//左側の鉛筆の個数表示
	vivid::DrawText(40, Yet_Pencil_Count_Text3, vivid::Vector2(310.0f, 550.0f), 0xff66ff33);

	//レシートの鉛筆表示
	vivid::DrawText(40, Receipt_Pencil_Text5, vivid::Vector2(500.0f, 460.0f), 0xff66ff33);

	vivid::DrawTexture("data\\Batu (2).png", vivid::Vector2(750.0f, 225.0f));
	vivid::DrawTexture("data\\Batu (2).png", vivid::Vector2(-150.0f, 140.0f));
}

void Pencil::Finalize(void)
{
}
