/*!
 *  @file           scene_manager.cpp
 *  @brief          シーン管理
 *  @author			Ryusei Shimizu
 *  @date           2026/10/09
 */
#include "scene_manager.h"
#include "scene/scene_object.h"

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
    this->Finalize();

    m_CurrentMainSceneID = MAIN_SCENE_ID::NONE;
    m_NextMainSceneID = MAIN_SCENE_ID::TITLE;
    m_CurrentSubSceneID = SUB_SCENE_ID::NONE;
    m_NextSubSceneID = SUB_SCENE_ID::NONE;

    m_ChangeMainScene = false;
    m_ChangeSubScene = false;

	// シーンエフェクト初期化
    m_SceneTransition.Initialize();
    m_SceneTransition.SetCovered();

    m_State = STATE::SCENE_CHANGE;
}

/*
 *  更新
 */
void
CSceneManager::
Update(void)
{
	// シーン状態に応じて処理を分岐
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
	// メインシーンとサブシーンの描画
    if (m_MainScene)
        m_MainScene->Draw();
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
    m_SceneTransition.Draw();
}

/*
 *  解放
 */
void
CSceneManager::
Finalize(void)
{
    if (m_MainScene)
    {
        m_MainScene->Finalize();
        delete m_MainScene;
        m_MainScene = nullptr;
    }

    if (m_SubScene)
    {
        m_SubScene->Finalize();
        delete m_SubScene;
        m_SubScene = nullptr;
    }

    m_CurrentMainSceneID = MAIN_SCENE_ID::NONE;
    m_CurrentSubSceneID = SUB_SCENE_ID::NONE;
}

/*
 *  メインシーン切り換え
 */
void
CSceneManager::
ChangeMainScene(MAIN_SCENE_ID id)
{
    if (id <= MAIN_SCENE_ID::NONE || id >= MAIN_SCENE_ID::MAX) return;
    if (m_CurrentMainSceneID == id || m_State != STATE::SCENE_UPDATE) return;

    m_NextMainSceneID = id;
    m_ChangeMainScene = true;
}

/*
 *  サブシーン切り換え
 */
void
CSceneManager::
ChangeSubScene(SUB_SCENE_ID id)
{
    if (id < SUB_SCENE_ID::NONE || id >= SUB_SCENE_ID::MAX) return;
    if (m_CurrentSubSceneID == id || m_State != STATE::SCENE_UPDATE) return;

    m_NextSubSceneID = id;
    m_ChangeSubScene = true;
}

/*
 *  メインシーン生成
 */
void
CSceneManager::
CreateMainScene(MAIN_SCENE_ID id)
{
	// メインシーンの生成
    switch (id)
    {
    case MAIN_SCENE_ID::TITLE:           m_MainScene = new CTitle();          break;
    case MAIN_SCENE_ID::STAGE_SELECT:    m_MainScene = new CStageSelect();    break;
    case MAIN_SCENE_ID::GAME_MAIN:       m_MainScene = new CGameMain();       break;
    case MAIN_SCENE_ID::RESULT:          m_MainScene = new CResult();         break;
    default:                             m_MainScene = nullptr;               break;
    }
}

/*
 *  サブシーン生成
 */
void
CSceneManager::
CreateSubScene(SUB_SCENE_ID id)
{
	// サブシーンの生成
    switch (id)
    {
    case SUB_SCENE_ID::PAUSE:    m_SubScene = new CPause(); break;
    default:                     m_SubScene = nullptr;     break;
    }
}

/*
 *  フェードイン（中央から外周へ黒タイルを消す）
 */
void
CSceneManager::
FadeIn(void)
{
	// シーンエフェクトの更新
    m_SceneTransition.Reveal();

	// フェードイン完了後はシーン更新へ
    if (m_SceneTransition.IsRevealed())
        m_State = STATE::SCENE_UPDATE;
}

/*
 *  シーン更新
 */
void
CSceneManager::
SceneUpdate(void)
{
	// メインシーンとサブシーンの更新
    if (m_SubScene)
        m_SubScene->Update();
    else if (m_MainScene)
        m_MainScene->Update();

	// メインシーンまたはサブシーンの変更があればフェードアウトへ
    if (m_CurrentMainSceneID != m_NextMainSceneID || m_ChangeMainScene)
    {
        m_State = STATE::FADEOUT;
        m_ChangeMainScene = false;
    }
    else if (m_CurrentSubSceneID != m_NextSubSceneID || m_ChangeSubScene)
    {
        this->SubSceneChange();
        m_ChangeSubScene = false;
    }
}

/*
 *  フェードアウト（外周から中央へ黒タイルを表示）
 */
void
CSceneManager::
FadeOut(void)
{
	// シーンエフェクトの更新
    m_SceneTransition.Cover();

	// フェードアウト完了後はシーン変更へ
    if (m_SceneTransition.IsCovered())
        m_State = STATE::SCENE_CHANGE;
}

/*
 *  メインシーン変更
 */
void
CSceneManager::
MainSceneChange(void)
{
    if (m_MainScene)
    {
        m_MainScene->Finalize();
        delete m_MainScene;
        m_MainScene = nullptr;
    }

    // メインシーン変更時はサブシーンも閉じる
    if (m_SubScene)
    {
        m_SubScene->Finalize();
        delete m_SubScene;
        m_SubScene = nullptr;
    }

    m_CurrentSubSceneID = SUB_SCENE_ID::NONE;
    m_NextSubSceneID = SUB_SCENE_ID::NONE;
    m_ChangeSubScene = false;

    this->CreateMainScene(m_NextMainSceneID);

    if (m_MainScene)
        m_MainScene->Initialize();

    m_CurrentMainSceneID = m_NextMainSceneID;
    m_State = STATE::FADEIN;
}

/*
 *  サブシーン変更
 */
void
CSceneManager::
SubSceneChange(void)
{
    if (m_SubScene)
    {
        m_SubScene->Finalize();
        delete m_SubScene;
        m_SubScene = nullptr;
    }

    this->CreateSubScene(m_NextSubSceneID);

    if (m_SubScene)
        m_SubScene->Initialize();

    m_CurrentSubSceneID = m_NextSubSceneID;
}
