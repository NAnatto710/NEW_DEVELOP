
/*!
 *  @file       bullet.cpp
 *  @brief      弾クラス
 *  @author     Ryusei Shimizu
 *  @date       2025/12/18
 */

#include "bullet.h"
#include "..\..\..\..\utility\utility.h"
#include "../../camera_manager/camera_manager.h"
#include "../../score_manager/score_manager.h"

const unsigned int  IBullet::m_player_color = 0xffffffff;   //!< プレイヤーの弾の色
const unsigned int  IBullet::m_enemy_color  = 0xffffffff;   //!< 敵の弾の色


/*
 *  コンストラクタ
 */
IBullet::
IBullet(std::string name, int width, int height)
    : m_TextureName(name)
    , m_Width(width)
    , m_Height(height)
    , m_Radius(m_Width / 2.0f)
    , m_Category(CHARACTER_CATEGORY::UNKNOW)
    , m_CenterPosition(vivid::Vector2::ZERO)
    , m_Position(vivid::Vector2::ZERO)
    , m_Velocity(vivid::Vector2::ZERO)
    , m_Color(0xffffffff)
    , m_Anchor(vivid::Vector2((float)m_Width / 2.0f, (float)m_Height / 2.0f))
    , m_Rect({ 0, 0, m_Width, m_Height })
    , m_Scale(vivid::Vector2(1.0f, 1.0f))
    , m_Rotation(0.0f)
    , m_ActiveFlg(true)
{
}

/*
 *  デストラクタ
 */
IBullet::
~IBullet(void)
{
}

/*
 *  初期化
 */
void
IBullet::
Initialize(CHARACTER_CATEGORY category, const vivid::Vector2& position, float direction, float damage, float speed, float duration)
{
    m_Category = category;
    m_Position = position - vivid::Vector2((float)m_Width / 2.0f, (float)m_Height / 2.0f);
    m_Velocity.x = cos(direction) * speed;
    m_Velocity.y = sin(direction) * speed;
    m_Color = (category == CHARACTER_CATEGORY::PLAYER ? m_player_color : m_enemy_color);
    m_ActiveFlg = true;

    m_Status[(int)BULLET_STATUS_ID::DAMAGE] = damage;
    m_MaxStatus[(int)BULLET_STATUS_ID::DAMAGE] = damage;
    m_Status[(int)BULLET_STATUS_ID::DURATION] = duration;
    m_MaxStatus[(int)BULLET_STATUS_ID::DURATION] = duration;
}

/*
 *  更新
 */
void
IBullet::
Update(void)
{
    // 移動計算
    m_Position += m_Velocity;

    // 中心位置更新
    m_CenterPosition = m_Position + vivid::Vector2(m_Width / 2.0f, m_Height / 2.0f);

    // 移動速度に合わせ得て回転値を算出
    m_Rotation = atan2(m_Velocity.y, m_Velocity.x);

    // 効果時間の減少
    m_Status[(int)BULLET_STATUS_ID::DURATION] -= vivid::GetDeltaTime();

    if (m_Status[(int)BULLET_STATUS_ID::DURATION] <= 0.0f)
    {
        m_ActiveFlg = false;

		// プレイヤーの弾が消えるとスコア減点
        if (m_Category == CHARACTER_CATEGORY::PLAYER)
        {
            CScoreManager::GetInstance().AddScore(SCORE_ID::MISSBULLET);
        }
    }
}

/*
 *  描画
 */
void
IBullet::
Draw(void)
{
    vivid::Vector2 pos = m_Position;
    pos -= CCameraManager::GetInstance().GetPosition();

    vivid::DrawTexture(m_TextureName, pos, m_Color, m_Rect, m_Anchor, m_Scale, m_Rotation);
}

/*
 *  解放
 */
void
IBullet::
Finalize(void)
{
}

/*
 *  位置取得
 */
vivid::Vector2
IBullet::
GetPosition(void)const
{
    return m_Position;
}

/*
 *  位置設定
 */
void
IBullet::
SetPosition(const vivid::Vector2& positioin)
{
    m_Position = positioin;
}

/*
 *  中心位置取得
 */
vivid::Vector2
IBullet::
GetCenterPosition(void)const
{
    return m_CenterPosition;
}

/*
 *  横幅取得
 */
int
IBullet::
GetWidth(void) const
{
    return m_Width;
}

/*
 *  高さ取得
 */
int
IBullet::
GetHeight(void) const
{
    return m_Height;
}

/*
 *  半径取得
 */
float
IBullet::
GetRadius(void)const
{
    return m_Radius;
}

/*
 *  回転値取得
 */
float
IBullet::
GetRotation(void) const
{
    return m_Rotation;
}

/*
 *  アクティブフラグ取得
 */
bool
IBullet::
IsActive(void)const
{
    return m_ActiveFlg;
}

/*
 *  アクティブフラグ取得
 */
void
IBullet::
SetActive(bool active)
{
    m_ActiveFlg = active;
}

/*
 *  ユニット識別子取得
 */
CHARACTER_CATEGORY
IBullet::
GetBulletCategory(void)const
{
    return m_Category;
}

/*
 *  弾の色取得
 */
unsigned int
IBullet::
GetBulletColor(void)const
{
    return m_Color;
}
/*
 *	ステータス取得
 */
float
IBullet::
GetStatus(BULLET_STATUS_ID status_id) const
{
    return m_Status[(int)status_id];
}

/*
 *	ステータス最大値取得
 */
float
IBullet::
GetMaxStatus(BULLET_STATUS_ID status_id) const
{
    return m_MaxStatus[(int)status_id];
}