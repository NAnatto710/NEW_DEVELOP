
/*!
 *  @file		camera_manager.cpp
 *  @brief		カメラ管理
 *  @author     Ryusei Shimizu
 *  @date       2026/04/16
 */

#include "camera_manager.h"
#include "../stage_manager/stage_manager.h"
#include "../player_manager/player_manager.h"
#include "../../../utility/utility.h"

const float         CCameraManager::m_max_camera_scale      = 1.1f;		//!< カメラの最大拡大率
const float         CCameraManager::m_min_camera_scale      = 0.7f;		//!< カメラの最小拡大率
const float         CCameraManager::m_complement_rate		= 0.08f;	//!< 補間率

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
	// カメラ位置の初期化
    m_Scroll = {};

    // ブロックサイズ取得
    m_BlockSize = CStageManager::GetInstance().GetBlockSize();
	// ステージの幅と高さの取得
    m_StageWidth = CStageManager::GetInstance().GetMapChipWidth();
    m_StageHeight = CStageManager::GetInstance().GetMapChipHeight();

	// カメラの拡大率の初期化
	m_CameraScale = 0.0f;

	// 初回のカメラ更新フラグを立てる
	m_FirstCameraUpdate = true;

	// カメラ中心の初期化
	m_CameraCenter = vivid::Vector2::ZERO;
	m_TargetScale = m_max_camera_scale;
	m_CameraScale = m_max_camera_scale;

	for(int i = 0; i < (int)PLAYER_ID::MAX; i++)
	{
		m_Player[i] = nullptr;
		m_PlayerPosition[i] = vivid::Vector2::ZERO;

		// プレイヤーごとのカメラ揺れの初期化
		m_PlayerShakeFlg[i] = false;
		m_PlayerShakeTime[i] = 0;
		m_PlayerShakeMaxTime[i] = 0;
		m_PlayerShakeIntensity[i] = {};
		m_PlayerShakeOffset[i] = {};

		// 継続微振動
		m_PlayerMicroShakeFlg[i] = false;
		m_PlayerMicroShakeFrame[i] = 0;
		m_PlayerMicroShakeIntensity[i] = {};
	}

    // プレイヤーの取得
	CPlayerManager& player_manager = CPlayerManager::GetInstance();
	for (int i = 0; i < player_manager.GetPlayerCount(); i++)
	{
		if (player_manager.GetPlayer((PLAYER_ID)i))
			m_Player[i] = player_manager.GetPlayer((PLAYER_ID)i);
	}

	// カメラ揺れの初期化
	m_CameraShakeFlg = false;
	m_CameraShakeTime = 0;
	m_CameraShakeMaxTime = 0;
	m_CameraShakeIntensity = {};
	m_CameraShakeOffset = {};

	m_MaxPlayerPosition = vivid::Vector2(999999.0f, 999999.0f);
	m_MinPlayerPosition = vivid::Vector2(-999999.0f, -999999.0f);

	// カメラ位置の更新
	this->CameraPositionUpdate();
}

/*
 *  更新
 */
void
CCameraManager::
Update(void)
{
	// カメラ位置の更新
	this->CameraPositionUpdate();
}

/*
 *  解放
 */
void
CCameraManager::
Finalize(void)
{
	for (int i = 0; i < static_cast<int>(PLAYER_ID::MAX); ++i)
	{
		m_Player[i] = nullptr;
		m_PlayerPosition[i] = vivid::Vector2::ZERO;

		m_PlayerShakeFlg[i] = false;
		m_PlayerShakeTime[i] = 0;
		m_PlayerShakeMaxTime[i] = 0;
		m_PlayerShakeIntensity[i] = {};
		m_PlayerShakeOffset[i] = {};

		m_PlayerMicroShakeFlg[i] = false;
		m_PlayerMicroShakeFrame[i] = 0;
		m_PlayerMicroShakeIntensity[i] = {};
	}
}

/*
 *  カメラ位置の更新
 */
