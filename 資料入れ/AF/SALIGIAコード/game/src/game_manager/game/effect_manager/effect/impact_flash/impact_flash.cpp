#include "impact_flash.h"
#include "../../../camera_manager/camera_manager.h"

const int CImpactFlash::m_width = 150;
const int CImpactFlash::m_height = 150;
const int CImpactFlash::m_left = 0;
const int CImpactFlash::m_top = 0;
const int CImpactFlash::m_right = 150;
const int CImpactFlash::m_bottom = 150;
const float CImpactFlash::m_fade_speed = 1.0f;
const float CImpactFlash::m_enlarge_speed = 15.0f;

CImpactFlash::CImpactFlash(void)
	:IEffect(m_width, m_height, m_left, m_top, m_right, m_bottom)
{
}

CImpactFlash::~CImpactFlash(void)
{
}

void CImpactFlash::Initialize(PLAYER_ID player_id, const vivid::Vector2& position, DIRECTION direction, vivid::Vector2 scale, unsigned int color, float rotation)
{
	IEffect::Initialize(player_id, position, direction, scale, color, rotation);
	m_Timer = 0.0f;

	vivid::Vector2 scroll = CCameraManager::GetInstance().GetScroll();
	float    camera_scale = CCameraManager::GetInstance().GetCameraScale();
	m_Scale = { CCameraManager::GetInstance().GetCameraScale() ,CCameraManager::GetInstance().GetCameraScale() };

	m_Position *= camera_scale;
	m_Position -= scroll;
}

void CImpactFlash::Update(void)
{
	m_Timer += vivid::GetDeltaTime();

	m_Scale += vivid::Vector2(m_enlarge_speed, m_enlarge_speed);

	if (m_Timer >= m_fade_speed)
		m_ActiveFlag = false;
}

void CImpactFlash::Draw(void)
{
	vivid::DrawTexture("data\\effect\\impact_flash.png", m_Position, m_Color, m_Rect, m_Anchor, m_Scale, m_Rotation);
}
