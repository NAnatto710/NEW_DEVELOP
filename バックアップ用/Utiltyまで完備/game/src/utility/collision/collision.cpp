
/*!
 *  @file       collision.cpp
 *  @brief      2D当たり判定
 *  @author     Ryusei Shimizu
 *  @date       2026/09/16
 */

#include <cmath>
#include "../utility.h"

namespace
{
    using Vec2 = vivid::Vector2;

    /*
     *  AABBの左・上・右・下の境界値をまとめて扱うための内部構造体
     */
    struct Bounds
    {
        float Left;
        float Top;
        float Right;
        float Bottom;
    };

    /*
     *  足し算
     */
    Vec2
    Add(const Vec2& a, const Vec2& b)
    {
        return Vec2(a.x + b.x, a.y + b.y);
    }
    
    /*
     *  引き算
     */
    Vec2
    Sub(const Vec2& a, const Vec2& b)
    {
        return Vec2(a.x - b.x, a.y - b.y);
    }

    /*
     *  倍率
     */
    Vec2
    Mul(const Vec2& v, float scalar)
    {
        return Vec2(v.x * scalar, v.y * scalar);
    }

    /*
     *  内積
     *  2つのベクトルがどれだけ同じ方向を見ているか
     */
    float
    Dot(const Vec2& a, const Vec2& b)
    {
        return a.x * b.x + a.y * b.y;
    }

    /*
     *  外積
     *  BがAから見てどちら側にあるか
     */
    float
    Cross(const Vec2& a, const Vec2& b)
    {
        return a.x * b.y - a.y * b.x;
    }

    /*
     *  円の当たり判定
     */
    float
    DistanceSq(const Vec2& a, const Vec2& b)
    {
        const float dx = b.x - a.x;
        const float dy = b.y - a.y;
        return dx * dx + dy * dy;
    }

    /*
     *  半径を絶対値で返す
     */
    float
    Radius(float radius)
    {
        return std::fabs(radius);
    }

    /*
     *  AABB用の補助処理
     *  Width / Heightが負の場合でも正しい境界を求められるようにする
     */
    Bounds 
    GetBounds(const Utility::AABB& aabb)
    {
        const float x2 = aabb.Position.x + aabb.Width;
        const float y2 = aabb.Position.y + aabb.Height;

        Bounds bounds;
        bounds.Left = Utility::Min(aabb.Position.x, x2);
        bounds.Top = Utility::Min(aabb.Position.y, y2);
        bounds.Right = Utility::Max(aabb.Position.x, x2);
        bounds.Bottom = Utility::Max(aabb.Position.y, y2);
        return bounds;
    }

    /*
     *  点が線分上に存在するか判定する
     *  外積で直線上かを確認し、内積で始点～終点の範囲内かを確認する
     */
    bool
    PointOnSegment(const Vec2& point, const Utility::Segment& segment)
    {
        const Vec2 direction = Sub(segment.End, segment.Start);
        const Vec2 toPoint = Sub(point, segment.Start);
        const float lengthSq = Dot(direction, direction);

        if (lengthSq <= Utility::EPSILON * Utility::EPSILON)
        {
            return DistanceSq(point, segment.Start) <= Utility::EPSILON * Utility::EPSILON;
        }

        if (std::fabs(Cross(direction, toPoint)) > Utility::EPSILON)
        {
            return false;
        }

        const float dot = Dot(toPoint, direction);
        return dot >= -Utility::EPSILON && dot <= lengthSq + Utility::EPSILON;
    }

