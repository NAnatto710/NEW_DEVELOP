
/*!
 *  @file       csv_loader_base.h
 *  @brief      CSVローダー基底クラス
 *  @author     Ryusei Shimizu
 *  @date       2026/07/07
 */

#pragma once

#include <fstream>
#include <string>

/*!
 *	@class		CCSVLoaderBase
 *
 *	@brief		CSVローダー基底クラス
 *
 *	@author     Ryusei Shimizu
 *
 *  @date       2026/07/07
 */
class CCSVLoaderBase
{
public:

    /*!
     *  @brief コンストラクタ
     */
    CCSVLoaderBase();

    /*!
     *  @brief デストラクタ
     */
    virtual ~CCSVLoaderBase() = default;

protected:

    /*!
     *  @brief CSVをロード
     *
     *  @param[in] filepath CSVパス
     *  @param[in] func     1行処理
     *
     *  @return 読み込み成功
     */
    template<class Func> bool  LoadFile(const std::string& filepath, Func&& func);

    /*!
     *  @brief ファイルを開く
     *
     *  @param[in] filepath ファイルパス
     *
     *  @return 成功したか
     */
    bool Open(const std::string& filepath);

    /*!
     *  @brief ファイルを閉じる
     */
    void Close();

    /*!
     *  @brief 1行取得
     *
     *  @param[out] line 行
     *
     *  @return 読み込み成功
     */
    bool ReadLine(std::string& line);

    /*!
     *  @brief ファイルが開いているか
     */
    bool IsOpen() const;

    std::ifstream m_File;     //!< CSVファイル
};

/*
 *  CSVをロード
 */
template<class Func>inline bool 
CCSVLoaderBase::
LoadFile(const std::string& filepath, Func&& func)
{
	// ファイルを開く
    if (!Open(filepath))
        return false;

    std::string line;

	// 1行ずつ処理
    while (ReadLine(line))
    {
		// 1行処理
        if (!func(line))
        {
            Close();
            return false;
        }
    }

    Close();

    return true;
}
