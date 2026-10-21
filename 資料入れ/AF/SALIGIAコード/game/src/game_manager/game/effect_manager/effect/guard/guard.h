#pragma once

#include "vivid.h"
#include "../effect.h"

class CGuard :public IEffect
{
public:
    // コンストラクタ 
    CGuard(void);
    // デストラクタ 
    ~CGuard(void);
    // 初期化 
    void Initialize(PLAYER_ID player_id, const vivid::Vector2& position, DIRECTION direction, vivid::Vector2 scale, unsigned int color, float rotation);
    // 更新 
    void Update(void);
    // 描画 
    void Draw(void);

private:
    static const int        m_width;        // 幅
    static const int        m_height;       // 高さ
    static const int        m_left;
    static const int        m_top;
    static const int        m_right;
    static const int        m_bottom;
    static const float      m_fade_speed;
    float                   m_GuardTime;
    float                   m_GuardCount;
    PLAYER_ID               m_Id;
};