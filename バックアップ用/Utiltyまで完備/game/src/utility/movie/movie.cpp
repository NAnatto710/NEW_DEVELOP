
/*!
 *  @file       movie.cpp
 *  @brief      動画再生Utility
 *  @author     Ryusei Shimizu
 *  @date       2026/09/16
 */

#include "movie.h"
#include "DxLib.h"

#include <string>

namespace
{
    int g_Handle = -1;
    std::string g_Path;
    bool g_Playing = false;

    /*!
     *  @brief  現在保持している動画ハンドルと再生状態を破棄
     *
     *  @note   Load()前の差し替え、Init()、Finalize()から共通で使用する
     */
    void
    ReleaseMovie()
    {
        if (g_Handle != -1)
        {
            DxLib::DeleteGraph(g_Handle);
            g_Handle = -1;
        }

        g_Path.clear();
        g_Playing = false;
    }
}

namespace Utility
{
    namespace Movie
    {
        /*
         *  Movieの状態を初期化する
         *  前回読み込んだ動画が残っている場合もここで解放する
         */
        void
        Init()
        {
            ReleaseMovie();
        }

        /*
         *  終了時にDxLibの動画ハンドルを解放する
         */
        void
        Finalize()
        {
            ReleaseMovie();
        }

        /*
         *  指定パスの動画をDxLibへ読み込み、再生できる状態まで準備する
         *  新しい動画を読む前に、以前の動画ハンドルは必ず解放する
         */
        bool
        Load(const std::string& path)
        {
            ReleaseMovie();

            if (path.empty()) return false;

            // 現在のプロジェクトはMultiByte設定なので、通常はchar版のパスをそのまま使用する
#ifndef UNICODE
            g_Handle = DxLib::OpenMovieToGraph(path.c_str(), 1);
#else
            // 将来UNICODE設定へ変更された場合はwstringへ変換して読み込む
            const std::wstring widePath(path.begin(), path.end());
            g_Handle = DxLib::OpenMovieToGraph(widePath.c_str(), 1);
#endif

            if (g_Handle == -1)
            {
                ReleaseMovie();
                return false;
            }

            g_Path = path;
            g_Playing = false;
            return true;
        }

        /*
         *  動画を読み込んでから再生するための簡易関数
         */
        bool
        Play(const std::string& path)
        {
            if (!Load(path)) return false;

            return Play();
        }

        /*
         *  Load()済みの動画をバックグラウンド再生する
         *  BACK再生にすることでゲームループを止めずに動画を進める
         */
        bool
        Play()
        {
            if (g_Handle == -1) return false;

            // ゲームループを停止させないよう、バックグラウンド再生を指定する
            DxLib::PlayMovieToGraph(g_Handle, DX_PLAYTYPE_BACK);
            g_Playing = true;
            return true;
        }

        /*
         *  再生中の動画を元サイズのまま指定座標へ描画する
         */
        void
        Draw(int x, int y)
        {
            if (!IsPlaying()) return;

            // 第4引数は透過フラグ。動画は通常不透明なので0を指定する
            DxLib::DrawGraph(x, y, g_Handle, 0);
        }

        /*
         *  再生中の動画を指定した幅・高さへ拡大縮小して描画する
         */
        void
        Draw(int x, int y, int width, int height)
        {
            if (!IsPlaying()) return;

            DxLib::DrawExtendGraph(x, y, x + width, y + height, g_Handle, 0);
        }

        /*
         *  動画を停止する
         */
        void
        Stop()
        {
            if (g_Handle == -1) return;

            // 再生位置を0へ戻し、次回Play()時に動画の先頭から再生できるようにする
            DxLib::SeekMovieToGraph(g_Handle, 0);
            g_Playing = false;
        }

        /*
         *  管理中フラグとDxLib側の再生状態を確認して、実際に再生中か判定
         */
        bool
        IsPlaying()
        {
            if (g_Handle == -1 || !g_Playing) return false;

            // 動画が最後まで再生された場合も検出できるよう、DxLib側の状態を確認する
            if (DxLib::GetMovieStateToGraph(g_Handle) != 1)
            {
                g_Playing = false;
                return false;
            }

            return true;
        }

        /*
         *  有効な動画ハンドルを保持しているか判定
         */
        bool
        IsLoaded()
        {
            return g_Handle != -1;
        }
    }
}
