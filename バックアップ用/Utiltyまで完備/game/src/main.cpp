
/*!
 *  @file       main.cpp
 *  @brief      エントリーポイント
 *  @author     Kazuya Maruyama
 *				Toshihito Ohashi
 *  @date       2023/05/08
 *  @version    1.1
 *
 *  Copyright (c) 2013-2023, Kazuya Maruyama. Toshihito Ohashi. All rights reserved.
 */

#include "vivid.h"
#include "game\\game.h"

CGame game;

/*!
 *  @brief      描画関数
 */
void
Display(void)
{
    // ゲーム更新
    game.GameUpdate();

    // ゲーム描画
    game.GameDraw();
}

/*!
 *  @brief      メイン関数
 *
 *  @param[in]  hInst       インスタンスハンドル
 *  @param[in]  hPrevInst   Win16時代の遺物
 *  @param[in]  lpCmdLine   コマンドライン文字列
 *  @param[in]  nCmdShow    ウィンドの表示方法
 *
 *  @return     メイン関数の成否
 */
int WINAPI
WinMain( _In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ LPSTR lpCmdLine, _In_ int nCmdShow )
{
    (void)hPrevInst;
    (void)lpCmdLine;
    (void)nCmdShow;

    // vividライブラリ初期化
    vivid::Initialize( hInst );

    // ゲーム初期化
    game.GameInitialize();

    // 更新/描画関数登録
    vivid::DisplayFunction( Display );

    // ゲームループ
    vivid::MainLoop( );

    // ゲーム解放
    game.GameFinalize();

    // vividライブラリ解放
    vivid::Finalize( );
}
