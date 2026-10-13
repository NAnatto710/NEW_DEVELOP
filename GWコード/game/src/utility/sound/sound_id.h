
/*!
 *  @file       sound_id.h
 *  @brief      サウンドID定義
 *  @author     Ryusei Shimizu
 *  @date       2026/09/16
 */

#pragma once

namespace Utility
{
    namespace Sound
    {
        enum class ID
        {
            None = -1,

            //================================================
            // ゲームで使用するBGM / SEのIDをここへ追加する
            // Sound::LoadBGM() / LoadSE()で、このIDとファイルを対応付けて使用する
            //================================================
            // 例:
            // TitleBGM,
            // MainBGM,
            // HitSE,
            // JumpSE,

            Max
        };
    }
}
