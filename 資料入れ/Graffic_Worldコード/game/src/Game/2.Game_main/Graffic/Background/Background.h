#pragma once
#include "../../../Base.h"
#include "../Stage_Object/Object_1/Object_1.h"
#include "../Stage_Object/Object_2/Object_2.h"
#include "../Stage_Object/Object_3/Object_3.h"
#include "../Stage_Object/Object_4/Object_4.h"
#include "../Stage_Object/Object_5/Object_5.h"
#include "../Stage_Object/Object_6/Object_6.h"
#include "../Stage_Object/Object_8/Object_8.h"
#include "../Stage_Object/Object_9/Object_9.h"
#include "../Stage_Object/Object_10/Object_10.h"
#include "../Stage_Object/Star_Object/Star_Object.h"
#include "../Stage_Object/Start_Zone/Start_Zone.h"
#include "../Stage_Object/Safe_Zone/Safe_Zone.h"
#include "../Stage_Object/Spike/Spike.h"
#include "../Stage_Object/Goal_Zone/Goal_Zone.h"


class Background : public Base
{
public:
	//コンストラクタ
	Background();
	//デストラクタ
	~Background();

	void Initialize(void);//初期化
	void Update(void);//更新
	void Draw(void);//描画
	void Finalize(void);//解放

	void Goal_Check(void);
	bool Get_Game_Clear_Flg(void) { return Game_Clear_Flg; }

	bool CheckHit_Start(void);
	bool CheckHit_Safe(int i);
private:
	Object_1 object_1;
	Object_2 object_2;
	Object_3 object_3;
	Object_4 object_4;
	Object_5 object_5;
	Object_6 object_6;
	Object_8 object_8;
	Object_9 object_9;
	Object_10 object_10;

	Star_Object star;
	Spike spike;
	Start_Zone start;
	Safe_Zone safe;
	Goal_Zone goal;

	//Trueになるとゲームクリアになる
	bool Game_Clear_Flg;
};