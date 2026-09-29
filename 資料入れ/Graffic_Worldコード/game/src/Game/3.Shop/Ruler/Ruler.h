#pragma once
#include "../../Base.h"

#include "../../99.Block_Manager/Block_Id.h"
#include "../../99.Block_Manager/Block_Manager.h""

using namespace std;

class Ruler : public Base
{
public:
	Ruler();
	~Ruler();

	void Initialize(void);
	int Update(void);
	void Draw(void);
	void Finalize(void);

protected:
	int Ruler_Cnt;				//所持していたものさしの数

	int Buy_Ruler_Cnt;			//ものさしを買った個数

	int Total_Ruler_Money;		//買った時のものさしの合計の値段

	string Ruler_Money_Text1 = "";
	string Ruler_Money_Text2 = "円";
	string Ruler_Money_Text3 = "";		//ものさしの値段テキスト

	string Yet_Ruler_Count_Text1 = "×";
	string Yet_Ruler_Count_Text2 = "";
	string Yet_Ruler_Count_Text3 = "";	//左側のものさしの個数表示テキスト

	string Receipt_Ruler_Text1 = "× ";
	string Receipt_Ruler_Text2 = "";
	string Receipt_Ruler_Text3 = "";
	string Receipt_Ruler_Text4 = " 円";
	string Receipt_Ruler_Text5 = "";		//レシートのものさしのテキスト

};
