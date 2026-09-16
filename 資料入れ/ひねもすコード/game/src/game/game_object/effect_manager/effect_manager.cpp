
/*!
 *  @file       effect_manager.cpp
 *  @brief      エフェクト管理
 *  @author     Ryusei Shimizu
 *  @date       2026/02/19
 */

#include "effect_manager.h"
#include "effect/effect_object.h"
#include "guide/guide_object.h"
#include "../parameter_manager/parameter_manager.h"

/*
 *  インスタンスの取得
 */
CEffectManager&
CEffectManager::
GetInstance(void)
{
	static CEffectManager instance;

	return instance;
}

/*
 *  初期化
 */
void
CEffectManager::
Initialize(void)
{
	m_EffectList.clear();
	m_GuideList.clear();

	vivid::Vector2 pos = vivid::Vector2(vivid::WINDOW_WIDTH/2,vivid::WINDOW_HEIGHT/2);
	vivid::Vector2 dis_pos1 = vivid::Vector2(300.0f, 200.0f);
	vivid::Vector2 dis_pos2 = vivid::Vector2(200.0f, -200.0f);

	this->Create(GUIDE_ID::RUSH, pos - dis_pos1);
	this->Create(GUIDE_ID::FIRE, pos + dis_pos2);
}

/*
 *  更新
 */
void
CEffectManager::
Update(void)
{
	// エフェクトの更新
    if (!m_EffectList.empty())
    {
        EFFECT_LIST::iterator it = m_EffectList.begin();
        while (it != m_EffectList.end())
        {
            IEffect* effect = (IEffect*)(*it);

            effect->Update();

            // エフェクトが⾮アクティブなら削除してリストから外す 
            if (!effect->IsActive())
            {
                effect->Finalize();
                delete effect;
                it = m_EffectList.erase(it);
                continue;
            }

            ++it;
        }
    }

	// ガイドの更新
	if (!m_GuideList.empty())
	{
		GUIDE_LIST::iterator it = m_GuideList.begin();
		while (it != m_GuideList.end())
		{
			IGuide* guide = (IGuide*)(*it);

			guide->Update();

			if (!guide->IsActive())
			{
				guide->Finalize();
				delete guide;
				it = m_GuideList.erase(it);
				continue;
			}
			++it;
		}
	}
}

/*
 *  描画
 */
void
CEffectManager::
Draw(void)
{
	// シーズン変更エフェクト中はキャラクター、バレットの更新を行わない
	if (!CGameParameterManager::GetInstance().IsSeasonChangeEffect())
	{
		// ガイドの描画
		if (!m_GuideList.empty())
		{
			GUIDE_LIST::iterator it = m_GuideList.begin();
			while (it != m_GuideList.end())
			{
				(*it)->Draw();
				++it;
			}
		}
	}

	// エフェクトの描画
    if (!m_EffectList.empty())
    {
        EFFECT_LIST::iterator it = m_EffectList.begin();

        while (it != m_EffectList.end())
        {
			if ((*it)->GetEffectID() != EFFECT_ID::SEASON_CHANGE)
				(*it)->Draw();

            ++it;
        }
    }
}

/*
 *  シーンエフェクト描画
 */
void
CEffectManager::
SceneEffectDraw(void)
{
	// シーズン変更エフェクトの描画
	if (!m_EffectList.empty())
	{
		EFFECT_LIST::iterator it = m_EffectList.begin();
		while (it != m_EffectList.end())
		{
			if ((*it)->GetEffectID() == EFFECT_ID::SEASON_CHANGE)
				(*it)->Draw();

			++it;
		}
	}
}

/*
 *  解放
 */
void
CEffectManager::
Finalize(void)
{
	// エフェクトの解放
	if (!m_EffectList.empty())
	{
		EFFECT_LIST::iterator it = m_EffectList.begin();

		while (it != m_EffectList.end())
		{
			(*it)->Finalize();
			delete (*it);
			++it;
		}
	}

	// ガイドの解放
	if (!m_GuideList.empty())
	{
		GUIDE_LIST::iterator it = m_GuideList.begin();
		while (it != m_GuideList.end())
		{
			(*it)->Finalize();
			delete (*it);
			++it;
		}
	}

    m_EffectList.clear();
	m_GuideList.clear();
}

/*
 *  エフェクトの生成
 */
void
CEffectManager::
Create(EFFECT_ID id, const vivid::Vector2& pos, unsigned int color, float rotation)
{
    IEffect* effect = nullptr;

    switch (id)
    {
	case EFFECT_ID::DEAD:				effect = new CDead();			break;
	case EFFECT_ID::RUSH_CHARGE:		effect = new CRushCharge();		break;
	case EFFECT_ID::SEASON_CHANGE:		effect = new CSeasonChange();   break;
    }

    if (!effect) return;

    effect->Initialize(pos, color, rotation);
    m_EffectList.push_back(effect);
}

/*
 *  ガイドの生成
 */
void
CEffectManager::
Create(GUIDE_ID guideId, const vivid::Vector2& pos)
{
	IGuide* guide = nullptr;

    switch (guideId)
    {
    case GUIDE_ID::RUSH:  guide = new CRushGuide();   break;
	case GUIDE_ID::FIRE:  guide = new CFireGuide();   break;
    }

	if (!guide) return;

	guide->Initialize(pos);
    m_GuideList.push_back(guide);
}

/*
 *  エフェクトの削除
 */
void
CEffectManager::
EffectDelete(void)
{
	if (m_EffectList.empty()) return;

	EFFECT_LIST::iterator it = m_EffectList.begin();

	while (it != m_EffectList.end())
	{
		(*it)->SetActive(false);
	}
}

/*
 *  コンストラクタ
 */
CEffectManager::
CEffectManager(void)
{
}

/*
 *  コピーコンストラクタ
 */
CEffectManager::
CEffectManager(const CEffectManager& rhs)
{
	(void)rhs;
}

/*
 *  デストラクタ
 */
CEffectManager::
~CEffectManager(void)
{
}

/*
 *  代入演算子
 */
CEffectManager&
CEffectManager::
operator=(const CEffectManager& rhs)
{
	(void)rhs;
	return *this;
}
