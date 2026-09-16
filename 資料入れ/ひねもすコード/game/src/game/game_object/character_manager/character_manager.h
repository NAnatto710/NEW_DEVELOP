
/*!
 *  @file       character_manager.h
 *  @brief      キャラクター管理
 *  @author     Ryusei Shimizu
 *  @date       2025/10/15
 */

#pragma once

#include "vivid.h"
#include "character/character.h"
#include "character_id.h"
#include "../parameter_manager/parameter_manager.h"
#include <list>

/*!
 *  @class      CCharacterManager
 *
 *  @brief      キャラクター管理クラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2025/10/15
 */
class CCharacterManager
{
public:

    /*!
      *  @brief      インスタンスの取得
      *
      *  @return     インスタンス
      */
    static CCharacterManager&   GetInstance(void);

    /*!
     *  @brief      初期化
     */
    void                        Initialize(void);

    /*!
     *  @brief      更新
     */
    void                        Update(void);

    /*!
     *  @brief      描画
     */
    void                        Draw(void);

    /*!
     *  @brief      解放
     */
    void                        Finalize(void);

    /*!
     *  @brief      キャラクター生成
     *
     *  @param[in]  id      キャラクターID
     *  @param[in]  pos     生成位置
     */
    void                        Create(CHARACTER_ID id, const vivid::Vector2& pos);

	/*!
	 *  @brief      エネミーの削除
	 */
	void                        EnemyDelete(void);

    /*!
     *  @brief      プレイヤーの取得
     *
     *  @return     キャラクタークラス
     */
    ICharacter*                 GetPlayer(void);

    /*!
     *  @brief      プレイヤーの取得
     *
     *  @return     キャラクタークラス
     */
    ICharacter*                 FindNearPlayer(ICharacter* enemy);

    /*
     *  @breif      弾との判定
     */
    void                        CharacterCheckHitBullet(IBullet* bullet);

    /*
     *  @brief       ラッシュした時の判定
     */
    void                        RushHit(void);

    /*
     *  @brief      キャラクターどうしの当たり判定
     */
    void                        CharacterHit(void);

	/*!
	  *  @brief      エネミーリストの取得
	  *
	  *  @return     エネミーリスト
	  */
    std::list<ICharacter*>       GetEnemyList(void)const;

private:

    /*!
     *  @brief      コンストラクタ
     */
    CCharacterManager(void);

    /*!
     *  @brief      コピーコンストラクタ
     *
     *  @param[in]  rhs     オブジェクト
     */
    CCharacterManager(const CCharacterManager& rhs);

    /*!
     *  @brief      ムーブコンストラクタ
     *
     *  @param[in]  rhs     オブジェクト
     */
    CCharacterManager(CCharacterManager&& rhs);

    /*!
     *  @brief      デストラクタ
     */
    ~CCharacterManager(void);

    /*!
     *  @brief      代入演算子
     *
     *  @param[in]  rhs 代入オブジェクト
     *
     *  @return     自身のオブジェクト
     */
    CCharacterManager& operator=(const CCharacterManager& rhs);


    /*!
     *  @brief      全キャラクターの更新
     */
    void        UpdateCharacter(void);


    static const float m_dummy_length;		                        //最短距離計測用ダミーデータ
    static const int   m_max_enemy_count[(int)SEASON_ID::MAX];		//最大エネミー数

    /*!
     *  @brief      キャラクターリスト型
     */
    using CHARACTER_LIST = std::list<ICharacter*>;

    /*!
     *  @brief      プレイヤーリスト型
     */
    using PLAYER_LIST = std::list<ICharacter*>;

    /*!
     *  @brief      エネミーリスト型
     */
    using ENEMY_LIST = std::list<ICharacter*>;

    CHARACTER_LIST          m_CharacterList;    //!< キャラクターリスト

    PLAYER_LIST             m_PlayerList;	    //!< プレイヤーリスト

    ENEMY_LIST              m_EnemyList;        //!< エネミーリスト
};