    /*
     *  2本の線分が交差しているか判定する
     *  外積を使って互いの端点が線分の両側にあるかを確認する
     */
    bool
    SegmentIntersectsSegment(const Utility::Segment& a, const Utility::Segment& b)
    {
        const Vec2 aDir = Sub(a.End, a.Start);
        const float c1 = Cross(aDir, Sub(b.Start, a.Start));
        const float c2 = Cross(aDir, Sub(b.End, a.Start));

        const Vec2 bDir = Sub(b.End, b.Start);
        const float c3 = Cross(bDir, Sub(a.Start, b.Start));
        const float c4 = Cross(bDir, Sub(a.End, b.Start));

        const bool aStraddles = (c1 > Utility::EPSILON && c2 < -Utility::EPSILON) ||
                                (c1 < -Utility::EPSILON && c2 > Utility::EPSILON);

        const bool bStraddles = (c3 > Utility::EPSILON && c4 < -Utility::EPSILON) ||
                                (c3 < -Utility::EPSILON && c4 > Utility::EPSILON);

        if (aStraddles && bStraddles) return true;

        return (std::fabs(c1) <= Utility::EPSILON && PointOnSegment(b.Start, a)) ||
               (std::fabs(c2) <= Utility::EPSILON && PointOnSegment(b.End, a)) ||
               (std::fabs(c3) <= Utility::EPSILON && PointOnSegment(a.Start, b)) ||
               (std::fabs(c4) <= Utility::EPSILON && PointOnSegment(a.End, b));
    }

    /*
     *  線分と点の最短距離を二乗値で求める
     *  sqrtを使わず二乗距離のまま比較することで計算量を抑える
     */
    float
    DistanceSq(const Utility::Segment& segment, const Vec2& point)
    {
        const Vec2 direction = Sub(segment.End, segment.Start);
        const float lengthSq = Dot(direction, direction);

        if (lengthSq <= Utility::EPSILON * Utility::EPSILON)
        {
            return DistanceSq(segment.Start, point);
        }

        float t = Dot(Sub(point, segment.Start), direction) / lengthSq;
        t = Utility::Clamp(t, 0.0f, 1.0f);

        const Vec2 nearest = Add(segment.Start, Mul(direction, t));
        return DistanceSq(nearest, point);
    }

    /*
     *  2本の線分同士の最短距離を二乗値で求める
     *  交差している場合の距離は0として扱う
     */
    float
    DistanceSq(const Utility::Segment& a, const Utility::Segment& b)
    {
        if (SegmentIntersectsSegment(a, b)) return 0.0f;

        return Utility::Min(Utility::Min(DistanceSq(a, b.Start), DistanceSq(a, b.End)),
                            Utility::Min(DistanceSq(b, a.Start), DistanceSq(b, a.End)));
    }

    /*
     *  点が四角形の中にあるか
     */
    bool PointInAABB(const Vec2& point, const Utility::AABB& aabb)
    {
        const Bounds bounds = GetBounds(aabb);

        return point.x >= bounds.Left - Utility::EPSILON &&
               point.x <= bounds.Right + Utility::EPSILON &&
               point.y >= bounds.Top - Utility::EPSILON &&
               point.y <= bounds.Bottom + Utility::EPSILON;
    }

    /*
     *  長方形の4辺を4本の線分として取り出す
     */
    void GetAABBEdges(const Utility::AABB& aabb, Utility::Segment (&edges)[4])
    {
        const Bounds b = GetBounds(aabb);
        const Vec2 lt(b.Left, b.Top);
        const Vec2 rt(b.Right, b.Top);
        const Vec2 rb(b.Right, b.Bottom);
        const Vec2 lb(b.Left, b.Bottom);

        edges[0] = Utility::Segment{ lt, rt };
        edges[1] = Utility::Segment{ rt, rb };
        edges[2] = Utility::Segment{ rb, lb };
        edges[3] = Utility::Segment{ lb, lt };
    }

    /*
     *  線分とAABBが交差しているか判定する
     *  端点が矩形内にある場合、または矩形4辺のどれかと交差する場合をHitとする
     */
    bool SegmentIntersectsAABB(const Utility::Segment& segment, const Utility::AABB& aabb)
    {
        if (PointInAABB(segment.Start, aabb) || PointInAABB(segment.End, aabb)) return true;

        Utility::Segment edges[4];
        GetAABBEdges(aabb, edges);

        for (int i = 0; i < 4; ++i)
            if (SegmentIntersectsSegment(segment, edges[i])) return true;

        return false;
    }

