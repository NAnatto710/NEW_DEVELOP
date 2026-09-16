
/*!
 *  @file       scene_manager.h
 *  @brief      シーン管理
 *  @author     Ryusei Shimizu
 *  @date       2025/10/08
 */

#pragma once

#include "vivid.h"
#include "scene_id.h"

class IScene;

/*!
 *  @class      CSceneManager
 *
 *  @brief      シーン管理クラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2025/10/08
 */
class CSceneManager
{
public:

    /*!
     *  @brief      インスタンスの取得
     *
     *  @return     インスタンス
     */
    static CSceneManager& GetInstance(void);

    /*!
     *  @brief      初期化
     */
    void        Initialize(void);

    /*!
     *  @brief      更新
     */
    void        Update(void);

    /*!
     *  @brief      描画
     */
    void        Draw(void);

    /*!
     *  @brief      シーンエフェクト描画
     */
    void        DrawSceneEffect(void);

    /*!
     *  @brief      解放
     */
    void        Finalize(void);

    /*!
     *  @brief      メインシーン切り換え
     *
     *  @param[in]  id  シーンID
     */
    void        ChangeMainScene(MAINSCENE_ID id);

    /*!
     *  @brief      サブシーン切り換え
     *
     *  @param[in]  id  シーンID
     */
    void        ChangeSubScene(SUBSCENE_ID id);

    /*!
     *  @brief      現在のメインシーンを取得
     *
     *  @return     現在のメインシーン
     */
    IScene*     GetMainScene(void)const;

    /*!
     *  @brief      現在のサブシーンを取得
     *
     *  @return     現在のサブシーン
     */
    IScene*     GetSubScene(void)const;

	/*!
	 *  @brief      現在のメインシーンIDの取得
	 *
	 *  @return     現在のメインシーンID
	 */
	MAINSCENE_ID GetMainSceneID(void) const { return m_CurrentMainSceneID; }    //!< 現在のメインシーンIDの取得

private:

    /*!
     *  @brief      コンストラクタ
     */
    CSceneManager(void);

    /*!
     *  @brief      コピーコンストラクタ
     *
     *  @param[in]  rhs     オブジェクト
     */
    CSceneManager(const CSceneManager& rhs);

    /*!
     *  @brief      ムーブコンストラクタ
     *
     *  @param[in]  rhs     オブジェクト
     */
    CSceneManager(CSceneManager&& rhs);

    /*!
     *  @brief      デストラクタ
     */
    ~CSceneManager(void);

    /*!
     *  @brief      代入演算子
     *
     *  @param[in]  rhs 代入オブジェクト
     *
     *  @return     自身のオブジェクト
     */
    CSceneManager& operator=(const CSceneManager& rhs);

    /*!
     *  @brief      メインシーン生成
     *
     *  @param[in]  id  シーンID
     */
    void    CreateMainScene(MAINSCENE_ID id);

    /*!
     *  @brief      サブシーン生成
     *
     *  @param[in]  id  シーンID
     */
    void    CreateSubScene(SUBSCENE_ID id);

    /*!
     *  @brief      フェードイン
     */
    void    FadeIn(void);

    /*!
     *  @brief      シーン更新
     */
    void    SceneUpdate(void);

    /*!
     *  @brief      フェードアウト
     */
    void    FadeOut(void);

    /*!
     *  @brief      メインシーン変更
     */
    void    MainSceneChange(void);

    /*!
     *  @brief      サブシーン変更
     */
    void    SubSceneChange(void);

    /*!
     *  @brief      状態ID
     */
    enum class STATE
    {
        FADEIN              //!< フェードイン
        , SCENE_UPDATE      //!< シーン更新
        , FADEOUT           //!< フェードアウト
        , SCENE_CHANGE      //!< シーン変更
    };

    static const int                m_fade_speed;           //!< フェード速度
    static const vivid::Vector2     m_fade_position;        //!< フェード表示位置
    static const unsigned int       m_fade_color;           //!< フェード色
    static const int                m_min_fade_alpha;       //!< フェード用アルファの最小値
    static const int                m_max_fade_alpha;       //!< フェード用アルファの最大値

    MAINSCENE_ID                    m_CurrentMainSceneID;   //!< 現在のメインシーンID
    MAINSCENE_ID                    m_NextMainSceneID;      //!< 次のメインシーンID

    SUBSCENE_ID						m_CurrentSubSceneID;    //!< 現在のサブシーンID
    SUBSCENE_ID						m_NextSubSceneID;       //!< 次のサブシーンID

    IScene*                         m_MainScene;            //!< メインシーンクラス
    IScene*                         m_SubScene;             //!< サブシーンクラス

    STATE                           m_State;                //!< 状態

    bool                            m_ChangeMainScene;      //!< メインシーン変更フラグ
    bool                            m_ChangeSubScene;       //!< サブシーン変更フラグ

    int                             m_FadeAlpha;            //!< フェード時のアルファ値
};