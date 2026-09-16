
/*!
 *  @file       sound.cpp
 *  @brief      BGM / SE管理Utility
 *  @author     Ryusei Shimizu
 *  @date       2026/09/16
 */

#include <map>
#include <string>

#include "sound.h"
#include "DxLib.h"

namespace
{
    typedef Utility::Sound::ID SoundID;

    //========================================================
    // 読み込んだサウンドの管理情報
    // IDをキーにしてDxLibのサウンドハンドルを保持する
    //========================================================
    std::map<SoundID, int> g_BGM;
    std::map<SoundID, int> g_SE;

    SoundID g_CurrentBGM = SoundID::None;

    // DxLibの音量は0～255なので、Utility側でも同じ範囲で管理する
    int g_Volume = 255;

    //========================================================
    // BGMフェード処理で使用する状態
    // Update()を呼ぶたびにg_FadeFrameを進めて音量を補間する
    //========================================================
    bool g_Fading = false;
    bool g_FadeIn = false;
    int g_FadeFrame = 0;
    int g_FadeFrames = 0;
    int g_FadeStartVolume = 0;
    int g_FadeEndVolume = 0;

    /*!
     *  @brief  音量をDxLibで使用できる0～255の範囲へ制限
     */
    int ClampVolume(int volume)
    {
        if (volume < 0) return 0;
        if (volume > 255) return 255;
        return volume;
    }

    /*!
     *  @brief  指定IDに対応するBGMハンドルを検索
     *
     *  @return 登録済みならDxLibハンドル、未登録なら-1
     */
    int FindBGM(SoundID id)
    {
        std::map<SoundID, int>::const_iterator it = g_BGM.find(id);
        return (it == g_BGM.end()) ? -1 : it->second;
    }

    /*!
     *  @brief  指定IDに対応するSEハンドルを検索
     *
     *  @return 登録済みならDxLibハンドル、未登録なら-1
     */
    int FindSE(SoundID id)
    {
        std::map<SoundID, int>::const_iterator it = g_SE.find(id);
        return (it == g_SE.end()) ? -1 : it->second;
    }

    /*!
     *  @brief  現在再生中として管理しているBGMだけ音量を変更
     */
    void SetCurrentBGMVolume(int volume)
    {
        const int handle = FindBGM(g_CurrentBGM);
        if (handle == -1) return;

        DxLib::SetVolumeSoundMem(ClampVolume(volume), handle);
    }

    /*!
     *  @brief  Mapに登録されているDxLibサウンドハンドルをすべて解放
     */
    void ReleaseSounds(std::map<SoundID, int>& sounds)
    {
        for (std::map<SoundID, int>::iterator it = sounds.begin(); it != sounds.end(); ++it)
        {
            if (it->second != -1)
            {
                DxLib::DeleteSoundMem(it->second);
            }
        }
        sounds.clear();
    }
}

namespace Utility
{
    namespace Sound
    {
        /*
         *  Sound管理を初期状態へ戻す
         *  再初期化でもハンドルが残らないよう、最初にFinalize()で既存データを解放する
         */
        void Init()
        {
            // 再初期化時に以前のサウンドハンドルが残らないよう、先に全データを解放する
            Finalize();

            g_CurrentBGM = ID::None;
            g_Volume = 255;

            g_Fading = false;
            g_FadeIn = false;
            g_FadeFrame = 0;
            g_FadeFrames = 0;
            g_FadeStartVolume = 0;
            g_FadeEndVolume = 0;
        }

        /*
         *  BGMフェードを1フレーム進める
         *  開始音量から終了音量まで、経過フレームの割合で線形補間する
         */
        void Update()
        {
            if (!g_Fading) return;

            if (g_FadeFrames <= 0)
            {
                g_Fading = false;
                return;
            }

            ++g_FadeFrame;

            float rate = static_cast<float>(g_FadeFrame) / static_cast<float>(g_FadeFrames);
            if (rate < 0.0f) rate = 0.0f;
            if (rate > 1.0f) rate = 1.0f;

            const int volume = static_cast<int>(
                g_FadeStartVolume + (g_FadeEndVolume - g_FadeStartVolume) * rate);

            SetCurrentBGMVolume(volume);

            if (g_FadeFrame >= g_FadeFrames)
            {
                g_Fading = false;

                if (g_FadeIn)
                {
                    SetCurrentBGMVolume(g_Volume);
                }
                else
                {
                    StopBGM();
                }
            }
        }

        /*
         *  再生中BGMを停止し、読み込んだBGM / SEのDxLibハンドルをすべて解放する
         */
        void Finalize()
        {
            StopBGM();
            ReleaseSounds(g_BGM);
            ReleaseSounds(g_SE);

            g_CurrentBGM = ID::None;
            g_Fading = false;
        }

