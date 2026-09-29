#include "vivid.h"
#include "Game/Game.h"

Game game;

void
Display(void)
{
    game.Update();

    game.Draw();
}
int WINAPI
WinMain( _In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ LPSTR lpCmdLine, _In_ int nCmdShow )
{
    (void)hPrevInst;
    (void)lpCmdLine;
    (void)nCmdShow;

    // vividライブラリ初期化
    vivid::Initialize( hInst );

    game.Initialize();

    // 更新/描画関数登録
    vivid::DisplayFunction( Display );

    // ゲームループ
    vivid::MainLoop( );

    game.Finalize();

    // vividライブラリ解放
    vivid::Finalize( );
}
