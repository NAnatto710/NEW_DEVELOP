#pragma once

#include"../../../../Base.h"

class Spike : public Base
{
public:
	Spike();
	~Spike();

	void Initialize(void);
	void Update(void);
	void Draw(void);
	void Finalize(void);

	vivid::Vector2 CheckHit_Block(vivid::Vector2 block_pos, float spike_radius, int num1, int num2);
private:

	vivid::Vector2 S_Center[20];
};