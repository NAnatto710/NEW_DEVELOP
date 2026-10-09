
/*!
 *  @file           scene_manager.h
 *  @brief          シーン管理
 *  @author			Ryusei Shimizu
 *  @date           2026/10/09
 */
#pragma once

#include "scene_id.h"
#include "scene_transition.h"

class IScene;

class CSceneManager
{
public:

    /*!
     *  @brief      インスタンスの取得
     */
    static CSceneManager& GetInstance(void);

    /*!
     *  @brief      初期化
	 */
    void            Initialize(void);

    /*!
	 *  @brief      更新
     */
    void            Update(void);

	/*!
	 *  @brief      描画
     */
    void            Draw(void);

	/*!
	 *  @brief      シーンエフェクト描画
     */
    void            DrawSceneEffect(void);

	/*!
	 *  @brief      解放
     */
    void            Finalize(void);

    /*!
     *  @brief      メインシーンの変更
     *  @param[in]  id      メインシーンID
	 */
    void            ChangeMainScene(MAIN_SCENE_ID id);

    /*!
     *  @brief      サブシーンの変更
	 *  @param[in]  id      サブシーンID
     */
    void            ChangeSubScene(SUB_SCENE_ID id);

    IScene*         GetMainScene(void) const { return m_MainScene; }
    IScene*         GetSubScene(void) const { return m_SubScene; }
    MAIN_SCENE_ID   GetMainSceneID(void) const { return m_CurrentMainSceneID; }
    SUB_SCENE_ID    GetSubSceneID(void) const { return m_CurrentSubSceneID; }

private:

    /*!
     *  @brief      シーン状態
	 */
    enum class STATE
    {
		FADEIN,         //!< フェードイン
		SCENE_UPDATE,   //!< シーン更新
		FADEOUT,        //!< フェードアウト
		SCENE_CHANGE    //!< シーン変更
    };

    /*!
	 *  @brief      メインシーンの作成
	 *  @param[in]  id      メインシーンID
     */
    void            CreateMainScene(MAIN_SCENE_ID id);

	/*!
	 *  @brief      サブシーンの作成
	 *  @param[in]  id      サブシーンID
     */
    void            CreateSubScene(SUB_SCENE_ID id);
    
	/*!
	 *  @brief      フェードイン
     */
    void            FadeIn(void);

	/*!
	 *  @brief      シーン更新
     */
    void            SceneUpdate(void);

	/*!
	 *  @brief      フェードアウト
     */
    void            FadeOut(void);

	/*!
	 *  @brief      メインシーン変更
     */
    void            MainSceneChange(void);

	/*!
	 *  @brief      サブシーン変更
     */
    void            SubSceneChange(void);

	IScene*         m_MainScene;                //!< メインシーン
    IScene*         m_SubScene;                 //!< サブシーン

	MAIN_SCENE_ID   m_CurrentMainSceneID;       //!< 現在のメインシーンID
	MAIN_SCENE_ID   m_NextMainSceneID;          //!< 次のメインシーンID
	SUB_SCENE_ID    m_CurrentSubSceneID;        //!< 現在のサブシーンID
	SUB_SCENE_ID    m_NextSubSceneID;           //!< 次のサブシーンID

	bool            m_ChangeMainScene;          //!< メインシーン変更フラグ
	bool            m_ChangeSubScene;           //!< サブシーン変更フラグ

	STATE           m_State;                    //!< シーン状態
	CSceneTransition m_SceneTransition;         //!< シーンエフェクト

	// 以下コンストラクタ類の削除
    CSceneManager(void) = default;
    ~CSceneManager(void) = default;
    CSceneManager(const CSceneManager&) = delete;
    CSceneManager& operator=(const CSceneManager&) = delete;
};
