#pragma once
#include "../../../../Base.h"

class Object_6 : public Base
{
public:
	Object_6();
	~Object_6();

	void Initialize(void);	//初期化
	void Update(void);		//更新
	void Draw(void);		//描画
	void Finalize(void);	//解放

	float Get_Object_6_Pos_Y(int i) { return Object_6_Pos[i].y; };

	//四角形とオブジェクトとで使用できる当たり判定用の関数
	vivid::Vector2 CheckHit_Block(vivid::Vector2 block_pos, int block_width, int block_height);
	//円とオブジェクトとで使用できる当たり判定用の関数
	vivid::Vector2 CheckHit_Object(vivid::Vector2 player_pos, float player_radius, int num);
private:
	//当たり判定の判定用の座標
	vivid::Vector2 O_Center[10];
};

