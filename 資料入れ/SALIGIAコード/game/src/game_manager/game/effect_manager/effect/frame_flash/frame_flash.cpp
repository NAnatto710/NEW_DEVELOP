#include "frame_flash.h"

const int CFrameFlash::m_width = 160;
const int CFrameFlash::m_height = 160;
const int CFrameFlash::m_left = 0;
const int CFrameFlash::m_top = 0;
const int CFrameFlash::m_right = 160;
const int CFrameFlash::m_bottom = 160;
const float CFrameFlash::m_enlarge_speed = 0.1f;
const float CFrameFlash::m_fade_speed = 15.0f;

CFrameFlash::CFrameFlash(void)
	:IEffect(m_width, m_height, m_left, m_top, m_right, m_bottom)
{
}

CFrameFlash::~CFrameFlash(void)
{
}

void CFrameFlash::Initialize(PLAYER_ID player_id, const vivid::Vector2& position, DIRECTION direction, vivid::Vector2 scale, unsigned int color, float rotation)
{
	IEffect::Initialize(player_id, position, direction, scale, color, rotation);
	m_Timer = 0.0f;

}

void CFrameFlash::Update(void)
{
	int alpha = (m_Color & 0xff000000) >> 24;
	alpha -= m_fade_speed;
	m_Scale += vivid::Vector2(m_enlarge_speed, m_enlarge_speed);
	if (alpha < 0)
	{
		alpha = 0;
		m_ActiveFlag = false;
	}
	m_Color = (alpha << 24) | (m_Color & 0x00ffffff);
}

void CFrameFlash::Draw(void)
{
	vivid::DrawTexture("data\\effect\\frame_flash.png", m_Position, m_Color, m_Rect, m_Anchor, m_Scale, m_Rotation );
}
