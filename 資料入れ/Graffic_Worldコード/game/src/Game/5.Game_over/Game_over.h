#pragma once
#include "vivid.h"
#include "../Game.h"

class Game_over
{
public:
	Game_over();
	~Game_over();

	void Initialize(void);	//‰Šú‰»
	void Update(void);		//XV
	void Draw(void);		//•`‰æ
	void Finalize(void);	//‰ğ•ú
private:
	Game game;

	vivid::Vector2 pos;
	vivid::Vector2 box_pos2;

	vivid::Vector2 p_pos;
};