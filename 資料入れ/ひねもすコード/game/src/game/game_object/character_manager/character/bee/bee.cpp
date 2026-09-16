
/*!
 *  @file       far_enemy.h
 *  @brief      ハチ クラス
 *  @author     Misaki Kawada
 *  @date       2026/01/27
 */

#include "bee.h"
#include "../../../parameter_manager/parameter_manager.h"


//!< ステータス配列
const float CBee::m_status[(int)STATUS_ID::MAX][(int)STATUS_DEFINITION::MAX] =
{
	//  初期値		最大値		最小値
	{    10.0f,		200.0f,		 0.0f },	//!< HP
	{     0.0f,		  0.0f,		 0.0f },	//!< 満腹度
	{     5.0f,		 30.0f,		 0.0f },	//!< 攻撃力
	{     1.9f,	      1.9f,	     0.0f },	//!< 速度
	{	  5.0f,		  5.0f,		 0.0f },	//!< 触れたときのダメージ
};


//強化ステータス配列	
const float CBee::m_upgrade_status[(int)UPGRADE_STATUS_ID::MAX] =
{
	  1.0f,		    //!< 弾のダメージ倍率
      5.0f,		    //!< 弾のクールタイム
      3.0f,		    //!< 弾の持続時間
	 10.0f,		    //!< 弾のスピード
		 0,			//!< 突進のダメージ倍率
		 0,			//!< 突進のクールタイム
		 0,			//!< 突進の持続時間
		 0,			//!< 突進のスピード

};


const int				CBee::m_size = 90;								//!< サイズ
const std::string		CBee::m_path_name = "data\\character\\bee2.png";		//!< 画像名
const float				CBee::m_neardistance = 400;								//!< 近づく一定の距離
const float				CBee::m_stopdistance = 600;								//!< 止まる距離
const float             CBee::m_moveawaydistance = 800;								//!< 離れる一定の距離
const float				CBee::m_animation = 10;											//!< Animationの区切り

const float				CBee::m_max_hp_add = 3;											//!< 最大HPの加算値
const float				CBee::m_max_attack_power_add = 5;									//!< 最大攻撃力の加算値


/*
 *	コンストラクタ
 */
CBee::
CBee()
	: ICharacter(m_size, m_size, m_path_name, CHARACTER_CATEGORY::ENEMY, CHARACTER_ID::BEE)
	, m_DirectionFlag(false)
	, m_Timer(3.0f)
	, m_Speed(1.0f)
	, m_HitFlag(false)
	, m_IntervalFlag(false)
	, m_Interval(0.0f)
	, m_FirstRotation(0.0f)
	, m_NearDistanceFlag(true)
	, m_StopDistanceFlag(false)
	, m_MoveawayDistanceFlag(false)

{
}

/*
 *	デストラクタ
 */
CBee::
~CBee(void)
{
}

/*
 *	初期化
 */
void
CBee::
Initialize(const vivid::Vector2& position)
{
	//描画範囲
	m_Rect.top = 0;
	m_Rect.bottom = m_size;
	m_Rect.left = 0;
	m_Rect.right = m_Rect.left + m_size;

	// 基底クラスのInitializeを呼ぶ
	ICharacter::Initialize(position);


	// 代入
	m_Interval = m_upgrade_status[(int)UPGRADE_STATUS_ID::BULLET_COOLTIME];
	m_FirstRotation = m_Rotation;

	CGameParameterManager& pm = CGameParameterManager::GetInstance();
	SEASON_ID season = pm.GetSeasonId();

	// ステータスの初期化
	for (int i = 0; i < (int)STATUS_ID::MAX; i++)
	{
		float add_value = 0.0f;

		// 季節に応じてステータスを加算
		if (i == (int)STATUS_ID::HP)
		{
			add_value = m_max_hp_add * ((int)season);
		}
		else if (i == (int)STATUS_ID::ATTACKPOWER)
		{
			add_value = m_max_attack_power_add * ((int)season);
		}

		// ステータスの中に HP,満腹度,攻撃力,速度
		m_Status[i] = m_status[i][(int)STATUS_DEFINITION::DEFAULT] + add_value;
		m_MaxStatus[i] = m_status[i][(int)STATUS_DEFINITION::DEFAULT] + add_value;
	}

	m_AnimationFrame = 0.0f;
	m_AnimationTimer = 0.0f;
	m_AnimationCount = 0;

}

/*
 *	動作
 */
