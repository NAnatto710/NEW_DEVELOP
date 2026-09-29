#pragma once
#include "vivid.h"
#include "../Game.h"
#include "../Base.h"

class Game_clear : public Base
{
public:
	Game_clear();
	~Game_clear();

	void Initialize(void);	//‰Šú‰»
	void Update(void);		//XV
	void Draw(void);		//•`‰æ
	void Finalize(void);	//‰ğ•ú
private:
	Game game;

	vivid::Vector2 pos;
	vivid::Vector2 box_pos2;
	vivid::Vector2 box_pos3;

	vivid::Vector2 p_pos;
};