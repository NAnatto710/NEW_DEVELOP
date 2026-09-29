#include "Block_Manager.h"

Block_Manager& Block_Manager::GetInstance(void)
{
	//クラスオブジェクトの取得
	static Block_Manager instance;

	return instance;
}

void Block_Manager::Set_Block_Cnt(BLOCK_ID block_id, int cnt)
{
	m_Block_Cnt[(int)block_id] = cnt;
}

void Block_Manager::Set_Block_Cnt(BLOCK_ID block_id,int cnt,int cnt2)
{
	m_Block_Cnt[(int)block_id] = cnt;
	m_Put_Block_Cnt[(int)block_id] = cnt2;
}

int Block_Manager::Get_Block_Cnt(BLOCK_ID block_id)
{
	switch (block_id)
	{
	case BLOCK_ID::RULER:
		return m_Block_Cnt[(int)BLOCK_ID::RULER];
		break;
	case BLOCK_ID::SETSQUARE:
		return m_Block_Cnt[(int)BLOCK_ID::SETSQUARE];
		break;
	case BLOCK_ID::COMPASS:
		return m_Block_Cnt[(int)BLOCK_ID::COMPASS];
		break;
	case BLOCK_ID::PENCIL:
		return m_Block_Cnt[(int)BLOCK_ID::PENCIL];
		break;
	}
}

int Block_Manager::Get_Put_Block_Cnt(BLOCK_ID block_id)
{
	switch (block_id)
	{
	case BLOCK_ID::RULER:
		return m_Put_Block_Cnt[(int)BLOCK_ID::RULER];
		break;
	case BLOCK_ID::SETSQUARE:
		return m_Put_Block_Cnt[(int)BLOCK_ID::SETSQUARE];
		break;
	case BLOCK_ID::COMPASS:
		return m_Put_Block_Cnt[(int)BLOCK_ID::COMPASS];
		break;
	case BLOCK_ID::PENCIL:
		return m_Put_Block_Cnt[(int)BLOCK_ID::PENCIL];
		break;
	}
}

int Block_Manager::Change_Cnt(BLOCK_ID block_id)
{
	if (Compare_Cnt(block_id))
	{
		m_Block_Cnt[(int)block_id] -= 1;
		m_Put_Block_Cnt[(int)block_id] += 1;
	}
	return m_Put_Block_Cnt[(int)block_id];
}

bool Block_Manager::Compare_Cnt(BLOCK_ID block_id)
{
	switch (block_id)
	{
	case BLOCK_ID::RULER:
		return m_Block_Cnt[(int)BLOCK_ID::RULER] > 0;
		break;
	case BLOCK_ID::SETSQUARE:
		return m_Block_Cnt[(int)BLOCK_ID::SETSQUARE] > 0;
		break;
	case BLOCK_ID::COMPASS:
		return m_Block_Cnt[(int)BLOCK_ID::COMPASS] > 0;
		break;
	case BLOCK_ID::PENCIL:
		return m_Block_Cnt[(int)BLOCK_ID::PENCIL] > 0;
		break;
	}
}

Block_Manager::Block_Manager(void)
	:m_Block_Cnt()
	,m_Put_Block_Cnt()
{
}

Block_Manager::Block_Manager(const Block_Manager& rhs)
{
	(void)rhs;
}

Block_Manager::Block_Manager(Block_Manager& rhs)
{
	(void)rhs;
}

Block_Manager::~Block_Manager(void)
{
}