void
CCameraManager::
CameraPositionUpdate(void)
{
	m_MinPlayerPosition = vivid::Vector2(999999.0f, 999999.0f);
	m_MaxPlayerPosition = vivid::Vector2(-999999.0f, -999999.0f);

	// プレイヤーの位置を取得
	for (int i = 0; i < (int)PLAYER_ID::MAX; i++)
	{
		CPlayer* player = m_Player[i];

		// プレイヤーが存在しない場合はスキップ
		if (player == nullptr || !player->IsActive()) continue;

		float half = player->GetPlayerData().Height * 0.5f;

		// プレイヤーの位置を取得（中心位置 - 半分の高さ）
		m_PlayerPosition[i] = player->GetPhysicsComponent().GetCenterPosition() - vivid::Vector2(0.0f, half);

		// プレイヤーの最小位置と最大位置を更新
		m_MinPlayerPosition.x =	MIN(m_MinPlayerPosition.x, m_PlayerPosition[i].x);
		m_MinPlayerPosition.y = MIN(m_MinPlayerPosition.y, m_PlayerPosition[i].y);

		m_MaxPlayerPosition.x =	MAX(m_MaxPlayerPosition.x, m_PlayerPosition[i].x);
		m_MaxPlayerPosition.y = MAX(m_MaxPlayerPosition.y, m_PlayerPosition[i].y);
	}

	// カメラのY位置の計算
	// 一番下のプレイヤーを基準にする
	float dy = m_MaxPlayerPosition.y - m_MinPlayerPosition.y;
	float targetY = m_MaxPlayerPosition.y;

	// 地上寄りの基準
	targetY -= dy * 0.15f;

	float diffY = targetY - m_CameraCenter.y;

	// 少し遅れて追従
	m_CameraCenter.y += diffY * m_complement_rate;

	// カメラのX位置の計算
	float targetCenterX = (m_MinPlayerPosition.x + m_MaxPlayerPosition.x) * 0.5f;
	m_CameraCenter.x +=	(targetCenterX - m_CameraCenter.x) * m_complement_rate;

	// 初回だけカメラを即座に中央へ移動
	if (m_FirstCameraUpdate)
	{
		m_CameraCenter.x = targetCenterX;
		m_CameraCenter.y = targetY;
	}

	// カメラ拡大率の更新
	CameraScaleUpdate();

	// プレイヤーのカメラ揺れの更新
	PlayerShakeUpdate();

	float halfWidth = vivid::WINDOW_WIDTH * 0.5f;
	float halfHeight = vivid::WINDOW_HEIGHT * 0.5f;

	vivid::Vector2 targetScroll;
	targetScroll.x = m_CameraCenter.x * m_CameraScale - halfWidth;
	targetScroll.y = m_CameraCenter.y * m_CameraScale - halfHeight;

	// 初回のカメラ更新時は、スクロールを即座に変更
	if (m_FirstCameraUpdate)
	{
		m_Scroll = targetScroll;
		m_FirstCameraUpdate = false;
	}
	else
	{
		// スクロールの補間
		m_Scroll += (targetScroll - m_Scroll) * m_complement_rate;
	}

	// カメラ揺れの更新
	CameraShakeUpdate();
}

/*
 *  カメラ拡大率の更新
 */
void
CCameraManager::
CameraScaleUpdate(void)
{
	// プレイヤー間の距離を計算
	float dx = m_MaxPlayerPosition.x - m_MinPlayerPosition.x;
	float dy = m_MaxPlayerPosition.y - m_MinPlayerPosition.y;

	// 水平方向と垂直方向の距離を基準距離で割って比率を計算
	float WidthRatio = dx / (vivid::WINDOW_WIDTH * 0.7f);
	float HeightRatio = dy / (vivid::WINDOW_HEIGHT * 1.8f);

	// 水平方向と垂直方向の比率のうち、より大きい方を採用
	float ratio = CLAMP(MAX(WidthRatio, HeightRatio), 0.0f, 1.0f);

	// 目標のカメラ拡大率を計算
	m_TargetScale = m_max_camera_scale - (m_max_camera_scale - m_min_camera_scale) * ratio;

	// 目標拡大率と現在の拡大率の差を計算
	float scaleDiff = m_TargetScale - m_CameraScale;

	// 拡大率の変化が小さい場合は、変化させない
	if (scaleDiff > 0.0f &&	scaleDiff < 0.02f)
	{
		scaleDiff = 0.0f;
	}

	// 拡大率の変化が拡大の場合は遅く、縮小の場合は速く
	float scaleFollow = (scaleDiff > 0.0f) ? 0.01f : 0.02f;

	// 初回のカメラ更新時は、拡大率を即座に変更
	if(m_FirstCameraUpdate)
	{
		scaleFollow = 1.0f;
	}

	// カメラの拡大率を更新
	m_CameraScale += scaleDiff * scaleFollow;
}

/*
 *  カメラ揺れの更新
 */
