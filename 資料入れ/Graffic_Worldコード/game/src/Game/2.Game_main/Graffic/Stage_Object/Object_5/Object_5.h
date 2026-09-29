#pragma once
#include "../../../../Base.h"

class Object_5 : public Base
{
public:
	Object_5();
	~Object_5();

	void Initialize(void);	//初期化
	void Update(void);		//更新
	void Draw(void);		//描画
	void Finalize(void);	//解放

	float Get_Object_5_Pos_Y(int i) { return Object_5_Pos[i].y; };

	//四角形とオブジェクトとで使用できる当たり判定用の関数
	vivid::Vector2 CheckHit_Block(vivid::Vector2 block_pos, int block_width, int block_height, int num);
	//円とオブジェクトとで使用できる当たり判定用の関数
	vivid::Vector2 CheckHit_Object(vivid::Vector2 player_pos, float player_radius, int num);
private:
	//当たり判定の判定用の座標
	vivid::Vector2 O_Center[10];
};
