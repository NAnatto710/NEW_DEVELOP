
/*!
 *  @file       upgrade_manager.cpp
 *  @brief      アップグレードステータス管理
 *  @author     Ryusei Shimizu
 *  @date       2026/02/20
 */

#include "upgrade_manager.h"
#include "../scene_manager/scene_manager.h"
#include "../sound_manager/sound_manager.h"
#include "../../../utility/utility.h"


const std::string			CUpgraeStatusManager::m_background_path				= "data\\object\\white.png";							//!< 背景のテクスチャパス
const std::string			CUpgraeStatusManager::m_select_scene_logo_path		= "data\\background\\select_scene_logo.png";			//!< 選択シーンロゴのテクスチャパス
const std::string			CUpgraeStatusManager::m_select_button_path			= "data\\background\\select_button.png";				//!< ボックスのテクスチャパス
const std::string			CUpgraeStatusManager::m_select_button_bar_path		= "data\\background\\select_button_bar.png";			//!< ボックスのテクスチャパス
const std::string			CUpgraeStatusManager::m_select_button_hilight_path	= "data\\background\\select_button_hilight.png";		//!< ボックスのハイライトテクスチャパス
const std::string			CUpgraeStatusManager::m_small_tab_path				= "data\\background\\tab1.png";							//!< 小タブのテクスチャパス
const std::string           CUpgraeStatusManager::m_large_tab_path				= "data\\background\\tab2.png";							//!< 大タブのテクスチャパス
const std::string           CUpgraeStatusManager::m_head_graph_path				= "data\\background\\head_graph.png";					//!< ヘッドグラフのテクスチャパス
const std::string           CUpgraeStatusManager::m_body_graph_path				= "data\\background\\body_graph.png";					//!< ボディグラフのテクスチャパス
const std::string			CUpgraeStatusManager::m_upgrade_status_path[4]		=														//!< アップグレードステータス名
{
	"data\\background\\bullet_damage_mag.png",
	"data\\background\\rush_damage_mag.png",
	"data\\background\\rush_duration.png",
	"data\\background\\rush_speed.png"
};

const int					CUpgraeStatusManager::m_select_scene_logo_width		= 500;						//!< 選択シーンロゴの幅
const int					CUpgraeStatusManager::m_select_button_width			= 150;						//!< 選択ボタンの幅
const int					CUpgraeStatusManager::m_select_button_height		= 50;						//!< 選択ボタンの高さ

const float                 CUpgraeStatusManager::m_select_time					= 0.4;                      //!< 選択の時間
const float                 CUpgraeStatusManager::m_select_cooltime				= 0.1;                      //!< 選択のクールタイム
const int                   CUpgraeStatusManager::m_upgrade_status_num			= 4;                        //!< アップグレードステータスの数

const vivid::Vector2        CUpgraeStatusManager::m_select_scene_logo_pos		= { (vivid::WINDOW_WIDTH - m_select_scene_logo_width) / 2.0f, 50.0f };		//!< 選択シーンロゴの位置
const vivid::Vector2        CUpgraeStatusManager::m_small_tab_pos				= { 320.0f, 305.0f };		//!< 小タブの位置
const vivid::Vector2        CUpgraeStatusManager::m_large_tab_pos				= { 830.0f, 305.0f };		//!< 大タブの位置

const vivid::Vector2		CUpgraeStatusManager::m_select_button_pos[4]		=							//!< 選択ボタンの位置
{
	{ 375.0f,  780.0f },
	{ 935.0f,  780.0f },
	{ 1190.0f, 780.0f },
	{ 1445.0f, 780.0f }
};

const vivid::Vector2        CUpgraeStatusManager::m_garph_pos[4][5]				=							//!< グラフの位置
{
	{ { 375.0f,  320.0f },  { 375.0f, 395.0f },  { 375.0f, 470.0f },  { 375.0f, 545.0f },{ 375.0f,  620.0f } },
	{ { 935.0f,  320.0f },  { 935.0f, 395.0f },  { 935.0f, 470.0f },  { 935.0f, 545.0f },{ 935.0f,  620.0f } },
	{ { 1190.0f, 320.0f }, { 1190.0f, 395.0f }, { 1190.0f, 470.0f }, { 1190.0f, 545.0f },{ 1190.0f, 620.0f } },
	{ { 1445.0f, 320.0f }, { 1445.0f, 395.0f }, { 1445.0f, 470.0f }, { 1445.0f, 545.0f },{ 1445.0f, 620.0f } }
};

const UPGRADE_STATUS_ID		CUpgraeStatusManager::m_upgrade_status[4]			=							//!< アップグレードステータスID
{
	UPGRADE_STATUS_ID::BULLET_DAMAGE_MAG,
	UPGRADE_STATUS_ID::RUSH_DAMAGE_MAG,
	UPGRADE_STATUS_ID::RUSH_DURATION,
	UPGRADE_STATUS_ID::RUSH_SPEED
};

