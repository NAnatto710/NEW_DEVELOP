#include "Inventory_Block.h"

bool Base::Block_1_Check[30];
bool Base::Block_2_Check[30];

Inventory_Block::Inventory_Block()
{
}

Inventory_Block::~Inventory_Block()
{
}

void Inventory_Block::Initialize(void)
{
	//ボックスの初期位置
	Block_Box_pos1 = { 0.0f,100.0f };
	Block_Box_pos2 = { 0.0f,250.0f };
	Block_Box_pos3 = { 0.0f,400.0f };
	Block_Box_pos4 = { 0.0f,550.0f };

	Cancel_Box_Pos = { 200.0f,50.0f };

	for (int i = 0; i < Star_Cnt; i++)
	{
		Star_Center_Pos[i] = { Star_Pos[i].x + Star_Radius, Star_Pos[i].y + Star_Radius };
	}

	Tab_Flg = false;

	//各種初期化
	for (int i = 0; i < 4; i++)
	{
		//ブロックの所持数の初期設定
		Block_Cnt[i] = 0;
		//フィールド上に設置されているブロックの数
		Put_Block_Cnt[i] = 0;

		for (int j = 0; j < 30; j++)
		{
			flg[i][j] = true;
			Move_Flg[i][j] = false;
			Block_1_Check[j] = true;
			Block_2_Check[j] = true;
		}
	}
	Compass_Flg = false;
	Pencil_Flg = false;
	Eraser_Flg = false;

	//Block_Managerにブロックの所持数を渡している
	Block_Manager::GetInstance().Set_Block_Cnt(BLOCK_ID::RULER, Block_Cnt[0], Put_Block_Cnt[0]);
	Block_Manager::GetInstance().Set_Block_Cnt(BLOCK_ID::SETSQUARE, Block_Cnt[1], Put_Block_Cnt[1]);
	Block_Manager::GetInstance().Set_Block_Cnt(BLOCK_ID::COMPASS, Block_Cnt[2], Put_Block_Cnt[2]);
	Block_Manager::GetInstance().Set_Block_Cnt(BLOCK_ID::PENCIL, Block_Cnt[3], Put_Block_Cnt[3]);

	block_1.Initialize();
	block_2.Initialize();
	block_3.Initialize();
	block_4.Initialize();

	Block_Check = 0;

	Text1 = "×";

	Ruler_Cnt_Text1 = "";
	Ruler_Cnt_Text2 = "";

	Setsquare_Cnt_Text1 = "";
	Setsquare_Cnt_Text2 = "";

	Compass_Cnt_Text1 = "";
	Compass_Cnt_Text2 = "";

	Pencil_Cnt_Text1 = "";
	Pencil_Cnt_Text2 = "";
}

