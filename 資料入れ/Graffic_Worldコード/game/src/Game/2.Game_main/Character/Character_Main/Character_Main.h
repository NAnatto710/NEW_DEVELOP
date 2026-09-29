#pragma once
#include "../../../Base.h"
#include "../Player/Player.h"
#include "../Enemy_1/Enemy_1.h"
#include "../Enemy_2/Enemy_2.h"
#include "../Enemy_3/Enemy_3.h"

#include "../../Inventory_Block/Inventory_Block.h"
#include "../../../99.Block_Manager/Block_Manager.h"

//キャラクタークラスの要素を呼び出している
//処理の関係でほかの場所から呼び出しているものもある
class Character_Main : public Base
{
public:
	Character_Main();
	~Character_Main();

	void Initialize(void);	//初期化
	void Update(void);		//更新
	void Draw(void);		//描画
	void Finalize(void);	//解放


	void Game_Over_Check(void);
	//HPがすべてなくなるとTRUEになりゲームオーバーになる
	bool Get_Game_Over_Flg(void) { return Game_Over_Flg; }

	//円と円のもので使用できる当たり判定用の関数
	//現在はプレイヤーと敵の当たり判定で使用している
	int Get_Character_length(vivid::Vector2 pos,vivid::Vector2 pos2, const float radius ,const float radius2);

	//円と四角形のもので使用できる当たり判定用の関数
	//現在はプレイヤーと定規、敵と定規との当たり判定で使用している
	vivid::Vector2 Check_Hit_Block
	(vivid::Vector2 pos, vivid::Vector2 block_pos, float radius, int block_width, int block_height);

	//円と三角形のもので使用できる当たり判定用の関数
	//現在はプレイヤーと三角定規、敵と三角定規との当たり判定で使用している
	vivid::Vector2 Check_Hit_SetSquare
	(vivid::Vector2 pos, float radius, int i);
private:
	Player player;

	Enemy_1 enemy_1;
	Enemy_2 enemy_2;
	Enemy_3 enemy_3;

	//ブロックとキャラクターの当たり判定がうまくいかないためここで呼び出して座標を取得している
	Inventory_Block inventory;

	int Put_Block_Cnt[(int)BLOCK_ID::MAX];

	float Length[3];

	//TRUEになるとゲームオーバーになる
	bool Game_Over_Flg;

	//プレイヤーがブロックに乗っている間はブロックが動かないようにする
	bool Move_Flg;
	bool Move_Flg2;
};