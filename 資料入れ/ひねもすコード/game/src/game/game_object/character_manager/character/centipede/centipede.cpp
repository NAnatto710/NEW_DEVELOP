
/*
 *  @file       centipede.cpp
 *  @brief      ムカデ クラス
 *  @author     Misaki Kawada
 *  @date       2026/02/04
 */

#include "centipede.h"
#include "../../../parameter_manager/parameter_manager.h"
#include "../../../sound_manager/sound_manager.h"


//!< ステータス配列
const float CCentipede::m_status[(int)STATUS_ID::MAX][(int)STATUS_DEFINITION::MAX] =
{
	//  初期値		最大値		最小値
	{    29.0f,		200.0f,		 0.0f },	//!< HP
	{     0.0f,		  0.0f,		 0.0f },	//!< 満腹度
	{    15.0f,		 50.0f,		 0.0f },	//!< 攻撃力
	{     3.0f,	      3.0f,	     0.0f },	//!< 速度
	{	  5.0f,		  5.0f,		 0.0f },	//!< 触れたときのダメージ
};


const float CCentipede::m_upgrade_status[(int)UPGRADE_STATUS_ID::MAX] =
{
	0,				//!< 弾のダメージ倍率
	0,				//!< 弾のクールタイム
	0,				//!< 弾の持続時間
	0,				//!< 弾のスピード
	1.5,			//!< 突進のダメージ倍率
	5.0,			//!< 突進のクールタイム
	3.0,			//!< 突進の持続時間
	3.5,			//!< 突進のスピード
};

const int			CCentipede::m_size = 90;												//!< サイズ
const std::string	CCentipede::m_head_name = "data\\character\\centipede_head.png";		//!< 頭の画像名
const std::string	CCentipede::m_body_name = "data\\character\\centipede_body.png";		//!< 体の画像名
const float			CCentipede::m_stillnesstimer = 3.0f;									//!< 静止タイマー
const int			CCentipede::m_default_body_quantity = 5;								//!< 体の数の初期値
const int			CCentipede::m_max_body_quantity = 10;									//!< 体の数の最大値
const int			CCentipede::m_min_body_quantity = 3;									//!< 体の数の最小値
const float			CCentipede::m_body_distance = 60;										//!< 体との間の距離
const int			CCentipede::m_usually_val = 1;											//!< 通常スピード

const float			CCentipede::m_max_hp_add = 3;											//!< 最大HPの加算値
const float			CCentipede::m_max_attack_power_add = 5;									//!< 最大攻撃力の加算値

/*
 *  コンストラクタ
 */
CCentipede::
CCentipede(void)
	: ICharacter(m_size, m_size, m_head_name, CHARACTER_CATEGORY::ENEMY, CHARACTER_ID::CENTIPEDE)
	, m_DurationTimer(m_upgrade_status[(int)UPGRADE_STATUS_ID::RUSH_DURATION])					//!< 持続タイマー
	, m_IntervalFlag(false)					//!< インターバルフラグ
	, m_ChargeFlag(false)					//!< チャージフラグ
	, m_StillnessFlag(false)
	, m_Interval(0.0f)
	, m_StillnessTimer(0.0f)
	, m_TargetBodyQuantity(0.0f)
	, m_BodyQuantity(0.0f)
{


}

/*
 *  デストラクタ
 */
CCentipede::
~CCentipede(void)
{
}

/*
 *	初期化
 */
void
CCentipede::
Initialize(const vivid::Vector2& position)
{
	CHudManager& ch = CHudManager::GetInstance();

	// 基底クラスのInitializeを呼ぶ
	ICharacter::Initialize(position);

	//リストの初期化
	m_BodyPartsList.clear();

	m_Color = 0xffffffff;
	m_Rotation = DEG_TO_RAD(90);

	// 代入
	m_Interval = m_upgrade_status[(int)UPGRADE_STATUS_ID::RUSH_COOLTIME];
	m_StillnessTimer = m_stillnesstimer;

	//体の数初期化
	m_BodyQuantity = 0;
	m_TargetBodyQuantity = m_default_body_quantity;

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

		//ステータスの中に HP,満腹度,攻撃力,速度
		m_Status[i] = m_status[i][(int)STATUS_DEFINITION::DEFAULT] + add_value;
		m_MaxStatus[i] = m_status[i][(int)STATUS_DEFINITION::DEFAULT] + add_value;
	}

	m_Val = m_usually_val;

	//アイコンの生成
	ch.GetCreate();
}

