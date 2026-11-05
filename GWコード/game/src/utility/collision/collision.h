
/*!
 *  @file       collision.h
 *  @brief      2D当たり判定Utility
 *  @author     Ryusei Shimizu
 *  @date       2026/09/16
 */

#pragma once

#include "vivid.h"

namespace Utility
{
    /*! 
     *  @brief  点 
     */
    struct Point
    {
        vivid::Vector2  Position;   //!< 点の座標
    };

    /*!
     *  @brief  線分
     */
    struct Segment
    {
        vivid::Vector2  Start;      //!< 始点
        vivid::Vector2  End;        //!< 終点
    };

    /*!
     *  @brief  円
     */
    struct Circle
    {
        vivid::Vector2  Center;     //!< 中心
        float           Radius;     //!< 半径（負値でも内部で絶対値として扱う
    };

    /*!
     *  @brief  カプセル
     */
    struct Capsule
    {
        vivid::Vector2  Start;      //!< 中心線の始点
        vivid::Vector2  End;        //!< 中心線の終点
        float           Radius;     //!< 半径
    };

    /*!
     *  @brief 軸に平行な矩形
     */
    struct AABB
    {
        vivid::Vector2  Position;   //!< 左上座標
        float           Width;      //!< 幅
        float           Height;     //!< 高さ
    };

    /*!
     *  @brief 回転可能な矩形
     */
    struct OBB
    {
        vivid::Vector2  Center;     //!< 中心座標
        vivid::Vector2  HalfSize;   //!< 半サイズ
        float           Rotation;   //!< 回転角（ラジアン）
    };


    /*!
     *  @brief  当たり判定結果
     * 
     *  Normalは第1引数から第2引数へ向かう押し戻し方向の基準軸
     *  第1引数を離す場合は Normal * PenetrationDepth を引く
     */
    struct CollisionResult
    {
        bool            IsHit               = false;                //!< 接触しているか
        vivid::Vector2  Normal              = vivid::Vector2::ZERO; //!< 第1引数から第2引数方向の単位法線
        float           PenetrationDepth    = 0.0f;                 //!< めり込み解消に必要な移動距離
        bool            HasSeparation       = false;                //!< 法線とめり込み量が計算可能か
    };

    namespace Collision
    {
        /*!
         *  @brief  詳細な当たり判定結果を取得する
         *  面積を持つCircle / AABB / OBB同士は押し戻し情報も返す
         *  それ以外の組み合わせはHit判定のみ（HasSeparation = false）
         */
        CollisionResult Check(const Segment& a, const Segment& b);
        CollisionResult Check(const Segment& a, const Circle& b);
        CollisionResult Check(const Segment& a, const Capsule& b);
        CollisionResult Check(const Segment& a, const AABB& b);
        CollisionResult Check(const Segment& a, const OBB& b);
        CollisionResult Check(const Circle& a, const Circle& b);
        CollisionResult Check(const Circle& a, const Capsule& b);
        CollisionResult Check(const Circle& a, const AABB& b);
        CollisionResult Check(const Circle& a, const OBB& b);
        CollisionResult Check(const Capsule& a, const Capsule& b);
        CollisionResult Check(const Capsule& a, const AABB& b);
        CollisionResult Check(const Capsule& a, const OBB& b);
        CollisionResult Check(const AABB& a, const AABB& b);
        CollisionResult Check(const AABB& a, const OBB& b);
        CollisionResult Check(const OBB& a, const OBB& b);

