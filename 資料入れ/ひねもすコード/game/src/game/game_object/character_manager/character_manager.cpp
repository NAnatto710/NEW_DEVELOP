
/*!
 *  @file       character_manager.cpp
 *  @brief      キャラクター管理
 *  @author     Ryusei Shimizu
 *  @date       2025/10/15
 */

#include "character_manager.h"
#include "../stage_manager/stage_manager.h"
#include "../camera_manager/camera_manager.h"
#include "character/character_object.h"

const float CCharacterManager::m_dummy_length = 999999.0f;                      //!< 最短距離計測用ダミーデータ
const int   CCharacterManager::m_max_enemy_count[(int)SEASON_ID::MAX] =         //!< 最大エネミー数
{
    5,      //!< 冬
    9,      //!< 春
    13,     //!< 夏
    15      //!< 秋
};

/*
 *  インスタンスの取得
 */
CCharacterManager&
CCharacterManager::
GetInstance(void)
{
    static CCharacterManager instance;

    return instance;
}

/*
 *  初期化
 */
void
CCharacterManager::
Initialize(void)
{
    // リストの初期化
    m_CharacterList.clear();
    m_PlayerList.clear();
    m_EnemyList.clear();

    CStageManager& sm = CStageManager::GetInstance();

    // プレイヤー作成
    this->Create(CHARACTER_ID::PLAYER, sm.GetStartBlockPosition());
}

/*
 *  更新
 */
void
CCharacterManager::
Update(void)
{
    // キャラクター更新
    UpdateCharacter();
}

/*
 *  描画
 */
void
CCharacterManager::
Draw(void)
{
    // キャラクターリストが空なら処理を行わない
    if (m_CharacterList.empty()) return;

    CHARACTER_LIST::iterator it = m_CharacterList.begin();

    while (it != m_CharacterList.end())
    {
        (*it)->Draw();
        ++it;
    }
}

/*
 *  解放
 */
void
CCharacterManager::
Finalize(void)
{
    if (!m_CharacterList.empty())
    {
        CHARACTER_LIST::iterator it = m_CharacterList.begin();

        while (it != m_CharacterList.end())
        {
            (*it)->Finalize();
            delete (*it);
            ++it;
        }

        // キャラクターリストを初期化する
        m_CharacterList.clear();
    }

	if (!m_PlayerList.empty())
	{
		// プレイヤーリストを初期化する
		m_PlayerList.clear();
	}

	if (!m_EnemyList.empty())
	{
        // エネミーリストを初期化する
        m_EnemyList.clear();
	}
}

/*
 *  キャラクター生成
 */
void
CCharacterManager::
Create(CHARACTER_ID id, const vivid::Vector2& pos)
{
    CGameParameterManager& pm = CGameParameterManager::GetInstance();
    SEASON_ID season_id = pm.GetSeasonId();

    // エネミーキャラクターは、季節ごとの最大エネミー数を超えて生成しない
    if (id != CHARACTER_ID::PLAYER && m_EnemyList.size() >= m_max_enemy_count[(int)season_id])return;

    ICharacter* unit = nullptr;

    // 引数ごとに生成するキャラクターを区別する
    switch (id)
    {
    case CHARACTER_ID::PLAYER:
        unit = new CPlayer();
        CCameraManager::GetInstance().SetPlayer((CPlayer*)unit);
        break;

    case CHARACTER_ID::CATERPILLAR:     unit = new CCaterpillar(); break;
    case CHARACTER_ID::CENTIPEDE:       unit = new CCentipede(); break;
    case CHARACTER_ID::BEE:             unit = new CBee(); break;
    }
    if (!unit) return;

    unit->Initialize(pos);

    // 作成したキャラクターをリストに追加する
    m_CharacterList.push_back(unit);

    if (unit->GetCharacterCategory() == CHARACTER_CATEGORY::PLAYER)
    {
        m_PlayerList.push_back(unit);
    }

    if (unit->GetCharacterCategory() == CHARACTER_CATEGORY::ENEMY)
    {
        m_EnemyList.push_back(unit);
    }
}

/*
 *  エネミーの削除
 */
void
CCharacterManager::
EnemyDelete(void)
{
    // キャラクターリストが空なら処理を行わない
    if (m_CharacterList.empty()) return;

	CHARACTER_LIST::iterator it = m_CharacterList.begin();
    while (it != m_CharacterList.end())
    {
        // リストからプレイヤークラスを取り出す
        if ((*it)->GetCharacterCategory() == CHARACTER_CATEGORY::ENEMY)
			(*it)->SetActive(false);

        ++it;
    }
}

/*
 *  プレイヤー取得
 */
ICharacter*
CCharacterManager::
GetPlayer(void)
{
    // キャラクターリストが空なら処理を行わない
    if (m_CharacterList.empty()) return nullptr;

    CHARACTER_LIST::iterator it = m_CharacterList.begin();
    while (it != m_CharacterList.end())
    {
        // リストからプレイヤークラスを取り出す
        if ((*it)->GetCharacterID() == CHARACTER_ID::PLAYER)
            return (*it);
        ++it;
    }
    // 見つからなければ空を返す
    return nullptr;
}

