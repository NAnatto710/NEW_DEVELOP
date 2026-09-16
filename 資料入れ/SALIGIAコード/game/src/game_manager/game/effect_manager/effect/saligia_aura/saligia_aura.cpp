#include "saligia_aura.h"
#include "../../../../../utility/utility.h"

const int CSaligiaAura::m_width = 150;
const int CSaligiaAura::m_height = 407;
const int CSaligiaAura::m_left = 0;
const int CSaligiaAura::m_top = 0;
const int CSaligiaAura::m_right = 150;
const int CSaligiaAura::m_bottom = 407;
const float CSaligiaAura::m_change_speed = 0.07;
const int CSaligiaAura::m_rect_count = 8;

CSaligiaAura::CSaligiaAura(void)
	:IEffect(m_width, m_height, m_left, m_top, m_right, m_bottom)
	,m_ChangeSpeed(0.0f)

{
}

CSaligiaAura::~CSaligiaAura(void)
{
}

void CSaligiaAura::Initialize(PLAYER_ID player_id, const vivid::Vector2& position, DIRECTION direction, vivid::Vector2 scale, unsigned int color, float rotation)
{
	IEffect::Initialize(player_id, position, direction, scale, color, rotation);
	m_ChangeSpeed = vivid::GetDeltaTime()* Utility::GetRandomInt(0, 8);
}

void CSaligiaAura::Update(void)
{
	m_ChangeSpeed += vivid::GetDeltaTime();


	for (int i = 0;i < m_rect_count;i++)
	{
		if (m_ChangeSpeed >= m_change_speed * i)
		{
			m_Rect = { m_width * i,m_top,m_width * (i + 1),m_bottom };
		}
	}

	if (m_ChangeSpeed >= m_change_speed * m_rect_count)
	{
		m_ChangeSpeed = 0.0f;
		m_Rect = { 0,0,m_width,m_height };
	}
		

}

void CSaligiaAura::Draw(void)
{
	vivid::DrawTexture("data\\effect\\saligia_aura.png", m_Position, m_Color, m_Rect);
}
