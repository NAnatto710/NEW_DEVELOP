
/*
 *  @file       enemy_temporary.cpp
 *  @brief      毛虫 クラス
 *  @author     Misaki Kawada
 *  @date       2026/02/04
 */

#include "caterpillar.h"
#include "../../../parameter_manager/parameter_manager.h"
#include "../../../sound_manager/sound_manager.h"


 //!< ステータス配列
const float CCaterpillar::m_status[(int)STATUS_ID::MAX][(int)STATUS_DEFINITION::MAX] =
{
	//  初期値		最大値		最小値
	{    26.0f,		200.0f,		 0.0f },	//!< HP
	{     0.0f,		  0.0f,		 0.0f },	//!< 満腹度
	{     0.0f,		  0.0f,		 0.0f },	//!< 攻撃力
	{     1.7f,	      2.0f,	     0.0f },	//!< 速度
	{	 10.0f,		 10.0f,		10.0f }, 	//!< 触れたときのダメージ
};

//強化ステータス配列
const float CCaterpillar::m_upgrade_status[(int)UPGRADE_STATUS_ID::MAX] =
{
	0,			//!< 弾のダメージ倍率
	0,			//!< 弾のクールタイム
	0,			//!< 弾の持続時間
	0,			//!< 弾のスピード
	0,			//!< 突進のダメージ倍率
	0,			//!< 突進のクールタイム
	0,			//!< 突進の持続時間
	0,			//!< 突進のスピード
};


const int			CCaterpillar::m_size = 90;											//!< サイズ
const std::string	CCaterpillar::m_head_name = "data\\character\\caterpillar_head.png";		//!< 頭の画像名
const std::string	CCaterpillar::m_body_name = "data\\character\\caterpillar_body.png";		//!< 体の画像名
const int			CCaterpillar::m_default_body_quantity = 5;											//!< 体の数の初期値
const int			CCaterpillar::m_max_body_quantity = 10;											//!< 体の数の最大値
const int			CCaterpillar::m_min_body_quantity = 3;											//!< 体の数の最小値
const float			CCaterpillar::m_body_distance = 50;											//!< 体との間の距離
const int			CCaterpillar::m_usually_val = 1;											//!< 通常の時の数値

const float			CCaterpillar::m_max_hp_add = 9;											//!< 最大HPの加算値

/*
 *  コンストラクタ
 */
CCaterpillar::
CCaterpillar(void)
	: ICharacter(m_size, m_size, m_head_name, CHARACTER_CATEGORY::ENEMY, CHARACTER_ID::CATERPILLAR)
	, m_TargetBodyQuantity(0.0f)
	, m_BodyQuantity(0.0f)
{
}

/*
 *  デストラクタ
 */
CCaterpillar::
~CCaterpillar(void)
{
}

/*
 *	初期化
 */
void
CCaterpillar::
Initialize(const vivid::Vector2& position)
{
	CHudManager& ch = CHudManager::GetInstance();
	// 基底クラスのInitializeを呼ぶ
	ICharacter::Initialize(position);
	//リストの初期化
	m_BodyPartsList.clear();

	m_Color = 0xffffffff;
	m_Rotation = DEG_TO_RAD(90);


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

		//ステータスの中に HP,満腹度,攻撃力,速度
		m_Status[i] = m_status[i][(int)STATUS_DEFINITION::DEFAULT] + add_value;
		m_MaxStatus[i] = m_status[i][(int)STATUS_DEFINITION::DEFAULT] + add_value;
	}

}

/*
 *	更新
 */
void
CCaterpillar::
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
CCaterpillar::
Draw(void)
{
	// 無敵時間中は点滅し、平常時は常に表示される
	if (m_InvincibleTime <= 0 || (int)(m_InvincibleTime / m_invincible_visible_interval) % 2 == 0)
	{
		ICharacter::Draw();

		BodyPartsList::iterator it = m_BodyPartsList.begin();
		while (it != m_BodyPartsList.end())
		{
			CParts* parts = nullptr;

			parts = (*it);
			parts->Draw();
			++it;
		}
	}
}

/*
 *	解放
 */
void
CCaterpillar::
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
void
CCaterpillar::
Createbody(void)
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
 * 弾との当たり判定
 */
bool CCaterpillar::CheckHitBullet(IBullet* bullet)
{
	// 弾が存在しない、もしくは同じカテゴリ、無敵状態、死亡状態なら処理を行わない
	if (!bullet || m_CharacterCategory == bullet->GetBulletCategory() || m_InvincibleFlg || m_CharacterAliveState == CHARACTER_ALIVE_STATE::DEAD)
		return false;

	//敵ダメージサウンド調節
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
 *	突進した時の判定
 */
bool CCaterpillar::RushCheckHit(ICharacter* player)
{
	// 自キャラが存在しない、もしくは同じカテゴリ、無敵状態、死亡状態なら処理を行わない
	if (!player || m_CharacterCategory == player->GetCharacterCategory() || m_InvincibleFlg || m_CharacterAliveState == CHARACTER_ALIVE_STATE::DEAD)
		return false;

	//敵ダメージサウンド調節
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
CCaterpillar::
Move(void)
{
	//	一番近いプレイヤーを参照する
	ICharacter* nearplayer = CCharacterManager::GetInstance().FindNearPlayer(this);
	ICharacter* player = CCharacterManager::GetInstance().GetPlayer();

	//プレイヤーの中心座標から敵の中心座標を引く
	vivid::Vector2 v = nearplayer->GetCenterPosition() - this->GetCenterPosition();

	//追尾
	float dir = atan2(v.y, v.x);

	//常にプレイヤーの方に向く
	m_Rotation = atan2(m_Velocity.y, m_Velocity.x);

	//速度計算を代入
	m_Accelerator.x = this->GetStatus(STATUS_ID::SPEED) * cos(dir);
	m_Accelerator.y = this->GetStatus(STATUS_ID::SPEED) * sin(dir);

	ICharacter::Move();
}

