#include "title_particle.h"
#include "../../../../../utility/utility.h"

const int CTitleParticle::m_width = 8;
const int CTitleParticle::m_height = 8;
const int CTitleParticle::m_left = 0;
const int CTitleParticle::m_top = 0;
const int CTitleParticle::m_right = 8;
const int CTitleParticle::m_bottom = 8;
const float CTitleParticle::m_drop_speed = -1.5;
const int CTitleParticle::m_fade_speed = 1.0f;

CTitleParticle::CTitleParticle(void)
	:IEffect(m_width, m_height, m_left, m_top, m_right, m_bottom)
	,m_ChangeSpeed(0.0f)
	,m_RandNumber(0)

{
}

CTitleParticle::~CTitleParticle(void)
{
}

void CTitleParticle::Initialize(PLAYER_ID player_id, const vivid::Vector2& position, DIRECTION direction, vivid::Vector2 scale, unsigned int color, float rotation)
{
	IEffect::Initialize(player_id, position, direction, scale, color, rotation);
	m_Rotation = DEG_TO_RAD(Utility::GetRandomInt(0, 360));
	m_RandNumber = Utility::GetRandomInt(-3, 3);
	/*m_Position.x += Utility::GetRandomInt(-800, 800);
	m_Position.y += Utility::GetRandomInt(-100, 100);*/
}

void CTitleParticle::Update(void)
{
	m_Rotation += DEG_TO_RAD(m_RandNumber);
	m_Position += vivid::Vector2(0.1*Utility::GetRandomInt(-1, 1), m_drop_speed);
	int alpha = (m_Color & 0xff000000) >> 24;
	alpha -= m_fade_speed;
	if (alpha < 0)
	{
		alpha = 0;
		m_ActiveFlag = false;
	}
	m_Color = (alpha << 24) | (m_Color & 0x00ffffff);
}

void CTitleParticle::Draw(void)
{
	vivid::DrawTexture("data\\effect\\particle.png", m_Position, m_Color, m_Rect, m_Anchor, m_Scale, m_Rotation, vivid::ALPHABLEND::ADD);
}
