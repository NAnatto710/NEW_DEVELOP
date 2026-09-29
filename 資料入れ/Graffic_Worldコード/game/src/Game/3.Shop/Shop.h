#pragma once
#include "../Base.h"
#include "../Game.h"

#include "Compass/Compass.h"
#include "Pencil/Pencil.h"
#include "Ruler/Ruler.h"
#include "Setsquare/Setsquare.h"

class Shop : public Base
{
public:
	Shop();
	~Shop();

	void Initialize(void);	//初期化
	void Update(void);		//更新
	void Draw(void);		//描画
	void Finalize(void);	//解放
private:
	Game game;

	Compass compass;
	Pencil pencil;
	Ruler ruler;
	Setsquare setsquare;

	int Total_Buy_Money;		//買った合計金額

	string Possession_Money_Text1 = "所持金";
	string Possession_Money_Text2 = "";
	string Possession_Money_Text3 = "円";					//所持金の表示テキスト

	string Receipt_Total_Buy_Money_Text1 = "計";
	string Receipt_Total_Buy_Money_Text2 = "";
	string Receipt_Total_Buy_Money_Text3 = "円";							//レシートの合計金額の処理
};