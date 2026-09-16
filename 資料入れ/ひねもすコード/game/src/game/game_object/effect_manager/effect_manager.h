
/*!
 *  @file       effect_manager.h
 *  @brief      エフェクト管理
 *  @author     Ryusei Shimizu
 *  @date       2026/02/19
 */

#pragma once

#include "vivid.h"
#include "effect_id.h"
#include "guide_id.h"
#include <list>

class IEffect;
class IGuide;

 /*!
  *  @class      CEffectManager
  *
  *  @brief      エフェクト管理クラス
  *
  *  @author     Ryusei Shimizu
  *
  *  @date       2026/02/19
  */
class CEffectManager
{
public:

    /*!
      *  @brief      インスタンスの取得
      *
      *  @return     インスタンス
      */
    static CEffectManager&  GetInstance(void);

    /*!
     *  @brief      初期化
     */
    void                    Initialize(void);

    /*!
     *  @brief      更新
     */
    void                    Update(void);

	/*!
	 *  @brief      描画
	 */
    void                    Draw(void);

    /*!
	 *  @brief      シーンエフェクト描画
     */
    void                    SceneEffectDraw(void);

    /*!
     *  @brief      解放
     */
    void                    Finalize(void);

	/*!
	  *  @brief      エフェクトの生成
	  *
	  *  @param[in]  id        エフェクトID
	  *  @param[in]  pos       位置
	  *  @param[in]  color     色
	  *  @param[in]  rotation  回転角度
	  */
    void                    Create(EFFECT_ID id, const vivid::Vector2& pos, unsigned int color, float rotation);

	/*!
	 *  @brief      ガイドの生成
     * 
	 *  @param[in]  guideId    ガイドID
	 *  @param[in]  pos        位置
     */
	void                    Create(GUIDE_ID guideId, const vivid::Vector2& pos);

	/*!
	  *  @brief      エフェクトの削除
	  */
    void                    EffectDelete(void);

private:

    /*!
     *  @brief      コンストラクタ
     */
    CEffectManager(void);

    /*!
     *  @brief      コピーコンストラクタ
     *
     *  @param[in]  rhs     オブジェクト
     */
    CEffectManager(const CEffectManager& rhs);

    /*!
     *  @brief      デストラクタ
     */
    ~CEffectManager(void);

    /*!
     *  @brief      代入演算子
     *
     *  @param[in]  rhs 代入オブジェクト
     *
     *  @return     自身のオブジェクト
     */
    CEffectManager& operator=(const CEffectManager& rhs);

    /*!
     *  @brief      エフェクトリスト型
     */
    using EFFECT_LIST = std::list<IEffect*>;

    /*!
	 *  @brief      ガイドリスト型
     */
	using GUIDE_LIST = std::list<IGuide*>;

	EFFECT_LIST         m_EffectList;       //!< エフェクトリスト

	GUIDE_LIST		    m_GuideList;        //!< ガイドリスト
};