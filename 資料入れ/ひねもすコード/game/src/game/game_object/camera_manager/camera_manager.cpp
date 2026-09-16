
/*!
 *  @file       camera_manager.cpp
 *  @brief      カメラ管理
 *  @author     Ryusei Shimizu
 *  @date       2025/10/28
 */

#include "camera_manager.h"
#include "../stage_manager/stage_manager.h"


CStageManager& sm                                               = CStageManager::GetInstance();        //!< ステージマネージャーのインスタンス

const int               CCameraManager::m_camera_up_limit       = sm.GetBlockSize();                                                                    //!< カメラの上移動限界値
const int               CCameraManager::m_camera_down_limit     = sm.GetMapChipWidth() * sm.GetBlockSize() - vivid::WINDOW_HEIGHT - sm.GetBlockSize();  //!< カメラの下移動限界値
const int               CCameraManager::m_camera_left_limit     = sm.GetBlockSize();                                                                    //!< カメラの左移動限界値
const int               CCameraManager::m_camera_right_limit    = sm.GetMapChipWidth() * sm.GetBlockSize() - vivid::WINDOW_WIDTH - sm.GetBlockSize();   //!< カメラの右移動限界値

/*
 *  インスタンスの取得
 */
CCameraManager&
CCameraManager::
GetInstance(void)
{
    static CCameraManager instance;

    return instance;
}

/*
 *  初期化
 */
void
CCameraManager::
Initialize(void)
{
    m_CameraStartPosition = sm.GetStartBlockPosition() - vivid::Vector2(vivid::WINDOW_WIDTH / 2.0f, vivid::WINDOW_HEIGHT / 2.0f);
    m_Position = m_CameraStartPosition;
    m_MovePosition = vivid::Vector2::ZERO;
    m_Velocity = vivid::Vector2::ZERO;
    m_MoveFlg = false;
    m_MoveCheck = true;

    for (int i = 0; i < 4; i++)
        m_CheckHit[i] = false;
}

/*
 *  更新
 */
void
CCameraManager::
Update(void)
{
    // カメラ移動範囲チェック
    MoveCheck();

    // カメラの追従処理
    Move();

    // 位置の更新
    m_Position = m_CameraStartPosition + m_MovePosition;
}

/*
 *  解放
 */
void
CCameraManager::
Finalize(void)
{
}

/*
 *  位置の取得
 */
vivid::Vector2
CCameraManager::
GetPosition(void)const
{
    // どこの画面端とも接触していなかったらカメラ座標を返す
    if (m_MoveCheck)    return m_Position;

    // どこかの画面端と接触していたらそれぞれ適した座標を返す
    if (m_CheckHit[0] && m_CheckHit[2]) return vivid::Vector2(m_camera_left_limit, m_camera_up_limit);
    if (m_CheckHit[1] && m_CheckHit[2]) return vivid::Vector2(m_camera_right_limit, m_camera_up_limit);
    if (m_CheckHit[0] && m_CheckHit[3]) return vivid::Vector2(m_camera_left_limit, m_camera_down_limit);
    if (m_CheckHit[1] && m_CheckHit[3]) return vivid::Vector2(m_camera_right_limit, m_camera_down_limit);
    if (m_CheckHit[0])  return vivid::Vector2(m_camera_left_limit, m_Position.y);
    if (m_CheckHit[1])  return vivid::Vector2(m_camera_right_limit, m_Position.y);
    if (m_CheckHit[2])  return vivid::Vector2(m_Position.x, m_camera_up_limit);
    if (m_CheckHit[3])  return vivid::Vector2(m_Position.x, m_camera_down_limit);
}

/*
 *  スクロール可能か判断
 */
void
CCameraManager::
MoveCheck(void)
{
    // 画面端との当たり判定

    m_CheckHit[0] = (m_Position.x < m_camera_left_limit);

    m_CheckHit[1] = (m_Position.x > m_camera_right_limit);

    m_CheckHit[2] = (m_Position.y < m_camera_up_limit);

    m_CheckHit[3] = (m_Position.y > m_camera_down_limit);

    // どこかの壁と接触していたらスクロールをしない
    if (m_CheckHit[0] || m_CheckHit[1] || m_CheckHit[2] || m_CheckHit[3])
        m_MoveCheck = false;
    else
        m_MoveCheck = true;
}

/*
 *  プレイヤーのセット
 */
void
CCameraManager::
SetPlayer(CPlayer* player)
{
    m_Player = player;
}

/*
 *  コンストラクタ
 */
CCameraManager::
CCameraManager(void)
    : m_Position({ 0.0f,0.0f })
    , m_Velocity({ 0.0f,0.0f })
    , m_MoveFlg(false)
    , m_MoveCheck(false)
{
}

/*
 *  コピーコンストラクタ
 */
CCameraManager::
CCameraManager(const CCameraManager& rhs)
{
    (void)rhs;
}

/*
 *  デストラクタ
 */
CCameraManager::
~CCameraManager(void)
{
}

/*
 *  代入演算子
 */
CCameraManager&
CCameraManager::
operator=(const CCameraManager& rhs)
{
    (void)rhs;
    return *this;
}

/*
 *  動作
 */
void
CCameraManager::
Move(void)
{
    // カメラ位置の取得
    vivid::Vector2 pos = m_Position + vivid::Vector2(vivid::WINDOW_WIDTH / 2.0f, vivid::WINDOW_HEIGHT / 2.0f);

    // プレイヤー位置の取得
    vivid::Vector2 p_pos = m_Player->GetPosition();

    // カメラ位置がプレイヤーを中心にとらえていなかった場合
    if (pos != p_pos)
    {
        // 各座標の差を求めて、カメラ位置を変更する
        vivid::Vector2 difference_pos = p_pos - pos;

        m_MovePosition += difference_pos;
    }
}