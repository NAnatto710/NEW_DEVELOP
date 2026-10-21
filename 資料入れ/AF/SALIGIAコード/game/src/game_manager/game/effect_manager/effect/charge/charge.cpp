#include "charge.h"
#include "../../../../../utility/utility.h"

const int CCharge::m_width = 16;
const int CCharge::m_height = 16;
const int CCharge::m_left = 0;
const int CCharge::m_top = 0;
const int CCharge::m_right = 16;
const int CCharge::m_bottom = 16;
const float CCharge::m_change_speed = 0.08;


CCharge::CCharge(void)
	:IEffect(m_width, m_height, m_left, m_top, m_right, m_bottom)
	,m_particle_position(0.0f,0.0f)
{
}

CCharge::~CCharge(void)
{
}

void CCharge::Initialize(PLAYER_ID player_id, const vivid::Vector2& position, DIRECTION direction, vivid::Vector2 scale, unsigned int color, float rotation)
{
	
	IEffect::Initialize(player_id, position, direction, scale, color, rotation);
	m_Rotation += Utility::GetRandomInt(-10,10);
	
	
}

void CCharge::Update(void)
{
	m_particle_position = m_Position + vivid::Vector2(Utility::GetRandomInt(-50, 50), Utility::GetRandomInt(-50, 50));
}

void CCharge::Draw(void)
{
	
	vivid::DrawTexture("data\\effect\\charge_particle.png", m_particle_position, m_Color, m_Rect, m_Anchor, m_Rotation);
	
	
}
