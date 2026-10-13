
/*!
 *  @file       utility.cpp
 *  @brief      Utility共通処理
 *  @author     Ryusei Shimizu
 *  @date       2026/09/16
 */

#include <cmath>
#include <cstdlib>
#include <utility>

#include "utility.h"

namespace
{
    /*!
     *  ColorIDをRGB値へ変換
     */
    unsigned int ToRGB(Utility::ColorID id)
    {
        switch (id)
        {
        case Utility::ColorID::White:  return 0x00FFFFFF;
        case Utility::ColorID::Black:  return 0x00000000;
        case Utility::ColorID::Red:    return 0x00FF0000;
        case Utility::ColorID::Green:  return 0x0000FF00;
        case Utility::ColorID::Blue:   return 0x000000FF;
        case Utility::ColorID::Yellow: return 0x00FFFF00;
        case Utility::ColorID::Brown:  return 0x008B4513;
        case Utility::ColorID::Purple: return 0x009932CC;
        case Utility::ColorID::Pink:   return 0x00FF1493;
        case Utility::ColorID::Orange: return 0x00FF8C00;
        case Utility::ColorID::Gray:   return 0x00808080;
        case Utility::ColorID::Cyan:   return 0x0000FFFF;
        default:                       return 0x00FFFFFF;
        }
    }
}

namespace Utility
{
    /*
     *  2値の差がepsilon以内なら「同じ値」として扱う
     */
    bool 
    Near(float a, float b, float epsilon)
    {
        // std::abs は絶対値を返している
        return std::abs(a - b) <= std::abs(epsilon);
    }

    /*
     *  ColorIDからRGBを取得し、完全不透明で返す
     */
    unsigned int 
    Color(ColorID id)
    {
        return 0xFF000000 | ToRGB(id);
    }

    /*
     *  ColorIDからRGBを取得し、指定された透明度で返す
     */
    unsigned int 
    ColorAlpha(ColorID id, unsigned int alpha)
    {
        if (alpha > 255)    
            alpha = 255;

        return (alpha << 24) | ToRGB(id);
    }

    /*
     *  minValue～maxValueの範囲からランダムな整数を1つ取得する
     */
    int 
    RandInt(int minValue, int maxValue)
    {
		// 最小値と最大値が逆の場合は、正しく入れ替える
        if (maxValue < minValue)
            std::swap(minValue, maxValue);

		// 範囲が1つの値しか持たない場合は、その値をそのまま返す
        if (minValue == maxValue)
            return minValue;

        return minValue + (std::rand() % (maxValue - minValue + 1));
    }

    /*
     *  minValue～maxValueの範囲からランダムなfloatを1つ取得する
     */
    float
    RandFloat(float minValue, float maxValue)
    {
		// 最小値と最大値が逆に指定されていた場合は、正しい順番へ入れ替える
        if (maxValue < minValue)
            std::swap(minValue, maxValue);

		// 範囲が1つの値しか持たない場合は、その値をそのまま返す
        if (Near(minValue, maxValue))
            return minValue;

		// std::rand()の結果を0.0～1.0の割合へ変換する
        const float rate = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);

		// 求めた割合をminValue～maxValueの範囲へ変換して返す
        return minValue + (maxValue - minValue) * rate;
    }
}
