/*!
 *  @file           ruler.h
 *  @brief          定規
 *  @date           2026/11/05
 */

#pragma once

#include "../stationery.h"

/*!
 *  @brief          定規クラス
 *
 *  文房具共通基底を継承する。
 *  この段階では画像描画と回転表示のみ扱う。
 */
class CRuler final
    : public IStationery
{
public:

    /*!
     *  @brief      コンストラクタ
     */
                    CRuler(void);

    /*!
     *  @brief      デストラクタ
     */
                    ~CRuler(void) override = default;

    /*!
     *  @brief      初期化
     *
     *  @param[in]  position    定規の中心座標
     */
    void            Initialize(const vivid::Vector2& position);

    /*!
     *  @brief      更新
     */
    void            Update(void) override;

    /*!
     *  @brief      描画
     */
    void            Draw(void) override;

private:

    static constexpr int    TEXTURE_WIDTH = 300;      //!< 画像の幅（px）
    static constexpr int    TEXTURE_HEIGHT = 50;      //!< 画像の高さ（px）
};
