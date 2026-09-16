
/*!
 *  @file       scene_manager.h
 *  @brief      シーン管理
 *  @author     Ryusei Shimizu
 *  @date       2026/04/10
 */

#pragma once

#include "vivid.h"
#include "scene_id.h"
#include "../player_manager/player_id.h"
#include "../player_manager/player/build_component/build_data.h"
#include <vector>

class IScene;

/*!
 *  @class      CSceneManager
 *
 *  @brief      シーン管理クラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2026/04/10
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
     *  @brief      戦闘データリセット
	 */
    void            ResetBattleData(void);

    /*!
     *  @brief      ビルドデータクリア
	 */
    void            ClearBuildData(void);

    /*!
     *  @brief      メインシーン切り換え
     *
     *  @param[in]  id  シーンID
     */
    void            ChangeMainScene(MAINSCENE_ID id);

    /*!
     *  @brief      サブシーン切り換え
     *
     *  @param[in]  id  シーンID
     */
    void            ChangeSubScene(SUBSCENE_ID id);

    /*!
     *  @brief      現在のメインシーンを取得
     *
     *  @return     現在のメインシーン
     */
    IScene*         GetMainScene(void)const { return m_MainScene; }

    /*!
     *  @brief      現在のサブシーンを取得
     *
     *  @return     現在のサブシーン
     */
    IScene*         GetSubScene(void)const { return m_SubScene; }

    /*!
     *  @brief      現在のメインシーンIDの取得
     *
     *  @return     現在のメインシーンID
     */
    MAINSCENE_ID    GetMainSceneID(void) const { return m_CurrentMainSceneID; }

    /*!
    *  @brief       ゲームの状態設定
    *
    *  @param[in]   state   ゲーム状態ID
    */
    void            SetGamePlayer(int count) { m_JoinCount = count; }

    /*!
     *  @brief      ゲームの人数取得
     *
     *  @return     ゲームの参加人数
     */
    int             GetJoinCount(void) const { return /*m_JoinCount*/ 2; }

    /*!
     *  @brief      キャラクター設定
     *
     *  @param[in]  player_id   プレイヤーID
     *  @param[in]  saligia_id  感情ID
     */
    void            SetSaligiaId(PLAYER_ID player_id, SALIGIA_ID saligia_id) { m_Saligia_Id[(int)player_id] = saligia_id; }

    /*!
     *  @brief      キャラクター取得
     *
     *  @param[in]  state   感情ID
     *
     *  @return     感情
     */
    SALIGIA_ID      GetSaligiaId(PLAYER_ID id) const { return m_Saligia_Id[(int)id]; }

    /*!
     *  @brief      ビルド設定
     *  
	 *  @param[in]  data    ビルド設定
     */
    void            SetBuildData(BuildData data) { m_BuildData.push_back(data); }

    /*!
     *  @brief      ビルド設定
     *
     *  @param[in]  state   感情取得
     */
	BuildData       GetBuildData(PLAYER_ID id) const { return m_BuildData[(int)id]; }

    /*!
     *  @brief      コントローラー取得
     *
     *  @param[in]  id          プレイヤーID
     *  @param[in]  controller  デバイスID
     */
    void            SetController(PLAYER_ID id, vivid::controller::DEVICE_ID controller) { m_Controller[(int)id] = controller; }

    /*!
     *  @brief      コントローラー取得
     *
     *  @return     コントローラーID
     */
    vivid::controller::DEVICE_ID    GetController(PLAYER_ID id) const { return m_Controller[(int)id]; }

    /*!
     *  @brief      死亡順番設定
     *
     *  @param[in]  id      プレイヤーID
     *  @param[in]  count   死亡順番
	 */
    void            SetDeadCount(PLAYER_ID id,int count) { m_DeadCount[(int)id] = count; }

    /*!
     *  @brief      死亡順番取得
     *
     *  @param[in]  id      プレイヤーID
     *
	 *  @return     死亡順番
     */
    int             GetDeadCount(PLAYER_ID id) const { return m_DeadCount[(int)id]; }

private:

    /*!
     *  @brief      メインシーン生成
     *
     *  @param[in]  id  シーンID
     */
    void            CreateMainScene(MAINSCENE_ID id);

    /*!
     *  @brief      サブシーン生成
     *
     *  @param[in]  id  シーンID
     */
    void            CreateSubScene(SUBSCENE_ID id);

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

    bool                            m_ChangeMainScene;      //!< メインシーン変更フラグ
    bool                            m_ChangeSubScene;       //!< サブシーン変更フラグ

    int                             m_FadeAlpha;            //!< フェード時のアルファ値

    MAINSCENE_ID                    m_CurrentMainSceneID;   //!< 現在のメインシーンID
    MAINSCENE_ID                    m_NextMainSceneID;      //!< 次のメインシーンID

    SUBSCENE_ID						m_CurrentSubSceneID;    //!< 現在のサブシーンID
    SUBSCENE_ID						m_NextSubSceneID;       //!< 次のサブシーンID

    IScene* m_MainScene;            //!< メインシーンクラス
    IScene* m_SubScene;             //!< サブシーンクラス

    STATE                           m_State;                //!< 状態

    int                             m_JoinCount;            //!< プレイ人数
    SALIGIA_ID                      m_Saligia_Id[4];        //!< 感情の状態

    vivid::controller::DEVICE_ID    m_Controller[4];

    std::vector<BuildData>          m_BuildData;            //!< 感情設定
    std::vector<PLAYER_ID>          m_JoinPlayer;           //!< 参加人数

    int                             m_DeadCount[(int)PLAYER_ID::MAX];


    // 以下コンストラクタ類
    CSceneManager() = default;
    ~CSceneManager() = default;
    CSceneManager(const CSceneManager& rhs) = delete;
    CSceneManager& operator=(const CSceneManager& rhs) = delete;
};