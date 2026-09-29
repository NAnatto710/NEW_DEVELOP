
/*!
 *  @file       sound.h
 *  @brief      BGM / SE管理Utility
 *  @author     Ryusei Shimizu
 *  @date       2026/09/16
 */

#pragma once

#include "sound_id.h"
#include <string>

namespace Utility
{
    namespace Sound
    {
        /*!
         *  @brief  サウンド管理を初期化
         *
         *  @note   読み込み済みのBGM / SEがある場合は一度解放してから初期化する
         */
        void Init();

        /*!
         *  @brief  BGMのフェード処理を1フレーム進める
         *
         *  @note   FadeInBGM() / FadeOutBGM() を使用した場合は毎フレーム呼び出す
         */
        void Update();

        /*!
         *  @brief  読み込んだBGM / SEをすべて解放
         */
        void Finalize();

        /*!
         *  @brief  BGMファイルを読み込み、指定IDへ登録
         *
         *  @param[in]  id      登録するサウンドID
         *  @param[in]  path    BGMファイルのパス
         *
         *  @return 読み込みに成功した場合はtrue、失敗した場合はfalse
         */
        bool LoadBGM(ID id, const std::string& path);

        /*!
         *  @brief  SEファイルを読み込み、指定IDへ登録
         *
         *  @param[in]  id      登録するサウンドID
         *  @param[in]  path    SEファイルのパス
         *
         *  @return 読み込みに成功した場合はtrue、失敗した場合はfalse
         */
        bool LoadSE(ID id, const std::string& path);

        /*!
         *  @brief  登録済みBGMをループ再生
         *
         *  @param[in]  id  再生するBGMのID
         */
        void PlayBGM(ID id);

        /*!
         *  @brief  現在再生中のBGMを停止
         */
        void StopBGM();

        /*!
         *  @brief  登録済みSEを1回再生
         *
         *  @param[in]  id  再生するSEのID
         */
        void PlaySE(ID id);

        /*!
         *  @brief  BGM / SEの共通音量を設定
         *
         *  @param[in]  volume  音量（0～255）
         *
         *  @note   0で無音、255で最大音量
         */
        void Volume(int volume);

        /*!
         *  @brief  指定BGMを0音量から徐々にフェードイン
         *
         *  @param[in]  id      フェードインするBGMのID
         *  @param[in]  frames  フェードに使用するフレーム数
         */
        void FadeInBGM(ID id, int frames);

        /*!
         *  @brief  現在再生中のBGMを徐々にフェードアウト
         *
         *  @param[in]  frames  フェードに使用するフレーム数
         */
        void FadeOutBGM(int frames);

        /*!
         *  @brief  現在再生中として管理しているBGM IDを取得
         *
         *  @return 現在のBGM ID。再生していない場合はID::None
         */
        ID CurrentBGM();
    }
}
