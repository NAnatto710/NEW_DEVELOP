#include "circle.h"

const int CCircle::m_width = 32;
const int CCircle::m_height = 32;
const int CCircle::m_left = 0;
const int CCircle::m_top = 0;
const int CCircle::m_right = 32;
const int CCircle::m_bottom = 32;
const float CCircle::m_enlarge_speed = 0.7f;

CCircle::CCircle(void)
	:IEffect(m_width, m_height, m_left, m_top, m_right, m_bottom)
{
}

CCircle::~CCircle(void)
{
}

void CCircle::Initialize(PLAYER_ID player_id, const vivid::Vector2& position, DIRECTION direction, vivid::Vector2 scale, unsigned int color, float rotation)
{
	IEffect::Initialize(player_id, position, direction, scale, color, rotation);
}

void CCircle::Update(void)
{
	m_Scale += vivid::Vector2(m_enlarge_speed, m_enlarge_speed);
	if (m_Scale.x >= 12)
	{
		m_ActiveFlag = false;
	}
}

void CCircle::Draw(void)
{
	vivid::DrawTexture("data\\effect\\circle.png", m_Position, m_Color, m_Rect, m_Anchor, m_Scale, m_Rotation, vivid::ALPHABLEND::ADD);
}
