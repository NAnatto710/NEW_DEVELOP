#pragma once
#include "../../Base.h"

#include "../../99.Block_Manager/Block_Id.h"
#include "../../99.Block_Manager/Block_Manager.h"

using namespace std;

class Setsquare : public Base
{
public:
	Setsquare();
	~Setsquare();

	void Initialize(void);
	int Update(void);
	void Draw(void);
	void Finalize(void);

protected:
	int Setsquare_Cnt;				//もともと持っていた三角定規の数

	int Buy_Setsquare_Cnt;			//三角定規を買った個数

	int Total_Setsquare_Money;			//買った時の三角定規の合計の値段

	string Setsquare_Money_Text1 = "";
	string Setsquare_Money_Text2 = "円";
	string Setsquare_Money_Text3 = "";			//三角定規の値段テキスト

	string Yet_Setsquare_Count_Text1 = "×";
	string Yet_Setsquare_Count_Text2 = "";
	string Yet_Setsquare_Count_Text3 = "";		//左側の三角定規の個数表示テキスト

	string Receipt_Setsquare_Text1 = "× ";
	string Receipt_Setsquare_Text2 = "";
	string Receipt_Setsquare_Text3 = "";
	string Receipt_Setsquare_Text4 = " 円";
	string Receipt_Setsquare_Text5 = "";		//レシートの三角定規のテキスト

};