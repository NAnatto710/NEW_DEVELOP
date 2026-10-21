#pragma once

#include "vivid.h"
#include "effect_id.h" 
#include "../../player_manager/player_id.h"

class IEffect
{
public:
    // コンストラクタ 
    IEffect(void);

    // コンストラクタ 
    IEffect(int width, int height, int left, int top, int right, int bottom);

    // デストラクタ 
    virtual ~IEffect(void);

    // 更新 
    virtual void Update(void);

    // 初期化 
    virtual void Initialize(PLAYER_ID player_id, const vivid::Vector2& position, DIRECTION direction, vivid::Vector2 scale ,unsigned int color, float rotation);

    // 描画 
    virtual void Draw(void);

    // 解放 
    virtual void Finalize(void);

    // 位置の取得
    vivid::Vector2 GetPosition(void);

    // 位置の設定 
    void SetPosition(const vivid::Vector2& position);

    // アクティブフラグの取得 
    bool GetActive(void);

    // アクティブフラグの設定 
    void SetActive(bool active);

protected:

    int                 m_Width;            // 幅
    int                 m_Height;           // 高さ
    int                 m_RectLeft;
    int                 m_RectTop;
    int                 m_RectRight;
    int                 m_RectBottom;
    vivid::Vector2      m_Position;         // 位置
    DIRECTION           m_Direction;        // 向き
    unsigned int        m_Color;            // 色
    vivid::Vector2      m_Anchor;           // 基準点
    vivid::Rect         m_Rect;             // 読み込み範囲
    vivid::Vector2      m_Scale;            // 拡大率
    float               m_Rotation;         // 回転率
    bool                m_ActiveFlag;       // アクティブフラグ
};