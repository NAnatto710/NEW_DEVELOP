
/*!
 *  @file       hud_manager.h
 *  @brief      HUD管理
 *  @author     Ryuusei Shimizu
 *  @date       2026/09/11
 */

#pragma once
#include "vivid.h"


class CPlayer;
class CResourceComponent;

struct BuildData;

enum class SALIGIA_ID;
enum class RESOURCE_ID;


/*!
 *  @class      CHUDManager
 *
 *  @brief      HUD管理クラス
 *
 *  @author     Ryuusei Shimizu
 *
 *  @date       2026/09/11
 */
class CHUDManager
{
public:

    /*!
     *  @brief      インスタンス取得
     *
     *  @return     インスタンス
     */
    static CHUDManager& GetInstance(void);


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
     *  @brief      解放
     */
    void            Finalize(void);


private:

    /*!
     *  @brief      テクスチャ読み込み
     */
    void            LoadTextures(void);

    /*!
     *  @brief      プレイヤー1人分のHUD描画
     *
     *  @param[in]  player          プレイヤー
     *  @param[in]  player_index    プレイヤー番号
     *  @param[in]  player_count    参加人数
     */
    void            DrawPlayerHUD(CPlayer* player, int player_index, int player_count);

    /*!
     *  @brief      プレイヤー頭上の番号表示
     *
     *  @param[in]  player          プレイヤー
     *  @param[in]  player_index    プレイヤー番号
     */
    void            DrawPlayerHeadMark(CPlayer* player, int player_index);

    /*!
     *  @brief      HUDフレーム描画
     *
     *  @param[in]  hud_position    HUD表示位置
     *  @param[in]  hud_scale       HUD表示倍率
     */
    void            DrawHUDFrame(const vivid::Vector2& hud_position, float hud_scale);


    /*!
     *  @brief      HPゲージ描画
     *
     *  @param[in]  hud_position    HUD表示位置
     *  @param[in]  resource        リソースコンポーネント
     *  @param[in]  hud_scale       HUD表示倍率
     */
    void            DrawHPGauge(const vivid::Vector2& hud_position, const CResourceComponent& resource, float hud_scale);

    /*!
     *  @brief      欲望ゲージ描画
     *
     *  @param[in]  hud_position    HUD表示位置
     *  @param[in]  resource        リソースコンポーネント
     *  @param[in]  hud_scale       HUD表示倍率
     */
    void            DrawDesireGauge(const vivid::Vector2& hud_position, const CResourceComponent& resource, float hud_scale);

    /*!
     *  @brief      ビルドアイコン描画
     *
     *  @param[in]  hud_position    HUD表示位置
     *  @param[in]  build           ビルドデータ
     *  @param[in]  hud_scale       HUD表示倍率
     */
    void            DrawBuildIcons(const vivid::Vector2& hud_position, const BuildData& build, float hud_scale);

    /*!
     *  @brief      P1～P4表示[
     *
     *  @param[in]  hud_position    HUD表示位置
     *  @param[in]  player_index    プレイヤー番号
     *  @param[in]  hud_scale       HUD表示倍率
     */
    void            DrawPlayerMark(const vivid::Vector2& hud_position, int player_index, float hud_scale);


    /*!
     *  @brief      残機表示
     *
     *  @param[in]  hud_position    HUD表示位置
     *  @param[in]  resource        リソースコンポーネント
     *  @param[in]  hud_scale       HUD表示倍率
     */
    void            DrawLife(const vivid::Vector2& hud_position, const CResourceComponent& resource, float hud_scale);


    /*!
     *  @brief      ゲージ描画
     *
     *  @param[in]  hud_position    HUD表示位置
     *  @param[in]  offset          描画位置オフセット
     *  @param[in]  source_rect     描画元矩形
     *  @param[in]  rate            ゲージ割合
     *  @param[in]  hud_scale       HUD表示倍率
     */
    void            DrawGauge(const vivid::Vector2& hud_position, const vivid::Vector2& offset, const vivid::Rect& source_rect, float rate, float hud_scale);


    /*!
     *  @brief      HUD表示位置取得
     *
     *  @param[in]  player_index    プレイヤー番号
     *  @param[in]  player_count    参加人数
     */
    vivid::Vector2  GetHUDPosition(int player_index, int player_count);

    /*!
     *  @brief      HUD表示倍率取得
     *
     *  @param[in]  player_count    参加人数
     *
     *  @return     HUD表示倍率
     */
    float           GetHUDScale(int player_count);


    /*!
     *  @brief      大罪アイコンパス取得
     *
     *  @param[in]  id  大罪ID
     *
     *  @return     大罪アイコンパス
     */
    const char* GetSaligiaIconPath(SALIGIA_ID id);

    /*!
     *  @brief      リソース割合取得
     *
     *  @param[in]  resource    リソースコンポーネント
     *  @param[in]  id          リソースID
     *
     *  @return     リソース割合
     */
    float           GetResourceRate(const CResourceComponent& resource, RESOURCE_ID id);

    /*!
     *  @brief      残機表示範囲へ制限
     *
     *  @param[in]  life    残機
     *
     *  @return     制限後の残機
     */
    int             ClampLife(int life);

    CHUDManager() = default;
    ~CHUDManager() = default;
    CHUDManager(const CHUDManager& rhs) = delete;
    CHUDManager& operator=(const CHUDManager& rhs) = delete;
};