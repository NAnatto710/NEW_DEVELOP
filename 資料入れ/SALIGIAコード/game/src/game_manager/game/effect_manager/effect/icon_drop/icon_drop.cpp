#include "icon_drop.h"
#include "../../../../../utility/utility.h"

const int CIconDrop::m_width = 160;
const int CIconDrop::m_height = 165;
const int CIconDrop::m_left = 0;
const int CIconDrop::m_top = 0;
const int CIconDrop::m_right = 160;
const int CIconDrop::m_bottom = 165;
const float CIconDrop::m_drop_speed = 18.0f;

CIconDrop::CIconDrop(void)
	:IEffect(m_width, m_height, m_left, m_top, m_right, m_bottom)
	,m_LandingFlag(false)
{
}

CIconDrop::~CIconDrop(void)
{
}

void CIconDrop::Initialize(PLAYER_ID player_id, const vivid::Vector2& position, DIRECTION direction, vivid::Vector2 scale, unsigned int color, float rotation)
{
	IEffect::Initialize(player_id, position, direction, scale, color, rotation);
	m_LandingFlag = false;
	int rand = Utility::GetRandomInt(0, 7);
	m_Rect = { rand * m_width,0,rand * m_width + m_width,m_height };
}

void CIconDrop::Update(void)
{
	m_Position.y += m_drop_speed;
}

void CIconDrop::Draw(void)
{
	vivid::DrawTexture("data\\object\\saligia_color_icon.png",m_Position, m_Color, m_Rect,m_Anchor,m_Scale,m_Rotation);

}