void Inventory_Block::Update(void)
{
	//マウスカーソルの位置の取得
	vivid::Point mpos = vivid::mouse::GetCursorPos();

	for (int i = 0; i < Star_Cnt; i++)
	{
		Star_Center_Pos[i] = { Star_Pos[i].x + Star_Radius, Star_Pos[i].y + Star_Radius };
	}

	//ブロックが動かされている間はUpdateをしない
	for (int i = 0; i < Put_Block_Cnt[1]; i++)
		if (flg[1][i])
		{
			block_2.Update(i);
		}
	//ブロックが動かされている間はUpdateをしない
	for (int i = 0; i < Put_Block_Cnt[0]; i++)
		if (flg[0][i])
		{
			block_1.Update(i);
		}

	//ブロックを動かしている最中以外は常にtrueにしておく
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			flg[i][j] = true;
			Block_1_Check[j] = true;
			Block_2_Check[j] = true;
		}
	}

	////Kが押されるとアイテム欄の表示が切り替わる
	//if (vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::K))
	//	Block_Check += 1;


	if (vivid::mouse::Trigger(vivid::mouse::BUTTON_ID::LEFT))
	{
		if (Cancel_Box(vivid::Vector2((float)mpos.x, (float)mpos.y)))
		{
			Tab_Flg = false;
		}
	}

	if (Tab_Flg)
	{

		//ブロックの数をBlock_Managerから取得する
		Block_Cnt[0] = Block_Manager::GetInstance().Get_Block_Cnt(BLOCK_ID::RULER);
		Block_Cnt[1] = Block_Manager::GetInstance().Get_Block_Cnt(BLOCK_ID::SETSQUARE);
		Block_Cnt[2] = Block_Manager::GetInstance().Get_Block_Cnt(BLOCK_ID::COMPASS);
		Block_Cnt[3] = Block_Manager::GetInstance().Get_Block_Cnt(BLOCK_ID::PENCIL);

		//Textの更新
		Ruler_Cnt_Text1 = std::to_string(Block_Cnt[(int)BLOCK_ID::RULER]);
		Ruler_Cnt_Text2 = Text1 + Ruler_Cnt_Text1;

		Setsquare_Cnt_Text1 = std::to_string(Block_Cnt[(int)BLOCK_ID::SETSQUARE]);
		Setsquare_Cnt_Text2 = Text1 + Setsquare_Cnt_Text1;

		Compass_Cnt_Text1 = std::to_string(Block_Cnt[(int)BLOCK_ID::COMPASS]);
		Compass_Cnt_Text2 = Text1 + Compass_Cnt_Text1;

		Pencil_Cnt_Text1 = std::to_string(Block_Cnt[(int)BLOCK_ID::PENCIL]);
		Pencil_Cnt_Text2 = Text1 + Pencil_Cnt_Text1;

		//左クリックを押すと処理が進む
		if (vivid::mouse::Trigger(vivid::mouse::BUTTON_ID::LEFT))
		{
			//マウスポインタがBox1の中にあるかの判定
			if (CheckHit_Box_1(vivid::Vector2((float)mpos.x, (float)mpos.y)))
			{
				Compass_Flg = false;
				Pencil_Flg = false;
				Eraser_Flg = false;
				//Block_Managerで定規が1つ以上あるかの判定
				if (Block_Manager::GetInstance().Compare_Cnt(BLOCK_ID::RULER))
				{
					//置いた定規の数と所持している定規の数を変動させる
					Put_Block_Cnt[0] = Block_Manager::GetInstance().Change_Cnt(BLOCK_ID::RULER);

					

					//定規をマウスポインタの位置に出現させる
					block_1.Hit(Put_Block_Cnt[0] - 1);
				}
			}
			//マウスポインタがBox2の中にあるかの判定
			if (CheckHit_Box_2(vivid::Vector2((float)mpos.x, (float)mpos.y)))
			{
				Compass_Flg = false;
				Pencil_Flg = false;
				Eraser_Flg = false;
				//Block_Managerで三角定規が1つ以上あるかの判定
				if (Block_Manager::GetInstance().Compare_Cnt(BLOCK_ID::SETSQUARE))
				{
					//置いた三角定規の数と所持している三角定規の数を変動させる
					Put_Block_Cnt[1] = Block_Manager::GetInstance().Change_Cnt(BLOCK_ID::SETSQUARE);



					//三角定規をマウスポインタの位置に出現させる
					block_2.Hit(Put_Block_Cnt[1] - 1);
				}
			}
			//0だとコンパス
			if (Block_Check % 2 == 0)
			{
				//マウスポインタがBox3の中にあるかの判定
				if (CheckHit_Box_3(vivid::Vector2((float)mpos.x, (float)mpos.y)))
				{
					//Block_Managerでコンパスが1つ以上あるかの判定
					if (Block_Manager::GetInstance().Compare_Cnt(BLOCK_ID::COMPASS))
					{
						Compass_Flg = true;
						Pencil_Flg = false;
						Eraser_Flg = false;
					}
				}
				if (Compass_Flg == true)
				{
					for (int i = 0; i < Star_Cnt; i++)
					{
						Star_Length[i] = CheckHit_Star(i);
						if (Star_Length[i] <= Star_Radius)
						{
							//置いたコンパスブロックの数と所持しているコンパスの数を変動させる
							Put_Block_Cnt[2] = Block_Manager::GetInstance().Change_Cnt(BLOCK_ID::COMPASS);
							//コンパスを星の下に出現させる
							block_3.Hit(Put_Block_Cnt[2] - 1, Star_Pos[i]);
						}
					}
				}
			}

			//マウスポインタがBox4の中にあるかの判定
			if (CheckHit_Box_4(vivid::Vector2((float)mpos.x, (float)mpos.y)))
			{
				Compass_Flg = false;
				Pencil_Flg = false;
				Eraser_Flg = true;
			}
			if (Eraser_Flg == true)
			{
				for (int i = 0; i < Put_Block_Cnt[0]; i++)
				{
					if (block_1.CheckHit_Block(vivid::Vector2((float)mpos.x, (float)mpos.y), i));
					{
						Ruler_Pos[i].x = -10000.0f;
					}
				}
				for (int i = 0; i < Put_Block_Cnt[1]; i++)
				{
					if (block_2.CheckHit_Block(vivid::Vector2((float)mpos.x, (float)mpos.y), i));
					{
						SetSquare_Pos[i].x = -10000.0f;
					}
				}
				for (int i = 0; i < Put_Block_Cnt[2]; i++)
				{
					if (block_3.CheckHit_Block(vivid::Vector2((float)mpos.x, (float)mpos.y), i));
					{
						Compass_Pos[i].x = -10000.0f;
					}
				}
			}

		}
	}


	if(Tab_Flg == false)
	{
		if (vivid::mouse::Trigger(vivid::mouse::BUTTON_ID::LEFT))
		{
			if (Make_Tab(vivid::Vector2((float)mpos.x, (float)mpos.y)))
			{
				Tab_Flg = true;
			}
		}
	}
		//左クリックを押している間だけ処理が進む
		if (!vivid::mouse::Button(vivid::mouse::BUTTON_ID::LEFT))
			return;

		//定規を置いた数だけ処理をする
		for (int i = 0; i < Put_Block_Cnt[0]; i++)
		{
			//マウスポインタが定規の判定の中であり、プレイヤーがブロックの上に乗っていないときのみ処理が進む
			if (block_1.CheckHit_Block(vivid::Vector2((float)mpos.x, (float)mpos.y), i) /*&&
				Move_Flg[0][i] == false*/)
			{
				//定規をマウスポインタの位置に出現させる
				block_1.Hit(i);

				Block_1_Check[i] = false;

				//選択されている定規のUpdateを処理させない
				flg[0][i] = false;
			}
		}
		//三角定規を置いた数だけ処理をする
		for (int i = 0; i < Put_Block_Cnt[1]; i++)
		{
			//マウスポインタが三角定規の判定の中であり、プレイヤーがブロックの上に乗っていないときのみ処理が進む
			if (block_2.CheckHit_Block(vivid::Vector2((float)mpos.x, (float)mpos.y), i) /*&&
				Move_Flg[1][i] == false*/)
			{
				//三角定規をマウスポインタの位置に出現させる
				block_2.Hit(i);

				Block_2_Check[i] = false;

				//選択されている三角定規のUpdateを処理させない
				flg[1][i] = false;
			}
		}
		/*if (Pencil_Flg == true)
		{

		}*/
}

