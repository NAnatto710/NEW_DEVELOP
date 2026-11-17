
 /*!
  *  @file           ruler.h
  *  @brief          定規
  *  @author         Ryusei Shimizu
  *  @date           2026/11/05
  */

#pragma once

#include "../stationery.h"

class CRuler
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

    static const int            m_texture_width;        //!< 画像の幅
    static const int            m_texture_height;       //!< 画像の高さ
    static const std::string    m_texture_path;         //!< 画像パス
};
