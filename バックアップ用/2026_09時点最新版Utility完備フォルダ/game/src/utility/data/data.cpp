/*!
 *  @file       data.cpp
 *  @brief      セーブデータUtility
 *  @author     Ryusei Shimizu
 *  @date       2026/09/16
 */

#include "data.h"

#include <fstream>

#ifdef _WIN32
#include <direct.h>
#endif

//============================================================
// Data内部で保持するセーブ状態
//============================================================
namespace
{
    Utility::Data::SaveData g_Data;
    std::string g_Path = "data/save/save.dat";

    /*!
     *  @brief  指定された1階層のフォルダを作成
     *
     *  @param[in]  path    作成するフォルダパス
     *
     *  @note   すでに存在する場合はそのまま処理を続ける
     */
    void MakeDirectory(const std::string& path)
    {
#ifdef _WIN32
        if (!path.empty())
        {
            _mkdir(path.c_str());
        }
#else
        (void)path;
#endif
    }

    /*!
     *  @brief  セーブファイルまでに必要な親フォルダを順番に作成
     *
     *  @param[in]  filePath    セーブファイルのパス
     */
    void EnsureParentDirectories(const std::string& filePath)
    {
        for (std::size_t i = 0; i < filePath.size(); ++i)
        {
            if (filePath[i] != '/' && filePath[i] != '\\')
            {
                continue;
            }

            const std::string directory = filePath.substr(0, i);

            // "C:" のようなドライブ名だけをフォルダとして作成しないよう除外する
            if (directory.size() == 2 && directory[1] == ':')
            {
                continue;
            }

            MakeDirectory(directory);
        }
    }
}

namespace Utility
{
    namespace Data
    {
        /*
         *  セーブ先を設定し、メモリ上のSaveDataを初期値へ戻す
         */
        void Init(const std::string& path)
        {
            g_Path = path.empty() ? "data/save/save.dat" : path;
            Reset();
        }

        /*
         *  現在のSaveDataをバイナリ形式で保存する
         *  保存前に必要な親フォルダを作成してからファイルを書き込む
         */
        bool Save()
        {
            EnsureParentDirectories(g_Path);

            std::ofstream file(g_Path.c_str(), std::ios::binary | std::ios::trunc);
            if (!file.is_open())
            {
                return false;
            }

            file.write(reinterpret_cast<const char*>(&g_Data), sizeof(g_Data));
            return file.good();
        }

        /*
         *  セーブファイルをバイナリ形式で読み込む
         *  初回起動などでファイルが無い場合は初期データを作成する
         */
        bool Load()
        {
            std::ifstream file(g_Path.c_str(), std::ios::binary);

            // セーブファイルが存在しない場合は初期値へ戻し、新しいセーブファイルを作成する
            if (!file.is_open())
            {
                Reset();
                Save();
                return false;
            }

            SaveData loaded;
            file.read(reinterpret_cast<char*>(&loaded), sizeof(loaded));

            if (!file.good())
            {
                Reset();
                return false;
            }

            g_Data = loaded;
            return true;
        }

        /*
         *  メモリ上のSaveDataをコンストラクタで定義された初期値へ戻す
         */
        void Reset()
        {
            g_Data = SaveData();
        }

        /*
         *  ゲーム側からSaveDataを書き換えられるよう参照を返す
         */
        SaveData& Get()
        {
            return g_Data;
        }

        /*
         *  現在使用しているセーブファイルパスを返す
         */
        const std::string& Path()
        {
            return g_Path;
        }
    }
}
