
/*!
 *  @file			game_main.h
 *  @brief			ゲームメインシーン
 *  @author			Ryusei Shimizu
 *  @date			2026/09/30
 */

#include "game_main.h"
#include "../../../scene_manager.h"
#include "../../../../game_object.h"
#include "../../../../stationery_manager/stationery/ruler/ruler.h"

/*
 *	コンストラクタ
 */
CGameMain::
CGameMain (void)
	: IScene("GameMain")
{
}

/*
 *	初期化
 */
void
CGameMain::
Initialize (void)
{
	CStageManager::GetInstance ().Initialize();
	CCharacterManager::GetInstance ().Initialize({100.0f, 100.0f}, 64, 64);

	// 文房具管理の初期化と、表示確認用定規の生成
	CStationeryManager::GetInstance().Initialize();
}

/*
 *	更新
 */
void
CGameMain::
Update (void)
{
	CStageManager::GetInstance ().Update();
	// マウス操作を反映してからプレイヤーの衝突を更新する
	CStationeryManager::GetInstance().Update();
	CCharacterManager::GetInstance ().Update ();

	if (vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::Q))
	{
		CRuler* ruler = CStationeryManager::GetInstance().CreateRuler({ 400.0f, 240.0f });

		if (ruler)
		{
			ruler->SetRotation(0.0f);
			ruler->Place();
		}
	}

	if (vivid::keyboard::Trigger(vivid::keyboard::KEY_ID::Z))
		CSceneManager::GetInstance().ChangeMainScene(MAIN_SCENE_ID::RESULT);
	if(vivid::keyboard::Trigger (vivid::keyboard::KEY_ID::X))
		CSceneManager::GetInstance ().ChangeSubScene(SUB_SCENE_ID::PAUSE);
}

/*
 *	描画
 */
void
CGameMain::
Draw (void)
{
	CStageManager::GetInstance ().Draw();
	CStationeryManager::GetInstance().Draw();
	CCharacterManager::GetInstance ().Draw ();

	IScene::Draw();
}

/*
 *	解放
 */
void
CGameMain::
Finalize (void)
{
	CStageManager::GetInstance ().Finalize();
	CCharacterManager::GetInstance ().Finalize ();
	CStationeryManager::GetInstance().Finalize();
}
