#pragma once
#include "../../Base.h"
#include "1.Ruler/1.Ruler.h"
#include "2.SetSquare/2.SetSquare.h"
#include "3.Compass/3.Compass.h"
#include "4.Pencil/4.Pencil.h"
#include "5.Eraser/5.Eraser.h"

#include "../../99.Block_Manager/Block_Id.h"
#include "../../99.Block_Manager/Block_Manager.h"

class Inventory_Block : public Base
{
public:
	Inventory_Block();
	~Inventory_Block();

	void Initialize(void);	//初期化
	void Update(void);		//更新
	void Draw(void);		//描画
	void Finalize(void);	//解放

	//Character_Mainから得るブロックを動かせるかどうかを判断するFlgを取得する関数
	void Get_Move_Flg_1(bool Flg, int i) { Move_Flg[0][i] = Flg; }
	void Get_Move_Flg_2(bool Flg, int i) { Move_Flg[1][i] = Flg; }

	//マウスとボックス1の当たり判定
	bool CheckHit_Box_1(const vivid::Vector2& pos);
	//マウスとボックス2の当たり判定
	bool CheckHit_Box_2(const vivid::Vector2& pos);
	//マウスとボックス3の当たり判定
	bool CheckHit_Box_3(const vivid::Vector2& pos);
	//マウスとボックス4の当たり判定
	bool CheckHit_Box_4(const vivid::Vector2& pos);

	bool Cancel_Box(const vivid::Vector2& pos);
	bool Make_Tab(const vivid::Vector2& pos);


	float CheckHit_Star(int i);
private:
	Ruler_Block		block_1;
	SetSquare_Block block_2;
	Compass_Block	block_3;
	Pencil_Block    block_4;
	Eraser_Block	block_5;

	int Block_Cnt[(int)BLOCK_ID::MAX];
	int Put_Block_Cnt[(int)BLOCK_ID::MAX];

	//ブロックボックスの位置
	vivid::Vector2 Block_Box_pos1;
	vivid::Vector2 Block_Box_pos2;
	vivid::Vector2 Block_Box_pos3;
	vivid::Vector2 Block_Box_pos4;

	vivid::Vector2 Cancel_Box_Pos;

	vivid::Vector2 Star_Center_Pos[10];

	float Star_Length[10];

	//Trueになるとブロックを動かせなくなる
	bool Move_Flg[4][30];

	//ブロックのUpdateなどを行うかどうかの判定Flg
	bool flg[4][30];

	bool Tab_Flg;

	//コンパスを選択しているかどうかのFlg
	bool Compass_Flg;

	bool Pencil_Flg;

	bool Eraser_Flg;

	//アイテム欄で鉛筆を表示するかコンパスを表示するかの判定Flg
	int Block_Check;

	//アイテムの所持数の表示のためのText
	std::string Text1;

	std::string Ruler_Cnt_Text1;
	std::string Ruler_Cnt_Text2;

	std::string Setsquare_Cnt_Text1;
	std::string Setsquare_Cnt_Text2;

	std::string Compass_Cnt_Text1;
	std::string Compass_Cnt_Text2;

	std::string Pencil_Cnt_Text1;
	std::string Pencil_Cnt_Text2;

};