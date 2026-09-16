
/*!
 *  @file       map.cpp
 *  @brief      マップ
 *  @author     Misaki Kawada
 *  @date       2026/02/12
 */

#include "map.h"

const float				CMap::m_size = 300.0f;														//!< サイズ
const float				CMap::m_between = 50.0f;														//!< 間
const vivid::Vector2	CMap::m_position = { vivid::WINDOW_WIDTH - (m_size + m_between),m_between };		//!< ポジション
const std::string		CMap::m_file_path = "data\\hud\\minimap.png";										//!< ファイルパス
const float				CMap::m_drawing_range = 3000;															//!< 描画範囲

CIcon			m_Icon;

/*
 * コンストラクタ
 */
CMap::
CMap(void)
{
}

/*
 * 初期化
 */
void
CMap::
Initialize(void)
{

	//m_IconList.clear();

}

/*
 * 更新
 */
void
CMap::
Update(void)
{
	ICONLIST::iterator it = m_IconList.begin();

	while (it != m_IconList.end())
	{
		(*it)->Update();
		++it;
	}
}
/*
 * 描画
 */
void
CMap::
Draw(void)
{
	vivid::DrawTexture(m_file_path, m_position, 0xa0ffffff);

	ICONLIST::iterator it = m_IconList.begin();

	while (it != m_IconList.end())
	{
		(*it)->Draw();
		++it;
	}
}

/*
 * 解放
 */
void
CMap::
Finalize(void)
{
	ICONLIST::iterator it = m_IconList.begin();

	while (it != m_IconList.end())
	{
		(*it)->Finalize();
		++it;
	}
}

/*
 * アイコン生成
 */
void
CMap::
Create(void)
{
	CIcon* icon = nullptr;

	icon = new CIcon;

	if (!icon)return;

	icon->Initialize();

	m_IconList.push_back(icon);
}

/*
 * マップポジション
 */
vivid::Vector2
CMap::
GetMapPosition(void)
{
	return m_position;
}

/*
 * サイズ
 */
float
CMap::
GetSize(void)
{
	return m_size;
}

/*
 * 描画範囲
 */
float
CMap::
GetDrawRange(void)
{
	return m_drawing_range;
}