/*
 *	更新
 */
void
CCentipede::
Update(void)
{
	ICharacter::Update();

	BodyPartsList::iterator it = m_BodyPartsList.begin();

	while (it != m_BodyPartsList.end())
	{
		CParts* parts = nullptr;

		parts = (*it);
		parts->Update(m_body_distance);
		++it;
	}

	//デフォルトの体の長さまで増やす
	if (m_BodyQuantity < m_TargetBodyQuantity)
	{
		this->Createbody();
	}
}

/*
 *	描画
 */
void
CCentipede::
Draw(void)
{
	// 無敵時間中は点滅し、平常時は常に表示される
	if (m_InvincibleTime <= 0 || (int)(m_InvincibleTime / m_invincible_visible_interval) % 2 == 0)
	{
		//体の描画
		BodyPartsList::iterator it = m_BodyPartsList.begin();
		while (it != m_BodyPartsList.end())
		{
			CParts* parts = nullptr;

			parts = (*it);
			parts->Draw();
			++it;
		}

		//頭の描画
		ICharacter::Draw();
	}
}

/*
 *	解放
 */
void
CCentipede::
Finalize(void)
{
	ICharacter::Finalize();


	// 体のパーツを全て削除
	BodyPartsList::iterator it = m_BodyPartsList.begin();
	while (it != m_BodyPartsList.end())
	{
		CParts* parts = (*it);
		parts->Finalize();
		parts->SetActive(false);
		delete parts;
		++it;
	}
	m_BodyPartsList.clear();
}


/*
 *	体生成
 */
void CCentipede::Createbody(void)
{

	if (m_BodyQuantity >= m_max_body_quantity) return;

	CParts* body = nullptr;

	body = new CParts(m_size, m_size, m_body_name, CHARACTER_CATEGORY::ENEMY, CHARACTER_ID::CATERPILLAR);

	BodyPartsList::iterator end = m_BodyPartsList.end();

	//パーツの初期化
	if (m_BodyQuantity == 0)
		body->Initialize(BODY_PART::BODY, (ICharacter*)this, m_Position);
	else
	{
		body->Initialize(BODY_PART::BODY, *--end, m_Position);
	}
	// パーツリストに追加
	m_BodyPartsList.push_back(body);

	++m_BodyQuantity;
}

/*
 *	弾との当たり判定
 */
bool
CCentipede::
CheckHitBullet(IBullet* bullet)
{
	// 弾が存在しない、もしくは同じカテゴリ、無敵状態、死亡状態なら処理を行わない
	if (!bullet || m_CharacterCategory == bullet->GetBulletCategory() || m_InvincibleFlg || m_CharacterAliveState == CHARACTER_ALIVE_STATE::DEAD)
		return false;

	CSoundManager::GetInstance().SetVolume(SOUND_ID::THREAD_ATTACK_DAMAGE, 9000);

	BodyPartsList::iterator it = m_BodyPartsList.begin();

	while (it != m_BodyPartsList.end())
	{
		CParts* parts = (*it);

		// 当たり判定チェック
		// 体のパーツ、もしくはプレイヤー本体に当たったら処理
		if (u_CheckHitObject(bullet->GetPosition(), bullet->GetWidth(), bullet->GetHeight(),
			parts->GetPosition() + vivid::Vector2(15.0f, 15.0f), parts->GetWidth() - 15, parts->GetHeight() - 15, parts->GetRotation()) ||
			u_CheckHitObject(bullet->GetPosition(), bullet->GetWidth(), bullet->GetHeight(),
				this->GetPosition() + vivid::Vector2(15.0f, 15.0f), this->GetWidth() - 15, this->GetHeight() - 15, this->GetRotation()))
		{
			bullet->SetActive(false);

			m_Status[(int)STATUS_ID::HP] -= bullet->GetStatus(BULLET_STATUS_ID::DAMAGE);

			//敵のヒットサウンド再生
			CSoundManager::GetInstance().Play(SOUND_ID::THREAD_ATTACK_DAMAGE, false);

			m_InvincibleTime = m_max_invincible_time;
			m_InvincibleFlg = true;
			return true;
		}
		++it;
	}

	return false;
}

