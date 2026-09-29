#pragma once
#include "../../../Base.h"
#include "../../Graffic/Stage_Object/Object_1/Object_1.h"
#include "../../Graffic/Stage_Object/Object_2/Object_2.h"
#include "../../Graffic/Stage_Object/Object_3/Object_3.h"
#include "../../Graffic/Stage_Object/Object_4/Object_4.h"
#include "../../Graffic/Stage_Object/Object_5/Object_5.h"
#include "../../Graffic/Stage_Object/Object_6/Object_6.h"
#include "../../Graffic/Stage_Object/Spike/Spike.h"
#include "../../Graffic/Stage_Object/Object_8/Object_8.h"
#include "../../Graffic/Stage_Object/Object_9/Object_9.h"
#include "../../Graffic/Stage_Object/Object_10/Object_10.h"
//ものさし
class Ruler_Block : public Base
{
public:
	Ruler_Block();
	~Ruler_Block();

	void Initialize(void);	//初期化
	void Update(int i);		//更新
	void Draw(int i);		//描画
	void Finalize(void);	//解放

	//マウスとブロックの当たり判定
	bool CheckHit_Block(const vivid::Vector2& pos, int i);

	//マウスに当たったらどうするか
	void Hit(int i);

	//四角と四角の当たり判定処理
	vivid::Vector2 CheckHit_Ruler(int i,vivid::Vector2 block_pos2, int block_width, int block_height);

	//四角と三角の当たり判定処理
	vivid::Vector2 CheckHit_SetSquare(int i, vivid::Vector2 block_pos, int block_width, int block_height);
private:
	Object_1 object_1;
	Object_2 object_2;
	Object_3 object_3;
	Object_4 object_4;
	Object_5 object_5;
	Object_6 object_6;
	Spike object_7;
	Object_8 object_8;
	Object_9 object_9;
	Object_10 object_10;
	Spike spike;

	bool    ActiveFlag[30];					//有効、無効フラグ
};