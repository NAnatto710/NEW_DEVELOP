#pragma once
#include "vivid.h"

class Base
{
public:
	static bool Change_Flg;

	static bool Scene_Flg;				//シーン切り替えでゲームメインを複数回Initializeしないための変数

	static bool Jamp_Flg;				//プレイヤーがジャンプできるかどうかの判定Flg

	static bool Block_1_Check[30];
	static bool Block_2_Check[30];

	static int HP_level;				//HPの残りの数

	static int Coin_All;				//所持しているコイン

	static int Scroll_Cnt;

	static float Ground_Line;		//地面のライン

	static vivid::Vector2 Player_Pos;		//プレイヤーの位置

	static vivid::Vector2 Object_1_Pos[10];	//オブジェクト1の位置
	static vivid::Vector2 Object_2_Pos[80];	//オブジェクト2の位置
	static vivid::Vector2 Object_3_Pos[10];	//オブジェクト3の位置
	static vivid::Vector2 Object_4_Pos[10]; //オブジェクト4の位置
	static vivid::Vector2 Object_5_Pos[10]; //オブジェクト5の位置
	static vivid::Vector2 Object_6_Pos[15]; //オブジェクト6の位置
	static vivid::Vector2 Object_8_Pos[10]; //オブジェクト8の位置
	static vivid::Vector2 Object_9_Pos[10]; //オブジェクト9の位置
	static vivid::Vector2 Object_10_Pos[10];//オブジェクト10の位置

	static vivid::Vector2 Spike_Pos[19];		//画鋲の位置

	static vivid::Vector2 Start_Zone_Pos;	//スタートゾーンの位置
	static vivid::Vector2 Safe_Zone_Pos[5];	//セーフゾーンの位置
	static vivid::Vector2 Goal_Zone_Pos;	//ゴールの位置

	static vivid::Vector2 Star_Pos[1];		//星の位置

	static vivid::Vector2 Ruler_Pos[30];        //定規の位置
	static vivid::Vector2 SetSquare_Pos[30];    //三角定規の位置
	static vivid::Vector2 Compass_Pos[30];		//コンパスの位置

	static vivid::Vector2 Pos_Background_Under_Left;	//背景の座標
	static vivid::Vector2 Pos_Background_Under_Right;
	static vivid::Vector2 Pos_Background_On_Left;
	static vivid::Vector2 Pos_Background_On_Right;

	static vivid::Vector2 Coin_1_Position[5];	//コイン1の位置
	static vivid::Vector2 Coin_5_Position[5];	//コイン5の位置
	static vivid::Vector2 Coin_10_Position[5];	//コイン10の位置
	static vivid::Vector2 Coin_50_Position[5];	//コイン50の位置

	static vivid::Vector2 Enemy_1_Position[5]; //敵1の位置
	static vivid::Vector2 Enemy_2_Position[5]; //敵2の位置
	static vivid::Vector2 Enemy_3_Position[5]; //敵3の位置

protected:
	static const float Gravity;			//重力

	static const float Player_Radius;	//プレイヤーの半径
	static const float Enemy_1_Radius;	//敵1の半径
	static const float Enemy_2_Radius;	//敵2の半径
	static const float Enemy_3_Radius;	//敵3の半径
	static const float Coin_Radius;		//コインの半径
	static const float Star_Radius;		//星の半径
	static const float Spike_Radius;	//画鋲の半径

	static const float Player_Width;	//プレイヤーの幅
	static const float Player_Height;	//プレイヤーの高さ
				
	static const float Enemy_1_Width;	//敵1の幅
	static const float Enemy_1_Height;	//敵1の高さ
				 
	static const float Enemy_2_Width;	//敵2の幅
	static const float Enemy_2_Height;	//敵2の高さ
				 
	static const float Enemy_3_Width;	//敵3の幅
	static const float Enemy_3_Height;	//敵3の高さ
				 
	static const float Coin_Width;		//コインの幅
	static const float Coin_Height;		//コインの高さ

	static const float Spike_Width;		//画鋲の幅
	static const float Spike_Height;	//画鋲の高さ
				 
	static const float Object_1_Width;	//Object_1の幅
	static const float Object_1_Height;	//Object_1の高さ

	static const float Object_2_Width;	//Object_2の高さ
	static const float Object_2_Height;	//Object_2の高さ

	static const float Object_3_Width;	//Object_3の高さ
	static const float Object_3_Height;	//Object_3の高さ

	static const float Object_4_Width;	//Object_4の高さ
	static const float Object_4_Height;	//Object_4の高さ

	static const float Object_5_Width;	//Object_5の高さ
	static const float Object_5_Height;	//Object_5の高さ

	static const float Object_6_Width;	//Object_6の高さ
	static const float Object_6_Height;	//Object_6の高さ

	static const float Object_8_Width;	//Object_8の高さ
	static const float Object_8_Height;	//Object_8の高さ

	static const float Object_9_Width;	//Object_9の高さ
	static const float Object_9_Height;	//Object_9の高さ

	static const float Object_10_Width;	//Object_10の高さ
	static const float Object_10_Height;//Object_10の高さ

	static const float Star_Width;		//Starの幅
	static const float Star_Height;		//Starの高さ
				 
	static const float Block_Box_Width;	//ブロックブロックの幅
	static const float Block_Box_Height;//ブロックブロックの高さ
				 
	static const float Ruler_Width;		//定規の幅
	static const float Ruler_Height;	//定規の高さ
				 
	static const float SetSquare_Width;	//三角定規の幅
	static const float SetSquare_Height;//三角定規の高さ
				
	static const float Compass_Width;	//コンパスブロックの幅
	static const float Compass_Height;	//コンパスブロックの高さ
				 
	static const float Window_Width;	//ウィンドウの縦幅
	static const float Window_Height;	//ウィンドウの横幅

	static const float Safe_Zone_Width;	//セーフゾーンの幅
	static const float Safe_Zone_Height;//セーフゾーンの高さ

	static const float Start_Zone_Width;	//スタートゾーンの幅
	static const float Start_Zone_Height;	//スタートゾーンの高さ

	static const float Goal_Zone_Width;	//ゴールの幅
	static const float Goal_Zone_Height;//ゴールの高さ

	static const int Coin_1_cnt;		//Coin＿1の個数
	static const int Coin_5_cnt;		//Coin＿5の個数
	static const int Coin_10_cnt;		//Coin＿10の個数
	static const int Coin_50_cnt;		//Coin＿50の個数

	static const int Object_1_Cnt;		//Object_1の個数
	static const int Object_2_Cnt;		//Object_2の個数
	static const int Object_3_Cnt;		//Object_3の個数
	static const int Object_4_Cnt;		//Object_4の個数
	static const int Object_5_Cnt;		//Object_5の個数
	static const int Object_6_Cnt;		//Object_6の個数
	static const int Object_8_Cnt;		//Object_8の個数
	static const int Object_9_Cnt;		//Object_9の個数
	static const int Object_10_Cnt;		//Object_10の個数
		  
	static const int Star_Cnt;			//星のオブジェクトの個数
	
	static const int Spike_Cnt;			//画鋲の個数
	
	static const int E_1_Cnt;			//Enemy_1の個数
	static const int E_2_Cnt;			//Enemy_2の個数
	static const int E_3_Cnt;			//Enemy_3の個数

	static const int Safe_Zone_Cnt;
	
	static const int Compass_Price;		//コンパスの値段
	static const int Ruler_Price;		//ものさしの値段
	static const int Pencil_Price;		//鉛筆の値段
	static const int Setsquare_Price;	//三角定規の値段
};