
/*!
 *  @file       collison_check.cpp
 *  @brief      当たり判定
 *  @author     Ryusei Shimizu
 *  @date       2026/03/20
 */

#include "collision_check.h"
#include "../utility.h"

namespace
{
    /*!
     *	@brief		2つの点の距離の二乗を計算
     *
     *	@param[in]	point1		点1
     *	@param[in]	point2		点2
     *
     *	@return		距離の二乗
     */
    float DistanceSq(const vivid::Vector2& point1, const vivid::Vector2& point2);

    /*!
     *	@brief		線分と点の距離の二乗を計算
     *
     *	@param[in]	segment		線分
     *	@param[in]	point		点
     *
     *	@return		距離の二乗
     */
    float DistanceSq(const Segment& segment, const vivid::Vector2& point);

    /*!
     *	@brief		線分と線分の距離の二乗を計算
     *
     *	@param[in]	segment1		線分1
     *	@param[in]	segment2		線分2
     *
     *	@return		距離の二乗
     */
    float DistanceSq(const Segment& segment1, const Segment& segment2);

    float
    DistanceSq(const vivid::Vector2& point1, const vivid::Vector2& point2)
    {
        float dx = point2.x - point1.x;
        float dy = point2.y - point1.y;

        return dx * dx + dy * dy;
    }

    float
    DistanceSq(const Segment& segment, const vivid::Vector2& point)
    {
        vivid::Vector2 ab = segment.End - segment.Start;

        float lengthSq = vivid::Vector2::Dot(ab, ab);

        if (lengthSq == 0.0f)
        {
            return DistanceSq(segment.Start, point);
        }

        float t = vivid::Vector2::Dot(point - segment.Start, ab) / lengthSq;

        t = CLAMP(t, 0.0f, 1.0f);

        vivid::Vector2 nearest = segment.Start + ab * t;

        return DistanceSq(nearest, point);
    }

    float
    DistanceSq(const Segment& segment1, const Segment& segment2)
    {
        const vivid::Vector2 d1 = segment1.End - segment1.Start;
        const vivid::Vector2 d2 = segment2.End - segment2.Start;
        const vivid::Vector2 r = segment1.Start - segment2.Start;

        const float a = vivid::Vector2::Dot(d1, d1);
        const float e = vivid::Vector2::Dot(d2, d2);
        const float f = vivid::Vector2::Dot(d2, r);

        // segment1が点の場合
        if (a <= EPSILON && e <= EPSILON)
        {
            return DistanceSq(segment1.Start, segment2.Start);
        }

        // segment1が点の場合
        if (a <= EPSILON)
        {
            float t = f / e;
            t = CLAMP(t, 0.0f, 1.0f);

            const vivid::Vector2 closest = segment2.Start + d2 * t;

            return DistanceSq(segment1.Start, closest);
        }

        // segment2が点の場合
        if (e <= EPSILON)
        {
            float s = -vivid::Vector2::Dot(d1, r) / a;

            s = CLAMP(s, 0.0f, 1.0f);

            const vivid::Vector2 closest = segment1.Start + d1 * s;

            return DistanceSq(closest, segment2.Start);
        }

        const float b = vivid::Vector2::Dot(d1, d2);
        const float c = vivid::Vector2::Dot(d1, r);

        const float denominator = a * e - b * b;

        float s = 0.0f;
        float t = 0.0f;

        // 線分同士の最近接点を求める
        if (denominator > EPSILON)
        {
            s = (b * f - c * e) / denominator;
            s = CLAMP(s, 0.0f, 1.0f);
        }
        else
        {
            // 平行に近い場合
            s = 0.0f;
        }

        t = (b * s + f) / e;

        // tが範囲外の場合はsを調整
        if (t < 0.0f)
        {
            t = 0.0f;

            s = -c / a;
            s = CLAMP(s, 0.0f, 1.0f);
        }
        else if (t > 1.0f)
        {
            t = 1.0f;

            s = (b - c) / a;
            s = CLAMP(s, 0.0f, 1.0f);
        }

        const vivid::Vector2 closest1 = segment1.Start + d1 * s;
        const vivid::Vector2 closest2 = segment2.Start + d2 * t;

        return DistanceSq(closest1, closest2);
    }

}

/*
 *	指定した点同士の当たり判定
 */
bool
IsCollision::
IsHit(const Point& point1, const Point& point2)
{
	return point1.Position == point2.Position;
}

/*
 *	指定した点と線分の当たり判定
 */
