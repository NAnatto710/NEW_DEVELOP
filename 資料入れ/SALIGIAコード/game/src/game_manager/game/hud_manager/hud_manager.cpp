
/*!
 *  @file       hud_manager.cpp
 *  @brief      HUD管理
 *  @author     Ryuusei Shimizu
 *  @date       2026/09/11
 */

#include "hud_manager.h"
#include "../player_manager/player_manager.h"
#include "../camera_manager/camera_manager.h"
#include "../../../utility/utility.h"

 /*
  *  インスタンス取得
  */
CHUDManager&
CHUDManager::
GetInstance(void)
{
    static CHUDManager instance;

    return instance;
}


/*
 *  初期化
 */
void
CHUDManager::
Initialize(void)
{
    LoadTextures();
}


/*
 *  更新
 */
void
CHUDManager::
Update(void)
{}


/*
 *  描画
 */
void
CHUDManager::
Draw(void)
{
    CPlayerManager& player_manager = CPlayerManager::GetInstance();

    const int player_count = player_manager.GetPlayerCount();

    vivid::DrawTexture("data\\hud\\sss.png", vivid::Vector2(0.0f, 30.0f));
    vivid::DrawTexture("data\\hud\\sss.png", vivid::Vector2(0.0f, 30.0f));

    // プレイヤーごとにHUD描画
    for (int i = 0; i < player_count; ++i)
    {
        CPlayer* player = player_manager.GetPlayer(static_cast<PLAYER_ID>(i));

        if (!player) continue;

        DrawPlayerHeadMark(player, i);
    }
    for (int i = 0; i < player_count; ++i)
    {
        CPlayer* player = player_manager.GetPlayer(static_cast<PLAYER_ID>(i));

        if (!player) continue;

        DrawPlayerHUD(player, i, player_count);
    }
}


/*
 *  解放
 */
void
CHUDManager::
Finalize(void)
{}


/*
 *  テクスチャ読み込み
 */
void
CHUDManager::
LoadTextures(void)
{
    vivid::LoadTexture("data\\hud\\new_new_UI.png");
    vivid::LoadTexture("data\\hud\\Selection.png");
    vivid::LoadTexture("data\\hud\\heart.png");
    vivid::LoadTexture("data\\hud\\icon\\superbia_icon.png");
    vivid::LoadTexture("data\\hud\\icon\\avaritia_icon.png");
    vivid::LoadTexture("data\\hud\\icon\\luxuria_icon.png");
    vivid::LoadTexture("data\\hud\\icon\\invidia_icon.png");
    vivid::LoadTexture("data\\hud\\icon\\gula_icon.png");
    vivid::LoadTexture("data\\hud\\icon\\ira_icon.png");
    vivid::LoadTexture("data\\hud\\icon\\acedia_icon.png");
}


/*
 *  プレイヤー1人分のHUD描画
 */
void
CHUDManager::
DrawPlayerHUD(CPlayer* player, int player_index, int player_count)
{
    if (!player) return;

    const vivid::Vector2 hud_position = GetHUDPosition(player_index, player_count);
    const float hud_scale = GetHUDScale(player_count);

    const CResourceComponent& resource = player->GetResourceComponent();
    const BuildData& build = player->GetBuildComponent().GetBuild();

    DrawHUDFrame(hud_position, hud_scale);
    DrawHPGauge(hud_position, resource, hud_scale);
    DrawDesireGauge(hud_position, resource, hud_scale);
    DrawBuildIcons(hud_position, build, hud_scale);
    DrawPlayerMark(hud_position, player_index, hud_scale);
    DrawLife(hud_position, resource, hud_scale);
}

/*
 *  プレイヤー頭上の番号表示
 */
void
CHUDManager::
DrawPlayerHeadMark(CPlayer* player, int player_index)
{
    if (!player) return;
    if (!player->IsActive()) return;
    if (player->IsRespawnDelay()) return;

    const char* texture_path = "data\\hud\\Selection.png";

    const int mark_width = 29;
    const int mark_height = 38;

    const PlayerData& player_data = player->GetPlayerData();
    vivid::Vector2 draw_position = player->GetPhysicsComponent().GetPosition();
    CCameraManager& camera = CCameraManager::GetInstance();

    const float camera_scale = camera.GetCameraScale();
    const vivid::Vector2 scroll = camera.GetScroll();

    draw_position *= camera_scale;
    draw_position -= scroll;

    draw_position.x += (player_data.Width * camera_scale) * 0.5f;
    draw_position.x -= mark_width * 0.5f;

    // 頭との距離
    const float head_margin = 8.0f;

    draw_position.y -= mark_height + head_margin;

    /*
     *  P1～P4の切り出し
     */
    const vivid::Rect source_rect =
    {
        mark_width * player_index,
        0,
        mark_width * (player_index + 1),
        mark_height
    };


    /*
     *  描画
     */
    vivid::DrawTexture(texture_path, draw_position, 0xffffffff, source_rect);
}

