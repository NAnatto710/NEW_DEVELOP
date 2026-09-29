#include "Time.h"

Time::Time()
	: time(0)
	, time_count(0)
	, check_time(false)
	, text_1(std::string(""))
	, text_2(std::string(""))
	, text_3(std::string(""))
{
}

Time::~Time()
{
}

void Time::Initialize(void)
{
	//初期の時間設定
	time = 300;

	time_count = 0;
	check_time = false;

	//画面上に残りタイムを表示するためのText
	text_1 = "Time:";
	text_2 = "";
	text_3 = "";
}

void Time::Update(void)
{
	//１秒にTimeを１カウント減らす
	time_count += 1;
	if (time > 0)
	{
		if (time_count % 60 == 0)
		{
			time -= 1;
		}
	}
	text_2 = std::to_string(time);
	text_3 = text_1 + text_2;

	//タイムが０になったらタイムオーバー
	Check_time();
	if (check_time == true)
	{
		//タイムオーバーへ
	}
}

void Time::Draw(void)
{
	vivid::DrawText(40, text_3, vivid::Vector2(0.0f,0.0f), 0xff0000ff);
}

void Time::Finalize(void)
{
}

void Time::Check_time(void)
{
	if (time > 0)
	{
		check_time = false;
	}
	else
	{
		check_time = true;
	}
}