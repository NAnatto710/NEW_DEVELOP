#include "ranking_twinkle.h"

const int CRankingTwinkle::m_width = 16;
const int CRankingTwinkle::m_height = 16;
const int CRankingTwinkle::m_left = 0;
const int CRankingTwinkle::m_top = 0;
const int CRankingTwinkle::m_right = 16;
const int CRankingTwinkle::m_bottom = 16;
const int CRankingTwinkle::m_fade_speed = 3.0f;
const float CRankingTwinkle::m_drop_speed = 1.5f;
const float CRankingTwinkle::m_enlarge_speed = 3.0f;

CRankingTwinkle::CRankingTwinkle(void)
	:IEffect(m_width, m_height, m_left, m_top, m_right, m_bottom)
	, m_EnlargeTimer(0.0f)
	, m_AlphaFlg(false)
{
}

CRankingTwinkle::~CRankingTwinkle(void)
{
}

void CRankingTwinkle::Initialize(PLAYER_ID player_id, const vivid::Vector2& position, DIRECTION direction, vivid::Vector2 scale, unsigned int color, float rotation)
{
	IEffect::Initialize(player_id, position, direction, scale, color, rotation);
	m_EnlargeTimer = 0.0f;
	m_AlphaFlg = false;
}

void CRankingTwinkle::Update(void)
{
	m_EnlargeTimer += vivid::GetDeltaTime();

	float alpha = (m_Color & 0xff000000) >> 24;
	float max_alpha = (0xffffffff & 0xff000000) >> 24;

	if (m_AlphaFlg == false)
	{
		if (m_Scale.x > 0.0f)
		{
			m_Scale -= vivid::Vector2(m_EnlargeTimer, m_EnlargeTimer);
		}
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
		if (m_Scale.x < 0.0f)
		{
			m_Scale -= vivid::Vector2(m_EnlargeTimer, m_EnlargeTimer);
		}
		alpha += m_fade_speed;
		if (alpha >= max_alpha)
		{
			m_AlphaFlg = false;

		}
	}
	m_Color = ((unsigned int)alpha << 24) | (m_Color & 0x00ffffff);

	if (m_EnlargeTimer > 7)
	{
		m_ActiveFlag = false;
	}
}

void CRankingTwinkle::Draw(void)
{
	vivid::DrawTexture("data\\effect\\charge_particle.png", m_Position, m_Color, m_Rect, m_Anchor, m_Scale, m_Rotation, vivid::ALPHABLEND::ADD);
}
