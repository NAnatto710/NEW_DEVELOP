#include "hp_light.h"

const int CHpLight::m_width = 150;
const int CHpLight::m_height = 150;
const int CHpLight::m_left = 0;
const int CHpLight::m_top = 0;
const int CHpLight::m_right = 150;
const int CHpLight::m_bottom = 150;
const float CHpLight::m_fade_speed = 5.0f;

CHpLight::CHpLight(void)
	:IEffect(m_width, m_height, m_left, m_top, m_right, m_bottom)
{
}

CHpLight::~CHpLight(void)
{
}

void CHpLight::Initialize(PLAYER_ID player_id, const vivid::Vector2& position, DIRECTION direction, vivid::Vector2 scale, unsigned int color, float rotation)
{
	IEffect::Initialize(player_id, position, direction, scale, color, rotation);
}

void CHpLight::Update(void)
{
	int alpha = (m_Color & 0xff000000) >> 24;
	alpha -= m_fade_speed;
	if (alpha < 0)
	{
		alpha = 0;
		m_ActiveFlag = false;
	}
	m_Color = (alpha << 24) | (m_Color & 0x00ffffff);
}

void CHpLight::Draw(void)
{
	vivid::DrawTexture("data\\effect\\impact.png", m_Position, m_Color, m_Rect, m_Anchor, m_Scale, m_Rotation);
}
