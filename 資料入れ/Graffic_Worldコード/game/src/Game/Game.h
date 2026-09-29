#pragma once
#include "vivid.h"
#include "Base.h"

enum class SCENE_ID
{
	TITLE,
	GAME_MAIN,
	SHOP,
	GAME_CLEAR,
	GAME_OVER,
};

class Game : public Base
{
public:
	Game();
	~Game();

	void Initialize(void);	//‰Šú‰»
	void Update(void);		//XV
	void Draw(void);		//•`‰æ
	void Finalize(void);	//‰ğ•ú

	void ChangeScene(SCENE_ID next_scene);
private:
	vivid::Vector2 POS;
};

