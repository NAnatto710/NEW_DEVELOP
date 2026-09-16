
/*!
 *  @file       icon.h
 *  @brief      アイコン
 *  @author     Misaki Kawada
 *  @date       2026/02/12
 */

#include "icon.h"
#include "../../../../stage_manager/stage_object/object/feed/feed.h"

const int CIcon::m_player_size = 30;							//!< プレイヤーサイズ
const float	CIcon::m_enemy_size = 20.0f;						//!< 敵サイズ
const float	CIcon::m_between = 50.0f;						//!< 間
const float CIcon::m_player_size_radius = m_player_size / 2;			//!< プレイヤーサイズの半径
const float CIcon::m_enemy_size_radius = m_enemy_size / 2;				//!< 敵サイズの半径
const std::string CIcon::m_player_file_path = "data\\hud\\player_icon.png";	//!< プレイヤーファイルパス
const std::string CIcon::m_enemy_file_path = "data\\hud\\enemy_icon.png";	//!< 敵ファイルパス
const float CIcon::m_size_range = m_enemy_size * 10;			//!< ミニマップの描画範囲
/*
 *	コンストラクタ
 */
CIcon::
CIcon(void)
	:m_PlayerRect({ 0,0,(int)m_player_size,(int)m_player_size })
	, m_PlayerAnchor(m_player_size_radius, m_player_size_radius)
	, m_PlayerScale(1.0f, 1.0f)
	, m_PlayerRotation(0.0f)
	, m_EnemyRect({ 0,0,(int)m_enemy_size,(int)m_enemy_size })
	, m_EnemyAnchor(m_enemy_size_radius, m_enemy_size_radius)
	, m_EnemyScale(1.0f, 1.0f)
	, m_EnemyRotation(0.0f)
	, m_FeedRotation(0.0f)
	, m_FeedRect({ 0,0,(int)m_enemy_size,(int)m_enemy_size })
	, m_FeedAnchor(m_enemy_size_radius, m_enemy_size_radius)
	, m_FeedScale(0.5f, 0.5f)

{
}

/*
 *	初期化
 */
void
CIcon::
Initialize(void)
{
	CHudManager& ch = CHudManager::GetInstance();


	//ミニマップ
	m_PlayerPosition.x = { ch.MapPosition().x + (ch.MapSize() / 2) - m_player_size_radius };
	m_PlayerPosition.y = { ch.MapPosition().y + (ch.MapSize() / 2) - m_player_size_radius };
	m_PlayerCenterPosition.x = { m_PlayerPosition.x + m_player_size_radius };
	m_PlayerCenterPosition.y = { m_PlayerPosition.y + m_player_size_radius };
	m_EnemyPosition = { 0.0f,0.0f };





}

/*
 * 更新
 */
void
CIcon::
Update(void)
{
	// プレイヤーアイコン動作
	this->PlayerIconMove();
}
/*
 *	描画
 */
void
CIcon::
Draw(void)
{
	// プレイヤーアイコン描画
	vivid::DrawTexture(m_player_file_path, m_PlayerPosition, 0xffffffff, m_PlayerRect, m_PlayerAnchor, m_PlayerScale, m_PlayerRotation);

	// 敵アイコン描画
	EnemyDraw();

	//餌アイコン描画
	FeedDraw();
}

/*
 *	解放
 */
void
CIcon::
Finalize(void)
{

}

/*
 *	プレイヤーアイコン動作
 */
void CIcon::PlayerIconMove(void)
{
	ICharacter* player = CCharacterManager::GetInstance().GetPlayer();

	if (!player)return;

	if (player)
	{
		m_PlayerRotation = player->GetRotation() + DEG_TO_RAD(90);
	}
}

/*
 *	敵ポジション探知
 */