    /*
     *  ベクトルの回転
     */
    Vec2
    Rotate(const Vec2& v, float angle)
    {
        const float c = std::cos(angle);
        const float s = std::sin(angle);
        return Vec2(v.x * c - v.y * s, v.x * s + v.y * c);
    }

    /*
     *  ローカル座標への変換
     */
    Vec2
    ToOBBLocal(const Vec2& point, const Utility::OBB& obb)
    {
        return Rotate(Sub(point, obb.Center), -obb.Rotation);
    }

    /*
     *  回転を取り除いたOBBを、普通のAABBとする
     */
    Utility::AABB
    OBBLocalAABB(const Utility::OBB& obb)
    {
        const float halfX = std::fabs(obb.HalfSize.x);
        const float halfY = std::fabs(obb.HalfSize.y);
        return Utility::AABB{ Vec2(-halfX, -halfY), halfX * 2.0f, halfY * 2.0f };
    }

    /*
     *  AABBの4つの角を取得
     */
    void
    GetAABBCorners(const Utility::AABB& aabb, Vec2 (&corners)[4])
    {
        const Bounds b = GetBounds(aabb);
        corners[0] = Vec2(b.Left, b.Top);
        corners[1] = Vec2(b.Right, b.Top);
        corners[2] = Vec2(b.Right, b.Bottom);
        corners[3] = Vec2(b.Left, b.Bottom);
    }

    /*
     *  回転しているOBBの4頂点の取得
     */
    void
    GetOBBCorners(const Utility::OBB& obb, Vec2 (&corners)[4])
    {
        const float halfX = std::fabs(obb.HalfSize.x);
        const float halfY = std::fabs(obb.HalfSize.y);
        const float c = std::cos(obb.Rotation);
        const float s = std::sin(obb.Rotation);

        const Vec2 axisX(c, s);
        const Vec2 axisY(-s, c);
        const Vec2 x = Mul(axisX, halfX);
        const Vec2 y = Mul(axisY, halfY);

        corners[0] = Sub(Sub(obb.Center, x), y);
        corners[1] = Add(Sub(obb.Center, y), x);
        corners[2] = Add(Add(obb.Center, x), y);
        corners[3] = Add(Sub(obb.Center, x), y);
    }

    /*
     *  四角形を指定した軸に投影して、どこからどこまで存在するか求める
     */
    void
    Project(const Vec2 (&corners)[4], const Vec2& axis, float& min, float& max)
    {
        min = Dot(corners[0], axis);
        max = min;

        for (int i = 1; i < 4; ++i)
        {
            const float value = Dot(corners[i], axis);
            min = Utility::Min(min, value);
            max = Utility::Max(max, value);
        }
    }

    /*
     *  軸のみを見たときにAとBが重なっているか
     */
    bool
    OverlapOnAxis(const Vec2 (&a)[4], const Vec2 (&b)[4], const Vec2& axis)
    {
        float minA = 0.0f;
        float maxA = 0.0f;
        float minB = 0.0f;
        float maxB = 0.0f;

        Project(a, axis, minA, maxA);
        Project(b, axis, minB, maxB);

        return maxA >= minB - Utility::EPSILON &&
               maxB >= minA - Utility::EPSILON;
    }

    /*
     *  軸の衝突判定
     */
    bool
    SAT(const Vec2 (&a)[4], const Vec2 (&b)[4], const Vec2 (&axes)[4])
    {
        for (int i = 0; i < 4; ++i)
            if (!OverlapOnAxis(a, b, axes[i])) return false;

        return true;
    }
}

namespace Utility
{
    namespace Collision
    {
        /*
         *  点と点
         */
        bool
        Hit(const Point& a, const Point& b)
        {
            return DistanceSq(a.Position, b.Position) <= EPSILON * EPSILON;
        }
        
