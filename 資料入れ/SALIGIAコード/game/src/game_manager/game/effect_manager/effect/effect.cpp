#include "effect.h"
#include "../../camera_manager/camera_manager.h"

IEffect::IEffect(void)
    : m_Width(0)
    , m_Height(0)
    , m_RectLeft(0)
    , m_RectTop(0)
    , m_RectRight(0)
    , m_RectBottom(0)
    , m_Position(vivid::Vector2(0.0f, 0.0f))
    , m_Color(0xffffffff)
    , m_Anchor(vivid::Vector2(0.0f, 0.0f))
    , m_Rect({ 0, 0, 0, 0 })
    , m_Scale(vivid::Vector2(1.0f, 1.0f))
    , m_Rotation(0.0f)
    , m_ActiveFlag(true)
{
}

IEffect::IEffect(int width, int height, int left, int top, int right, int bottom)
    : m_Width(width)
    , m_Height(height)
    , m_RectLeft(left)
    , m_RectTop(top)
    , m_RectRight(right)
    , m_RectBottom(bottom)
    , m_Position(vivid::Vector2(0.0f, 0.0f))
    , m_Color(0xffffffff)
    , m_Anchor(vivid::Vector2((float)m_Width / 2.0f, (float)m_Height / 2.0f))
    , m_Rect({ m_RectLeft, m_RectTop, m_Width+m_RectRight, m_Height+m_RectBottom })
    , m_Scale(vivid::Vector2(1.0f, 1.0f))
    , m_Rotation(0.0f)
    , m_ActiveFlag(true)
{
}

IEffect::~IEffect(void)
{
}

void IEffect::Update(void)
{
}

void IEffect::Initialize(PLAYER_ID player_id,const vivid::Vector2& position, DIRECTION direction, vivid::Vector2 scale, unsigned int color, float rotation)
{
    m_Position = position;
    m_Direction = direction;
    m_Color = color;
    m_Scale = scale;
    m_Rotation = rotation;
    m_ActiveFlag = true;

    

    switch (m_Direction)
    {
    case DIRECTION::TOP:
        break;
    case DIRECTION::BOTTOM:
        m_Scale = scale * vivid::Vector2(1.0f, -1.0f);
        break;
    case DIRECTION::RIGHT:
        m_Scale = scale*vivid::Vector2(1.0f, 1.0f);
        break;
    case DIRECTION::LEFT:
        m_Scale = scale * vivid::Vector2(-1.0f, 1.0f);
        break;
    default:
        break;
    }
}

void IEffect::Draw(void)
{
}

void IEffect::Finalize(void)
{
}

vivid::Vector2 IEffect::GetPosition(void)
{
    return m_Position;
}

void IEffect::SetPosition(const vivid::Vector2& position)
{
    m_Position = position;
}

bool IEffect::GetActive(void)
{
    return m_ActiveFlag;
}

void IEffect::SetActive(bool active)
{
    m_ActiveFlag = active;
}