/*
 *	突進の当たり判定
 */
bool
CCentipede::
RushCheckHit(ICharacter* player)
{
	// 自キャラが存在しない、もしくは同じカテゴリ、無敵状態、死亡状態なら処理を行わない
	if (!player || m_CharacterCategory == player->GetCharacterCategory() || m_InvincibleFlg || m_CharacterAliveState == CHARACTER_ALIVE_STATE::DEAD)
		return false;

	CSoundManager::GetInstance().SetVolume(SOUND_ID::ENEMY_RUSH_DAMAGE, 8000);

	BodyPartsList::iterator it = m_BodyPartsList.begin();

	while (it != m_BodyPartsList.end())
	{
		CParts* parts = (*it);

		// 当たり判定チェック
		// 体のパーツ、もしくは頭に当たったら処理
		if (u_CheckHitObject(player->GetPosition(), player->GetWidth(), player->GetHeight(),
			parts->GetPosition() + vivid::Vector2(15.0f, 15.0f), parts->GetWidth() - 15, parts->GetHeight() - 15, parts->GetRotation()) ||
			u_CheckHitObject(player->GetPosition(), player->GetWidth(), player->GetHeight(),
				this->GetPosition() + vivid::Vector2(15.0f, 15.0f), this->GetWidth() - 15, this->GetHeight() - 15, this->GetRotation()))
		{
			m_Status[(int)STATUS_ID::HP] -= player->GetStatus(STATUS_ID::ATTACKPOWER) * player->GetUpgradeStatus(UPGRADE_STATUS_ID::RUSH_DAMAGE_MAG);

			//敵のヒットサウンド再生
			CSoundManager::GetInstance().Play(SOUND_ID::ENEMY_RUSH_DAMAGE, false);

			m_InvincibleTime = m_max_invincible_time;
			m_InvincibleFlg = true;

			return true;
		}
		++it;
	}

	return false;
}

/*
 *	操作
 */
void
CCentipede::
Move(void)
{
	CAttackManager& am = CAttackManager::GetInstance();

	BodyPartsList::iterator it = m_BodyPartsList.begin();

	//	一番近いプレイヤーを参照する
	ICharacter* nearplayer = CCharacterManager::GetInstance().FindNearPlayer(this);
	ICharacter* player = CCharacterManager::GetInstance().GetPlayer();

	//プレイヤーの情報かつ静止フラグがfalse
	if (nearplayer && !m_StillnessFlag)
	{
		//プレイヤーの中心座標から敵の中心座標を引く
		vivid::Vector2 v = nearplayer->GetCenterPosition() - this->GetCenterPosition();

		//追尾
		float dir = atan2(v.y, v.x);
		//常にプレイヤーの方に向く
		m_Rotation = atan2(m_Velocity.y, m_Velocity.x);

		//速度計算を代入
		m_Velocity = am.Rush(m_Velocity, this->GetStatus(STATUS_ID::SPEED), dir, m_Val);

		//チャージフラグがfalseは普通のスピード
		if (!m_ChargeFlag)
		{
			m_Val = m_usually_val;
		}
		//	加速
		else
		{
			m_Val = m_upgrade_status[(int)UPGRADE_STATUS_ID::RUSH_SPEED];
			m_DurationTimer -= vivid::GetDeltaTime();
			while (it != m_BodyPartsList.end())
			{
				CParts* parts = (*it);
				++it;
			}
		}
		//突進タイマーが0以下になったら
		if (m_DurationTimer <= 0)
		{
			m_ChargeFlag = false;
			m_DurationTimer = m_upgrade_status[(int)UPGRADE_STATUS_ID::RUSH_DURATION];
			m_Interval = m_upgrade_status[(int)UPGRADE_STATUS_ID::RUSH_COOLTIME];
		}
	}

	ICharacter::Move();
}

/*
 *	攻撃
 */
void
CCentipede::
Attack(void)
{
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
			//描画範囲に入ってきて突進するまでのインターバル
			m_Interval -= vivid::GetDeltaTime();
			if (m_Interval <= 0.0f)
			{
				m_ChargeFlag = true;
			}
		}
	}
}