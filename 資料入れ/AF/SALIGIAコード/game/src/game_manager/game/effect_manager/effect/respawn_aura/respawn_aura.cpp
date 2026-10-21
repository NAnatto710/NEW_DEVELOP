#include "respawn_aura.h"

const int CRespawnAura::m_width = 200;
const int CRespawnAura::m_height = 222;
const int CRespawnAura::m_left = 0;
const int CRespawnAura::m_top = 0;
const int CRespawnAura::m_right = 200;
const int CRespawnAura::m_bottom = 222;
const float CRespawnAura::m_change_speed = 0.07;
const int CRespawnAura::m_rect_count = 15;

CRespawnAura::CRespawnAura(void)
	:IEffect(m_width, m_height, m_left, m_top, m_right, m_bottom)
	,m_ChangeSpeed(0.0f)
{
}

CRespawnAura::~CRespawnAura(void)
{
}

void CRespawnAura::Initialize(PLAYER_ID player_id, const vivid::Vector2& position, DIRECTION direction, vivid::Vector2 scale, unsigned int color, float rotation)
{
	IEffect::Initialize(player_id, position, direction, scale, color, rotation);
	m_ChangeSpeed = 0.0f;
}

void CRespawnAura::Update(void)
{
	m_ChangeSpeed += vivid::GetDeltaTime();


	for (int i = 0;i < m_rect_count;i++)
	{
		if (m_ChangeSpeed >= m_change_speed * i)
		{
			m_Rect = { m_width * i,m_top,m_width * (i+1),m_bottom };
		}
	}

	if (m_ChangeSpeed >= m_change_speed*m_rect_count)
		m_ActiveFlag = false;

}

void CRespawnAura::Draw(void)
{
	vivid::DrawTexture("data\\effect\\aura.png", m_Position, m_Color, m_Rect);
}