bool
IsCollision::
IsHit(const Point& point, const Segment& segment)
{
	// 線分のベクトルと点から線分の始点へのベクトルを計算
    vivid::Vector2 ab = segment.End - segment.Start;
	// 点から線分の始点へのベクトルを計算
    vivid::Vector2 ap = point.Position - segment.Start;

	// 外積を計算して、点が線分上にあるかどうかを判定
    float cross = vivid::Vector2::Cross(ab, ap);

    // 線上にない
    if (fabs(cross) > EPSILON)
        return false;

	// 内積を計算して、点が線分の範囲内にあるかどうかを判定
    float dot = vivid::Vector2::Dot(ap, ab);

	// 点が線分の始点より前にある場合
    if (dot < 0.0f)
        return false;

	// 点が線分の終点より後ろにある場合
    float lengthSq = vivid::Vector2::Dot(ab, ab);

    // 点が線分の終点より後ろにある場合
    if (dot > lengthSq)
        return false;

    return true;
}

/*
 *	指定した点と円の当たり判定
 */
bool
IsCollision::
IsHit(const Point& point, const Circle& circle)
{
    return DistanceSq(point.Position, circle.Center) <= circle.Radius * circle.Radius;
}

/*
 *	指定した点と長円の当たり判定
 */
bool
IsCollision::
IsHit(const Point& point, const Capsule& capsule)
{
	 Segment segment =
    {
        capsule.Start,
        capsule.End
    };

    return DistanceSq(segment, point.Position) <= capsule.Radius * capsule.Radius;
}

/*
 *	指定した点と長方形の当たり判定
 */
bool
IsCollision::
IsHit(const Point& point, const AABB& aabb)
{
	return	point.Position.x >= aabb.Position.x &&
			point.Position.x <= aabb.Position.x + aabb.Width &&
			point.Position.y >= aabb.Position.y &&
			point.Position.y <= aabb.Position.y + aabb.Height;
}

/*
 *	指定した点と回転長方形の当たり判定
 */
bool IsCollision::IsHit(const Point& point, const OBB& obb)
{
	return false;
}

/*
 *	指定した線分同士の当たり判定
 */
bool
IsCollision::
IsHit(const Segment& segment1, const Segment& segment2)
{
    // AABBが重なっていないなら交差しない
    if (MAX(segment1.Start.x, segment1.End.x) < MIN(segment2.Start.x, segment2.End.x) ||
        MAX(segment2.Start.x, segment2.End.x) < MIN(segment1.Start.x, segment1.End.x) ||
        MAX(segment1.Start.y, segment1.End.y) < MIN(segment2.Start.y, segment2.End.y) ||
        MAX(segment2.Start.y, segment2.End.y) < MIN(segment1.Start.y, segment1.End.y))
    {
        return false;
    }

    float cross1 = vivid::Vector2::Cross(segment1.End - segment1.Start, segment2.Start - segment1.Start);
    float cross2 = vivid::Vector2::Cross(segment1.End - segment1.Start, segment2.End - segment1.Start);
    float cross3 = vivid::Vector2::Cross(segment2.End - segment2.Start, segment1.Start - segment2.Start);
    float cross4 = vivid::Vector2::Cross(segment2.End - segment2.Start, segment1.End - segment2.Start);

    return (cross1 * cross2 <= EPSILON) && (cross3 * cross4 <= EPSILON);
}

/*
 *	指定した線分と円の当たり判定
 */
bool
IsCollision::
IsHit(const Segment& segment, const Circle& circle)
{
    return DistanceSq(segment, circle.Center) <= circle.Radius * circle.Radius;
}

/*
 *	指定した線分と長円の当たり判定
 */
bool
IsCollision::
IsHit(const Segment& segment, const Capsule& capsule)
{
    Segment capsuleSegment =
    {
        capsule.Start,
        capsule.End
    };

    return DistanceSq(segment, capsuleSegment) <= capsule.Radius * capsule.Radius;
}

/*
 *	指定した線分と長方形の当たり判定
 */
bool
IsCollision::
IsHit(const Segment& segment, const AABB& aabb)
{
    // 始点または終点がAABB内なら当たっている
    if (IsHit(Point{ segment.Start }, aabb))
        return true;

    if (IsHit(Point{ segment.End }, aabb))
        return true;

    // AABBの四辺を作成
    Segment top =
    {
        aabb.Position,
        { aabb.Position.x + aabb.Width, aabb.Position.y }
    };

    Segment bottom =
    {
        { aabb.Position.x, aabb.Position.y + aabb.Height },
        { aabb.Position.x + aabb.Width, aabb.Position.y + aabb.Height }
    };

    Segment left =
    {
        aabb.Position,
        { aabb.Position.x, aabb.Position.y + aabb.Height }
    };

    Segment right =
    {
        { aabb.Position.x + aabb.Width, aabb.Position.y },
        { aabb.Position.x + aabb.Width, aabb.Position.y + aabb.Height }
    };

	// 線分とAABBの四辺の交差判定
    return IsHit(segment, top) ||
        IsHit(segment, bottom) ||
        IsHit(segment, left) ||
        IsHit(segment, right);
}

