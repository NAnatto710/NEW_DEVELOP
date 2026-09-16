
/*!
 *  @file       scene_manager.cpp
 *  @brief      シーン管理
 *  @author     Ryusei Shimizu
 *  @date       2026/04/10
 */

#include "scene_manager.h"
#include "scene/scene_object.h"

const int               CSceneManager::m_fade_speed     = 5;                    //!< フェード速度
const vivid::Vector2    CSceneManager::m_fade_position  = { 0.0f, 0.0f };       //!< フェード表示位置
const unsigned int      CSceneManager::m_fade_color     = 0xff000000;           //!< フェード色
const int               CSceneManager::m_min_fade_alpha = 0;                    //!< フェード用アルファの最小値
const int               CSceneManager::m_max_fade_alpha = 255;                  //!< フェード用アルファの最大値

/*
 *  インスタンスの取得
 */
CSceneManager&
CSceneManager::
GetInstance(void)
{
    static CSceneManager instance;

    return instance;
}

/*
 *  初期化
 */
void
CSceneManager::
Initialize(void)
{
    m_MainScene = nullptr;
    m_SubScene = nullptr;
    m_CurrentMainSceneID = MAINSCENE_ID::NONE;
    m_NextMainSceneID = MAINSCENE_ID::TITLE;
    m_CurrentSubSceneID = SUBSCENE_ID::NONE;
    m_NextSubSceneID = SUBSCENE_ID::NONE;
    m_ChangeMainScene = false;
    m_ChangeSubScene = false;
    m_FadeAlpha = m_max_fade_alpha;

    for (int i = 0;i < m_JoinCount;i++)
    {
        m_DeadCount[i] = 0;
    }

    m_BuildData.clear();

    // シーン変更
    m_State = STATE::SCENE_CHANGE;
}

/*
 *  更新
 */
void
CSceneManager::
Update(void)
{
    switch (m_State)
    {
    case STATE::FADEIN:          FadeIn();          break;
    case STATE::SCENE_UPDATE:    SceneUpdate();     break;
    case STATE::FADEOUT:         FadeOut();         break;
    case STATE::SCENE_CHANGE:    MainSceneChange();
                                 SubSceneChange();  break;
    }
}

/*
 *  描画
 */
void
CSceneManager::
Draw(void)
{
    // メインシーン描画
    if (m_MainScene)
        m_MainScene->Draw();

    // サブシーン描画
    if (m_SubScene)
        m_SubScene->Draw();
}

/*
 *  シーンエフェクト描画
 */
void
CSceneManager::
DrawSceneEffect(void)
{
    // アルファ値とフェードカラーの合成
    unsigned int color = (m_FadeAlpha << 24) | (m_fade_color & 0x00ffffff);

    // シーンエフェクトを描画
    vivid::DrawTexture("data\\object\\white.png", m_fade_position, color);
}

/*
 *  解放
 */
void
CSceneManager::
Finalize(void)
{
    // メインシーン解放
    if (m_MainScene)
    {
        m_MainScene->Finalize();

        delete m_MainScene;

        m_MainScene = nullptr;
    }

    // サブシーン解放
    if (m_SubScene)
    {
        m_SubScene->Finalize();

        delete m_SubScene;

        m_SubScene = nullptr;
    }
}

/*
 *  戦闘データリセット
 */
void
CSceneManager::
ResetBattleData(void)
{
    for (int i = 0; i < static_cast<int>(PLAYER_ID::MAX); ++i)
    {
        m_DeadCount[i] = -1;
    }
}

/*
 *  ビルドデータクリア
 */
void
CSceneManager::
ClearBuildData(void)
{
    m_BuildData.clear();
}

/*
 *  メインシーン切換え
 */
void
CSceneManager::
ChangeMainScene(MAINSCENE_ID id)
{
    // 次のメインシーンIDを登録
    m_NextMainSceneID = id;

    m_ChangeMainScene = true;
}

/*
 *  サブシーン切換え
 */
