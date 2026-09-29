#pragma once
#include "vivid.h"
#include "../../../Base.h"


class Coin : public Base
{
public:
	Coin();
	~Coin();

	void Initialize(void);	//‰Šú‰»
	void Update(void);		//XV
	void Draw(void);		//•`‰æ
	void Finalize(void);	//‰ğ•ú

	//‰~‚Æ‰~‚Ì”»’è‚ğ“¾‚é“–‚½‚è”»’è—p‚ÌŠÖ”
	//‚±‚±‚Å‚ÍƒvƒŒƒCƒ„[‚ÆƒRƒCƒ“‚Ì“–‚½‚è”»’è‚ğ‚µ‚Ä‚¢‚é
	int Get_Coin_length(vivid::Vector2 pos, vivid::Vector2 pos2, const float radius, const float radius2);
private:
	bool Coin_1_Flg[30];		//CoinQ1‚Ì•`‰æİ’è(‚ ‚½‚é‚ÆƒRƒCƒ“‚ªÁ‚¦‚é‚æ‚¤‚É‚·‚éj
	float C_1_Length[30];	//CoinQ1‚Ì“–‚½‚è”»’è

	bool Coin_5_Flg[30];		//CoinQ5‚Ì•`‰æİ’è
	float C_5_Length[30];	//CoinQ5‚Ì“–‚½‚è”»’è

	bool Coin_10_Flg[30];	//CoinQ10‚Ì•`‰æİ’è
	float C_10_Length[30];	//CoinQ10‚Ì“–‚½‚è”»’è

	bool Coin_50_Flg[30];	//CoinQ50‚Ì•`‰æİ’è
	float C_50_Length[30];	//CoinQ50‚Ì“–‚½‚è”»’è

	std::string text_1;		//•¶š‚Ì•\¦‚Ì‚½‚ß‚Ì•Ï”
	std::string text_2;
	std::string text_3;
};