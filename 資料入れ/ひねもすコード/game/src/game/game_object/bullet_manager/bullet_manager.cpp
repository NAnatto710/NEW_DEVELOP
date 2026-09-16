
/*!
 *  @file       bullet_manager.cpp
 *  @brief      射撃管理
 *  @author     Ryusei Shimizu
 *  @date       2025/12/18
 */

#include "bullet_manager.h"
#include "bullet_object/bullet_object.h"
#include "../character_manager/character_manager.h"

/*
 *  インスタンスの取得
 */
CBulletManager&
CBulletManager::
GetInstance(void)
{
    static CBulletManager instance;

    return instance;
}

/*
 *  初期化
 */
void
CBulletManager::
Initialize(void)
{
    // リストのクリア
    m_BulletList.clear();
}

/*
 *  更新
 */
void
CBulletManager::
Update(void)
{
    // リストが空なら終了
    if (m_BulletList.empty()) return;

    BULLET_LIST::iterator it = m_BulletList.begin();

    // 弾の更新
    while (it != m_BulletList.end())
    {
        IBullet* bullet = (IBullet*)(*it);

        bullet->Update();

        // キャラクターとの当たり判定
        CCharacterManager::GetInstance().CharacterCheckHitBullet(bullet);

        // 弾が非アクティブなら削除してリストから外す
        if (!bullet->IsActive())
        {
            bullet->Finalize();

            delete bullet;

            it = m_BulletList.erase(it);

            continue;
        }

        ++it;
    }

#ifdef VIVID_DEBUG

#endif
}

/*
 *  描画
 */
void
CBulletManager::
Draw(void)
{
    // リストが空なら終了
    if (m_BulletList.empty()) return;

    BULLET_LIST::iterator it = m_BulletList.begin();

    // 弾の描画
    while (it != m_BulletList.end())
    {
        (*it)->Draw();

        ++it;
    }

#ifdef VIVID_DEBUG

#endif
}

/*
 *  解放
 */
void
CBulletManager::
Finalize(void)
{
    // リストが空なら終了
    if (m_BulletList.empty()) return;

    BULLET_LIST::iterator it = m_BulletList.begin();

    // 弾の解放
    while (it != m_BulletList.end())
    {
        (*it)->Finalize();

        delete (*it);

        ++it;
    }

    m_BulletList.clear();
}

/*
 *  弾生成
 */
void
CBulletManager::
Create(CHARACTER_CATEGORY category, BULLET_ID id, const vivid::Vector2& pos, float dir, float damage, float speed, float duration)
{
    IBullet* bullet = nullptr;

	// 弾IDに応じた弾の生成
    switch (id)
    {
    case BULLET_ID::NOMAL_BULLET:   bullet = new CNomalBullet();        break;
	case BULLET_ID::BEE_BULLET:     bullet = new CBeeBullet();          break;
    default:                                                            break;
    }

    if (!bullet) return;

    bullet->Initialize(category, pos, dir, damage, speed, duration);

    // 生成した弾をリストに追加
    m_BulletList.push_back(bullet);
}

/*
 *  コンストラクタ
 */
CBulletManager::
CBulletManager(void)
{
}

/*
 *  コピーコンストラクタ
 */
CBulletManager::
CBulletManager(const CBulletManager& rhs)
{
    (void)rhs;
}

/*
 *  デストラクタ
 */
CBulletManager::
~CBulletManager(void)
{
}

/*
 *  代入演算子
 */
CBulletManager&
CBulletManager::
operator=(const CBulletManager& rhs)
{
    (void)rhs;

    return *this;
}