/*
 *  一番近いプレイヤーを取得
 */
ICharacter*
CCharacterManager::
FindNearPlayer(ICharacter* enemy)
{
    // プレイヤーリストが空なら処理を行わない
    if (m_PlayerList.empty())return nullptr;

    CHARACTER_LIST::iterator it = m_PlayerList.begin();

    // 最短距離計測用変数
    vivid::Vector2 v;
    float min = m_dummy_length;

    ICharacter* player = nullptr;

    while (it != m_PlayerList.end())
    {
        // 敵キャラクターとプレイヤーの距離ベクトルを計算
        v = (*it)->GetCenterPosition() - enemy->GetCenterPosition();

        // 最短距離のプレイヤーを取得
        if (v.Length() <= min)
        {
            min = v.Length();
            player = (*it);
        }

        ++it;

    }
    return player;

}

/*
 *	弾との判定
 */
void
CCharacterManager::
CharacterCheckHitBullet(IBullet* bullet)
{
    if (m_CharacterList.empty())return;

    CHARACTER_LIST::iterator it = m_CharacterList.begin();

    while (it != m_CharacterList.end())
    {
        // 弾とキャラクターの当たり判定
        if (((*it)->CheckHitBullet(bullet)))
            return;
        ++it;
    }
}

/*
 *  ラッシュ攻撃の当たり判定
 */
void
CCharacterManager::
RushHit(void)
{
    if (m_EnemyList.empty())return;

    ENEMY_LIST::iterator enemy_it = m_EnemyList.begin();

    while (enemy_it != m_EnemyList.end())
    {
        ICharacter* enemy = (ICharacter*)(*enemy_it);
        CPlayer* player = (CPlayer*)this->GetPlayer();

        if (player && player->IsControlFlg() == true)
            if (enemy->RushCheckHit(player))
                return;

        ++enemy_it;
    }
}

/*
 *  キャラクターどうしの当たり判定
 */
void
CCharacterManager::
CharacterHit(void)
{
    if (m_EnemyList.empty())return;

    ENEMY_LIST::iterator enemy_it = m_EnemyList.begin();

    while (enemy_it != m_EnemyList.end())
    {
        ICharacter* enemy = (ICharacter*)(*enemy_it);
        CPlayer* player = (CPlayer*)this->GetPlayer();

        // 敵とプレイヤーの当たり判定
        if (player)
            if (player->Hit(enemy))
                return;

        ++enemy_it;
    }
}

std::list<ICharacter*>
CCharacterManager::
GetEnemyList(void)const
{
    return m_EnemyList;
}

/*
 *  コンストラクタ
 */
CCharacterManager::
CCharacterManager(void)
{
}

/*
 *  コピーコンストラクタ
 */
CCharacterManager::
CCharacterManager(const CCharacterManager& rhs)
{
    (void)rhs;
}

/*
 *  デストラクタ
 */
CCharacterManager::
~CCharacterManager(void)
{
}

/*
 *  代入演算子
 */
CCharacterManager&
CCharacterManager::
operator=(const CCharacterManager& rhs)
{
    (void)rhs;
    return *this;
}

/*
 *  キャラクター更新
 */
void
CCharacterManager::
UpdateCharacter(void)
{
    // キャラクターリストが空なら処理を行わない
    if (m_CharacterList.empty()) return;

    CHARACTER_LIST::iterator it = m_CharacterList.begin();
    PLAYER_LIST::iterator player_it = m_PlayerList.begin();
    ENEMY_LIST::iterator enemy_it = m_EnemyList.begin();

    while (it != m_CharacterList.end())
    {
        ICharacter* unit = (ICharacter*)(*it);
        // 各キャラクターでUpdateを行う
        unit->Update();

        // キャラクターどうしの当たり判定
        this->CharacterHit();

		// ラッシュ攻撃の当たり判定
        this->RushHit();

        // アクティブフラグが無効なら処理を行う
        if (!unit->IsActive())
        {
            unit->Finalize();

            // プレイヤーキャラクターが死亡した場合、プレイヤーリストから削除する
            if (unit->GetCharacterCategory() == CHARACTER_CATEGORY::PLAYER)
            {
                player_it = std::find(m_PlayerList.begin(), m_PlayerList.end(), unit);
                if (player_it != m_PlayerList.end())
                    m_PlayerList.erase(player_it);
            }

            // エネミーキャラクターが死亡した場合、エネミーリストから削除する
            if (unit->GetCharacterCategory() == CHARACTER_CATEGORY::ENEMY)
            {
                enemy_it = std::find(m_EnemyList.begin(), m_EnemyList.end(), unit);
                if (enemy_it != m_EnemyList.end())
                    m_EnemyList.erase(enemy_it);
            }

            delete unit;
            it = m_CharacterList.erase(it);
            continue;
        }
        ++it;
    }
}
