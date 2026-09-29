#pragma once
#include "../../../Base.h"

//鉛筆
class Pencil_Block : public Base
{
public:
	Pencil_Block();
	~Pencil_Block();

	void Initialize(void);	//初期化
	void Update(void);		//更新
	void Draw(int i);		//描画
	void Finalize(void);	//解放
private:
	bool    ActiveFlag[30];					//有効、無効フラグ
};