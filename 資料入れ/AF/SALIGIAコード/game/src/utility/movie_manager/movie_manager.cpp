
/*!
 *  @file       movie_manager.h
 *  @brief      動画再生
 *  @author     Ryusei Shimizu
 *  @date       2026/03/20
 */

#include "movie_manager.h"
#include "DxLib.h"

 /*
  *  インスタンス取得
  */
CMovieManager&
CMovieManager::
GetInstance()
{
    static CMovieManager instance;
    return instance;
}

/*
 *  初期化
 */
void
CMovieManager::
Initialize()
{
    // DXライブラリ初期化
    m_Handle = -1;
    m_IsPlaying = false;
}

/*
 *  解放
 */
void
CMovieManager::
Finalize()
{
    // 動画ハンドルが有効なら解放
    if (m_Handle != -1)
    {
        DeleteGraph(m_Handle);
        m_Handle = -1;
    }

    // DXライブラリ終了
}

/*
 *  動画をロード
 */
bool
CMovieManager::
Load(const std::string& filepass)
{
    // 既存の動画を解放
    if (m_Handle != -1)
    {
        DeleteGraph(m_Handle);
        m_Handle = -1;
    }

#ifndef UNICODE
    // ファイル名から動画をロード
    m_Handle = OpenMovieToGraph(filepass.c_str(), TRUE);
#else
    // UNICODEの場合はwstringに変換してロード
    std::wstring wfilepass(filepass.begin(), filepass.end());
    m_Handle = OpenMovieToGraph(wfilepass.c_str(), TRUE);
#endif

    // ロード失敗時
    if (m_Handle == -1)
    {
        m_IsPlaying = false;
        return false;
    }

    m_FilePass = filepass;
    m_IsPlaying = false; // 再生はしない
    return true;
}

/*
 *  動画を再生
 */
bool
CMovieManager::
Play(const std::string& filepass)
{
    // 既存の動画を解放
    if (m_Handle != -1)
    {
        DeleteGraph(m_Handle);
        m_Handle = -1;
    }

#ifndef UNICODE
    // ファイル名から動画をロード
    m_Handle = OpenMovieToGraph(filepass.c_str(), TRUE);
    // ロード成功時に再生開始
    if (m_Handle != -1) PlayMovieToGraph(m_Handle, DX_PLAYTYPE_BACK);
#else
    // UNICODEの場合はwstringに変換してロード
    std::wstring wfilepass(filepass.begin(), filepass.end());
    m_Handle = OpenMovieToGraph(wfilepass.c_str(), TRUE);
    if (m_Handle != -1) PlayMovieToGraph(m_Handle, DX_PLAYTYPE_BACK);
#endif

    // ロードまたは再生失敗時
    if (m_Handle == -1)
    {
        m_IsPlaying = false;
        return false;
    }

    m_FilePass = filepass;
    m_IsPlaying = true;
    return true;
}

/*
 *  動画を指定場所に描画
 */
void
CMovieManager::
Display(int x, int y)
{
    if (m_Handle != -1 && m_IsPlaying)
    {
        // 再生中なら描画
        if (GetMovieStateToGraph(m_Handle) == 1)
        {
            DrawGraph(x, y, m_Handle, FALSE);
        }
        else {
            // 再生が終わったらフラグを下げる
            m_IsPlaying = false;
        }
    }
}

void
CMovieManager::
Display(int x, int y, int width, int height)
{
    if (m_Handle != -1 && m_IsPlaying)
    {
        if (GetMovieStateToGraph(m_Handle) == 1)
        {
            DrawExtendGraph(x, y, x + width, y + height, m_Handle, FALSE);
        }
        else
        {
            m_IsPlaying = false;
        }
    }
}

/*
 *  動画再生停止
 */
void
CMovieManager::
Stop()
{
    if (m_Handle != -1)
    {
        SeekMovieToGraph(m_Handle, 0); // 先頭にシーク
        m_IsPlaying = false;
    }
}

/*
 *  動画再生中かどうか
 */
bool
CMovieManager::
IsPlaying() const
{
    return m_IsPlaying;
}