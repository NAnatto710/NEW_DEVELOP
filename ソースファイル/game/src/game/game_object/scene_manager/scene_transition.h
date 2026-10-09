
/*!
 *  @file           scene_transition.h
 *  @brief          シーンチェンジエフェクト
 *  @author			Ryusei Shimizu
 *  @date           2026/10/09
 */
#pragma once

#include <vector>

class CSceneTransition
{
public:
    /*!
     *  @brief      初期化
     */
    void            Initialize(void);

    /*!
     *  @brief      外周から中央へ黒くする
     */
    void            Cover(void);

    /*!
     *  @brief      中央から外周へ黒を消す
     */
    void            Reveal(void);

    /*!
     *  @brief      シーンエフェクト描画
     */
    void            Draw(void);

    /*!
     *  @brief      全面が黒いか
     */
    bool            IsCovered(void) const;

    /*!
     *  @brief      全面を表示したか
     */
    bool            IsRevealed(void) const;

    /*!
     *  @brief      全面を黒に設定
     */
    void            SetCovered(void);

private:

    /*!
     *  @brief      タイル情報
	 */
    struct TILE
    {
		int         x;          //!< タイルの左上X座標
		int         y;          //!< タイルの左上Y座標
		int         width;      //!< タイルの幅
		int         height;     //!< タイルの高さ
		float       distance;   //!< 中心からの距離
    };

    /*!
     *  @brief      タイル作成
	 */
    void            CreateTiles(void);

    static const int        m_tile_size;            //!< タイルサイズ
    static const int        m_change_frame;         //!< Cover/Revealそれぞれのフレーム数（60FPS想定）

    bool                    m_Cover;                //!< true:覆う / false:開く
    int                     m_Width;                //!< 画面幅
    int                     m_Height;               //!< 画面高さ
    int                     m_Count;                //!< 演出経過フレーム

    std::vector<TILE>       m_Tiles;                //!< 描画タイル
};