/*
 *  HUDフレーム描画
 */
void
CHUDManager::
DrawHUDFrame(const vivid::Vector2& hud_position, float hud_scale)
{
    const char* texture_path = "data\\hud\\new_new_UI.png";
    const vivid::Rect frame_rect = { 0, 0, 400, 100 };
    const vivid::Vector2 scale = { hud_scale, hud_scale };

    vivid::DrawTexture(texture_path, hud_position, 0xffffffff, frame_rect, vivid::Vector2::ZERO, scale);
}


/*
 *  HPゲージ描画
 */
void
CHUDManager::
DrawHPGauge(const vivid::Vector2& hud_position, const CResourceComponent& resource, float hud_scale)
{
    const vivid::Rect source_rect = { 123, 130, 382, 149 };

    /*
     *  HUD上での表示位置
     */
    vivid::Vector2 offset =
    {
        123.0f,
        30.0f
    };

    offset *= hud_scale;

    const float rate = GetResourceRate(resource, RESOURCE_ID::HP);

    DrawGauge(hud_position, offset, source_rect, rate, hud_scale);
}


/*
 *  欲望ゲージ描画
 */
void
CHUDManager::
DrawDesireGauge(const vivid::Vector2& hud_position, const CResourceComponent& resource, float hud_scale)
{
    const vivid::Rect source_rect = { 123, 255, 383, 274 };

    /*
     *  HUD上での表示位置
     */
    vivid::Vector2 offset =
    {
        123.0f,
        55.0f
    };

    offset *= hud_scale;

    const float rate = GetResourceRate(resource, RESOURCE_ID::DESIRE);

    DrawGauge(hud_position, offset, source_rect, rate, hud_scale);
}

/*
 *  ビルドアイコン描画
 */
void
CHUDManager::
DrawBuildIcons(const vivid::Vector2& hud_position, const BuildData& build, float hud_scale)
{
    /*
     *  添付画像の配置基準
     */
    vivid::Vector2 primary_offset =
    {
        36.0f,
        22.0f
    };

    vivid::Vector2 secondary_offset =
    {
        74.0f,
        29.0f
    };

    primary_offset *= hud_scale;
    secondary_offset *= hud_scale;


    const char* primary_icon = GetSaligiaIconPath(build.Primary);
    const char* secondary_icon = GetSaligiaIconPath(build.Secondary);

    const vivid::Vector2 scale =
    {
        hud_scale,
        hud_scale
    };

    if (primary_icon)
    {
        const int width = vivid::GetTextureWidth(primary_icon);
        const int height = vivid::GetTextureHeight(primary_icon);

        const vivid::Rect source_rect =
        {
            0,
            0,
            width,
            height
        };

        vivid::DrawTexture(primary_icon, hud_position + primary_offset, 0xffffffff, source_rect, vivid::Vector2::ZERO, scale);
    }

    if (secondary_icon)
    {
        const int width = vivid::GetTextureWidth(secondary_icon);
        const int height = vivid::GetTextureHeight(secondary_icon);

        const vivid::Rect source_rect =
        {
            0,
            0,
            width,
            height
        };

        vivid::DrawTexture(secondary_icon, hud_position + secondary_offset, 0xffffffff, source_rect, vivid::Vector2::ZERO, scale);
    }
}

/*
 *  P1～P4表示
 */
void
CHUDManager::
DrawPlayerMark(const vivid::Vector2& hud_position, int player_index, float hud_scale)
{
    const char* texture_path = "data\\hud\\Selection.png";

    const int mark_width = 29;
    const int mark_height = 38;

    /*
     *  添付画像の配置基準
     */
    vivid::Vector2 offset =
    {
        8.0f,
        32.0f
    };

    offset *= hud_scale;

    const vivid::Rect source_rect =
    {
        mark_width * player_index,
        0,
        mark_width * (player_index + 1),
        mark_height
    };

    const vivid::Vector2 scale = { hud_scale, hud_scale };

    vivid::DrawTexture(texture_path, hud_position + offset, 0xffffffff, source_rect, vivid::Vector2::ZERO, scale);
}


/*
 *  残機表示
 */
