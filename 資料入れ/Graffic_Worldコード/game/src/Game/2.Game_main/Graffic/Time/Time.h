#pragma once
#include "vivid.h"

class Time
{
public:
	Time();
	~Time();

	void Initialize(void);	//初期化
	void Update(void);		//更新
	void Draw(void);		//描画
	void Finalize(void);	//解放

	//タイムが０になったらTrueになるタイムオーバーのための関数
	void Check_time(void);

	bool Get_Time_Flg(void)
	{
		return check_time;
	};
private:
	//時間を数えるための変数
	int time;
	int time_count;

	//タイムが０になったらTrueになるタイムオーバーのための変数
	bool check_time;

	//タイムを表示するためのText
	std::string text_1;
	std::string text_2;
	std::string text_3;
};