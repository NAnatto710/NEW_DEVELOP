#include "smoke.h"
#include "../../../camera_manager/camera_manager.h"

const int CSmoke::m_width = 75;
const int CSmoke::m_height = 150;
const int CSmoke::m_left = 0;
const int CSmoke::m_top = 0;
const int CSmoke::m_right = 75;
const int CSmoke::m_bottom = 150;
const int CSmoke::m_rect_count = 6;
const float CSmoke::m_fade_speed = 2.8;
const float CSmoke::m_change_speed = 0.08;

CSmoke::CSmoke(void)
	:IEffect(m_width, m_height, m_left, m_top, m_right, m_bottom)
{
}

CSmoke::~CSmoke(void)
{
}

void CSmoke::Initialize(PLAYER_ID player_id, const vivid::Vector2& position, DIRECTION direction, vivid::Vector2 scale, unsigned int color, float rotation)
{
	IEffect::Initialize(player_id, position, direction, scale, color, rotation);
	m_ChangeSpeed = 0.0f;

	vivid::Vector2 scroll = CCameraManager::GetInstance().GetScroll();
	float    camera_scale = CCameraManager::GetInstance().GetCameraScale();
	m_Scale = { CCameraManager::GetInstance().GetCameraScale() ,CCameraManager::GetInstance().GetCameraScale() };

	m_Position *= camera_scale;
	m_Position -= scroll;
}

void CSmoke::Update(void)
{
	m_ChangeSpeed += vivid::GetDeltaTime();

	for (int i = 0;i < m_rect_count;i++)
	{
		if (m_ChangeSpeed >= m_change_speed * i)
		{
			m_Rect = { m_width * i,m_top,m_width * (i + 1),m_bottom };
		}
	}

	int alpha = (m_Color & 0xff000000) >> 24;
	alpha -= m_fade_speed;
	if (alpha < 0)
	{
		alpha = 0;
		m_ActiveFlag = false;
	}
	m_Color = (alpha << 24) | (m_Color & 0x00ffffff);
}

void CSmoke::Draw(void)
{
	vivid::DrawTexture("data\\effect\\smoke.png", m_Position, m_Color, m_Rect, m_Anchor,m_Scale, m_Rotation, vivid::ALPHABLEND::ADD);
}
