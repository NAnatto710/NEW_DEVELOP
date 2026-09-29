#include "Enemy_3.h"

vivid::Vector2 Base::Enemy_3_Position[];

Enemy_3::Enemy_3()
{
}

Enemy_3::~Enemy_3()
{
}

void Enemy_3::Initialize(void)
{
    if (Change_Flg == false)
    {
        Enemy_3_Position[0] = { 1700.0f,630.0f };
        Enemy_3_Position[1] = { 1900.0f,630.0f };
    }
    else
    {
        Enemy_3_Position[0] = { 1900.0f,630.0f };
        Enemy_3_Position[1] = { -1900.0f,630.0f };
    }

    for (int i = 0; i < E_3_Cnt; i++)
    {
        movingLR[i] = true;
    }
}

void Enemy_3::Update(int i)
{
    for (int j = 0; j < Object_1_Cnt; j++)
    {
        Enemy_3_Position[i] = object_1.CheckHit_Object(Enemy_3_Position[i], Enemy_3_Radius, j);
    }
    for (int j = 0; j < 39; j++)
    {
        bool A = Enemy_3_Position[i].x == Object_2_Pos[j].x - Enemy_3_Width;
        bool B = Enemy_3_Position[i].x == Object_2_Pos[j].x + Object_2_Width; 

        Enemy_3_Position[i] = object_2.CheckHit_Object(Enemy_3_Position[i], Enemy_3_Radius, j);

        if (A || B)
        {
            if (A)
            {
                movingLR[i] = true;
            }
            if (B)
            {
                movingLR[i] = false;
            }
        }
    }
    for (int j = 0; j < Object_3_Cnt; j++)
    {
        Enemy_3_Position[i] = object_3.CheckHit_Object(Enemy_3_Position[i], Enemy_3_Radius, j);
    }
    for (int j = 0; j < Object_4_Cnt; j++)
    {
        Enemy_3_Position[i] = object_4.CheckHit_Object(Enemy_3_Position[i], Enemy_3_Radius, j);
    }
    for (int j = 0; j < Object_5_Cnt; j++)
    {
        Enemy_3_Position[i] = object_5.CheckHit_Object(Enemy_3_Position[i], Enemy_3_Radius, j);
    }
    for (int j = 0; j < Object_8_Cnt; j++)
    {
        Enemy_3_Position[i] = object_8.CheckHit_Object(Enemy_3_Position[i], Enemy_3_Radius, j);
    }
    for (int j = 0; j < Object_9_Cnt; j++)
    {
        Enemy_3_Position[i] = object_9.CheckHit_Object(Enemy_3_Position[i], Enemy_3_Radius, j);
    }
    for (int j = 0; j < Object_10_Cnt; j++)
    {
        Enemy_3_Position[i] = object_10.CheckHit_Object(Enemy_3_Position[i], Enemy_3_Radius, j);
    }


    switch (movingLR[i])
    {
    case true:
        Enemy_3_Position[i].x -= 1.0f;
        break;
    case false:
        Enemy_3_Position[i].x += 1.0f;
        break;
    }
}

void Enemy_3::Draw(int i)
{
    if (movingLR[i] == true)
        vivid::DrawTexture("data\\teki.2 2-1.png", Enemy_3_Position[i]);
    if (movingLR[i] == false)
    {
        vivid::DrawTexture("data\\teki.2 2.png", Enemy_3_Position[i]);
    }

}


void Enemy_3::Finalize(void)
{

}