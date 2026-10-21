#include "respawn_particle.h"
#include "../../../../../utility/utility.h"
#include "../../../camera_manager/camera_manager.h"

const int CRespawnParticle::m_width = 32;
const int CRespawnParticle::m_height = 32;
const int CRespawnParticle::m_left = 0;
const int CRespawnParticle::m_top = 0;
const int CRespawnParticle::m_right = 32;
const int CRespawnParticle::m_bottom = 32;
const float CRespawnParticle::m_fade_speed = 15.0f;

CRespawnParticle::CRespawnParticle(void)
	:IEffect(m_width, m_height, m_left, m_top, m_right, m_bottom)
{
}

CRespawnParticle::~CRespawnParticle(void)
{
}

void CRespawnParticle::Initialize(PLAYER_ID player_id, const vivid::Vector2& position, DIRECTION direction, vivid::Vector2 scale, unsigned int color, float rotation)
{
	IEffect::Initialize(player_id, position, direction, scale, color, rotation);
	m_Position += vivid::Vector2(Utility::GetRandomInt(0,140),Utility::GetRandomInt(0,140));
	m_Destination = m_Position + vivid::Vector2(70.0f, 70.0f);
	m_Velocity = vivid::Vector2(0.0f, 0.0f);

	vivid::Vector2 scroll = CCameraManager::GetInstance().GetScroll();
	float    camera_scale = CCameraManager::GetInstance().GetCameraScale();
	m_Scale = { CCameraManager::GetInstance().GetCameraScale() ,CCameraManager::GetInstance().GetCameraScale() };

	m_Position *= camera_scale;
	m_Position -= scroll;
}

void CRespawnParticle::Update(void)
{
	vivid::Vector2 v = m_Destination - m_Position;
	float angle = atan2(v.y, v.x);
	m_Velocity.x = cos(angle);
	m_Velocity.y = sin(angle);

	m_Position += m_Velocity;

	int alpha = (m_Color & 0xff000000) >> 24;
	alpha -= m_fade_speed;
	if (alpha < 0)
	{
		alpha = 0;
		m_ActiveFlag = false;
	}
	m_Color = (alpha << 24) | (m_Color & 0x00ffffff);

}

void CRespawnParticle::Draw(void)
{
	vivid::DrawTexture("data\\effect\\respawn_particle.png", m_Position, m_Color, m_Rect, m_Anchor, m_Scale, m_Rotation);

}
