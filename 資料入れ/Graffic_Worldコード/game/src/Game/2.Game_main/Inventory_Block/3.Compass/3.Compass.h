#pragma once
#include "../../../Base.h"

//コンパス
class Compass_Block : public Base
{
public:
	Compass_Block();
	~Compass_Block();

	void Initialize(void);	//初期化
	void Update(void);		//更新
	void Draw(int i);		//描画
	void Finalize(void);	//解放

	//マウスとブロックの当たり判定
	bool CheckHit_Block(const vivid::Vector2& pos, int i);

	//星とマウスが当たったらどうするか
	void Hit(int i, vivid::Vector2 pos);
private:
	bool    ActiveFlag[30];					//有効、無効フラグ
};