void
CBee::
Move(void)
{
	//一番近いプレイヤーを参照する
	ICharacter* player = CCharacterManager::GetInstance().FindNearPlayer(this);
	if (player)
	{
		//近づくフラグが立つ
		if (m_NearDistanceFlag && m_MoveawayDistanceFlag == false)
		{
			//敵がプレイヤーに近づく計算
			vivid::Vector2 v1 = player->GetCenterPosition() - this->GetCenterPosition();
			//角度
			float dir = atan2(v1.y, v1.x);

			Animation();

			m_Accelerator.x = this->GetStatus(STATUS_ID::SPEED) * cos(dir);
			m_Accelerator.y = this->GetStatus(STATUS_ID::SPEED) * sin(dir);
			//回転
			m_Rotation = atan2(m_Velocity.y, m_Velocity.x) + DEG_TO_RAD(270);

			//止まる一定の距離までいく
			if (v1.Length() <= m_stopdistance && v1.Length() >= m_neardistance)
			{
				m_StopDistanceFlag = true;
				m_Velocity = { 0.0f,0.0f };
			}
			//長さが近づく一定の距離に以下になったら
			if (v1.Length() <= m_neardistance)
			{
				m_NearDistanceFlag = false;
				m_StopDistanceFlag = false;
				m_MoveawayDistanceFlag = true;
			}
		}

		//逃げるフラグが立つ
		if (m_NearDistanceFlag == false && m_MoveawayDistanceFlag)
		{
			//敵からプレイヤーを引き、離れる計算
			vivid::Vector2 v2 = this->GetCenterPosition() - player->GetCenterPosition();
			//角度
			float dir = atan2(v2.y, v2.x);

			Animation();

			m_Accelerator.x = this->GetStatus(STATUS_ID::SPEED) * cos(dir);
			m_Accelerator.y = this->GetStatus(STATUS_ID::SPEED) * sin(dir);
			//回転
			m_Rotation = atan2(m_Velocity.y, m_Velocity.x) + DEG_TO_RAD(270);

			//長さが近づく一定の距離に以下になったら
			if (v2.Length() >= m_moveawaydistance)
			{
				m_StopDistanceFlag = true;
				m_NearDistanceFlag = true;
				m_MoveawayDistanceFlag = false;
			}
		}
	}

	ICharacter::Move();
}

/*
 *	攻撃
 */
void
CBee::
Attack(void)
{
	CBulletManager& bm = CBulletManager::GetInstance();
	CCameraManager& camera = CCameraManager::GetInstance();
	ICharacter* player = CCharacterManager::GetInstance().GetPlayer();

	//プレイヤーが見つかる
	if (player)
	{
		//	ワールド座標のGetCenterPositionからcameraのGetPositionを引いてローカル座標を出す
		vivid::Vector2 camera_position = GetCenterPosition() - camera.GetPosition();

		// ローカル座標の描画範囲内に入ってきたら
		if (0 <= camera_position.x && camera_position.x <= vivid::GetWindowWidth() &&
			0 <= camera_position.y && camera_position.y <= vivid::GetWindowHeight())
		{
			m_Interval -= vivid::GetDeltaTime();

			if (m_Interval <= 0.0f && m_StopDistanceFlag)
			{
				//
				vivid::Vector2 v = player->GetCenterPosition() - GetCenterPosition();
				m_FirstRotation = atan2f(v.y, v.x);
				// 弾を生成
				bm.Create(m_CharacterCategory, BULLET_ID::BEE_BULLET, GetCenterPosition(), m_FirstRotation + DEG_TO_RAD(340.0f), m_Status[(int)STATUS_ID::ATTACKPOWER], m_upgrade_status[(int)UPGRADE_STATUS_ID::BULLET_SPEED], m_upgrade_status[(int)UPGRADE_STATUS_ID::BULLET_DURATION]);
				bm.Create(m_CharacterCategory, BULLET_ID::BEE_BULLET, GetCenterPosition(), m_FirstRotation, m_Status[(int)STATUS_ID::ATTACKPOWER], m_upgrade_status[(int)UPGRADE_STATUS_ID::BULLET_SPEED], m_upgrade_status[(int)UPGRADE_STATUS_ID::BULLET_DURATION]);
				bm.Create(m_CharacterCategory, BULLET_ID::BEE_BULLET, GetCenterPosition(), m_FirstRotation + DEG_TO_RAD(20.0f), m_Status[(int)STATUS_ID::ATTACKPOWER], m_upgrade_status[(int)UPGRADE_STATUS_ID::BULLET_SPEED], m_upgrade_status[(int)UPGRADE_STATUS_ID::BULLET_DURATION]);


				m_Interval = m_upgrade_status[(int)UPGRADE_STATUS_ID::BULLET_COOLTIME];
			}
		}
	}
}

/*
 *  アニメーション
 */
void CBee::Animation(void)
{
	m_AnimationFrame++;

	//フレームの方が大きくなった時に動かす
	if (m_AnimationFrame > m_animation)
	{
		m_AnimationTimer += 1;
		m_AnimationFrame = 0;
	}

	//0行目の4列目まで行ったら次の行へ移動
	if (m_AnimationTimer >= 4 && m_AnimationCount == 0)
	{
		ChangeAnimation();
	}
	//1,2行目の5列目まで行ったら
	if (m_AnimationTimer >= 5)
	{
		ChangeAnimation();
	}
	//最後の行までの描画範囲指定が終わったら元に戻す
	if (m_AnimationCount >= 3)
	{
		m_Rect.top = 0;
		m_Rect.bottom = m_size;
		m_AnimationCount = 0;
	}


	m_Rect.left = m_size * m_AnimationTimer;
	m_Rect.right = m_Rect.left + m_size;

}

/*
 *  アニメーションに使う実数値の更新
 */
void CBee::ChangeAnimation(void)
{
	m_AnimationCount += 1;
	m_Rect.top = m_size * m_AnimationCount;
	m_Rect.bottom = m_Rect.top + m_size;
	m_AnimationTimer = 0;
}
