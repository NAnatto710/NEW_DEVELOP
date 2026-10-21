
/*!
 *  @file       sound_manager.cpp
 *  @brief      サウンド管理
 *  @author     Ryusei Shimizu
 *  @date       2026/03/20
 */

#include "sound_manager.h"

/*
 *  インスタンス取得
 */
CSoundManager&
CSoundManager::
GetInstance()
{
	static CSoundManager instance;

	return instance;
}

/*
 *  初期化
 */
void
CSoundManager::
Initialize()
{
    // フェード制御初期化
    m_IsFading = false;           // フェード中かどうか
    m_IsFadeIn = false;             // true:フェードイン false:フェードアウト
    m_FadeTimer = 0;              // 経過フレーム
    m_FadeDuration = 0;           // フェード時間
    m_TargetVolume = 255;         // 目標音量（0～255）
    m_CurrentBGM = SOUND_ID::MAX; // 現在再生中のBGM

    m_BgmFiles =
    {
        {SOUND_ID::TITLE_BGM,"data\\sound\\title_bgm.mp3"},
        {SOUND_ID::TITLE_BGM,"data\\sound\\player_join_bgm.mp3"},
        {SOUND_ID::BUILD_SELECT_BGM,"data\\sound\\build_select_bgm.mp3"},
        {SOUND_ID::MAIN_BGM,"data\\sound\\main_bgm.mp3"},
        {SOUND_ID::RESULY_BGM,"data\\sound\\result_bgm.mp3"}
    };

    m_SeFiles =
    {
        {SOUND_ID::HIT,"data\\sound\\hit.wav"},
        {SOUND_ID::MISS,"data\\sound\\miss.wav"},
        {SOUND_ID::DROP,"data\\sound\\drop.wav"},
        {SOUND_ID::GUARD,"data\\sound\\guard.wav"},
        {SOUND_ID::GUARD_BREAK,"data\\sound\\guard_break.wav"},
        {SOUND_ID::JUMP,"data\\sound\\jump.wav"},
        {SOUND_ID::LANDING,"data\\sound\\landing.wav"},
        {SOUND_ID::RESPAWN,"data\\sound\\respawn"},
        {SOUND_ID::JOIN,"data\\sound\\join.wav"},
        {SOUND_ID::BUILD,"data\\sound\\build.wav"},
        {SOUND_ID::PUSH,"data\\sound\\push.wav"},
        {SOUND_ID::CANCEL,"data\\sound\\cancel.wav"},
        {SOUND_ID::START,"data\\sound\\start.wav"},
        {SOUND_ID::SELECT,"data\\sound\\select.wav"},
        {SOUND_ID::OK,"data\\sound\\ok.wav"}
    
    };

    // BGMを事前ロードしてハンドルを保持
    for (const auto& pair : m_BgmFiles)
    {
        // LoadSoundMemで音声を読み込み、ハンドルを取得
        int handle = LoadSoundMem(pair.second.c_str());

        // SOUND_ID と ハンドルを紐付け
        m_BgmHandles[pair.first] = handle;
    }

    // SEを事前ロードしてハンドルを保持
    for (const auto& pair : m_SeFiles)
    {
        int handle = LoadSoundMem(pair.second.c_str());
        m_SeHandles[pair.first] = handle;
    }
}

/*
 *  更新
 */
void
CSoundManager::
Update()
{
    // フェード中でなければ何もしない
    if (!m_IsFading) return;

    // 経過フレーム加算
    m_FadeTimer++;

    // 進行率（0.0～1.0）
    float rate = (float)m_FadeTimer / m_FadeDuration;
    if (rate > 1.0f) rate = 1.0f;

    if (m_CurrentBGM != SOUND_ID::MAX)
    {
        // 現在再生中BGMのハンドル取得
        int handle = m_BgmHandles[m_CurrentBGM];

        // フェードインなら徐々に上げる
        // フェードアウトなら徐々に下げる
        int volume = m_IsFadeIn
            ? static_cast<int>(m_TargetVolume * rate)
            : static_cast<int>(m_TargetVolume * (1.0f - rate));

        // DXLibで音量変更
        SetVolumeSoundMem(volume, handle);
    }

    // フェード終了判定
    if (m_FadeTimer >= m_FadeDuration)
    {
        // フェードアウト終了時は停止
        if (!m_IsFadeIn && m_CurrentBGM != SOUND_ID::MAX)
        {
            StopBGM();
        }

        m_IsFading = false; // フェード終了
    }
}

/*
 *  解放
 */
void
CSoundManager::
Finalize()
{
    // 読み込んだ音声を全て削除
    for (auto& pair : m_BgmHandles)
    {
        DeleteSoundMem(pair.second);
    }

    for (auto& pair : m_SeHandles)
    {
        DeleteSoundMem(pair.second);
    }
}

/*
 *  BGM再生（ループ）
 */
void
CSoundManager::
PlayBGM(SOUND_ID id)
{
    // 同じBGMなら何もしない
    if (m_CurrentBGM == id) return;

    // 既存BGM停止
    StopBGM();

    auto it = m_BgmHandles.find(id);
    if (it != m_BgmHandles.end())
    {
        // ループ再生
        PlaySoundMem(it->second, DX_PLAYTYPE_LOOP);

        // 現在音量設定
        SetVolumeSoundMem(m_TargetVolume, it->second);

        m_CurrentBGM = id;
    }
}

/*
 *  BGM停止
 */
void
CSoundManager::
StopBGM()
{
    if (m_CurrentBGM != SOUND_ID::MAX)
    {
        StopSoundMem(m_BgmHandles[m_CurrentBGM]);
        m_CurrentBGM = SOUND_ID::MAX;
    }
}

/*
 *  SE再生
 */
void
CSoundManager::
PlaySE(SOUND_ID id)
{
    auto it = m_SeHandles.find(id);

    if (it != m_SeHandles.end())
    {
        // 単発再生
        PlaySoundMem(it->second, DX_PLAYTYPE_BACK);
    }
}

/*
 *  ボリューム設定
 */
void
CSoundManager::
SetVolume(int volume)
{
    // 目標音量を更新
    m_TargetVolume = volume;

    // 現在再生中BGMがあれば即時反映
    if (m_CurrentBGM != SOUND_ID::MAX)
    {
        int handle = m_BgmHandles[m_CurrentBGM];
        SetVolumeSoundMem(volume, handle);
    }
}

/*
 *  BGMフェードイン
 */
void
CSoundManager::
FadeInBGM(SOUND_ID id, int duration)
{
    PlayBGM(id);

    m_IsFading = true;
    m_IsFadeIn = true;
    m_FadeTimer = 0;
    m_FadeDuration = duration;
    m_TargetVolume = 255;
    // 最初は音量0にしておく
    if (m_CurrentBGM != SOUND_ID::MAX)
    {
        int handle = m_BgmHandles[m_CurrentBGM];
        SetVolumeSoundMem(0, handle);
    }
}

/*
 *  BGMフェードアウト
 */
void
CSoundManager::
FadeOutBGM(int duration)
{
    if (m_CurrentBGM == SOUND_ID::MAX) return;

    m_IsFading = true;
    m_IsFadeIn = false;
    m_FadeTimer = 0;
    m_FadeDuration = duration;
    m_TargetVolume = 255;
}
