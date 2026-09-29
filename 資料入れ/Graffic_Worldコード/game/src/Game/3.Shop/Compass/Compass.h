#pragma once
#include "../../Base.h"
#include "../../99.Block_Manager/Block_Id.h"
#include "../../99.Block_Manager/Block_Manager.h""

using namespace std;

class Compass : public Base
{
public:
	Compass();
	~Compass();

	void Initialize(void);
	int Update(void);
	void Draw(void);
	void Finalize(void);

protected:
	int Compass_Cnt;				//もともと持っていたコンパスの数

	int Buy_Compass_Cnt;			//コンパスを買った個数

	int Total_Compass_Money;		//買った時のコンパスの合計の値段

	string Compass_Money_Text1 = "";
	string Compass_Money_Text2 = "円";
	string Compass_Money_Text3 = "";		//コンパスの値段テキスト

	string Yet_Compass_Count_Text1 = "×";
	string Yet_Compass_Count_Text2 = "";
	string Yet_Compass_Count_Text3 = "";	//左側のコンパスの個数表示テキスト

	string Receipt_Compass_Text1 = "× ";
	string Receipt_Compass_Text2 = "";
	string Receipt_Compass_Text3 = "";
	string Receipt_Compass_Text4 = " 円";
	string Receipt_Compass_Text5 = "";		//レシートのコンパスのテキスト
};