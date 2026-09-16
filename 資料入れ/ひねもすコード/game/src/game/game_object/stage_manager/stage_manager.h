
/*!
 *  @file       stage_manager.h
 *  @brief      ステージ管理
 *  @author     Ryusei Shimizu
 *  @date       2025/10/09
 */

#pragma once

#include "vivid.h"
#include <vector>
#include <list>
#include "stage_object/stage_object.h"
#include "../../../utility/CSV_loader/CSV_loader.h"

class CEnemySpawn;

/*!
 *  @class      CStageManager
 *
 *  @brief      ステージ管理クラス
 *
 *  @author     Ryusei Shimizu
 *
 *  @date       2025/10/09
 */
class CStageManager
{
public:

    /*!
     *  @brief      インスタンスの取得
     *
     *  @return     インスタンス
     */
    static CStageManager& GetInstance(void);

    /*!
     *  @brief      初期化
     */
    void            Initialize(void);

    /*!
     *  @brief      更新
     */
    void            Update(void);

    /*!
     *  @brief      描画
     */
    void            Draw(void);

    /*!
     *  @brief      解放
     */
    void            Finalize(void);

    /*!
     *  @brief   ブロックの大きさ取得
     *
     *  @return  ブロックサイズ
     */
    int             GetBlockSize(void)const;

    /*!
     *  @brief   マップの横幅に何ブロック存在するか取得
     *
     *  @return  横幅に何ブロック存在するか
     */
    int             GetMapChipWidth(void)const;

    /*!
     *  @brief   マップの縦幅に何ブロック存在するか取得
     *
     *  @return  縦幅に何ブロック存在するか
     */
    int             GetMapChipHeight(void)const;

    /*!
     *  @brief   画面に表示するマップの横幅取得
     *
     *  @return  画面に表示するマップの横幅
     */
    int             GetMapDrawWidth(void)const;

    /*!
     *  @brief   画面に表示するマップの縦幅取得
     *
     *  @return  画面に表示するマップの縦幅
     */
    int 		   GetMapDrawHeight(void)const;

    /*!
     *  @brief   スタート位置を返す
     *
     *  @return  スタート位置座標
     */
    vivid::Vector2  GetStartBlockPosition(void)const;

    /*!
     *  @brief   敵の出現オブジェクトを返す
     *
     *  @return  敵の出現オブジェクト
     */
    CEnemySpawn     GetEnemySpawnObject(void)const;

    /*!
     *  @brief   壁の当たり判定
     *
     *  @param[in]  x  x座標
     *  @param[in]  y  y座標
     *
     *  @return  壁があるか否か
     */
    bool            IsWall(int x, int y);

    /*!
     *  @brief   プレイヤースポーンブロックの当たり判定
     *
     *  @param[in]  x  x座標
     *  @param[in]  y  y座標
     *
     *  @return  プレイヤースポーンブロックがあるか否か
     */
    bool            IsPlayerSpawnBlock(int x, int y);

    /*!
     *  @brief      オブジェクトの配置
     */
    void            ArrangementObject(void);

    /*!
     *  @brief      オブジェクトの再配置
     */
    void            ReinstallationObject(void);

    /*!
     *  @breif      ステージオブジェクトリスト取得
     * 
	 *  @return     ステージオブジェクトリスト
     */
    std::list<IStageObject*> GetFeedObject(void)const;

private:

    /*!
     *  @brief      コンストラクタ
     */
    CStageManager(void);

    /*!
     *  @brief      コピーコンストラクタ
     *
     *  @param[in]  rhs     オブジェクト
     */
    CStageManager(const CStageManager& rhs);

    /*!
     *  @brief      ムーブコンストラクタ
     *
     *  @param[in]  rhs     オブジェクト
     */
    CStageManager(CStageManager&& rhs);

    /*!
     *  @brief      デストラクタ
     */
    ~CStageManager(void);

    /*!
     *  @brief      代入演算子
     *
     *  @param[in]  rhs 代入オブジェクト
     *
     *  @return     自身のオブジェクト
     */
    CStageManager& operator=(const CStageManager& rhs);


    /*!
	 *  @brief  エサオブジェクトの配置
     * 
	 *  @param[in]  size    配置するエサオブジェクトの数
     */
    void           FeedObjectArrangement(void);

	/*!
	 *  @brief  敵出現オブジェクトの配置
     */ 
	void           EnemySpawnObjectArrangement(void);

	/*!
	 *  @brief      レクトのの更新
	 */
	void           RectUpdate(void);

    static const int            m_block_size;                   //!< ブロックの大きさ
    static const int            m_map_chip_count_width;         //!< マップの横幅に何ブロック存在するか
    static const int            m_map_chip_count_height;        //!< マップの縦幅に何ブロック存在するか
    static const int            m_map_chip_draw_width;          //!< 画面に表示するブロック数の横幅
    static const int            m_map_chip_draw_height;         //!< 画面に表示するブロック数の縦幅

    static const int            m_playerspawn_block_count;      //!< プレイヤースポーンブロック数
    static const int            m_feed_object_count;		    //!< エサオブジェクト数
    static const int            m_enemyspawn_object_count;      //!< 敵出現オブジェクト数
    static const int            m_object_interval;              //!< オブジェクトの配置間隔

    static const float          m_enemy_spawn_interval;         //!< 敵の出現間隔

    static const std::string    m_map_file_name;                //!< マップファイル名
    static const std::string    m_block_data_name_1;            //!< ブロックデータ名
    static const std::string    m_block_data_name_2;            //!< ブロックデータ名

    float                       m_EnemySpawnTimer;              //!< 敵の出現タイマー

	int                         m_FeedObjectCount;              //!< エサオブジェクトの数
	int						    m_EnemySpawnObjectCount;        //!< 敵出現オブジェクトの数

    CCSVLoader	                m_CSVLoader;                    //!< CSVローダー

    std::vector<std::vector<unsigned char>>   m_MapBlock;       //!< マップブロックデータ
    std::vector<std::vector<vivid::Rect>>     m_MapBlockRect;   //!< マップブロックの描画範囲データ  
	std::vector<std::vector<bool>>            m_MapBlockFlg;    //!< マップブロックのフラグデータ 

    /*!
     *  @brief      マップオブジェクトリスト型
     */
    using StageObjectList = std::list<IStageObject*>;

    StageObjectList                             m_StageObjectList;  //!< ステージのオブジェクト格納リスト
};