        /*
         *  点と線
         */
        bool
        Hit(const Point& point, const Segment& segment)
        {
            return PointOnSegment(point.Position, segment);
        }
        
        /*
         *  点と円
         */
        bool
        Hit(const Point& point, const Circle& circle)
        {
            const float radius = Radius(circle.Radius);
            return DistanceSq(point.Position, circle.Center) <= radius * radius;
        }
        
        /*
         *  点とカプセル
         */
        bool
        Hit(const Point& point, const Capsule& capsule)
        {
            const Segment segment{ capsule.Start, capsule.End };
            const float radius = Radius(capsule.Radius);
            return DistanceSq(segment, point.Position) <= radius * radius;
        }
        
        /*
         *  点と四角形
         */
        bool
        Hit(const Point& point, const AABB& aabb)
        {
            return PointInAABB(point.Position, aabb);
        }
        
        /*
         *  点と回転する四角形
         */
        bool
        Hit(const Point& point, const OBB& obb)
        {
            // 点をOBBのローカル座標へ変換し、回転していないAABBとして判定する
            const Point local{ ToOBBLocal(point.Position, obb) };
            return Hit(local, OBBLocalAABB(obb));
        }
        
        /*
         *  線分と線分
         */
        bool
        Hit(const Segment& a, const Segment& b)
        {
            return SegmentIntersectsSegment(a, b);
        }
        
        /*
         *  線分と円
         */
        bool
        Hit(const Segment& segment, const Circle& circle)
        {
            const float radius = Radius(circle.Radius);
            return DistanceSq(segment, circle.Center) <= radius * radius;
        }
        
        /*
         *  線分とカプセル
         */
        bool
        Hit(const Segment& segment, const Capsule& capsule)
        {
            const Segment capsuleSegment{ capsule.Start, capsule.End };
            const float radius = Radius(capsule.Radius);
            return DistanceSq(segment, capsuleSegment) <= radius * radius;
        }
        
        /*
         *  線分と四角形
         */
        bool
        Hit(const Segment& segment, const AABB& aabb)
        {
            return SegmentIntersectsAABB(segment, aabb);
        }
        
        /*
         *  線分と回転する四角形
         */
        bool
        Hit(const Segment& segment, const OBB& obb)
        {
            // 線分の始点・終点をOBBのローカル座標へ変換してAABB判定を再利用する
            const Segment local
            {
                ToOBBLocal(segment.Start, obb),
                ToOBBLocal(segment.End, obb)
            };
        
            return Hit(local, OBBLocalAABB(obb));
        }
        
        /*
         *  円と円
         */
        bool
        Hit(const Circle& a, const Circle& b)
        {
            const float radius = Radius(a.Radius) + Radius(b.Radius);
            return DistanceSq(a.Center, b.Center) <= radius * radius;
        }
        
        /*
         *  円とカプセル
         */
        bool
        Hit(const Circle& circle, const Capsule& capsule)
        {
            const Segment segment{ capsule.Start, capsule.End };
            const float radius = Radius(circle.Radius) + Radius(capsule.Radius);
            return DistanceSq(segment, circle.Center) <= radius * radius;
        }
        
        /*
         *  円と四角形
         */
        bool
        Hit(const Circle& circle, const AABB& aabb)
        {
            // 円の中心から矩形内で最も近い点を求め、その点までの距離と半径を比較する
            const Bounds b = GetBounds(aabb);
            const float closestX = Clamp(circle.Center.x, b.Left, b.Right);
            const float closestY = Clamp(circle.Center.y, b.Top, b.Bottom);
            const float radius = Radius(circle.Radius);
        
            return DistanceSq(circle.Center, Vec2(closestX, closestY)) <= radius * radius;
        }
        
