#pragma once
#include "vivid.h"
#include "../Game.h"
#include "../Base.h"

class Title : public Base
{
public:
	Title();
	~Title();

	void Initialize(void);	//‰Šú‰»
	void Update(void);		//XV
	void Draw(void);		//•`‰æ
	void Finalize(void);	//‰ğ•ú

private:
	Game game;

	vivid::Vector2 EraserPos = { -300.0f,245.0f };
	vivid::Vector2 PencilPos = { 290.0f,250.0f };

	int Time_Cnt;
};