void Inventory_Block::Draw(void)
{
	for (int i = 0; i < Put_Block_Cnt[2]; i++)
		block_3.Draw(i);

	for (int i = 0; i < Put_Block_Cnt[0]; i++)
		block_1.Draw(i);

	for (int i = 0; i < Put_Block_Cnt[1]; i++)
		block_2.Draw(i);

	if (Tab_Flg)
	{

		vivid::DrawTexture("data\\tab背景薄.png", Block_Box_pos1, 0xff080808);
		vivid::DrawTexture("data\\tab背景薄.png", Block_Box_pos2, 0xff080808);
		vivid::DrawTexture("data\\tab背景薄.png", Block_Box_pos3, 0xff080808);
		vivid::DrawTexture("data\\tab背景薄.png", Block_Box_pos4, 0xff080808);

		vivid::DrawTexture("data\\TabRuler.png", vivid::Vector2(10.0f, 140.0f));
		vivid::DrawTexture("data\\TabSet square.png", vivid::Vector2(10.0f, 270.0f));
		vivid::DrawTexture("data\\eraser.png", vivid::Vector2(10.0f, 570.0f));

		if (Block_Check % 2 == 0)
		{
			vivid::DrawTexture("data\\compass.png", vivid::Vector2(10.0f, 400.0f));
			vivid::DrawText(30, Compass_Cnt_Text2, vivid::Vector2(180.0f, 520.0f), 0xff00ff00);
			if (Compass_Flg == true)
				vivid::DrawText(30, "選択中", vivid::Vector2(80.0f, 520.0f));
		}
		if (Block_Check % 2 == 1)
		{
			vivid::DrawTexture("data\\Tabpencil.png", vivid::Vector2(10.0f, 440.0f));
			vivid::DrawText(30, Pencil_Cnt_Text2, vivid::Vector2(180.0f, 520.0f), 0xff00ff00);
			if (Pencil_Flg == true)
				vivid::DrawText(30, "選択中", vivid::Vector2(80.0f, 520.0f));
		}
		if (Eraser_Flg)
		{
			vivid::DrawText(30, "選択中", vivid::Vector2(80.0f, 690.0f));
		}

		vivid::DrawText(30, Ruler_Cnt_Text2, vivid::Vector2(180.0f, 220.0f), 0xff00ff00);
		vivid::DrawText(30, Setsquare_Cnt_Text2, vivid::Vector2(180.0f, 370.0f), 0xff00ff00);

		vivid::DrawTexture("data\\box50.png", Cancel_Box_Pos, 0xffc0c0c0);
		vivid::DrawTexture("data\\batu50.png", vivid::Vector2(Cancel_Box_Pos.x + 5.0f, Cancel_Box_Pos.y + 5.0f), 0xffffffff);

	}
	else
	{
		vivid::DrawTexture("data\\w_box50_100.png", Block_Box_pos1, 0xffc0c0c0);
		vivid::DrawTexture("data\\batu20.png", vivid::Vector2(Block_Box_pos1.x + 10.0f, Block_Box_pos1.y + 30.0f), 0xffffffff);

	}
}

