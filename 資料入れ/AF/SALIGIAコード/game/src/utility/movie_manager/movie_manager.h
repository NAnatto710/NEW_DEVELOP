
/*!
 *  @file       movie_manager.h
 *  @brief      動画再生
 *  @author     Ryusei Shimizu
 *  @date       2026/03/20
 */

#pragma once

#include <string>

 /*!
  *  @class      CMovieManager
  *
  *  @brief      動画再生
  *
  *  @author     Ryusei Shimizu
  *
  *  @date       2026/03/20
  */
class CMovieManager
{
public:

    /*!
     *  @brief          インスタンス取得
     *
     *  @return         インスタンス
     */
    static CMovieManager& GetInstance();

    /*!
     * @brief           初期化
     */
    void Initialize();

    /*!
     *  @brief          解放
     */
    void Finalize();

    /*!
     *  @brief          動画ロード
     *
     *  @param[in]      filepass    ファイルパス
     *
     *  @return         成功したらtrue
     */
    bool Load(const std::string& filepass);

    /*!
     *  @brief          動画再生
     *
     *  @param[in]      filepass    ファイルパス
     *
     *  @return         成功したらtrue
     */
    bool Play(const std::string& filepass);

    /*!
     *  @brief           動画描画
     *
     *  @param[in]      x           描画X座標
     *  @param[in]      y           描画Y座標
     */
    void Display(int x, int y);
    void Display(int x, int y, int width, int height);

    /*!
     *  @brief           動画再生停止
     */
    void Stop();

    /*!
     *  @brief           動画再生中かどうか
     *
     *  @return          再生中ならtrue
     */
    bool IsPlaying() const;

private:

    std::string     m_FilePass;             //!< ファイルパス
    int             m_Handle = -1;          //!< 動画ロード(-1=未ロード・無効)
    bool            m_IsPlaying = false;    //!< プレイ中フラグ


    // 以下コンストラク類
    CMovieManager() = default;
    ~CMovieManager() = default;
    CMovieManager(const CMovieManager&) = delete;
    CMovieManager& operator=(const CMovieManager&) = delete;
};