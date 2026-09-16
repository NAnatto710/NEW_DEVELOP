
/*!
 *  @file       character.h
 *  @brief      キャラクターベースクラス
 *  @author     Ryusei Shimizu
 *  @date       2025/10/15
 */

#pragma once

#include "vivid.h"
#include "../character_id.h"
#include "character_status_id.h"
#include "../../bullet_manager/bullet_object/bullet.h"
#include "../../upgrade_manager/upgrade_status/upgrade_status_id.h"

/*!
 *  @class		 ICharacter
 *
 *  @brief      キャラクターベースクラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2025/10/15
 */
class ICharacter
{
public:

	/*!
	 *	@brief	コンストラクタ
	 *
	 *	@param[in]	width			幅
	 *	@param[in]	height			高さ
	 *	@param[in]	name			画像名
	 *	@param[in]	category		キャラクター識別子
	 *	@param[in]	character_id	キャラクターID
	 */
	ICharacter(int width, int height, std::string name, CHARACTER_CATEGORY category, CHARACTER_ID character_id);

	/*!
	 *	@brief	デストラクタ
	 */
	virtual ~ICharacter(void);

	/*
	 *	@brief	初期化
	 *
	 *	@param[in]	position	位置
	 */
	virtual void        Initialize(const vivid::Vector2& position);

	/*!
	 *  @brief      更新
	 */
	virtual void	    Update(void);

	/*!
	 *  @brief      描画
	 */
	virtual void	    Draw(void);

	/*!
	 *  @brief      解放
	 */
	virtual void	    Finalize(void);

	/*!
	 *  @brief      キャラクターID取得
	 *
	 *  @return     キャラクターID
	 */
	CHARACTER_ID        GetCharacterID(void)const;

	/*!
	 *  @brief      位置取得
	 *
	 *  @return     位置
	 */
	vivid::Vector2      GetPosition(void)const;

	/*!
	 *  @brief      位置設定
	 *
	 *  @param[in]  position    位置
	 */
	void                SetPosition(const vivid::Vector2& position);

	/*!
	 *  @brief      中心位置取得
	 *
	 *  @return     中心位置
	 */
	vivid::Vector2      GetCenterPosition(void)const;

	/*!
	 *  @brief      横幅取得
	 *
	 *  @return     横幅
	 */
	int                 GetWidth(void)const;

	/*!
	 *  @brief      高さ取得
	 *
	 *  @return     高さ
	 */
	int                 GetHeight(void)const;

	/*!
	 *  @brief      回転値取得
	 *
	 *	@return		回転値
	 */
	float				GetRotation(void)const;

	/*!
	 *  @brief      アクティブフラグ取得
	 *
	 *  @return     アクティブフラグ
	 */
	bool                IsActive(void)const;

	/*!
	 *  @brief      アクティブフラグ設定
	 *
	 *  @param[in]  active  アクティブフラグ
	 */
	void                SetActive(bool active);

	/*!
	 *  @brief      ユニット識別子取得
	 *
	 *  @return     ユニット識別子
	 */
	CHARACTER_CATEGORY  GetCharacterCategory(void)const;

	/*!
	 *  @brief      ステータス取得
	 *
	 *  @param[in]  status_id       ステータスID
	 *
	 *  @return     ステータス値
	 */
	float				GetStatus(STATUS_ID id) const;

	/*!
	 *  @brief      ステータス最大値取得
	 *
	 *  @param[in]  status_id       ステータスID
	 *
	 *  @return     ステータス最大値
	 */
	float				GetMaxStatus(STATUS_ID id) const;

	/*!
     *  @brief      強化ステータス取得
     *
     *  @param[in]  upgrade_status_id       強化ステータスID
     *
     *  @return     強化ステータス値
     */
	float               GetUpgradeStatus(UPGRADE_STATUS_ID id) const;

	/*!
	 *  @brief      ステータス上昇
	 *
	 *  @param[in]  status_id       ステータスID
	 *  @param[in]  value           ステータス値
	 */
	void				IncreaseStatus(STATUS_ID status_id, float value);

	/*!
	 *	@brief		弾との当たり判定
	 *
	 *	@param[in]	bullet	弾
	 *
	 *	@return		当たったかどうか
	 */
	virtual bool		CheckHitBullet(IBullet* bullet);

	/*!
	 *	@brief		突進の当たり判定
	 *
	 *	@param[in]	player	プレイヤー
	 *
	 *	@return		当たったかどうか
	 */
	virtual bool		RushCheckHit(ICharacter* player);

protected:

	/*
	 *  @brief  生存
	 */
	virtual void		Alive(void);

	/*
	 *  @brief  死亡
	 */
	virtual void		Dead(void);

	/*
	 *	@brief  攻撃
	 */
	virtual void		Attack(void);

	/*
	 *  @brief  発射
	 */
	virtual void        Fire(void);

	/*
	 *  @brief  無敵更新
	 */
	virtual void        invincibleUpdate(void);

	/*
	 *  @brief  動作
	 */
	virtual void		Move(void);

	/*
	 *  @brief  ブロックとの当たり判定
	 */
	virtual void		CheckHitBlock(void);


	static const float				m_max_invincible_time;			//!< 無敵時間
	static const float				m_invincible_visible_interval;	//!< 無敵時間中の点滅間隔
	static const float              m_friction;			            //!< 移動時の摩擦力

	int								m_Width;						//!< 幅
	int								m_Height;						//!< 高さ
	std::string						m_PathName;						//!< 画像名
	vivid::Vector2					m_Accelerator;				    //!< 加速度
	vivid::Vector2					m_PreviousPosition;			    //!< 前フレームの位置
	vivid::Vector2                  m_CenterPosition;				//!< 中心位置
	vivid::Vector2					m_Position;						//!< 位置
	vivid::Vector2					m_Velocity;						//!< 速度
	vivid::Rect						m_Rect;							//!< 読み込み範囲
	vivid::Vector2					m_Anchor;						//!< 基準点
	vivid::Vector2					m_Scale;						//!< 拡大率
	unsigned int					m_Color;						//!< 色
	float							m_Rotation;						//!< 回転値
	float							m_MoveAccelerator;			    //!< 移動用加速度
	float							m_InvincibleTime;				//!< 無敵時間
	bool							m_MoveFlg;						//!< 移動したかどうかの判定
	bool							m_ActiveFlg;					//!< アクティブフラグ
	bool							m_InvincibleFlg;				//!< 無敵フラグ

	CHARACTER_CATEGORY				m_CharacterCategory;			//!< キャラクター識別子
	CHARACTER_ID					m_CharacterID;					//!< キャラクターID
	CHARACTER_ALIVE_STATE			m_CharacterAliveState;			//!< キャラクター生存状態

	float							m_Status[(int)STATUS_ID::MAX];					//!< ステータスの値
	float							m_MaxStatus[(int)STATUS_ID::MAX];				//!< ステータスの最大値
	float                           m_UpgradeStatus[(int)UPGRADE_STATUS_ID::MAX];	//!< 強化ステータス配列
};