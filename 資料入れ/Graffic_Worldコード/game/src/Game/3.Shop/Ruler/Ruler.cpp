#include "Ruler.h"

Ruler::Ruler()
{
}

Ruler::~Ruler()
{
}

void Ruler::Initialize(void)
{
	//もともと持っていたものさしの数
	Ruler_Cnt = Block_Manager::GetInstance().Get_Block_Cnt(BLOCK_ID::RULER);

	//買った時のものさしの合計の値段
	Total_Ruler_Money = 0;

	//ものさしを買った個数
	Buy_Ruler_Cnt = 0;

	//ものさしの値段を表示するテキスト
	Ruler_Money_Text1 = to_string(Ruler_Price);
	Ruler_Money_Text3 = Ruler_Money_Text1 + Ruler_Money_Text2;
}

int Ruler::Update(void)
{
	//マウスカーソルの座標取得
	vivid::Point point = vivid::mouse::GetCursorPos();

	//30cmものさしを買う処理
	if ((vivid::mouse::Trigger(vivid::mouse::BUTTON_ID::LEFT)) &&
		((915.0f < point.x) && (105.0f < point.y && point.y < 250.0f)) &&
		(Coin_All >= Ruler_Price))
	{
		//ものさしの金額分引かれる処理
		Coin_All -= Ruler_Price;

		//買うものさしの個数を一つ追加処理
		Buy_Ruler_Cnt += 1;

		//元々持っているものさしの個数を一個追加
		Ruler_Cnt += 1;
	}

	//30cmものさしを売る処理
	if ((vivid::mouse::Trigger(vivid::mouse::BUTTON_ID::RIGHT)) &&
		((915.0f < point.x) && (105.0f < point.y && point.y < 250.0f)) &&
		(1 <= Buy_Ruler_Cnt || 1 <= Ruler_Cnt))
	{
		//所持金にものさしの値段を追加する処理
		Coin_All += Ruler_Price;

		if (1 <= Buy_Ruler_Cnt)
		{
			//買うものさしの個数を一つ減らす処理
			Buy_Ruler_Cnt -= 1;
			Ruler_Cnt -= 1;
		}
		else 
		{
			//元々持っているものさしの個数を一つ減らす処理
			Ruler_Cnt -= 1;
		}
	}
	Total_Ruler_Money = Ruler_Price * Buy_Ruler_Cnt;

	//左側のものさしの個数表示テキスト
	Yet_Ruler_Count_Text2 = std::to_string(Ruler_Cnt);
	Yet_Ruler_Count_Text3 = Yet_Ruler_Count_Text1 + Yet_Ruler_Count_Text2;

	//ものさしのレシートテキスト
	Receipt_Ruler_Text2 = std::to_string(Ruler_Cnt);
	Receipt_Ruler_Text3 = std::to_string(Total_Ruler_Money);
	Receipt_Ruler_Text5 = Receipt_Ruler_Text1 + Receipt_Ruler_Text2 + "   "
									+ Receipt_Ruler_Text3 + Receipt_Ruler_Text4;

	Block_Manager::GetInstance().Set_Block_Cnt(BLOCK_ID::RULER, Ruler_Cnt);

	return Total_Ruler_Money;
}

void Ruler::Draw(void)
{
	vivid::DrawTexture("data\\w_box.png", vivid::Vector2(1145.0f, 200.0f), 0xff3b7258);
	//ものさしの値段表示
	vivid::DrawText(40, Ruler_Money_Text3, vivid::Vector2(1150.0f, 210.0f), 0xff66ff33);

	//左側のものさしの個数表示
	vivid::DrawText(40, Yet_Ruler_Count_Text3, vivid::Vector2(310.0f, 180.0f), 0xff66ff33);

	//レシートのものさし表示
	vivid::DrawText(40, Receipt_Ruler_Text5, vivid::Vector2(500.0f, 190.0f), 0xff66ff33);
}

void Ruler::Finalize(void)
{
}
