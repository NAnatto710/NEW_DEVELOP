#include "twinkle.h"

const int CTwinkle::m_width = 8;
const int CTwinkle::m_height = 8;
const int CTwinkle::m_left = 0;
const int CTwinkle::m_top = 0;
const int CTwinkle::m_right = 8;
const int CTwinkle::m_bottom = 8;
const int CTwinkle::m_fade_speed = 5.0f;
const float CTwinkle::m_drop_speed = 1.5f;
const float CTwinkle::m_spin_speed = 1.0f;

CTwinkle::CTwinkle(void)
	:IEffect(m_width, m_height, m_left, m_top, m_right, m_bottom)
	,m_SpinTimer(0.0f)
	,m_AlphaFlg(false)
{
}

CTwinkle::~CTwinkle(void)
{
}

void CTwinkle::Initialize(PLAYER_ID player_id, const vivid::Vector2& position, DIRECTION direction, vivid::Vector2 scale, unsigned int color, float rotation)
{
	IEffect::Initialize(player_id, position, direction, scale, color, rotation);
	m_SpinTimer = 0.0f;
	m_AlphaFlg = false;
}

void CTwinkle::Update(void)
{
	m_SpinTimer += vivid::GetDeltaTime();

	float alpha = (m_Color & 0xff000000) >> 24;
	float max_alpha = (0xffffffff & 0xff000000) >> 24;

	if (m_AlphaFlg == false)
	{
		alpha -= m_fade_speed;
		if (alpha < 0)
		{
			alpha = 0;
			m_AlphaFlg = true;
			m_ActiveFlag = false;
		}
	}

	if (m_AlphaFlg == true)
	{
		alpha += m_fade_speed;
		if (alpha >= max_alpha)
		{
			m_AlphaFlg = false;
			
		}
	}
	m_Color = ((unsigned int)alpha << 24) | (m_Color & 0x00ffffff);

		
}

void CTwinkle::Draw(void)
{
	vivid::DrawTexture("data\\effect\\twinkle.png", m_Position, m_Color, m_Rect, m_Anchor, m_Scale, m_Rotation, vivid::ALPHABLEND::ADD);
}
