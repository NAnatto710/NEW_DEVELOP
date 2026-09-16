
/*!
 *  @file       data.h
 *  @brief      セーブデータUtility
 *  @author     Ryusei Shimizu
 *  @date       2026/09/16
 */

#pragma once

#include <string>

namespace Utility
{
    namespace Data
    {
        /*!
         *  @brief  セーブデータ
         */
        struct SaveData
        {
            int Score;

            SaveData()
                : Score(0)
            {
            }
        };

        /*!
         *  @brief  セーブファイルのパスを設定し、保持データを初期化
         *
         *  @param[in]  path    使用するセーブファイルのパス
         */
        void Init(const std::string& path = "data/save/save.dat");

        /*!
         *  @brief  現在保持しているSaveDataをファイルへ保存
         *
         *  @return 保存に成功した場合はtrue、失敗した場合はfalse
         */
        bool Save();

        /*!
         *  @brief  セーブファイルからSaveDataを読み込む
         *
         *  @return 読み込みに成功した場合はtrue、ファイルが無い・読み込み失敗の場合はfalse
         *
         *  @note   ファイルが存在しない場合は初期値でセーブファイルを作成する
         */
        bool Load();

        /*!
         *  @brief  保持しているSaveDataを初期値へ戻す
         */
        void Reset();

        /*!
         *  @brief  書き換え可能なSaveDataを取得
         *
         *  @return 現在保持しているSaveDataへの参照
         */
        SaveData& Get();

        /*!
         *  @brief  現在使用しているセーブファイルのパスを取得
         *
         *  @return セーブファイルパスへの参照
         */
        const std::string& Path();
    }
}
