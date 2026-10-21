
/*!
 *  @file		build_component.h
 *  @brief		ビルドコンポーネント
 *  @author     Ryusei Shimizu
 *  @date       2026/06/26
 */

#pragma once

#include "build_data.h"
#include "skill_resolver/skill_resolver.h"
#include "../status/attribute_component/attribute_id.h"
#include "../../../../../utility/csv_loader/loader/passive_csv_loader/passive_csv_loader.h"
#include "../state_controller/state_id.h"

#include <array>

class CAttributeComponent;
class CStateController;

/*
 *  @brief リソースIDをインデックスに変換
 *
 *  @param[in] id リソースID
 *
 *  @return インデックス
 */
static constexpr size_t
ToIndex(ATTRIBUTE_ID id)
{
	return static_cast<size_t>(id);
}

/*!
 *	@class		CBuildComponent
 *
 *	@brief		ビルドコンポーネントクラス
 *
 *	@author     Ryusei Shimizu
 *
 *  @date       2026/06/26
 */
class CBuildComponent
{
public:

	/*!
	 *	@brief	コンストラクタ
	 */
	CBuildComponent();

	/*!
	 *	@brief	デストラクタ
	 */
	~CBuildComponent() = default;

	/*!
	 *  @brief					初期化
	 *
	 *  @param[in]				build					ビルドデータ
	 *	@param[in]				passive_csv_path		パッシブCSVファイルパス
	 *	@param[in]				state_controller		状態管理クラス
	 */
	void						Initialize(const BuildData& build, const std::string& passive_csv_path, CStateController* state_controller);

	/*!
	 *  @brief					パッシブ更新
	 *
	 *  @param[in]				attribute_component		属性コンポーネント
	 */
	void						PassiveUpdate(CAttributeComponent& attribute_component);

	/*!
	 *  @brief					ビルドデータ取得
	 *
	 *  @return					ビルドデータ
	 */
	const BuildData&			GetBuild(void) const;

	/*!
	 *  @brief					選ばれたビルドが同じかどうか
	 *
	 *  @return					同じビルドかどうか
	 */
	bool						IsSameBuild(void) const;

private:

	/*!
	 *  @brief					パッシブ読み込み
	 */
	void						LoadPassive(void);

	/*!
	 *  @brief					パッシブ適用
	 *
	 *  @param[in]				passiveModifiers		パッシブ補正値
	 *	@param[in]				passiveInfo				パッシブ情報
	 */
	void						ApplyPassive(std::array<AttributeModifier, ToIndex(ATTRIBUTE_ID::MAX)>& passiveModifiers, const PassiveInfo& passiveInfo);

	bool						m_IsPassiveApplied;		//!< パッシブ適用済みか

	CPassiveCSVLoader			m_PassiveCSVLoader;		//!< パッシブCSVローダー
	BuildData					m_Build;				//!< ビルドデータ
	CStateController*			m_StateController;		//!< 状態管理クラス
	DESIRE_STATE				m_PreviousDesireState;	//!< 前回の欲望状態

	std::array<AttributeModifier, ToIndex(ATTRIBUTE_ID::MAX)>		m_NormalPassive;		//!< 通常時パッシブ
	std::array<AttributeModifier, ToIndex(ATTRIBUTE_ID::MAX)>		m_MuyokuPassive;		//!< 無欲時パッシブ
};