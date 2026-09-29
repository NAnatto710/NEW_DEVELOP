#pragma once
#include<ctime>
#include "../../../Base.h"

#include "../../Graffic/Stage_Object/Object_1/Object_1.h"
#include "../../Graffic/Stage_Object/Object_2/Object_2.h"
#include "../../Graffic/Stage_Object/Object_3/Object_3.h"
#include "../../Graffic/Stage_Object/Object_4/Object_4.h"
#include "../../Graffic/Stage_Object/Object_5/Object_5.h"
#include "../../Graffic/Stage_Object/Object_6/Object_6.h"
#include "../../Graffic/Stage_Object/Object_8/Object_8.h"
#include "../../Graffic/Stage_Object/Object_9/Object_9.h"
#include "../../Graffic/Stage_Object/Object_10/Object_10.h"


//敵1の動きについてのクラス
class Enemy_1 : public Base
{
public:
	Enemy_1();
	~Enemy_1();

	void Initialize(void);	//初期化
	void Update(int i);		//更新
	void Draw(int i);		//描画
	void Finalize(void);	//解放

	//呼び出されると敵1がジャンプする
	void Jump(int i) { E_1_Jp[i] = 180; }

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

	int E_1_Jp[5];		//敵1のジャンプの制御のための変数

	//今は敵1は3秒に一回方向転換をするためそのための数を数えるための変数
	int Time[5];		
	int Time_cnt[5];
};