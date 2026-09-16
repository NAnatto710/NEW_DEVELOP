
/*!
 *  @file       scene_id.h
 *  @brief      シーンID
 *  @author     Ryusei Shimizu
 *  @date       2025/10/08
 */

#pragma once

/*!
 *  @brief      メインシーン管理ID
 */
enum class MAINSCENE_ID
{
    DUMMY        //!< ダミーID
    , TITLE         //!< タイトルシーン
    , GAMEMAIN      //!< ゲームメイン
    , RESULT        //!< リザルトシーン

    , MAX           //!< シーンID数
};

/*!
 *  @brief      サブシーン管理ID
 */
enum class SUBSCENE_ID
{
    DUMMY        //!< ダミーID
    , SELECT        //!< セレクトシーン
    , PAUSE         //!< ポーズシーン

    , MAX           //!< シーンID数
};