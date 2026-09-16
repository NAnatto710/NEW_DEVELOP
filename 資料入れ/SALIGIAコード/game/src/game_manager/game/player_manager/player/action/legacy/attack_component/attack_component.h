
/*!
 *  @file		attack_component.h
 *  @brief		攻撃コンポーネント
 *  @author     Ryusei Shimizu
 *  @date       2026/05/26
 */

#pragma once

#include "vivid.h"
#include "../../../../../../utility/csv_loader/loader/attack_csv_loader/attack_id.h"

#include <unordered_set>

class CPlayer;

/*!
 *	@class		CircleHitBox
 *
 *	@brief		円形ヒットボックス構造体
 */
struct CircleHitBox
{
	vivid::Vector2 Position;    //!< ヒットボックスの中心位置（キャラクターの中心からの相対位置）

	float Radius = 0.0f;        //!< ヒットボックスの半径

	DamageInfo Damage;          //!< ダメージ情報
};

/*!
 *	@class		CAttackComponent
 *
 *	@brief		攻撃コンポーネントクラス
 *
 *	@author     Ryusei Shimizu
 *
 *  @date       2026/05/26
 */
class CAttackComponent
{
public:

    /*!
      *	@brief	コンストラクタ
	  */
    CAttackComponent();

    /*
     *	@brief	デストラクタ
	 */
	~CAttackComponent(void) = default;

    /*!
      *	@brief	初期化
      * 
      *  @param[in]  owner   所有するキャラクター
	  */
    void        Initialize(CPlayer* owner);

    /*!
     *	@brief	更新
	 */
    void        Update();

    /*!
	 *	@brief	描画
     */
	void        DebugDraw();

    /*!
      *	@brief	解放
	  */
	void        Finalize();

    /*!
      *	@brief	現在の攻撃情報を取得
      *
      *	@return	現在の攻撃情報
	  */
	AttackInfo  GetCurrentAttack() const { return *m_CurrentAttack; }

    /*!
      *	@brief	現在の攻撃情報を設定
      *
	  *	@param[in]	attack  設定する攻撃情報
      */
    void        SetAttack(const AttackInfo& attack);

    /*!
      *	@brief	攻撃の有効フラグを設定
      *
	  *	@param[in]	active  設定するフラグ
	  */
	void        SetActive(bool active) { m_AttackActive = active; }

    /*!
      *	@brief	攻撃の有効フラグを取得
      *
      *	@return	攻撃の有効フラグ
	  */
	bool        IsActive() const { return m_AttackActive; }

    /*!
     *	@brief	行動情報のクリア
	 */
    void        Clear();

    /*!
      *	@brief	現在のヒットボックスを取得
      *
	  *	@return	現在のヒットボックス
      */ 
    const std::vector<CircleHitBox>& GetHitCircles() const;

    /*!
      *	@brief	特定のプレイヤーに既にヒットしているかどうかを確認
      *
      *	@param[in]	target  確認するプレイヤー
      *
	  *	@return	既にヒットしていればtrue、そうでなければfalse
      */ 
    bool        HasHit(CPlayer* target) const;

    /*!
      *	@brief	特定のプレイヤーをヒット対象として追加
      *
      *	@param[in]	target  追加するプレイヤー
	  */
    void        AddHitTarget(CPlayer* target);

private:

	bool                            m_AttackActive;     //!< 攻撃が有効かどうかのフラグ

	const AttackInfo*               m_CurrentAttack;    //!< 現在の攻撃情報

	CPlayer*                        m_Owner;            //!< 所有するプレイヤーへのポインタ

    std::vector<CircleHitBox>       m_HitCircles;		//!< 現在出ている判定

	std::unordered_set<CPlayer*>    m_HitTargets;       //!< 既にヒットしたプレイヤーのセット（重複ヒット防止用）
};