const unsigned int			CUpgraeStatusManager::m_upgrade_status_name_color[4] = 						//!< アップグレードステータス名の色
{
	0xff00ff00,		//!< 緑
	0xffff0000,		//!< 赤
	0xffffff00,		//!< 黄
	0xff00ffff		//!< シアン
};

const unsigned int          CUpgraeStatusManager::m_background_color			= 0x60000000;			//!< 背景の色
const unsigned int          CUpgraeStatusManager::m_select_button_bar_color		= 0x60ffffff;			//!< 選択ボタンのバーの色
const vivid::Rect			CUpgraeStatusManager::m_select_button_bar_rect		= { 0, 0, m_select_button_width, m_select_button_height };	//!< 選択ボタンのバーの矩形


/*
 *  インスタンスの取得
 */
CUpgraeStatusManager&
CUpgraeStatusManager::
GetInstance(void)
{
	static CUpgraeStatusManager instance;

	return instance;
}

/*
 *  初期化
 */
void
CUpgraeStatusManager::
Initialize(void)
{
	m_Select = 0;
	m_SelectCoolTimer = m_select_cooltime;
	m_SelectButtonBarRect = m_select_button_bar_rect;
	m_SelectTimer = 0.0f;

	m_UpgradeStatus.Initialize();
}

/*
 *  更新
 */
void
CUpgraeStatusManager::
Update(void)
{
	m_UpgradeStatus.Update();
}

/*
 *  描画
 */
void
CUpgraeStatusManager::
Draw(void)
{
	m_UpgradeStatus.Draw();
}

/*
 *  解放
 */
void
CUpgraeStatusManager::
Finalize(void)
{
	m_UpgradeStatus.Finalize();
}

/*
 *  強化ステータス選択
 */
void
CUpgraeStatusManager::
SelectUpgradeStatus(void)
{
	namespace keyboard = vivid::keyboard;
	namespace controller = vivid::controller;
	namespace mouse = vivid::mouse;

	bool select = false;

	// マウスの当たり判定
	for (int i = 0; i < m_upgrade_status_num; i++)
	{
		if (u_CheckHitMouse(m_select_button_pos[i], m_select_button_width, m_select_button_height))
		{
			m_Select = i;

			if (mouse::Button(mouse::BUTTON_ID::LEFT))
			{
				select = true;
			}
		}
	}

	// コントローラーの選択
	// 左右のトリガーで選択
	if (controller::Trigger(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::LEFT) ||
		keyboard::Trigger(keyboard::KEY_ID::LEFT) ||
		keyboard::Trigger(keyboard::KEY_ID::A))
	{
		m_Select--;
		if (m_Select < 0)
			m_Select = m_upgrade_status_num - 1;

		//セレクト切り替えサウンド
		CSoundManager::GetInstance().Play(SOUND_ID::SELECT_CHARGE, false);
	}
	else if (controller::Trigger(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::RIGHT) ||
			keyboard::Trigger(keyboard::KEY_ID::RIGHT) ||
			keyboard::Trigger(keyboard::KEY_ID::D))
	{
		m_Select++;
		if (m_Select > m_upgrade_status_num - 1)
			m_Select = 0;

		//セレクト切り替えサウンド
		CSoundManager::GetInstance().Play(SOUND_ID::SELECT_CHARGE, false);
	}

	vivid::Vector2 input_dir = { 0.0f,0.0f };
	const float dead_zone = 0.5f; // デッドゾーンの閾値

	// コントローラーの左スティックの入力を取得
	input_dir = controller::GetAnalogStickLeft(controller::DEVICE_ID::PLAYER1);

	// デッドゾーンのチェック
	bool left_stick = abs(input_dir.Length()) > dead_zone;

	if (m_SelectCoolTimer > 0)
		m_SelectCoolTimer -= vivid::GetDeltaTime();

	// 左スティックで選択
	if (left_stick &&
		m_SelectCoolTimer <= 0)
	{
		m_SelectCoolTimer = m_select_cooltime;

		if (input_dir.x < 0)
		{
			m_Select--;
			if (m_Select < 0)
				m_Select = m_upgrade_status_num - 1;

			//セレクト切り替えサウンド
			CSoundManager::GetInstance().Play(SOUND_ID::SELECT_CHARGE, false);
		}
		else if (input_dir.x > 0)
		{
			m_Select++;
			if (m_Select > m_upgrade_status_num - 1)
				m_Select = 0;

			//セレクト切り替えサウンド
			CSoundManager::GetInstance().Play(SOUND_ID::SELECT_CHARGE, false);
		}
	}

	// 決定ボタンで選択
	if (controller::Button(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::B) ||
		controller::Button(controller::DEVICE_ID::PLAYER1, controller::BUTTON_ID::A) ||
		keyboard::Button(keyboard::KEY_ID::RETURN) ||
		keyboard::Button(keyboard::KEY_ID::SPACE) ||
		keyboard::Button(keyboard::KEY_ID::Z))
	{
		select = true;
	}

	bool scene_change = false;

	if (select)
	{
		m_SelectTimer += vivid::GetDeltaTime();

		float rate = m_SelectTimer / m_select_time;
		m_SelectButtonBarRect.right = static_cast<int>(m_select_button_width * rate);

		if (m_SelectTimer >= m_select_time)
		{
			m_SelectTimer = 0.0f;
			scene_change = true;
		}
	}
	else
	{
		m_SelectTimer = 0.0f;
		m_SelectButtonBarRect.right = 0;
	}

	if (scene_change)
	{
		m_UpgradeStatus.SetUpgradeStatusID(m_upgrade_status[m_Select]);
		CSceneManager::GetInstance().ChangeSubScene(SUBSCENE_ID::DUMMY);

		//クリックサウンド
		CSoundManager::GetInstance().Play(SOUND_ID::CLICK, false);
	}
}

