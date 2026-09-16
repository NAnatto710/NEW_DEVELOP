/*!
 *  @file       CSoundManager.h
 *  @brief      サウンド管理
 *  @author    Misaki Kawada
 *  @date       2026/02/26
 */

#pragma once

#include"vivid.h"


 /*!
  *  @class      CSoundManager
  *
  *  @brief      サウンド管理クラス
  *
  *  @author     Misaki Kawada
  *
  *  @date        2026/02/26
  */


  /*
   * @breif サウンドID
   */

enum class SOUND_ID
{
    TITLE,                  //タイトル
    GAMEMAIN_BGM,           //ゲームメインBGM
    THREAD_ATTACK,          //糸攻撃
    THREAD_ATTACK_DAMAGE,   //糸攻撃ヒット音
    SUBSIST,                //餌を食べる
    SELECT_CHARGE,          //セレクトチェンジ
    CLICK,                  //クリック音
    CHARGE,                 //突進チャージ
    RUSH,                   //突進
    BUFF_UPGRADE,           //バフ強化
    PLAYER_DAMAGE,          //プレイヤーダメージ音
    ENEMY_RUSH_DAMAGE,      //敵突進ダメージ音
    RESULT,                 //リザルト
    MAX,                    //最大
};



class CSoundManager
{
public:

    /*!
     *  @brief      インスタンスの取得
     *
     *  @return     インスタンス
     */
    static CSoundManager& GetInstance(void);

    void Initialize(void);

    void Finalize(void);

    void Play(SOUND_ID id, bool loop);

    void Stop(SOUND_ID id);

    void SetVolume(SOUND_ID id, int volume);


private:

    static const std::string   m_file_path[(int)SOUND_ID::MAX];

    int                        m_SetVolume;

    /*!
   *  @brief      コンストラクタ
   */
    CSoundManager(void);

    /*!
     *  @brief      コピーコンストラクタ
     *
     *  @param[in]  rhs     オブジェクト
     */
    CSoundManager(const CSoundManager& rhs);

    /*!
     *  @brief      ムーブコンストラクタ
     *
     *  @param[in]  rhs     オブジェクト
     */
    CSoundManager(CSoundManager&& rhs);

    /*!
     *  @brief      デストラクタ
     */
    ~CSoundManager(void);

    /*!
     *  @brief      代入演算子
     *
     *  @param[in]  rhs 代入オブジェクト
     *
     *  @return     自身のオブジェクト
     */
    CSoundManager& operator=(const CSoundManager& rhs);
};