bool IsCollision::IsHit(const Segment& segment, const OBB& obb)
{
    return false;
}

/*
 *	指定した円同士の当たり判定
 */
bool
IsCollision::
IsHit(const Circle& circle1, const Circle& circle2)
{
    float radius = circle1.Radius + circle2.Radius;

    return DistanceSq(circle1.Center, circle2.Center) <= radius * radius;
}

/*
 *	指定した円と長円の当たり判定
 */
bool
IsCollision::
IsHit(const Circle& circle, const Capsule& capsule)
{
    Segment segment =
    {
        capsule.Start,
        capsule.End
    };

	// 半径の和を計算
    float radius = circle.Radius + capsule.Radius;

	// 線分と円の中心の距離の二乗が半径の和の二乗以下なら当たっている
    return DistanceSq(segment, circle.Center) <= radius * radius;
}

/*
 *	指定した円と長方形の当たり判定
 */
bool
IsCollision::
IsHit(const Circle& circle, const AABB& aabb)
{
	// 円の中心からAABBの最も近い点を求める
    float closestX = CLAMP( circle.Center.x, aabb.Position.x, aabb.Position.x + aabb.Width);
    float closestY = CLAMP( circle.Center.y, aabb.Position.y, aabb.Position.y + aabb.Height);

    vivid::Vector2 closest =
    {
        closestX,
        closestY
    };

	// 最近点と円の中心との距離の二乗が半径の二乗以下なら当たっている
    return DistanceSq(circle.Center, closest) <= circle.Radius * circle.Radius;
}

/*
 *	指定した円と回転長方形の当たり判定
 */
bool
IsCollision::
IsHit(const Circle& circle, const OBB& obb)
{
    return false;
}

/*
 *	指定した長円同士の当たり判定
 */
bool
IsCollision::
IsHit(const Capsule& capsule1, const Capsule& capsule2)
{
    Segment segment1 =
    {
        capsule1.Start,
        capsule1.End
    };

    Segment segment2 =
    {
        capsule2.Start,
        capsule2.End
    };

    float radius = capsule1.Radius + capsule2.Radius;

    return DistanceSq(segment1, segment2) <= radius * radius;
}

/*
 *	指定した長円と長方形の当たり判定
 */
bool
IsCollision::
IsHit(const Capsule& capsule, const AABB& aabb)
{
    Segment segment =
    {
        capsule.Start,
        capsule.End
    };

    // 始点または終点がAABB内
    if (IsHit(Point{ capsule.Start }, aabb))
        return true;

    if (IsHit(Point{ capsule.End }, aabb))
        return true;

    Segment top =
    {
        aabb.Position,
        { aabb.Position.x + aabb.Width, aabb.Position.y }
    };

    Segment bottom =
    {
        { aabb.Position.x, aabb.Position.y + aabb.Height },
        { aabb.Position.x + aabb.Width, aabb.Position.y + aabb.Height }
    };

    Segment left =
    {
        aabb.Position,
        { aabb.Position.x, aabb.Position.y + aabb.Height }
    };

    Segment right =
    {
        { aabb.Position.x + aabb.Width, aabb.Position.y },
        { aabb.Position.x + aabb.Width, aabb.Position.y + aabb.Height }
    };

    return  DistanceSq(segment, top) <= capsule.Radius * capsule.Radius ||
            DistanceSq(segment, bottom) <= capsule.Radius * capsule.Radius ||
            DistanceSq(segment, left) <= capsule.Radius * capsule.Radius ||
            DistanceSq(segment, right) <= capsule.Radius * capsule.Radius;
}

/*
 *	指定した長円と回転長方形の当たり判定
 */
bool
IsCollision::
IsHit(const Capsule& capsule, const OBB& obb)
{
    return false;
}

/*
 *	指定した長方形同士の当たり判定
 */
bool
IsCollision::
IsHit(const AABB& aabb1, const AABB& aabb2)
{
    return  aabb1.Position.x <= aabb2.Position.x + aabb2.Width &&
            aabb1.Position.x + aabb1.Width >= aabb2.Position.x &&
            aabb1.Position.y <= aabb2.Position.y + aabb2.Height &&
            aabb1.Position.y + aabb1.Height >= aabb2.Position.y;
}   

/*
 *	指定した長方形と回転長方形の当たり判定
 */
bool
IsCollision::
IsHit(const AABB& aabb, const OBB& obb)
{
    return false;
}

/*
 *	指定した回転長方形同士の当たり判定
 */
bool
IsCollision::
IsHit(const OBB& obb1, const OBB& obb2)
{
    return false;
}