void
CSceneManager::
ChangeSubScene(SUBSCENE_ID id)
{
    // 次のサブシーンIDを登録
    m_NextSubSceneID = id;

    m_ChangeSubScene = true;
}

/*
 *  メインシーン生成
 */
void
CSceneManager::
CreateMainScene(MAINSCENE_ID id)
{
    //IDを基準にシーン多分岐
    switch (id)
    {
    case MAINSCENE_ID::TITLE:               m_MainScene = new CTitle();             break;
    case MAINSCENE_ID::PLAYER_JOIN:         m_MainScene = new CPlayerJoin();        break;
    case MAINSCENE_ID::BUILD_SELECT:        m_MainScene = new CBuildSelect();       break;
    case MAINSCENE_ID::GAME_MAIN:           m_MainScene = new CGameMain();          break;
    case MAINSCENE_ID::RESULT:              m_MainScene = new CResult();            break;
    }
}

/*
 *  サブシーン生成
 */
void
CSceneManager::
CreateSubScene(SUBSCENE_ID id)
{
    //IDを基準にシーン多分岐
    switch (id)
    {
    case SUBSCENE_ID::NONE:     m_SubScene = nullptr;       break;
    case SUBSCENE_ID::PAUSE:    m_SubScene = new CPause();  break;
    }
}

/*
 *  フェードイン
 */
void
CSceneManager::
FadeIn(void)
{
    m_FadeAlpha -= m_fade_speed;

    if (m_FadeAlpha < m_min_fade_alpha)
    {
        m_FadeAlpha = m_min_fade_alpha;

        // シーン更新
        m_State = STATE::SCENE_UPDATE;
    }
}

/*
 *  シーン更新
 */
void
CSceneManager::
SceneUpdate(void)
{
    // サブシーン更新
    if (m_SubScene)
    {
        m_SubScene->Update();
    }
    // メインシーン更新
    else
    {
        m_MainScene->Update();
    }

    // メインシーン変更が発生
    if (m_CurrentMainSceneID != m_NextMainSceneID || m_ChangeMainScene)
    {
        // フェードアウト
        m_State = STATE::FADEOUT;

        m_ChangeMainScene = false;
    }

    // サブシーン変更が発生
    if (m_CurrentSubSceneID != m_NextSubSceneID || m_ChangeSubScene)
    {
        SubSceneChange();

        m_ChangeSubScene = false;
    }
}

/*
 *  フェードアウト
 */
void
CSceneManager::
FadeOut(void)
{
    m_FadeAlpha += m_fade_speed;

    if (m_FadeAlpha > m_max_fade_alpha)
    {
        m_FadeAlpha = m_max_fade_alpha;

        // シーン変更
        m_State = STATE::SCENE_CHANGE;
    }
}

/*
 *  メインシーン変更
 */
void
CSceneManager::
MainSceneChange(void)
{
    // メインシーン解放
    if (m_MainScene)
    {
        m_MainScene->Finalize();

        delete m_MainScene;

        m_MainScene = nullptr;
    }

    // 新しいメインシーン生成
    CreateMainScene(m_NextMainSceneID);

    // サブシーンはダミーに戻す
    CreateSubScene(SUBSCENE_ID::NONE);

    // メインシーン初期化
    m_MainScene->Initialize();

    // メインシーンIDを合わせる
    m_CurrentMainSceneID = m_NextMainSceneID;

    // フェードイン
    m_State = STATE::FADEIN;
}

/*
 *  サブシーン変更
 */
void
CSceneManager::
SubSceneChange(void)
{
    // サブシーン解放
    if (m_SubScene)
    {
        m_SubScene->Finalize();

        delete m_SubScene;

        m_SubScene = nullptr;
    }

    // 新しいサブシーン生成
    CreateSubScene(m_NextSubSceneID);

    // サブシーン初期化
    if (m_SubScene)
        m_SubScene->Initialize();

    // サブシーン更新
    if (m_SubScene)
        m_SubScene->Update();

    // サブシーンIDを合わせる
    m_CurrentSubSceneID = m_NextSubSceneID;
}