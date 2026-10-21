
/*!
 *  @file		attack_component.h
 *  @brief		攻撃コンポーネント
 *  @author     Ryusei Shimizu
 *  @date       2026/08/25
 */

#pragma once

#include "vivid.h"
#include "../../arm/arm.h"
#include "../../../../../../utility/csv_loader/loader/attack_csv_loader/attack_id.h"
#include "../../../../../../utility/csv_loader/loader/attack_csv_loader/attack_csv_loader.h"
#include "../../build_component/build_data.h"

class CResourceComponent;
class CAttributeComponent;

/*!
 *  @class		CAttackComponent
 *
 *  @brief		攻撃コンポーネントクラス
 *
 *  @author     Ryusei Shimizu
 * 
 *  @date       2026/08/25
 */
class CAttackComponent
{
public:
	/*!
	 *  @brief		コンストラクタ
	 */
	CAttackComponent();

	/*!
	 *  @brief		デストラクタ
	 */
	~CAttackComponent() = default;

	/*!
	 *  @brief		初期化
	 *
	 *  @param[in]	player_position		Playerの位置
	 *  @param[in]	facing_right		Playerが右を向いているかどうか
	 *	@param[in]	build_data			ビルドデータ
	 *  @param[in]	file_path			攻撃CSVファイルのパス
	 */
	void			Initialize(const vivid::Vector2& player_position, bool facing_right, const BuildData& build_data, std::string file_path);

	/*!
	 *  @brief		更新
	 *
	 *  @param[in]	player_position		Playerの位置
	 *  @param[in]	facing_right		Playerが右を向いているかどうか
	 *  @param[in]	attack_input		攻撃入力
	 *  @param[in]	guarding			ガード中かどうか
	 */
	void			Update(const vivid::Vector2& player_position, bool facing_right, const vivid::Vector2& attack_input, bool guarding);

	/*!
	 *  @brief		右腕の描画
	 */
	void			RightArmDraw(void);

	/*!
	 *  @brief		左腕の描画
	 */
	void			LeftArmDraw(void);

	/*!
	 *  @brief		解放
	 */
	void			Finalize(void);

	/*!
	 *  @brief		攻撃開始
	 * 
	 *	@param[in]	attack_id			攻撃ID
	 *	@param[in]	resource_component	リソースコンポーネント
	 *	@param[in]	attribute_component	属性コンポーネント
	 *	@param[in]	facing_right		Playerが右を向いているかどうか
	 *	@param[in]	arm_id				腕のID
	 */
	void			Attack(ATTACK_ID attack_id, CResourceComponent& resource_component, CAttributeComponent& attribute_component, bool facing_right, ARM_ID arm_id = ARM_ID::BOTH);

	/*!
	 *  @brief		攻撃キャンセル
	 */
	void			Cancel(void);

	/*!
	 *  @brief		ガードブレイク演出開始
	 *
	 *  @param[in]	duration	ガードブレイク硬直時間
	 */
	void			StartGuardBreak(int duration);

	/*!
	 *  @brief		攻撃中かどうかの取得
	 *
	 *  @return		攻撃中かどうか
	 */
	bool            IsAttacking(void) const;

	/*!
	 *  @brief		攻撃中の腕の取得
	 *
	 *  @return		攻撃中の腕
	 */
	CArm*			GetActiveAttackArm(void);

	/*!
	 *  @brief		左腕の取得
	 *
	 *  @return		左腕
	 */
	CArm&			GetLeftArm(void){ return m_LeftArm; };

	/*!
	 *  @brief		右腕の取得
	 *
	 *  @return		右腕
	 */
	CArm&			GetRightArm(void){ return m_RightArm; };

	/*!
	 *  @brief		通常攻撃かどうかの取得
	 *
	 *  @param[in]	attack_id	攻撃ID
	 *
	 *  @return		通常攻撃かどうか
	 */
	bool            IsAttack(ATTACK_ID attack_id) const;

private:
	
	static const vivid::Vector2 m_left_arm_position;		//!< プレイヤーから見た相対的な左腕の位置
	static const vivid::Vector2 m_right_arm_position;		//!< プレイヤーから見た相対的な右腕の位置

	CArm						m_LeftArm;					//!< 左腕
	CArm						m_RightArm;					//!< 右腕

	CAttackCSVLoader			m_AttackCSVLoader;			//!< 攻撃CSVローダー
};