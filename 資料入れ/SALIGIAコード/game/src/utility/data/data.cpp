
 /*!
  *  @file       data.cpp
  *  @brief      セーブデータ読み書き
  *  @author     Ryusei Shimizu
  *  @date       2026/03/20
  */

#include "data.h"

const std::string CData::m_save_filepath = "data/save/save.dat";

/*
 *  コンストラクタ
 */
CData::
CData()
{
    m_SaveData.Score = 0;
}

/*
 *  セーブ
 */
void
CData::
Save() const
{
    FILE* fp = nullptr;
    errno_t err = fopen_s(&fp, m_save_filepath.c_str(), "wb");

    if (err == 0 && fp)
    {
        fwrite(&m_SaveData, sizeof(SAVE_DATA), 1, fp);
        fclose(fp);
        fp = nullptr;
    }
}

/*
 *  ロード
 */
void
CData::
Load()
{
    FILE* fp = nullptr;
    errno_t err = fopen_s(&fp, m_save_filepath.c_str(), "rb");

    // ファイルが開けないもしくはない
    if (err != 0 || !fp)
    {
        // 新しいファイルを作成
        err = fopen_s(&fp, m_save_filepath.c_str(), "wb");

        if (err == 0 && fp)
        {
            // データをリセットする
            Reset();

            // 初期値のセーブデータを書き出す
            fwrite(&m_SaveData, sizeof(SAVE_DATA), 1, fp);

            fclose(fp);
            fp = nullptr;
        }

        return;
    }

    // ファイルを読み込む
    fread(&m_SaveData, sizeof(SAVE_DATA), 1, fp);

    fclose(fp);
}

/*
 *  リセット
 */
void
CData::
Reset()
{
    m_SaveData.Score = 0;
}

/*
 *  セーブデータ取得
 */
SAVE_DATA*
CData::
GetSaveData()
{
    return &m_SaveData;
}

/*
 *  セーブデータ取得
 */
const SAVE_DATA*
CData::
GetSaveData() const
{
    return &m_SaveData;
}