void
CCameraManager::
PlayerShakeUpdate(void)
{
	for (int i = 0; i < (int)PLAYER_ID::MAX; i++)
	{
		// 毎フレーム一度リセット
		m_PlayerShakeOffset[i] = vivid::Vector2::ZERO;

		// プレイヤーが存在しない、または非アクティブ
		if (m_Player[i] == nullptr || !m_Player[i]->IsActive()) continue;

		// 時間が残っている場合は揺れを計算
		if (m_PlayerShakeFlg[i])
		{
			if (m_PlayerShakeTime[i] > 0)
			{
				// 残り時間から減衰率を計算
				float rate = (float)m_PlayerShakeTime[i] / (float)m_PlayerShakeMaxTime[i];

				// 左右交互
				float sign = (m_PlayerShakeTime[i] % 2 == 0) ? 1.0f : -1.0f;

				m_PlayerShakeOffset[i].x += sign * m_PlayerShakeIntensity[i].x * rate;
				m_PlayerShakeOffset[i].y += sign * m_PlayerShakeIntensity[i].y * rate *	0.25f;

				m_PlayerShakeTime[i]--;
			}
			else
			{
				m_PlayerShakeIntensity[i] = vivid::Vector2::ZERO;
				m_PlayerShakeTime[i] = 0;
				m_PlayerShakeMaxTime[i] = 0;
				m_PlayerShakeFlg[i] = false;
			}
		}

		// 継続微振動の計算
		if (m_PlayerMicroShakeFlg[i])
		{
			float signX = (m_PlayerMicroShakeFrame[i] % 3 == 0) ? 1.0f : -1.0f;
			float signY = ((m_PlayerMicroShakeFrame[i] / 2) % 3 == 0) ? 1.0f : -1.0f;

			m_PlayerShakeOffset[i].x += signX *	m_PlayerMicroShakeIntensity[i].x;
			m_PlayerShakeOffset[i].y +=	signY *	m_PlayerMicroShakeIntensity[i].y;

			m_PlayerMicroShakeFrame[i]++;
		}
		else
		{
			m_PlayerMicroShakeFrame[i] = 0;
		}
	}
}

/*
 *  カメラ揺れの更新
 */
void
CCameraManager::
CameraShakeUpdate(void)
{
	if (!m_CameraShakeFlg) return;

	// カメラ揺れの残り時間が0以下の場合、揺れを終了する
	if (m_CameraShakeMaxTime <= 0)
	{
		m_CameraShakeFlg = false;
		m_CameraShakeOffset = vivid::Vector2::ZERO;
		return;
	}

	// 0.0 ～ 1.0
	float t = 1.0f - (float)m_CameraShakeTime /	(float)m_CameraShakeMaxTime;

	// 徐々に弱くする
	float decay = 1.0f - t;

	// 高速で方向を切り替える
	float shakeX = (sinf(t * 50.0f) >= 0.0f) ? 1.0f : -1.0f;
	float shakeY = (sinf(t * 65.0f) >= 0.0f) ? 1.0f : -1.0f;

	m_CameraShakeOffset.x =	m_CameraShakeDirection.x * m_CameraShakeIntensity.x * shakeX * decay;
	m_CameraShakeOffset.y =	m_CameraShakeDirection.y * m_CameraShakeIntensity.y * shakeY * decay;

	m_Scroll += m_CameraShakeOffset;

	m_CameraShakeTime--;

	if (m_CameraShakeTime <= 0)
	{
		m_CameraShakeFlg = false;
		m_CameraShakeTime = 0;
		m_CameraShakeMaxTime = 0;
		m_CameraShakeOffset = vivid::Vector2::ZERO;
	}
}

/*
 *  カメラ揺れの設定
 */
void
CCameraManager::
SetPlayerShake(PLAYER_ID player_id, const int time, const vivid::Vector2 intensity)
{
	m_PlayerShakeTime[(int)player_id] = time;
	m_PlayerShakeMaxTime[(int)player_id] = time;
	m_PlayerShakeIntensity[(int)player_id] = intensity;
	m_PlayerShakeFlg[(int)player_id] = true;
}

/*
 *  プレイヤーの継続微振動設定
 */
void
CCameraManager::
SetPlayerMicroShake(PLAYER_ID player_id, const bool flg, const vivid::Vector2 intensity)
{
	int index = (int)player_id;

	m_PlayerMicroShakeFlg[index] = flg;

	if (flg)
	{
		m_PlayerMicroShakeIntensity[index] = intensity;
	}
	else
	{
		m_PlayerMicroShakeIntensity[index] = vivid::Vector2::ZERO;

		m_PlayerMicroShakeFrame[index] = 0;
	}
}

/*
 *  カメラ揺れの設定
 */
void
CCameraManager::
SetCameraShake(const int time, const vivid::Vector2 intensity, const vivid::Vector2 direction)
{
	m_CameraShakeTime = time;
	m_CameraShakeMaxTime = time;
	m_CameraShakeIntensity = intensity;
	m_CameraShakeDirection = direction;
	m_CameraShakeFlg = true;
}

/*
 *  プレイヤー設定
 */
void
CCameraManager::
SetPlayer(PLAYER_ID player_id, CPlayer* player)
{
	m_Player[(int)player_id] = player;
}

/*
 *  プレイヤーがカメラ揺れ中かどうかを取得
 */
bool
CCameraManager::
IsPlayerShaking(PLAYER_ID player_id) const
{
	int index = (int)player_id;

	return m_PlayerShakeFlg[index] || m_PlayerMicroShakeFlg[index];
}
