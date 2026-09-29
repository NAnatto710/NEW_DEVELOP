#include "Compass.h"

Compass::Compass()
{
}

Compass::~Compass()
{
}

void Compass::Initialize(void)
{
	//すでに持っているコンパスの個数
	Compass_Cnt = Block_Manager::GetInstance().Get_Block_Cnt(BLOCK_ID::COMPASS);

	//買った時のコンパスの合計の値段
	Total_Compass_Money = 0;

	//コンパスを買った個数
	Buy_Compass_Cnt = 0;

	//コンパスの値段を表示するテキスト
	Compass_Money_Text1 = std::to_string(Compass_Price);
	Compass_Money_Text3 = Compass_Money_Text1 + Compass_Money_Text2;
}

int Compass::Update(void)
{
	//マウスカーソルの座標取得
	vivid::Point point = vivid::mouse::GetCursorPos();

	//コンパスを買う処理
	if ((vivid::mouse::Trigger(vivid::mouse::BUTTON_ID::LEFT)) &&
	   ((915.0f < point.x) && (401.0f < point.y && point.y < 550.0f)) &&
	   (Coin_All >= Compass_Price))
	{
		//コンパスの金額分引かれる処理
		Coin_All -= Compass_Price;

		//買うコンパスの個数を一つ追加処理
		Buy_Compass_Cnt += 1;

		//元々持っているコンパスの個数を一個追加
		Compass_Cnt += 1;
	}


	//コンパスを売る処理
	if ((vivid::mouse::Trigger(vivid::mouse::BUTTON_ID::RIGHT)) &&
		((915.0f < point.x) && (401.0f < point.y && point.y < 550.0f)) &&
		(1 <= Buy_Compass_Cnt || 1 <= Compass_Cnt))
	{
		//所持金にコンパスの値段を追加する処理
		Coin_All += Compass_Price;

		if (1 <= Buy_Compass_Cnt)
		{
			//買うコンパスの個数を一つ減らす処理
			Buy_Compass_Cnt -= 1;
			Compass_Cnt -= 1;
		}
		else 
		{
			//元々持っているコンパスの個数を一つ減らす処理
			Compass_Cnt -= 1;
		}
	}

	Total_Compass_Money = Compass_Price * Buy_Compass_Cnt;

	//左側のコンパスの個数表示テキスト
	Yet_Compass_Count_Text2 = std::to_string(Compass_Cnt);
	Yet_Compass_Count_Text3 = Yet_Compass_Count_Text1 + Yet_Compass_Count_Text2;


	//コンパスのレシートテキスト
	Receipt_Compass_Text2 = std::to_string(Buy_Compass_Cnt);
	Receipt_Compass_Text3 = std::to_string(Total_Compass_Money);
	Receipt_Compass_Text5 = Receipt_Compass_Text1 + Receipt_Compass_Text2 + "   "
							+ Receipt_Compass_Text3 + Receipt_Compass_Text4;

	Block_Manager::GetInstance().Set_Block_Cnt(BLOCK_ID::COMPASS,Compass_Cnt);

	return Total_Compass_Money;
}

void Compass::Draw(void)
{
	vivid::DrawTexture("data\\w_box.png", vivid::Vector2(1145.0f, 500.0f), 0xff3b7258);
	//コンパスの値段表示
	vivid::DrawText(40, Compass_Money_Text3, vivid::Vector2(1150.0f, 510.0f), 0xff66ff33);

	//左側のコンパスの個数表示
	vivid::DrawText(40, Yet_Compass_Count_Text3, vivid::Vector2(310.0f, 420.0f), 0xff66ff33);

	//レシートのコンパス表示
	vivid::DrawText(40, Receipt_Compass_Text5, vivid::Vector2(500.0f, 370.0f), 0xff66ff33);
}

void Compass::Finalize(void)
{
}
