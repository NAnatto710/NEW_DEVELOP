#include "jump.h"
#include "../../../camera_manager/camera_manager.h"

const int CJump::m_width = 100;
const int CJump::m_height = 136;
const int CJump::m_left = 0;
const int CJump::m_top = 0;
const int CJump::m_right = 100;
const int CJump::m_bottom = 1360;
const int CJump::m_rect_count = 6;
const float CJump::m_fade_speed = 2.8;
const float CJump::m_change_speed = 0.08;

CJump::CJump(void)
	:IEffect(m_width, m_height, m_left, m_top, m_right, m_bottom)
	,m_ChangeSpeed(0.0f)

{
}

CJump::~CJump(void)
{
}

void CJump::Initialize(PLAYER_ID player_id, const vivid::Vector2& position, DIRECTION direction, vivid::Vector2 scale, unsigned int color, float rotation)
{
	IEffect::Initialize(player_id, position, direction, scale, color, rotation);
	m_ChangeSpeed = 0.0f;

	vivid::Vector2 scroll = CCameraManager::GetInstance().GetScroll();
	float    camera_scale = CCameraManager::GetInstance().GetCameraScale();
	m_Scale = { CCameraManager::GetInstance().GetCameraScale() ,CCameraManager::GetInstance().GetCameraScale() };

	m_Position *= camera_scale;
	m_Position -= scroll;
}

void CJump::Update(void)
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

void CJump::Draw(void)
{
	vivid::DrawTexture("data\\effect\\jump.png", m_Position, m_Color, m_Rect, m_Anchor, m_Scale, m_Rotation, vivid::ALPHABLEND::ADD);
}
