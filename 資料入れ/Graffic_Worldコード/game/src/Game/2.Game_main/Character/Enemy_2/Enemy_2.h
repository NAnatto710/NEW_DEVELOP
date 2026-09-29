#pragma once
#include "../../../Base.h"

#include "../../Graffic/Stage_Object/Object_1/Object_1.h"
#include "../../Graffic/Stage_Object/Object_2/Object_2.h"
#include "../../Graffic/Stage_Object/Object_3/Object_3.h"
#include "../../Graffic/Stage_Object/Object_4/Object_4.h"
#include "../../Graffic/Stage_Object/Object_5/Object_5.h"
#include "../../Graffic/Stage_Object/Object_6/Object_6.h"
#include "../../Graffic/Stage_Object/Object_8/Object_8.h"
#include "../../Graffic/Stage_Object/Object_9/Object_9.h"
#include "../../Graffic/Stage_Object/Object_10/Object_10.h"

class Enemy_2 : public Base
{
public:
	Enemy_2();
	~Enemy_2();

	void Initialize(void);	//èâä˙âª
	void Update(int i);		//çXêV
	void Draw(int i);		//ï`âÊ
	void Finalize(void);	//âï˙

private:
	Object_1 object_1;
	Object_2 object_2;
	Object_3 object_3;
	Object_4 object_4;
	Object_5 object_5;
	Object_6 object_6;
	Object_8 object_8;
	Object_9 object_9;
	Object_10 object_10;

	float angle;
};