        /*!
         *  @brief  引数の順番を入れ替える補助オーバーロード
         */
        inline CollisionResult Check(const Circle& a, const Segment& b)
        {
            CollisionResult result = Check(b, a);
            if (result.HasSeparation)
            {
                result.Normal.x = -result.Normal.x;
                result.Normal.y = -result.Normal.y;
            }
            return result;
        }
        inline CollisionResult Check(const Capsule& a, const Segment& b)
        {
            CollisionResult result = Check(b, a);
            if (result.HasSeparation)
            {
                result.Normal.x = -result.Normal.x;
                result.Normal.y = -result.Normal.y;
            }
            return result;
        }
        inline CollisionResult Check(const AABB& a, const Segment& b)
        {
            CollisionResult result = Check(b, a);
            if (result.HasSeparation)
            {
                result.Normal.x = -result.Normal.x;
                result.Normal.y = -result.Normal.y;
            }
            return result;
        }
        inline CollisionResult Check(const OBB& a, const Segment& b)
        {
            CollisionResult result = Check(b, a);
            if (result.HasSeparation)
            {
                result.Normal.x = -result.Normal.x;
                result.Normal.y = -result.Normal.y;
            }
            return result;
        }
        inline CollisionResult Check(const Capsule& a, const Circle& b)
        {
            CollisionResult result = Check(b, a);
            if (result.HasSeparation)
            {
                result.Normal.x = -result.Normal.x;
                result.Normal.y = -result.Normal.y;
            }
            return result;
        }
        inline CollisionResult Check(const AABB& a, const Circle& b)
        {
            CollisionResult result = Check(b, a);
            if (result.HasSeparation)
            {
                result.Normal.x = -result.Normal.x;
                result.Normal.y = -result.Normal.y;
            }
            return result;
        }
        inline CollisionResult Check(const OBB& a, const Circle& b)
        {
            CollisionResult result = Check(b, a);
            if (result.HasSeparation)
            {
                result.Normal.x = -result.Normal.x;
                result.Normal.y = -result.Normal.y;
            }
            return result;
        }
        inline CollisionResult Check(const AABB& a, const Capsule& b)
        {
            CollisionResult result = Check(b, a);
            if (result.HasSeparation)
            {
                result.Normal.x = -result.Normal.x;
                result.Normal.y = -result.Normal.y;
            }
            return result;
        }
        inline CollisionResult Check(const OBB& a, const Capsule& b)
        {
            CollisionResult result = Check(b, a);
            if (result.HasSeparation)
            {
                result.Normal.x = -result.Normal.x;
                result.Normal.y = -result.Normal.y;
            }
            return result;
        }
        inline CollisionResult Check(const OBB& a, const AABB& b)
        {
            CollisionResult result = Check(b, a);
            if (result.HasSeparation)
            {
                result.Normal.x = -result.Normal.x;
                result.Normal.y = -result.Normal.y;
            }
            return result;
        }

        /*!
         *  @brief  Pointと各形状の当たり判定
         */
        bool Hit(const Point& a, const Point& b);
        bool Hit(const Point& point, const Segment& segment);
        bool Hit(const Point& point, const Circle& circle);
        bool Hit(const Point& point, const Capsule& capsule);
        bool Hit(const Point& point, const AABB& aabb);
        bool Hit(const Point& point, const OBB& obb);

        /*!
         *  @brief  Segmentと各形状の当たり判定
         */
        bool Hit(const Segment& a, const Segment& b);
        bool Hit(const Segment& segment, const Circle& circle);
        bool Hit(const Segment& segment, const Capsule& capsule);
        bool Hit(const Segment& segment, const AABB& aabb);
        bool Hit(const Segment& segment, const OBB& obb);

        /*!
         *  @brief  Circleと各形状の当たり判定
         */
        bool Hit(const Circle& a, const Circle& b);
        bool Hit(const Circle& circle, const Capsule& capsule);
        bool Hit(const Circle& circle, const AABB& aabb);
        bool Hit(const Circle& circle, const OBB& obb);

        /*!
         *  @brief  Capsuleと各形状の当たり判定
         */
        bool Hit(const Capsule& a, const Capsule& b);
        bool Hit(const Capsule& capsule, const AABB& aabb);
        bool Hit(const Capsule& capsule, const OBB& obb);

        /*!
         *  @brief  AABB / OBB同士の当たり判定
         */
        bool Hit(const AABB& a, const AABB& b);
        bool Hit(const AABB& aabb, const OBB& obb);
        bool Hit(const OBB& a, const OBB& b);

        /*!
         *  @brief  引数の順番を逆にしても同じHit()を使用できるようにする補助オーバーロード
         */
        inline bool Hit(const Segment& segment, const Point& point)       { return Hit(point, segment); }
        inline bool Hit(const Circle& circle, const Point& point)         { return Hit(point, circle); }
        inline bool Hit(const Capsule& capsule, const Point& point)       { return Hit(point, capsule); }
        inline bool Hit(const AABB& aabb, const Point& point)             { return Hit(point, aabb); }
        inline bool Hit(const OBB& obb, const Point& point)               { return Hit(point, obb); }

        inline bool Hit(const Circle& circle, const Segment& segment)     { return Hit(segment, circle); }
        inline bool Hit(const Capsule& capsule, const Segment& segment)   { return Hit(segment, capsule); }
        inline bool Hit(const AABB& aabb, const Segment& segment)         { return Hit(segment, aabb); }
        inline bool Hit(const OBB& obb, const Segment& segment)           { return Hit(segment, obb); }

        inline bool Hit(const Capsule& capsule, const Circle& circle)     { return Hit(circle, capsule); }
        inline bool Hit(const AABB& aabb, const Circle& circle)           { return Hit(circle, aabb); }
        inline bool Hit(const OBB& obb, const Circle& circle)             { return Hit(circle, obb); }

        inline bool Hit(const AABB& aabb, const Capsule& capsule)         { return Hit(capsule, aabb); }
        inline bool Hit(const OBB& obb, const Capsule& capsule)           { return Hit(capsule, obb); }

        inline bool Hit(const OBB& obb, const AABB& aabb)                 { return Hit(aabb, obb); }
    }
}