/*
 *  強化ステータスグラフの描画
 */
void
CUpgraeStatusManager::
DrawUpgradeStatusGraph(bool flg)
{
	// 背景の描画
	vivid::DrawTexture(m_background_path,vivid::Vector2::ZERO,m_background_color);

	// 選択シーンロゴの描画
	vivid::DrawTexture(m_select_scene_logo_path, m_select_scene_logo_pos);

	// タブの描画
	vivid::DrawTexture(m_small_tab_path, m_small_tab_pos);
	vivid::DrawTexture(m_large_tab_path, m_large_tab_pos);

	if (flg)
	{
		// 選択されたステータスのハイライト描画
		vivid::DrawTexture(m_select_button_hilight_path, m_select_button_pos[m_Select] - vivid::Vector2(2.0f, 2.0f));
	}

	// 選択ボタンとステータス名の描画
	for (int i = 0; i < m_upgrade_status_num; i++)
	{
		if (flg)
		{
			vivid::DrawTexture(m_select_button_path, m_select_button_pos[i]);
			vivid::DrawTexture(m_select_button_bar_path, m_select_button_pos[m_Select], m_select_button_bar_color, m_SelectButtonBarRect);
		}

		vivid::DrawTexture(m_upgrade_status_path[i], m_select_button_pos[i] + vivid::Vector2(-40.0f, -550.0f));

		vivid::DrawTexture(m_head_graph_path, m_garph_pos[i][0], m_upgrade_status_name_color[i]);

		for (int j = 0; j < m_UpgradeStatus.GetUpgradeStatusCount(m_upgrade_status[i]); j++)
		{
			vivid::DrawTexture(m_body_graph_path, m_garph_pos[i][j + 1], m_upgrade_status_name_color[i]);
		}
	}
}

/*
 *  強化ステータスID取得
 */
UPGRADE_STATUS_ID
CUpgraeStatusManager::
GetUpgradeStatusID(UPGRADE_STATUS_ID id) const
{
	return m_UpgradeStatus.GetUpgradeStatusID(id);
}

/*
 *  強化ステータスカウント取得
 */
int
CUpgraeStatusManager::
GetUpgradeStatusCount(UPGRADE_STATUS_ID id) const
{
	return m_UpgradeStatus.GetUpgradeStatusCount(id);
}

/*
 *  強化ステータスID設定
 */
void
CUpgraeStatusManager::
SetUpgradeStatusID(UPGRADE_STATUS_ID id)
{
	m_UpgradeStatus.SetUpgradeStatusID(id);
}

/*
 *  強化倍率取得
 */
float
CUpgraeStatusManager::
GetUpgrade(UPGRADE_STATUS_ID id) const
{
	return m_UpgradeStatus.GetUpgrade(id);
}

/*
 *  強化ステータスID取得
 */
UPGRADE_STATUS_ID
CUpgraeStatusManager::
GetUpgradeStatusID() const
{
	return m_UpgradeStatus.GetUpgradeStatusID();
}

/*
 *  満腹度設定
 */
void
CUpgraeStatusManager::
SetFullness(float value)
{
	m_UpgradeStatus.SetFullness(value);
}

/*
 *  強化ステータスの色取得
 */
unsigned int
CUpgraeStatusManager::
GetUpgradeStatusNameColor(void) const
{
	if (m_UpgradeStatus.GetUpgradeStatusID() == UPGRADE_STATUS_ID::MAX)
		return 0xffffffff;	//!< 白

	// 強化ステータスIDに対応する色を返す
	for (int i = 0; i < m_upgrade_status_num; i++)
		if(m_UpgradeStatus.GetUpgradeStatusID() == m_upgrade_status[i])
			return m_upgrade_status_name_color[i];
}

/*
 *  コンストラクタ
 */
CUpgraeStatusManager::
CUpgraeStatusManager(void)
{
}

/*
 *  コピーコンストラクタ
 */
CUpgraeStatusManager::
CUpgraeStatusManager(const CUpgraeStatusManager& rhs)
{
	(void)rhs;
}

/*
 *  デストラクタ
 */
CUpgraeStatusManager::
~CUpgraeStatusManager(void)
{
}

/*
 *  代入演算子
 */
CUpgraeStatusManager&
CUpgraeStatusManager::
operator=(const CUpgraeStatusManager& rhs)
{
	(void)rhs;
	return *this;
}
