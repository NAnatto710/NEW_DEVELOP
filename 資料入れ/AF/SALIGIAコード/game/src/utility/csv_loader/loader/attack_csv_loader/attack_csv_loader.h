
/*!
 *  @file       attack_csv_loader.h
 *  @brief      攻撃CSVローダー
 *  @author     Ryusei Shimizu
 *  @date       2026/09/01
 */

#pragma once

#include "../../csv_loader.h"

#include "attack_id.h"

#include <array>
#include <unordered_map>

/*
 *  攻撃CSVをインデックスに変換
 */
namespace
{
    constexpr size_t ToIndex(ATTACK_CSV id)
    {
        return static_cast<size_t>(id);
    }
}

/*!
 *	@class		CAttackCSVLoader
 *
 *	@brief		攻撃CSVローダー
 *
 *	@author     Ryusei Shimizu
 *
 *  @date       2026/09/01
 */
class CAttackCSVLoader
    : public CCSVLoaderBase
{
public:

    /*!
     *  @brief コンストラクタ
     */
    CAttackCSVLoader();

    /*!
     *  @brief デストラクタ
     */
    ~CAttackCSVLoader() = default;

    /*!
     *  @brief CSVロード
     *
     *  @param[in] filepath CSVファイル
     *
     *  @return 読み込み成功
     */
    bool Load(const std::string& filepath);

    /*!
     *  @brief 攻撃情報取得
     *
     *  @param[in] id 攻撃ID
     *
     *  @return 攻撃情報
     */
    const AttackData& GetAttackData(ATTACK_ID id) const;

private:

    /*!
     *  @brief 1行ロード
     *
     *  @param[in] line CSVの1行
     *
     *  @return 読み込み成功
     */
    bool LoadLine(const std::string& line);

    /*!
     *  @brief 攻撃情報読み込み
     *
     *  @param[in] values CSVセル
     *  @param[out] attack 攻撃情報
     */
    void ReadAttack(const std::vector<std::string>& values, AttackInfo& attack);

    static const std::unordered_map<std::string, ATTACK_ID> m_Table; //!< 攻撃ID変換テーブル
    static const std::unordered_map<std::string, ATTACK_EASING_TYPE> m_EasingTable;
    static const std::unordered_map<std::string, ARM_ROTATION_TYPE> m_RotationTable;

    std::array<AttackData, static_cast<size_t>(ATTACK_ID::MAX)> m_AttackDatas; //!< 攻撃データ
};






//
///*!
// *  @file       attack_csv_loader.h
// *  @brief      攻撃CSVローダー
// *  @author     Ryusei Shimizu
// *  @date       2026/07/07
// */
//
//#pragma once
//
//#include "../../csv_loader.h"
//
//#include "attack_id.h"
//
//#include <array>
//#include <unordered_map>
//
///*
// *  属性IDをインデックスに変換
// */
//namespace
//{
//    constexpr size_t ToIndex(ATTACK_CSV id)
//    {
//        return static_cast<size_t>(id);
//    }
//}
//
///*!
// *	@class		CAttackCSVLoader
// *
// *	@brief		攻撃CSVローダー
// *
// *	@author     Ryusei Shimizu
// *
// *  @date       2026/07/07
// */
//class CAttackCSVLoader
//    : public CCSVLoaderBase
//{
//public:
//
//    /*!
//     *  @brief コンストラクタ
//     */
//    CAttackCSVLoader();
//
//    /*!
//     *  @brief デストラクタ
//     */
//    ~CAttackCSVLoader() = default;
//
//    /*!
//     *  @brief CSVロード
//     *
//     *  @param[in] filepath CSVファイル
//     *
//     *  @return 読み込み成功
//     */
//    bool Load(const std::string& filepath);
//
//    /*!
//     *  @brief 攻撃情報取得
//     *
//     *  @param[in] id 攻撃ID
//     *
//     *  @return 攻撃情報
//     */
//    const AttackInfo& GetAttackInfo(ATTACK_ID id) const;
//
//private:
//
//    /*!
//     *  @brief 1行ロード
//     *
//     *  @param[in] line CSV1行
//     *
//     *  @return 成功
//     */
//    bool LoadLine(const std::string& line);
//
//    /*!
//     *  @brief 攻撃情報読み込み
//     *
//     *  @param[in] values CSVセル
//     *  @param[out] attack 攻撃情報
//     */
//    void ReadAttack(const std::vector<std::string>& values, AttackInfo& attack);
//
//    /*!
//     *  @brief 攻撃円生成
//     *
//     *  @param[in] values CSVセル
//     *
//     *  @return 攻撃円
//     */
//    AttackCircleInfo CreateAttackCircle( const std::vector<std::string>& values) const;
//
//
//    static const std::unordered_map<std::string, ATTACK_ID> m_Table;                 //! ID変換テーブル
//    
//    std::array<AttackInfo, static_cast<size_t>(ATTACK_ID::MAX)> m_AttackInfos;      //! 攻撃情報
//};