void Inventory_Block::Finalize(void)
{
	block_1.Finalize();

	block_2.Finalize();
}

bool Inventory_Block::CheckHit_Box_1(const vivid::Vector2& pos)
{
	if (pos.x > Block_Box_pos1.x && pos.x < Block_Box_pos1.x + Block_Box_Width
		&& pos.y > Block_Box_pos1.y && pos.y < Block_Box_pos1.y + Block_Box_Height)
	{
		return true;
	}
	return false;
}

bool Inventory_Block::CheckHit_Box_2(const vivid::Vector2& pos)
{
	if (pos.x > Block_Box_pos2.x && pos.x < Block_Box_pos2.x + Block_Box_Width
		&& pos.y > Block_Box_pos2.y && pos.y < Block_Box_pos2.y + Block_Box_Height)
	{
		return true;
	}
	return false;
}

bool Inventory_Block::CheckHit_Box_3(const vivid::Vector2& pos)
{
	if (pos.x > Block_Box_pos3.x && pos.x < Block_Box_pos3.x + Block_Box_Width
		&& pos.y > Block_Box_pos3.y && pos.y < Block_Box_pos3.y + Block_Box_Height)
	{
		return true;
	}
	return false;
}

bool Inventory_Block::CheckHit_Box_4(const vivid::Vector2& pos)
{
	if (pos.x > Block_Box_pos4.x && pos.x < Block_Box_pos4.x + Block_Box_Width
		&& pos.y > Block_Box_pos4.y && pos.y < Block_Box_pos4.y + Block_Box_Height)
	{
		return true;
	}
	return false;
}

bool Inventory_Block::Cancel_Box(const vivid::Vector2& pos)
{
	if (pos.x > Cancel_Box_Pos.x && pos.x < Cancel_Box_Pos.x + 50.0f
		&& pos.y > Cancel_Box_Pos.y && pos.y < Cancel_Box_Pos.y + 50.0f)
	{
		return true;
	}
	return false;
}

bool Inventory_Block::Make_Tab(const vivid::Vector2& pos)
{
	if (pos.x > Block_Box_pos1.x && pos.x < Block_Box_pos1.x + 50.0f
		&& pos.y > Block_Box_pos1.y && pos.y < Block_Box_pos1.y + 100.0f)
	{
		return true;
	}
	return false;
}

float Inventory_Block::CheckHit_Star(int i)
{
	//マウスカーソルの位置の取得
	vivid::Point mpos = vivid::mouse::GetCursorPos();

	float x = mpos.x - Star_Center_Pos[i].x;
	float y = mpos.y - Star_Center_Pos[i].y;
	float length = sqrt(x * x + y * y);
	return length;
}
