
/*!
 *  @file       utility.h
 *  @brief      ユーティリティ
 *  @author     Ryusei Shimizu
 *  @date       2025/12/09
 */

#pragma once

#include "vivid.h"

/*!
 *  @brief      角度をラジアン値に変換
 *
 *  @param[in]  d				角度
 *
 *  @return     ラジアン値
 */
#define DEG_TO_RAD(d) (float)((d) * 3.1415f) / 180.0f

/*!
 *	@brief      ２つの値の大きい方を取得
 *
 *	@param[in]  a				値1
 *	@param[in]  b				値2
 *
 *	@return     大きい方の値
 */
#define max(a,b) ((a)>(b)?(a):(b))

/*!
 *	@brief      ２つの値の小さい方を取得
 *
 *	@param[in]  a				値1
 *	@param[in]  b				値2
 *
 *	@return     小さい方の値
 */
#define min(a,b) ((a)<(b)?(a):(b))

/*!
 *  @brief      指定した四角形型のオブジェクトとマウスポインタの当たり判定
 *
 *  @param[in]  pos				オブジェクトの座標
 *  @param[in]  w				オブジェクトの横幅
 *  @param[in]  h				オブジェクトの縦幅
 *
 *  @return     当たり判定の有無
 */
bool u_CheckHitMouse(vivid::Vector2 pos, int w, int h);

/*!
 *  @brief      指定した四角形型のオブジェクト同士の当たり判定
 *
 *  @param[in]  pos1			オブジェクト1の座標
 *  @param[in]  w1				オブジェクト1の横幅
 *  @param[in]  h1				オブジェクト1の縦幅
 *  @param[in]  pos2			オブジェクト2の座標
 *  @param[in]  w2				オブジェクト2の横幅
 *  @param[in]  h2				オブジェクト2の縦幅
 *
 *  @return     当たり判定の有無
 */
bool u_CheckHitObject(vivid::Vector2 pos1, int w1, int h1, vivid::Vector2 pos2, int w2, int h2);

/*!
 *  @brief      指定した四角形型のオブジェクト同士の当たり判定（回転対応版）
 *
 *  @param[in]  pos1			オブジェクト1の座標
 *  @param[in]  w1				オブジェクト1の横幅
 *  @param[in]  h1				オブジェクト1の縦幅
 *  @param[in]  pos2			オブジェクト2の座標
 *  @param[in]  w2				オブジェクト2の横幅
 *  @param[in]  h2				オブジェクト2の縦幅
 *  @param[in]  rotation		オブジェクト1の回転角度（ラジアン）
 *
 *  @return     当たり判定の有無
 */
bool u_CheckHitObject(vivid::Vector2 pos1, int w1, int h1, vivid::Vector2 pos2, int w2, int h2, float rotataion);

/*!
 *  @brief      指定した円形型のオブジェクト同士の当たり判定
 *
 *  @param[in]  pos1			円形オブジェクト1の中心座標
 *  @param[in]  r1				円形オブジェクト1の半径
 *  @param[in]  pos2			円形オブジェクト2の中心座標
 *  @param[in]  r2				円形オブジェクト2の半径
 *
 *  @return     当たり判定の有無
 */
bool u_CheckHitCircle(vivid::Vector2 pos1, float r1, vivid::Vector2 pos2, float r2);

/*!
 *  @brief      指定した円形型のオブジェクトと四角形型のオブジェクトの当たり判定
 *
 *  @param[in]  pos1			円形オブジェクトの中心座標
 *  @param[in]  r1				円形オブジェクトの半径
 *  @param[in]  pos2			四角形オブジェクトの座標
 *  @param[in]  w1				四角形オブジェクトの横幅
 *  @param[in]  h1				四角形オブジェクトの縦幅
 *
 *  @return     当たり判定の有無
 */
bool u_CheckHitCircleObject(vivid::Vector2 pos1, float r1, vivid::Vector2 pos2, int w1, int h1);

/*!
 *  @brief      指定した範囲内でランダムな整数値を取得
 *
 *  @param[in]  min				最小値
 *  @param[in]  max				最大値
 *
 *  @return     ランダムな整数値
 */
float u_RandomInt(float min, float max);

/*!
 *  @brief      指定した範囲内で値を修正
 *
 *  @param[in]  value			値
 *  @param[in]  min				最小値
 *  @param[in]  max				最大値
 *
 *  @return     修正後の値
 */
float u_Clamp(float value, float min, float max);

/*!
 *  @brief      ２つの線分が交差しているか判定
 *
 *  @param[in]  A				線分1の始点
 *  @param[in]  B				線分1の終点
 *  @param[in]  C				線分2の始点
 *  @param[in]  D				線分2の終点
 *
 *  @return     交差しているかどうか
 */
bool u_CheckCross(vivid::Vector2 A, vivid::Vector2 B, vivid::Vector2 C, vivid::Vector2 D);