void CIcon::EnemyDraw(void)
{
	CCameraManager& cm = CCameraManager::GetInstance();
	CHudManager& ch = CHudManager::GetInstance();

	using ENEMY_LIST = std::list<ICharacter*>;

	ENEMY_LIST		m_EnemyList;

	//リストの情報を持ってくる
	m_EnemyList = CCharacterManager::GetInstance().GetEnemyList();

	ICharacter* enemy = nullptr;

	ICharacter* player = CCharacterManager::GetInstance().GetPlayer();

	ENEMY_LIST::iterator it = m_EnemyList.begin();

	while (it != m_EnemyList.end())
	{
		enemy = (*it);

		//プレイヤーのローカル座標から描画範囲指定したものを引く
		vivid::Vector2 drawrange = player->GetCenterPosition() - vivid::Vector2(ch.DrawRange() / 2, ch.DrawRange() / 2);

		//vivid::DrawText(40, std::to_string(drawrange.x), vivid::Vector2(700, 700));
		vivid::Vector2 enemyposition = enemy->GetCenterPosition();

		//指定した範囲内に入っていれば描画
		if (drawrange.x < enemyposition.x
			&& enemyposition.x < drawrange.x + ch.DrawRange() - m_size_range
			&& drawrange.y < enemyposition.y
			&& enemyposition.y < drawrange.y + ch.DrawRange() - m_size_range)
		{

			//描画範囲に対してミニマップがどれくらいの比率かの計算
			float scale = ch.MapSize() / ch.DrawRange();
			//敵の中心部分から描画範囲の
			vivid::Vector2 distance = enemyposition - drawrange;
			//ミニマップの座標に足す座標
			vivid::Vector2 minimappos = distance * scale;
			//マップのポジションをミニマップに反映
			m_EnemyPosition = ch.MapPosition() + minimappos;
			//vivid::Vector2 enemypos = m_EnemyPostion - vivid::Vector2(m_enemy_size_radius, m_enemy_size_radius);



			m_EnemyRotation = enemy->GetRotation() + DEG_TO_RAD(90);

			//蜂のみ回転値変える
			if (enemy->GetCharacterID() == CHARACTER_ID::BEE)
			{
				m_EnemyRotation = enemy->GetRotation() + DEG_TO_RAD(180);
			}

			if (enemy->GetCharacterCategory() == CHARACTER_CATEGORY::ENEMY)
			{
				m_EnemyPosition.x = u_Clamp(m_EnemyPosition.x, ch.MapPosition().x, ch.MapPosition().x + ch.MapSize());
				m_EnemyPosition.y = u_Clamp(m_EnemyPosition.y, ch.MapPosition().y, ch.MapPosition().y + ch.MapSize());
				vivid::DrawTexture(m_enemy_file_path, m_EnemyPosition, 0xffffffff, m_EnemyRect, m_PlayerAnchor, m_EnemyScale, m_EnemyRotation);

			}
		}

		++it;
	}


}

void CIcon::FeedDraw(void)
{
	CCameraManager& cm = CCameraManager::GetInstance();
	CHudManager& ch = CHudManager::GetInstance();


	using FEED_LIST = std::list<IStageObject*>;

	FEED_LIST			m_FeedList;

	//リスト代入
	m_FeedList = CStageManager::GetInstance().GetFeedObject();

	IStageObject* feed = nullptr;

	ICharacter* player = CCharacterManager::GetInstance().GetPlayer();

	FEED_LIST::iterator it = m_FeedList.begin();

	while (it != m_FeedList.end())
	{
		feed = (*it);

		//プレイヤーのローカル座標から描画範囲指定したものを引く
		vivid::Vector2 drawrange = player->GetCenterPosition() - vivid::Vector2(ch.DrawRange() / 2, ch.DrawRange() / 2);

		//vivid::DrawText(40, std::to_string(drawrange.x), vivid::Vector2(700, 700));
		vivid::Vector2 feedposition = feed->GetCenterPosition();

		//指定した範囲内に入っていれば描画
		if (drawrange.x < feedposition.x
			&& feedposition.x < drawrange.x + ch.DrawRange() - m_size_range
			&& drawrange.y < feedposition.y
			&& feedposition.y < drawrange.y + ch.DrawRange() - m_size_range)
		{

			//描画範囲に対してミニマップがどれくらいの比率かの計算
			float scale = ch.MapSize() / ch.DrawRange();
			//敵の中心部分から描画範囲の
			vivid::Vector2 distance = feedposition - drawrange;
			//ミニマップの座標に足す座標
			vivid::Vector2 minimappos = distance * scale;
			//マップのポジションをミニマップに反映
			m_FeedPosition = ch.MapPosition() + minimappos;
			//vivid::Vector2 enemypos = m_EnemyPostion - vivid::Vector2(m_enemy_size_radius, m_enemy_size_radius);
			m_FeedRotation = DEG_TO_RAD(180);


			if (feed->GetStageObjectID() == STAGE_OBJECT_ID::FEED_OBJECT)
			{
				m_FeedPosition.x = u_Clamp(m_FeedPosition.x, ch.MapPosition().x, ch.MapPosition().x + ch.MapSize());
				m_FeedPosition.y = u_Clamp(m_FeedPosition.y, ch.MapPosition().y, ch.MapPosition().y + ch.MapSize());
				vivid::DrawTexture(m_enemy_file_path, m_FeedPosition, 0xffffff00, m_FeedRect, m_FeedAnchor, m_FeedScale, m_FeedRotation);

			}



		}

		++it;
	}


}
