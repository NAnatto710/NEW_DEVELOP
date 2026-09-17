
/*!
 *  @file       utility.h
 *  @brief      ゲーム全体で使用する共通Utility
 *  @author     Ryusei Shimizu
 *  @date       2026/09/16
 */

#pragma once

namespace Utility
{
    /*! 
     *  @brief  円周率
     */
    constexpr float PI = 3.14159265358979323846f;

    /*! 
     *  @brief  許容誤差
     */
    constexpr float EPSILON = 0.00001f;


    /*!
	 *  @brief  色のID
     */
    enum class ColorID
    {
		White,      //!< 白
		Black,      //!< 黒
		Red,        //!< 赤
        Green,      //!< 緑
        Blue,       //!< 青
        Yellow,     //!< 黄
        Brown,      //!< 茶
        Purple,     //!< 紫
		Pink,       //!< ピンク
		Orange,     //!< オレンジ
		Gray,       //!< グレー
		Cyan,       //!< シアン
    };

    /*!
     *  @brief  度数をラジアンへ変換
     * 
	 *  @param  degree  度数
     * 
	 *  @return ラジアン
     */
    constexpr float DegToRad(float degree)
    {
        return degree * PI / 180.0f;
    }

    /*!
	 *  @brief  ラジアンを度数へ変換
     * 
	 *  @param  radian  ラジアン
     * 
	 *  @return 度数
     */
    constexpr float RadToDeg(float radian)
    {
        return radian * 180.0f / PI;
    }


    /*! 
     *  @brief  2つの値の大きい方を返す 
     * 
	 *  @param  a   比較する値1
	 *  @param  b   比較する値2
     * 
	 *  @return 大きい方の値
     */
    template <class T>
    constexpr T Max(T a, T b)
    {
        return (a < b) ? b : a;
    }

    /*!
     *  @brief  2つの値の小さい方を返す
     * 
	 *  @param  a   比較する値1
	 *  @param  b   比較する値2
     * 
	 *  @return 小さい方の値
     */
    template <class T>
    constexpr T Min(T a, T b)
    {
        return (b < a) ? b : a;
    }

	/*!
	 *  @brief  値をminValue～maxValueの範囲に収める
     * 
	 *  @param  value       収める値
	 *  @param  minValue    最小値
	 *  @param  maxValue    最大値
     * 
	 *  @return 収めた値
	 */
    template <class T>
    constexpr T Clamp(T value, T minValue, T maxValue)
    {
        return (value < minValue) ? minValue : ((maxValue < value) ? maxValue : value);
    }

    /*!
	 *  @brief 0.0～1.0の割合で線形補間する
     * 
	 *  @param  start   開始値
	 *  @param  end     終了値
	 *  @param  t       補間割合(0.0～1.0)
     * 
	 *  @return 補間後の値
	 */
    template <class T>
    constexpr T Lerp(const T& start, const T& end, float t)
    {
        return start + (end - start) * t;
    }

    /*! 
     *  @brief  float同士が誤差範囲内で等しいかを判定
     * 
	 *  @param  a           比較する値1
	 *  @param  b           比較する値2
	 *  @param  epsilon     許容誤差
     * 
	 *  @return 等しい場合はtrue、そうでない場合はfalse
     */
    bool Near(float a, float b, float epsilon = EPSILON);

    /*!
     *  @brief  ColorIDからカラー値を取得
     * 
	 *  @param  id  色のID
     * 
	 *  @return カラー値
     */
    unsigned int Color(ColorID id);

	/*!
	 *  @brief  ColorIDからカラー値を取得（透明度指定付き）
     * 
	 *  @param  id      色のID
	 *  @param  alpha   透明度(0x00～0xFF)
     * 
	 *  @return カラー値
	 */
    unsigned int ColorAlpha(ColorID id, unsigned int alpha = 255);

    /*!
	 *  @brief  min～maxの範囲でランダムな整数を取得
     * 
	 *  @param  minValue    最小値
	 *  @param  maxValue    最大値
     * 
	 *  @return minValue～maxValueの範囲でランダムな整数
     */
    int RandInt(int minValue, int maxValue);

    /*!
     *  @brief  min～maxの範囲でランダムなfloatを取得
     * 
	 *  @param  minValue    最小値
	 *  @param  maxValue    最大値
     * 
	 *  @return minValue～maxValueの範囲でランダムなfloat
     */
    float RandFloat(float minValue, float maxValue);
}

//============================================================
// 共通関数を先に宣言してから各機能をincludeする
// これによりCollision側などからUtility::Clamp等を安全に使用できる
//============================================================
#include "csv/csv.h"
#include "sound/sound.h"
#include "movie/movie.h"
#include "data/data.h"
#include "collision/collision.h"
