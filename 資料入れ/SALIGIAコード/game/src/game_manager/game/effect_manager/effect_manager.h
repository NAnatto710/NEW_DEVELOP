#pragma once
/*!
 *  @file       effect_manager.cpp
 *  @brief      エフェクト管理
 *  @author     Hiroto Maniwa
 *  @date       2026/08/20
 */

#include "vivid.h"
#include "effect/effect_id.h"
#include "effect/effect_object.h"
#include <list>
#include "effect/effect.h"
#include "../player_manager/player_id.h"

class IEffect;
class CEffectManager
{
public:
    // インスタンスの取得 
    static CEffectManager& GetInstance(void);

    // 初期化 
    void        Initialize(void);

    // 更新 
    void        Update(void);

    // 描画 
    void        Draw(void);

    // 解放 
    void        Finalize(void);

    /*!
     *	@brief		エフェクトの生成
     *
     *	@param[in]	effect_id	エフェクトID
     *	@param[in]	position	生成位置
     *	@param[in]	direction   向き
     *	@param[in]	scale       大きさ
     *  @param[in]  color       色
     *  @param[in]  rotation    回転値
     */
    void        Create(EFFECT_ID effect_id,PLAYER_ID player_id,vivid::Vector2 position,DIRECTION direction,vivid::Vector2 scale,unsigned int color,float rotation);

private:

    // コンストラクタ 
    CEffectManager(void);

    // コピーコンストラクタ 
    CEffectManager(const CEffectManager& rhs);

    // デストラクタ 
    ~CEffectManager(void);

    // 代⼊演算⼦ 
    CEffectManager& operator=(const CEffectManager& rhs);

    // エフェクトリスト型の定義 
    using EFFECT_LIST = std::list<IEffect*>;
    EFFECT_LIST         m_EffectList;       // エフェクトリスト 
};
