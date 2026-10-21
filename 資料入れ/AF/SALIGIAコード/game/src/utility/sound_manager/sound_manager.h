
/*!
 *  @file       sound_manager.h
 *  @brief      サウンド管理
 *  @author     Ryusei Shimizu
 *  @date       2026/03/20
 */

#pragma once
#include <map>
#include <unordered_map>
#include <string>
#include "DxLib.h"
#include "sound_id.h"

 /*!
  *  @class     CSoundManager
  *
  *  @brief     サウンド管理クラス
  *
  *  @author    Ryusei Shimizu
  * 
  *  @date      2026/03/20
  */
class CSoundManager
{
public:

    /*!
     *  @brief      インスタンス取得
     *
     *  @return     インスタンス
     */
    static CSoundManager& GetInstance();

    /*!
     *  @brief      初期化
     */
    void Initialize();

    /*!
     *  @brief      更新
     */
    void Update();

    /*!
     *  @brief      解放
     */
    void Finalize();

    /*!
     *  @brief              BGM再生（ループ）
     *
     *  @param[in]  id      BGMのSOUND_ID
     */
    void PlayBGM(SOUND_ID id);

    /*!
     *  @brief      BGM停止
     */
    void StopBGM();

    /*!
     *  @brief      SE再生
     *
     *  @param[in]  id      SEのSOUND_ID
     */
    void PlaySE(SOUND_ID id);

    /*!
     *  @brief      音量設定
     *
     *  @param[in]  volume  音量(0-255)
     */
    void SetVolume(int volume);

    /*!
     *  @brief      BGMフェードイン再生
     *
     *  @param[in]  id       再生するBGMのSOUND_ID
     *  @param[in]  duration フェード時間（フレーム数）
     */
    void FadeInBGM(SOUND_ID id, int duration);

    /*!
     *  @brief      BGMフェードアウト
     *
     *  @param[in]  duration  フェード時間（フレーム数）
     */
    void FadeOutBGM(int duration);

private:
    std::map<SOUND_ID, std::string>     m_BgmFiles;     //!< BGMファイル名管理
    std::map<SOUND_ID, std::string>     m_SeFiles;      //!< SEファイル名管理

    std::unordered_map<SOUND_ID, int>   m_BgmHandles;   //!< BGMのサウンドハンドル
    std::unordered_map<SOUND_ID, int>   m_SeHandles;    //!< SEのサウンドハンドル

    SOUND_ID    m_CurrentBGM;       //!< 現在再生中のBGM

    bool        m_IsFading;         //!< フェード処理中フラグ
    bool        m_IsFadeIn;         //!< フェードイン中かどうか
    int         m_FadeTimer;        //!< フェード経過フレーム数
    int         m_FadeDuration;     //!< フェード完了までのフレーム数
    int         m_TargetVolume;     //!< フェード目標音量（通常255）

    /* 以下コンストラクタ類 */
    CSoundManager() = default;
    ~CSoundManager() = default;
    CSoundManager(const CSoundManager& rhs) = delete;
    CSoundManager& operator=(const CSoundManager& rhs) = delete;
};