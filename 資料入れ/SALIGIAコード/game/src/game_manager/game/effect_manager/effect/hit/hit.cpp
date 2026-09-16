#include "hit.h"
#include "../../../camera_manager/camera_manager.h"

const int CHit::m_width = 50;
const int CHit::m_height = 65;
const int CHit::m_left = 0;
const int CHit::m_top = 0;
const int CHit::m_right = 0;
const int CHit::m_bottom = 65;
const float CHit::m_change_speed = 0.03;

CHit::CHit(void)
	:IEffect(m_width, m_height, m_left, m_top, m_right, m_bottom)
{
}

CHit::~CHit(void)
{
}

void CHit::Initialize(PLAYER_ID player_id, const vivid::Vector2& position, DIRECTION direction, vivid::Vector2 scale, unsigned int color, float rotation)
{
	IEffect::Initialize(player_id, position, direction, scale, color, rotation);
	m_ChangeSpeed = 0;

	vivid::Vector2 scroll = CCameraManager::GetInstance().GetScroll();
	float    camera_scale = CCameraManager::GetInstance().GetCameraScale();
	m_Scale = { CCameraManager::GetInstance().GetCameraScale() ,CCameraManager::GetInstance().GetCameraScale() };

	m_Position *= camera_scale;
	m_Position -= scroll;
}

void CHit::Update(void)
{
	m_ChangeSpeed += vivid::GetDeltaTime();
	if (m_ChangeSpeed >= m_change_speed)
	{
		m_Rect = vivid::Rect{ m_width,m_top,m_width * 2,m_bottom };
	}

	if (m_ChangeSpeed >= m_change_speed * 2)
	{
		m_Rect = vivid::Rect{ m_width * 2,m_top,m_width * 3,m_bottom };
	}

	if (m_ChangeSpeed >= m_change_speed * 3)
	{
		m_ActiveFlag = false;
	}

}

void CHit::Draw(void)
{
	vivid::DrawTexture("data\\effect\\hit.png", m_Position, m_Color, m_Rect, m_Anchor, m_Scale, m_Rotation);
}