void
CHUDManager::
DrawLife(const vivid::Vector2& hud_position, const CResourceComponent& resource, float hud_scale)
{
    const char* texture_path = "data\\hud\\heart.png";

    const int life_width = 58;
    const int life_height = 20;

    /*
     *  添付画像の配置基準
     */
    vivid::Vector2 offset =
    {
        51.0f,
        68.0f
    };

    offset *= hud_scale;

    // 残機数を0～3に制限
    const int life = ClampLife(static_cast<int>(resource.GetCurrent(RESOURCE_ID::LIFE)));

    const vivid::Rect source_rect =
    {
        life_width * life,
        0,
        life_width * (life + 1),
        life_height
    };

    const vivid::Vector2 scale = { hud_scale, hud_scale };

    vivid::DrawTexture(texture_path, hud_position + offset, 0xffffffff, source_rect, vivid::Vector2::ZERO, scale);
}


/*
 *  ゲージ描画
 */
void
CHUDManager::
DrawGauge(const vivid::Vector2& hud_position, const vivid::Vector2& offset, const vivid::Rect& source_rect, float rate, float hud_scale)
{
    const char* texture_path = "data\\hud\\new_new_UI.png";

    const int source_width = source_rect.right - source_rect.left;
    const float clamped_rate = CLAMP(rate, 0.0f, 1.0f);

    const int draw_width = static_cast<int>(source_width * clamped_rate);

    if (draw_width <= 0) return;

    vivid::Rect draw_rect = source_rect;

    draw_rect.right = draw_rect.left + draw_width;

    const vivid::Vector2 scale = { hud_scale, hud_scale };

    vivid::DrawTexture(texture_path, hud_position + offset, 0xffffffff, draw_rect, vivid::Vector2::ZERO, scale);
}


/*
 *  HUD位置取得
 */
vivid::Vector2
CHUDManager::
GetHUDPosition(int player_index, int player_count)
{
    const float hud_scale = GetHUDScale(player_count);

    const float hud_width = 400.0f * hud_scale;
    const float hud_height = 100.0f * hud_scale;

    const float margin_x = 10.0f;

    const float window_width = static_cast<float>(vivid::GetWindowWidth());
    const float window_height = static_cast<float>(vivid::GetWindowHeight());

    const float position_y = window_height - hud_height;

    /*
     *  1人の場合は中央
     */
    if (player_count <= 1)
        return vivid::Vector2((window_width - hud_width) * 0.5f, position_y);

    /*
     *  HUDを置ける横幅
     */
    const float available_width = window_width - margin_x * 2.0f;

    /*
     *  HUD全体が占める横幅
     */
    const float total_hud_width = hud_width * player_count;

    /*
     *  HUD間の空白
     */
    float interval = (available_width - total_hud_width) / static_cast<float>(player_count - 1);

    /*
     *  画面幅不足時
     */
    if (interval < 0.0f)
    {
        interval = 0.0f;
    }

    const float position_x = margin_x + (hud_width + interval) * player_index;

    return vivid::Vector2(position_x, position_y);
}


/*
 *  HUD表示倍率取得
 */
float
CHUDManager::
GetHUDScale(int player_count)
{
    switch (player_count)
    {
    case 2:     return 1.80f;
    case 3:     return 1.40f;
    case 4:     return 1.20f;
    default:    return 1.00f;
    }
}


/*
 *  大罪アイコンのパス取得
 */
const char*
CHUDManager::
GetSaligiaIconPath(SALIGIA_ID id)
{
    switch (id)
    {
    case SALIGIA_ID::SUPERBIA:      return  "data\\hud\\icon\\superbia_icon.png";
    case SALIGIA_ID::AVARITIA:      return  "data\\hud\\icon\\avaritia_icon.png";
    case SALIGIA_ID::LUXURIA:       return  "data\\hud\\icon\\luxuria_icon.png";
    case SALIGIA_ID::INVIDIA:       return  "data\\hud\\icon\\invidia_icon.png";
    case SALIGIA_ID::GULA:          return  "data\\hud\\icon\\gula_icon.png";
    case SALIGIA_ID::IRA:           return  "data\\hud\\icon\\ira_icon.png";
    case SALIGIA_ID::ACEDIA:        return  "data\\hud\\icon\\acedia_icon.png";
    default:                        return nullptr;
    }
}


/*
 *  リソース割合取得
 */
float
CHUDManager::
GetResourceRate(const CResourceComponent& resource, RESOURCE_ID id)
{
    const float max_value = resource.GetMax(id);


    if (max_value <= 0.0f) return 0.0f;

    const float current_value = resource.GetCurrent(id);

    return CLAMP(current_value / max_value, 0.0f, 1.0f);
}

/*
 *  残機表示範囲へ制限
 */
int
CHUDManager::
ClampLife(int life)
{
    const int min_life = 0;
    const int max_life = 3;

    if (life < min_life) return min_life;
    if (life > max_life) return max_life;
    return life;
}