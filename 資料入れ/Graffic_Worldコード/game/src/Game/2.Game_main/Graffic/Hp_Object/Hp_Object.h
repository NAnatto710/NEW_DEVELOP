#pragma once
#include "vivid.h"
#include "../../../Base.h"

class Hp_Object : public Base
{
public:
	Hp_Object();
	~Hp_Object();

	void Initialize(void);	//‰Šú‰»
	void Update(void);		//XV
	void Draw(void);		//•`‰æ
	void Finalize(void);	//‰ğ•ú
private:
	vivid::Vector2 pos_HP_1;//HP‚ÌÀ•W
	vivid::Vector2 pos_HP_2;
	vivid::Vector2 pos_HP_3;
};