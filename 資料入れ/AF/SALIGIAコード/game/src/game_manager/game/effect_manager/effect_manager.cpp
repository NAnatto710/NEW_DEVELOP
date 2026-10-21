#include "effect_manager.h"

CEffectManager& CEffectManager::GetInstance(void)
{
	static CEffectManager instance;
	return instance;
}

void CEffectManager::Initialize(void)
{
	m_EffectList.clear();
}

void CEffectManager::Update(void)
{
    // リストが空なら終了 
    if (m_EffectList.empty()) return;
    EFFECT_LIST::iterator it = m_EffectList.begin();
    while (it != m_EffectList.end())
    {
        IEffect* effect = (IEffect*)(*it);
        effect->Update();
        // エフェクトが⾮アクティブなら削除してリストから外す 
        if (!effect->GetActive())
        {
            effect->Finalize();
            delete effect;
            it = m_EffectList.erase(it);
            continue;
        }
        ++it;
    }
}

void CEffectManager::Draw(void)
{
    if (m_EffectList.empty()) return;
    EFFECT_LIST::iterator it = m_EffectList.begin();
    while (it != m_EffectList.end())
    {
        (*it)->Draw();
        ++it;
    }
}

void CEffectManager::Finalize(void)
{
    if (m_EffectList.empty()) return;
    EFFECT_LIST::iterator it = m_EffectList.begin();
    while (it != m_EffectList.end())
    {
        (*it)->Finalize();
        delete (*it);
        ++it;
    }
    m_EffectList.clear();
}

void CEffectManager::Create(EFFECT_ID effect_id,PLAYER_ID player_id, vivid::Vector2 position, DIRECTION direction, vivid::Vector2 scale, unsigned int color, float rotation)
{
    IEffect* effect = nullptr;
    switch (effect_id)
    {
    case EFFECT_ID::HIT:effect = new CHit(); break;
    case EFFECT_ID::IMPACT:effect = new CImpact();break;
    case EFFECT_ID::GUARD:effect = new CGuard();break;
    case EFFECT_ID::CHARGE:effect = new CCharge();break;
    case EFFECT_ID::HP_PARTICLE:effect = new CHpParticle();break;
    case EFFECT_ID::RESPAWN_AURA:effect = new CRespawnAura();break;
    case EFFECT_ID::SALIGIA_AURA:effect = new CSaligiaAura();break;
    case EFFECT_ID::SMOKE:effect = new CSmoke();break;
    case EFFECT_ID::JAMP:effect = new CJump();break;
    case EFFECT_ID::TITLE_PARTICLE:effect = new CTitleParticle();break;
    case EFFECT_ID::SIRCLE:effect = new CCircle();break;
    case EFFECT_ID::TWINKLE:effect = new CTwinkle();break;
    case EFFECT_ID::RANKING_TWINKLE:effect = new CRankingTwinkle();break;
    case EFFECT_ID::ICON_DROP:effect = new CIconDrop();break;
    case EFFECT_ID::FRAME_FLASH:effect = new CFrameFlash();break;
    case EFFECT_ID::AURA_PARTICLE:effect = new CAuraParticle();break;
    case EFFECT_ID::IMPACT_FLASH:effect = new CImpactFlash();break;
    case EFFECT_ID::RESPAWN_PARTICLE:effect = new CRespawnParticle();break;
    }

    if (!effect) return;

    effect->Initialize(player_id ,position,direction,scale, color, rotation);
    m_EffectList.push_back(effect);
}

CEffectManager::CEffectManager(void)
{
}

CEffectManager::CEffectManager(const CEffectManager& rhs)
{
}

CEffectManager::~CEffectManager(void)
{
}

CEffectManager& CEffectManager::operator=(const CEffectManager& rhs)
{
    (void)rhs;
    return *this;
}
