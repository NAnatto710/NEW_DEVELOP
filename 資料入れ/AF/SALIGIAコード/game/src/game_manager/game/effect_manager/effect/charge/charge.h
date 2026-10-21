#include "vivid.h"
#include "../effect.h"

class CCharge :public IEffect
{
public:
    // コンストラクタ 
    CCharge(void);
    // デストラクタ 
    ~CCharge(void);
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
    static const float      m_change_speed;
    float                   m_ChangeSpeed;
    vivid::Vector2          m_particle_position;
};