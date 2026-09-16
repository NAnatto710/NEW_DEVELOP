#include "hp_particle.h"
#include "../../../../../utility/utility.h"

const int CHpParticle::m_width = 16;
const int CHpParticle::m_height = 16;
const int CHpParticle::m_left = 0;
const int CHpParticle::m_top = 0;
const int CHpParticle::m_right = 16;
const int CHpParticle::m_bottom = 16;
const float CHpParticle::m_drop_speed = 1.5;
const float CHpParticle::m_fade_speed = 2.0f;

CHpParticle::CHpParticle(void)
	:IEffect(m_width, m_height, m_left, m_top, m_right, m_bottom)
{
}

CHpParticle::~CHpParticle(void)
{
}

void CHpParticle::Initialize(PLAYER_ID player_id, const vivid::Vector2& position, DIRECTION direction, vivid::Vector2 scale, unsigned int color, float rotation)
{
	IEffect::Initialize(player_id, position, direction, scale, color, rotation);
	m_Rotation = DEG_TO_RAD(Utility::GetRandomInt(0, 360));
	m_Position.x += Utility::GetRandomInt(-15, 15);
}

void CHpParticle::Update(void)
{
	m_Rotation += DEG_TO_RAD(Utility::GetRandomInt(0,5));
	m_Position += vivid::Vector2(0.1*Utility::GetRandomInt(-1, 1), m_drop_speed);
	int alpha = (m_Color & 0xff000000) >> 24;
	alpha -=m_fade_speed;
	if (alpha < 0)
	{
		alpha = 0;
		m_ActiveFlag = false;
	}
	m_Color = (alpha << 24) | (m_Color & 0x00ffffff);
}

void CHpParticle::Draw(void)
{
	vivid::DrawTexture("data\\effect\\hp_particle_light.png", m_Position, m_Color, m_Rect, m_Anchor,m_Scale, m_Rotation,vivid::ALPHABLEND::ADD);
	vivid::DrawTexture("data\\effect\\hp_particle.png", m_Position, m_Color, m_Rect, m_Anchor,m_Scale, m_Rotation,vivid::ALPHABLEND::ADD);
}
