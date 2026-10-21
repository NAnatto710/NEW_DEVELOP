#include "guard.h"
#include "../../../player_manager/player_manager.h"
#include "../../../player_manager/player/status/resource_component/resource_component.h"
#include "../../../camera_manager/camera_manager.h"

const int CGuard::m_width = 150;
const int CGuard::m_height = 150;
const int CGuard::m_left = 0;
const int CGuard::m_top = 0;
const int CGuard::m_right = 150;
const int CGuard::m_bottom = 150;
const float CGuard::m_fade_speed = 1.0f;

CGuard::CGuard(void)
	:IEffect(m_width, m_height,m_left, m_top, m_right, m_bottom)
	,m_GuardCount(0)
	,m_GuardTime(0.0f)
{
}

CGuard::~CGuard(void)
{
}

void CGuard::Initialize(PLAYER_ID player_id,const vivid::Vector2& position, DIRECTION direction, vivid::Vector2 scale, unsigned int color, float rotation)
{
	IEffect::Initialize(player_id, position, direction, scale, color, rotation);

	vivid::Vector2 scroll = CCameraManager::GetInstance().GetScroll();
	float    camera_scale = CCameraManager::GetInstance().GetCameraScale();
	m_Scale = { CCameraManager::GetInstance().GetCameraScale() ,CCameraManager::GetInstance().GetCameraScale() };

	m_Position *= camera_scale;
	m_Position -= scroll;
}

void CGuard::Update(void)
{
	//// ハイエナサーっぽい
	//m_GuardTime -= vivid::GetDeltaTime();
	//m_Scale = vivid::Vector2(m_width - (m_width / m_GuardTime), m_height - (m_height / m_GuardTime));
	//if (m_GuardTime < 0)
	//{
	//	
	//	m_ActiveFlag = false;
	//}
	int alpha = (m_Color & 0xff000000) >> 24;
	alpha -= m_fade_speed;
	if (alpha < 0)
	{
		alpha = 0;
		m_ActiveFlag = false;
	}
	m_Color = (alpha << 24) | (m_Color & 0x00ffffff);

}

void CGuard::Draw(void)
{
	vivid::DrawTexture("data\\effect\\guard.png", m_Position, m_Color, m_Rect, m_Anchor, m_Scale, m_Rotation);
}
