#pragma once
#include "../Game.h"
#include "Character/Character_Main/Character_Main.h"
#include "Graffic/Background/Background.h"
#include "Graffic/Coin/Coin.h"
#include "Graffic/Hp_Object/Hp_Object.h"
#include "Graffic/Time/Time.h"

#include "../Base.h"

//ゲームメインの要素のすべてを呼び出しているクラス
class Game_main : public Base
{
public:
	Game_main();
	~Game_main();

	void Initialize(void);	//初期化
	void Update(void);		//更新
	void Draw(void);		//描画
	void Finalize(void);	//解放
private:
	Game game;

	Character_Main character;
	Background background;
	Coin coin;
	Hp_Object Hp;
	Time time;

	bool UI;
	bool check;
	int cnt;
};