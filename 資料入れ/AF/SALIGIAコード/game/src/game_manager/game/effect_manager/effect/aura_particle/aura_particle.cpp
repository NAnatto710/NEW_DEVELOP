#include "aura_particle.h"

const int CAuraParticle::m_width = 32;
const int CAuraParticle::m_height = 32;
const int CAuraParticle::m_left = 0;
const int CAuraParticle::m_top = 0;
const int CAuraParticle::m_right = 32;
const int CAuraParticle::m_bottom = 32;
const float CAuraParticle::m_drop_speed = -3.5;
const float CAuraParticle::m_fade_speed = 5.0f;

CAuraParticle::CAuraParticle(void)
	:IEffect(m_width, m_height, m_left, m_top, m_right, m_bottom)
	,m_Timer(0.0f)
{
}

CAuraParticle::~CAuraParticle(void)
{
}

void CAuraParticle::Initialize(PLAYER_ID player_id, const vivid::Vector2& position, DIRECTION direction, vivid::Vector2 scale, unsigned int color, float rotation)
{
	IEffect::Initialize(player_id, position, direction, scale, color, rotation);
	m_Timer = 0.0f;
}

void CAuraParticle::Update(void)
{
	m_Position.y += m_drop_speed;
	int alpha = (m_Color & 0xff000000) >> 24;
	alpha -= m_fade_speed;
	if (alpha < 0)
	{
		alpha = 0;
		m_ActiveFlag = false;
	}
	m_Color = (alpha << 24) | (m_Color & 0x00ffffff);
}

void CAuraParticle::Draw(void)
{
	vivid::DrawTexture("data\\effect\\aura_particle2.png", m_Position, m_Color, m_Rect, m_Anchor, m_Scale, m_Rotation, vivid::ALPHABLEND::ADD);
}
