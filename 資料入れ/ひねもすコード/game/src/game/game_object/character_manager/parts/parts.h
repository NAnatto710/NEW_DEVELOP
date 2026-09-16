
/*!
 *  @file       parts.h
 *  @brief      パーツクラス
 *  @author     Ryusei Shimizu
 *  @date       2026/01/22
 */

#pragma once

#include "vivid.h"
#include "../character_id.h"
#include "../character/character.h"
#include "../../character_manager/character_manager.h"

/*
 *	@brief	体の部位
 */
enum class BODY_PART
{
	HEAD,	//!< 頭
	BODY,	//!< 体

	MAX,	//!< 最大値
};

/*!
  *  @class      CParts
  *
  *  @brief      パーツクラス
  *
  *  @author     Ryusei Shimizu
  *
  *  @date       2026/01/22
  */
class CParts
	:public ICharacter
{
public:

	/*!
	 *  @brief      コンストラクタ
	 *
	 *  @param[in]  width           横幅
	 *  @param[in]  height          高さ
	 *  @param[in]  name            画像名
	 *  @param[in]  category        キャラクター識別子
	 */
	CParts(int width, int height, std::string name, CHARACTER_CATEGORY category, CHARACTER_ID character_id);

	/*!
	 *  @brief      デストラクタ
	 */
	~CParts(void);

	/*!
	 *  @brief      初期化
	 *
	 *  @param[in]  parts           部位
	 *  @param[in]  target_parts    対象パーツ
	 *  @param[in]  position        初期位置
	 */
	void				Initialize(BODY_PART parts, ICharacter* target_parts, const vivid::Vector2& position);

	/*!
	 *  @brief      更新
	 */
	void				Update(void);

	/*!
	 *  @brief      更新
	 * 
	 *	@param[in]	bodydistance	体との間の距離
	 */
	void				Update(float bodydistance);

	/*!
	 *  @brief      描画
	 */
	void				Draw(void);

	/*!
	 *  @brief      描画
	 */
	void				Draw(unsigned int color);

	/*!
	 *  @brief      解放
	 */
	void				Finalize(void);

	/*!
	 *	@brief		移動停止
	 */
	void				StopMove(void);

	/*!
	 *	@brief		目標パーツ取得
	 *
	 *	@return		目標パーツ
	 */
	ICharacter*			GetTargetParts(void)const;

	/*!
	 *  @brief      目標パーツ設定
	 *
	 *  @param[in]  parts   部位
	 */
	void                SetTarget(CParts& parts);

	/*!
	 *	@brief		部位取得
	 *
	 *	@return		部位
	 */
	BODY_PART			GetBodyPart(void)const;

	/*!
	 *  @brief      速度設定
	 *
	 *  @param[in]  velocity	速度
	 */
	void				SetVeloctiy(vivid::Vector2 velocity);

	/*!
	 *  @brief      回転設定
	 *
	 *  @param[in]  rotation   回転
	 */
	void				SetRotation(float rotation);


	/*!
	 *  @brief      ユニット識別子取得
	 *
	 *  @return     ユニット識別子
	 */
	CHARACTER_CATEGORY  GetCharacterCategory(void)const;


	/*
	 *  @brief  動作
	 */
	void				Move(void);

	/*
	 *  @brief  追従移動
	 *
	 *  @param[in]  bodydistance   体との間の距離
	 */
	void				ForrowMove(float bodydistance);


private:

	static const float              m_friction;			            //!< 移動時の摩擦力
	static const float              m_body_distance;                //!< 体の間の距離
	static const float              m_body_extension_speed;			//!< 体が伸びる速度
	static const float              m_body_extension_max_timer;     //!< 体の伸縮最大タイマー
	static const float              m_body_extension_min_timer;     //!< 体の伸縮最小タイマー

	vivid::Vector2					m_TargetPosition;				//!< 目標位置
	bool							m_ForrowMoveFlg;                //!< 追従フラグ
	bool							m_UpdirectionFlg;               //!< 上方向フラグ
	bool							m_ExtensionFlg;					//!< 伸縮フラグ
	float							m_ExtensionTimer;				//!< 伸縮タイマー

	BODY_PART						m_BodyPart;						//!< 部位

	ICharacter*						m_TargetParts;					//!< 目標パーツ
};