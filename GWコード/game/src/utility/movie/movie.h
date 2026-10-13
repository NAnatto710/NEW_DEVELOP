
/*!
 *  @file       movie.h
 *  @brief      動画再生Utility
 *  @author     Ryusei Shimizu
 *  @date       2026/09/16
 */

#pragma once

#include <string>

namespace Utility
{
    namespace Movie
    {
        /*!
         *  @brief  動画管理を初期化
         *
         *  @note   すでに動画が読み込まれている場合は解放して初期状態へ戻す
         */
        void Init();

        /*!
         *  @brief  読み込んだ動画を解放
         */
        void Finalize();

        /*!
         *  @brief  動画ファイルを読み込む
         *
         *  @param[in]  path    動画ファイルのパス
         *
         *  @return 読み込みに成功した場合はtrue、失敗した場合はfalse
         *
         *  @note   Load()だけでは再生を開始しない
         */
        bool Load(const std::string& path);

        /*!
         *  @brief  指定した動画を読み込んで、そのまま再生を開始
         *
         *  @param[in]  path    動画ファイルのパス
         *
         *  @return 再生開始に成功した場合はtrue、失敗した場合はfalse
         */
        bool Play(const std::string& path);

        /*!
         *  @brief  Load()で読み込み済みの動画を再生
         *
         *  @return 再生開始に成功した場合はtrue、動画が未読み込みの場合はfalse
         */
        bool Play();

        /*!
         *  @brief  再生中の動画を元サイズで描画
         *
         *  @param[in]  x   描画する左上X座標
         *  @param[in]  y   描画する左上Y座標
         */
        void Draw(int x, int y);

        /*!
         *  @brief  再生中の動画を指定サイズへ拡大縮小して描画
         *
         *  @param[in]  x       描画する左上X座標
         *  @param[in]  y       描画する左上Y座標
         *  @param[in]  width   描画幅
         *  @param[in]  height  描画高さ
         */
        void Draw(int x, int y, int width, int height);

        /*!
         *  @brief  動画の再生を停止し、再生位置を先頭へ戻す
         */
        void Stop();

        /*!
         *  @brief  現在動画が再生中か確認
         *
         *  @return 再生中の場合はtrue、それ以外はfalse
         */
        bool IsPlaying();

        /*!
         *  @brief  動画が読み込み済みか確認
         *
         *  @return 読み込み済みの場合はtrue、それ以外はfalse
         */
        bool IsLoaded();
    }
}