        /*
         *  円と回転する四角形
         */
        bool
        Hit(const Circle& circle, const OBB& obb)
        {
            // 円の中心をOBBのローカル座標へ変換し、Circle × AABB判定を再利用する
            const Circle local{ ToOBBLocal(circle.Center, obb), Radius(circle.Radius) };
            return Hit(local, OBBLocalAABB(obb));
        }
        
        /*
         *  カプセルとカプセル
         */
        bool
        Hit(const Capsule& a, const Capsule& b)
        {
            const Segment segmentA{ a.Start, a.End };
            const Segment segmentB{ b.Start, b.End };
            const float radius = Radius(a.Radius) + Radius(b.Radius);
        
            return DistanceSq(segmentA, segmentB) <= radius * radius;
        }
        
        /*
         *  カプセルと四角形
         */
        bool
        Hit(const Capsule& capsule, const AABB& aabb)
        {
            // まずカプセル中心線がAABBと交差するか確認し、交差しない場合は各辺との距離を調べる
            const Segment centerLine{ capsule.Start, capsule.End };
        
            if (SegmentIntersectsAABB(centerLine, aabb)) return true;
        
            Utility::Segment edges[4];
            GetAABBEdges(aabb, edges);
        
            const float radius = Radius(capsule.Radius);
            const float radiusSq = radius * radius;
        
            for (int i = 0; i < 4; ++i)
                if (DistanceSq(centerLine, edges[i]) <= radiusSq) return true;
        
            return false;
        }
        
        /*
         *  カプセルと回転する四角形
         */
        bool
        Hit(const Capsule& capsule, const OBB& obb)
        {
            // カプセル全体をOBBのローカル座標へ変換し、Capsule × AABB判定を再利用する
            const Capsule local
            {
                ToOBBLocal(capsule.Start, obb),
                ToOBBLocal(capsule.End, obb),
                Radius(capsule.Radius)
            };
        
            return Hit(local, OBBLocalAABB(obb));
        }
        
        /*
         *  四角形と四角形
         */
        bool
        Hit(const AABB& a, const AABB& b)
        {
            const Bounds boundsA = GetBounds(a);
            const Bounds boundsB = GetBounds(b);
        
            return boundsA.Left <= boundsB.Right + EPSILON &&
                   boundsA.Right >= boundsB.Left - EPSILON &&
                   boundsA.Top <= boundsB.Bottom + EPSILON &&
                   boundsA.Bottom >= boundsB.Top - EPSILON;
        }
        
        /*
         *  四角形と回転する四角形
         */
        bool Hit(const AABB& aabb, const OBB& obb)
        {
            // AABBとOBBそれぞれの4頂点を作り、両方の矩形軸を使ってSAT判定する
            Vec2 a[4];
            Vec2 b[4];
            GetAABBCorners(aabb, a);
            GetOBBCorners(obb, b);
        
            const float c = std::cos(obb.Rotation);
            const float s = std::sin(obb.Rotation);
        
            const Vec2 axes[4]
            {
                Vec2(1.0f, 0.0f),
                Vec2(0.0f, 1.0f),
                Vec2(c, s),
                Vec2(-s, c)
            };
        
            return SAT(a, b, axes);
        }
        
        /*
         *  回転する四角形と回転する四角形
         */
        bool
        Hit(const OBB& a, const OBB& b)
        {
            // 2つのOBBの4頂点を作り、それぞれの回転軸を使ってSAT判定する
            Vec2 cornersA[4];
            Vec2 cornersB[4];
            GetOBBCorners(a, cornersA);
            GetOBBCorners(b, cornersB);
        
            const float ca = std::cos(a.Rotation);
            const float sa = std::sin(a.Rotation);
            const float cb = std::cos(b.Rotation);
            const float sb = std::sin(b.Rotation);
        
            const Vec2 axes[4]
            {
                Vec2(ca, sa),
                Vec2(-sa, ca),
                Vec2(cb, sb),
                Vec2(-sb, cb)
            };
        
            return SAT(cornersA, cornersB, axes);
        }
    }
}
