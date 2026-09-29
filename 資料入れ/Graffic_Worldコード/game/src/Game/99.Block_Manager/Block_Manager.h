#pragma once
#include "vivid.h"
#include "Block_Id.h"

class Block_Manager
{
public:
	static Block_Manager& GetInstance(void);

	//ブロックの所持数を設定する
	void Set_Block_Cnt(BLOCK_ID block_id, int cnt);
	//ブロックの所持数と設置したブロックの数を設定する
	void Set_Block_Cnt(BLOCK_ID block_id,int cnt,int cnt2);

	//ブロックの所持数を返す
	int Get_Block_Cnt(BLOCK_ID block_id);
	//ブロックをおいている数を返す
	int Get_Put_Block_Cnt(BLOCK_ID block_id);

	//ブロックの個数を1つずつ変動させる
	int Change_Cnt(BLOCK_ID block_id);
	//ブロックの所持数が1以上あるかどうかの判定
	bool Compare_Cnt(BLOCK_ID block_id);
private:
	Block_Manager(void);
	Block_Manager(const Block_Manager& rhs);
	Block_Manager(Block_Manager& rhs);
	~Block_Manager(void);

	//ブロックの所持数
	int m_Block_Cnt[(int)BLOCK_ID::MAX];
	//ブロックが設置してある数
	int m_Put_Block_Cnt[(int)BLOCK_ID::MAX];

};