        /*
         *  BGMファイルをDxLibで読み込み、IDとハンドルを対応付けて保存する
         *  同じIDがすでに登録されている場合は古いハンドルを先に解放する
         */
        bool LoadBGM(ID id, const std::string& path)
        {
            if (id == ID::None || id == ID::Max || path.empty())
            {
                return false;
            }

            // 同じIDを再登録する場合は、メモリリークを防ぐため古いハンドルを先に破棄する
            std::map<SoundID, int>::iterator old = g_BGM.find(id);
            if (old != g_BGM.end())
            {
                if (old->second != -1) DxLib::DeleteSoundMem(old->second);
                g_BGM.erase(old);
            }

            const int handle = DxLib::LoadSoundMem(path.c_str());
            if (handle == -1)
            {
                return false;
            }

            DxLib::SetVolumeSoundMem(g_Volume, handle);
            g_BGM[id] = handle;
            return true;
        }

        /*
         *  SEファイルをDxLibで読み込み、IDとハンドルを対応付けて保存する
         *  同じIDがすでに登録されている場合は古いハンドルを先に解放する
         */
        bool LoadSE(ID id, const std::string& path)
        {
            if (id == ID::None || id == ID::Max || path.empty())
            {
                return false;
            }

            std::map<SoundID, int>::iterator old = g_SE.find(id);
            if (old != g_SE.end())
            {
                if (old->second != -1) DxLib::DeleteSoundMem(old->second);
                g_SE.erase(old);
            }

            const int handle = DxLib::LoadSoundMem(path.c_str());
            if (handle == -1)
            {
                return false;
            }

            DxLib::SetVolumeSoundMem(g_Volume, handle);
            g_SE[id] = handle;
            return true;
        }

        /*
         *  指定IDのBGMをループ再生する
         *  別のBGMが再生中なら停止してから切り替える
         */
        void PlayBGM(ID id)
        {
            const int handle = FindBGM(id);
            if (handle == -1) return;

            // すでに同じBGMを再生している場合は、先頭から再生し直さずそのまま継続する
            if (g_CurrentBGM == id) return;

            StopBGM();

            DxLib::SetVolumeSoundMem(g_Volume, handle);
            DxLib::PlaySoundMem(handle, DX_PLAYTYPE_LOOP);
            g_CurrentBGM = id;
        }

        /*
         *  現在再生しているBGMを停止し、管理中のBGM IDとフェード状態をリセットする
         */
        void StopBGM()
        {
            const int handle = FindBGM(g_CurrentBGM);
            if (handle != -1)
            {
                DxLib::StopSoundMem(handle);
            }

            g_CurrentBGM = ID::None;
            g_Fading = false;
        }

        /*
         *  指定IDのSEを1回だけ再生する
         *  BGMとは別管理なので、現在のBGM再生状態には影響しない
         */
        void PlaySE(ID id)
        {
            const int handle = FindSE(id);
            if (handle == -1) return;

            DxLib::SetVolumeSoundMem(g_Volume, handle);
            DxLib::PlaySoundMem(handle, DX_PLAYTYPE_BACK);
        }

        /*
         *  共通音量を更新し、すでに読み込み済みのBGM / SEすべてへ即時反映する
         */
        void Volume(int volume)
        {
            g_Volume = ClampVolume(volume);

            // 変更した共通音量を、すでに読み込まれている全BGM / SEへ反映する
            for (std::map<SoundID, int>::iterator it = g_BGM.begin(); it != g_BGM.end(); ++it)
            {
                if (it->second != -1) DxLib::SetVolumeSoundMem(g_Volume, it->second);
            }

            for (std::map<SoundID, int>::iterator it = g_SE.begin(); it != g_SE.end(); ++it)
            {
                if (it->second != -1) DxLib::SetVolumeSoundMem(g_Volume, it->second);
            }
        }

        /*
         *  指定BGMを音量0で再生開始し、framesフレームかけて通常音量まで上げる
         */
        void FadeInBGM(ID id, int frames)
        {
            if (FindBGM(id) == -1) return;

            if (frames <= 0)
            {
                PlayBGM(id);
                return;
            }

            PlayBGM(id);
            SetCurrentBGMVolume(0);

            g_Fading = true;
            g_FadeIn = true;
            g_FadeFrame = 0;
            g_FadeFrames = frames;
            g_FadeStartVolume = 0;
            g_FadeEndVolume = g_Volume;
        }

        /*
         *  現在のBGMをframesフレームかけて音量0まで下げ、完了後に停止する
         */
        void FadeOutBGM(int frames)
        {
            if (g_CurrentBGM == ID::None) return;

            if (frames <= 0)
            {
                StopBGM();
                return;
            }

            g_Fading = true;
            g_FadeIn = false;
            g_FadeFrame = 0;
            g_FadeFrames = frames;
            g_FadeStartVolume = g_Volume;
            g_FadeEndVolume = 0;
        }

        /*
         *  現在再生中として管理しているBGM IDを返す
         */
        ID CurrentBGM()
        {
            return g_CurrentBGM;
        }
    }
}
