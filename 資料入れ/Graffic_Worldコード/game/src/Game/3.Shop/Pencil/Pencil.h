#pragma once
#include "../../Base.h"
#include "../../99.Block_Manager/Block_Id.h"
#include "../../99.Block_Manager/Block_Manager.h""

using namespace std;

class Pencil : public Base
{
public:
	Pencil();
	~Pencil();

	void Initialize(void);
	int Update(void);
	void Draw(void);
	void Finalize(void);

protected:
	int Pencil_Cnt;				//もともと持っていた鉛筆の数

	int Buy_Pencil_Cnt;			//鉛筆を買った個数

	int Total_Pencil_Money;			//買った時の鉛筆の合計の値段

	string Pencil_Money_Text1 = "";
	string Pencil_Money_Text2 = "円";
	string Pencil_Money_Text3 = "";			//鉛筆の値段テキスト

	string Yet_Pencil_Count_Text1 = "×";
	string Yet_Pencil_Count_Text2 = "";
	string Yet_Pencil_Count_Text3 = "";		//左側の鉛筆の個数表示テキスト

	string Receipt_Pencil_Text1 = "× ";
	string Receipt_Pencil_Text2 = "";
	string Receipt_Pencil_Text3 = "";
	string Receipt_Pencil_Text4 = " 円";
	string Receipt_Pencil_Text5 = "";		//レシートの鉛筆のテキスト
};