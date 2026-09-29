#pragma once
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

//プレイヤーの動きに関するクラス
class Player : public Base
{
public:
	Player();
	~Player();

	void Initialize(void);	//初期化
	void Update(void);		//更新
	void Draw(void);		//描画
	void Finalize(void);	//解放

	//呼び出されるとプレーヤーがジャンプする
	void Jump(void) { P_Jp = 180; }

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

	//プレイヤーのジャンプ制御にための変数
	int P_Jp;
};