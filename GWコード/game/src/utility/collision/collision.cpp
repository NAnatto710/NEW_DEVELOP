
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

        // 線の長さがほぼ0なら、始点と同じ場所にあるかだけを見る
        if (lengthSq <= Utility::EPSILON * Utility::EPSILON)
        {
            return DistanceSq(point, segment.Start) <= Utility::EPSILON * Utility::EPSILON;
        }

        // 点が線から外れているなら、線の上にはない
        if (std::fabs(Cross(direction, toPoint)) > Utility::EPSILON)
        {
            return false;
        }

        // 点が始点と終点の間にあるかを確認する
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

        // お互いの線をまたいでいれば交差している
        if (aStraddles && bStraddles) return true;

        // 線の端や、線が重なる場合も確認する
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

        // 点から一番近い位置が、線の始点からどれくらい先か求める
        float t = Dot(Sub(point, segment.Start), direction) / lengthSq;
        // 始点より前や終点より先には出ないようにする
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
        // 交差している2本の線の距離は0
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
        // 線の端が四角形の中なら当たっている
        if (PointInAABB(segment.Start, aabb) || PointInAABB(segment.End, aabb)) return true;

        Utility::Segment edges[4];
        GetAABBEdges(aabb, edges);

        // 線が四角形の辺のどれかと交差するか調べる
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
        // 四角形を回していない状態に合わせると、内側かを調べやすい
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

        // 中心から左右と上下へずらして、回転後の4つの角を作る
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
        // 四角形の4つの角が、この方向にどこまで広がるかを調べる
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
        // 4方向のうち1方向でも離れていれば、当たっていない
        for (int i = 0; i < 4; ++i)
            if (!OverlapOnAxis(a, b, axes[i])) return false;

        return true;
    }
    /*
     *  めり込み情報が計算できない形状の判定結果を作る
     */
    Utility::CollisionResult
    HitOnly(bool hit)
    {
        Utility::CollisionResult result;
        // この形状では当たったかどうかだけを記録する
        result.IsHit = hit;
        return result;
    }

    /*
     *  押し戻し可能な衝突結果を作る
     *  Normalは第1引数から第2引数へ向ける
     */
    Utility::CollisionResult
    MakeContact(const Vec2& normal, float depth)
    {
        Utility::CollisionResult result;
        // 当たり・押し戻す向き・重なりの深さをまとめる
        result.IsHit = true;
        result.Normal = normal;
        result.PenetrationDepth = Utility::Max(0.0f, depth);
        result.HasSeparation = true;
        return result;
    }

    /*
     *  接触点を取得できた結果へ追加する
     */
    Utility::CollisionResult
    WithPoint(Utility::CollisionResult result, const Vec2& point)
    {
        // どのあたりで当たったかを追加する
        result.ContactPoint = point;
        result.HasContactPoint = true;
        return result;
    }

    /*
     *  線分上で指定座標に一番近い点を取得する
     */
    Vec2
    ClosestPoint(const Utility::Segment& segment, const Vec2& point)
    {
        const Vec2 direction = Sub(segment.End, segment.Start);
        const float lengthSq = Dot(direction, direction);
        // 線がほぼ点なら、その点を返す
        if (lengthSq <= Utility::EPSILON * Utility::EPSILON) return segment.Start;

        const float t = Utility::Clamp(Dot(Sub(point, segment.Start), direction) / lengthSq, 0.0f, 1.0f);
        return Add(segment.Start, Mul(direction, t));
    }

    /*
     *  線分同士の最近接点を求める
     *  交差・平行・長さ0の線分にも対応する
     */
    void
    ClosestPoints(const Utility::Segment& a, const Utility::Segment& b, Vec2& pointA, Vec2& pointB)
    {
        const Vec2 d1 = Sub(a.End, a.Start);
        const Vec2 d2 = Sub(b.End, b.Start);
        const Vec2 r = Sub(a.Start, b.Start);
        const float lenA = Dot(d1, d1);
        const float lenB = Dot(d2, d2);
        const float epsilonSq = Utility::EPSILON * Utility::EPSILON;
        float s = 0.0f;
        float t = 0.0f;

        // 両方とも長さがない場合は、それぞれの始点を使う
        if (lenA <= epsilonSq && lenB <= epsilonSq)
        {
            pointA = a.Start;
            pointB = b.Start;
            return;
        }
        // Aだけ長さがない場合は、B上で一番近い位置を探す
        if (lenA <= epsilonSq)
        {
            t = Utility::Clamp(Dot(d2, r) / lenB, 0.0f, 1.0f);
        }
        // Bだけ長さがない場合は、A上で一番近い位置を探す
        else if (lenB <= epsilonSq)
        {
            s = Utility::Clamp(-Dot(d1, r) / lenA, 0.0f, 1.0f);
        }
        else
        {
            const float e = Dot(d2, r);
            const float bDot = Dot(d1, d2);
            // 2本が平行に近いときは、無理に割り算しない
            const float denominator = lenA * lenB - bDot * bDot;
            if (denominator > epsilonSq)
                s = Utility::Clamp((bDot * e - Dot(d1, r) * lenB) / denominator, 0.0f, 1.0f);
            t = (bDot * s + e) / lenB;
            // 一番近い位置がBの外に出たら、Bの端に合わせ直す
            if (t < 0.0f)
            {
                t = 0.0f;
                s = Utility::Clamp(-Dot(d1, r) / lenA, 0.0f, 1.0f);
            }
            else if (t > 1.0f)
            {
                t = 1.0f;
                s = Utility::Clamp((bDot - Dot(d1, r)) / lenA, 0.0f, 1.0f);
            }
        }
        // 最後に、それぞれの線の上で一番近い2点を作る
        pointA = Add(a.Start, Mul(d1, s));
        pointB = Add(b.Start, Mul(d2, t));
    }

    /*
     *  AABBの中心と半サイズを求める
     */
    Utility::OBB
    ToOBB(const Utility::AABB& aabb)
    {
        const Bounds bounds = GetBounds(aabb);
        return Utility::OBB
        {
            Vec2((bounds.Left + bounds.Right) * 0.5f, (bounds.Top + bounds.Bottom) * 0.5f),
            Vec2((bounds.Right - bounds.Left) * 0.5f, (bounds.Bottom - bounds.Top) * 0.5f),
            0.0f
        };
    }

    /*
     *  SATの軸ごとに分離に必要な移動距離を比較する
     *  片方がもう片方を包む場合も正しく最短の脱出距離を求める
     */
    Utility::CollisionResult
    RectangleContact(const Utility::OBB& a, const Utility::OBB& b)
    {
        Vec2 cornersA[4];
        Vec2 cornersB[4];
        // 2つの四角形の角を求める
        GetOBBCorners(a, cornersA);
        GetOBBCorners(b, cornersB);

        const float ca = std::cos(a.Rotation);
        const float sa = std::sin(a.Rotation);
        const float cb = std::cos(b.Rotation);
        const float sb = std::sin(b.Rotation);
        // それぞれの四角形の横・縦、合計4方向で重なりを確認する
        const Vec2 axes[4]
        {
            Vec2(ca, sa), Vec2(-sa, ca),
            Vec2(cb, sb), Vec2(-sb, cb)
        };

        float bestDepth = 3.402823466e+38f;
        Vec2 bestNormal(0.0f, 0.0f);

        // 各方向で重なり具合を調べ、一番小さい重なりを探す
        for (int i = 0; i < 4; ++i)
        {
            float minA, maxA, minB, maxB;
            Project(cornersA, axes[i], minA, maxA);
            Project(cornersB, axes[i], minB, maxB);

            // この方向で離れていれば、四角形同士は当たっていない
            if (maxA < minB - Utility::EPSILON || maxB < minA - Utility::EPSILON)
                return Utility::CollisionResult{};

            // 左右どちらへ離すと少ない移動ですむか比べる
            const float moveLeft = maxA - minB;
            const float moveRight = maxB - minA;
            const float depth = Utility::Max(0.0f, Utility::Min(moveLeft, moveRight));
            const Vec2 normal = moveLeft <= moveRight ? axes[i] : Mul(axes[i], -1.0f);

            // ここまでで最も少ない移動ですむ方向を保存する
            if (depth < bestDepth)
            {
                bestDepth = depth;
                bestNormal = normal;
            }
        }

        return MakeContact(bestNormal, bestDepth);
    }

    /*
     *  円を矩形のローカル空間で判定して、押し戻し情報を求める
     */
    Utility::CollisionResult
    CircleRectangleContact(const Utility::Circle& circle, const Utility::OBB& obb)
    {
        // 四角形の回転を一度取り除いて、円の位置を調べる
        const Vec2 local = ToOBBLocal(circle.Center, obb);
        const float halfX = std::fabs(obb.HalfSize.x);
        const float halfY = std::fabs(obb.HalfSize.y);
        const float radius = Radius(circle.Radius);
        // 円の中心から一番近い、四角形上または内側の点を求める
        const Vec2 nearest(Utility::Clamp(local.x, -halfX, halfX),
                           Utility::Clamp(local.y, -halfY, halfY));
        const Vec2 delta = Sub(nearest, local);
        const float distSq = Dot(delta, delta);

        // 円が四角形まで届かなければ当たっていない
        if (distSq > radius * radius) return Utility::CollisionResult{};

        Vec2 normal;
        float depth;
        // 円の中心が四角形の外なら、近い点までの方向を使う
        if (distSq > Utility::EPSILON * Utility::EPSILON)
        {
            const float dist = std::sqrt(distSq);
            normal = Mul(delta, 1.0f / dist);
            depth = radius - dist;
        }
        else
        {
            // 円の中心が四角形の中なら、一番近い辺の方向を使う
            const float dx = halfX - std::fabs(local.x);
            const float dy = halfY - std::fabs(local.y);
            if (dx <= dy)
            {
                normal = Vec2(local.x >= 0.0f ? -1.0f : 1.0f, 0.0f);
                depth = dx + radius;
            }
            else
            {
                normal = Vec2(0.0f, local.y >= 0.0f ? -1.0f : 1.0f);
                depth = dy + radius;
            }
        }

        // 当たった場所を四角形の表面から求める
        Vec2 surfacePoint = nearest;
        if (distSq <= Utility::EPSILON * Utility::EPSILON)
        {
            if (std::fabs(normal.x) > 0.5f)
                surfacePoint.x = normal.x < 0.0f ? halfX : -halfX;
            else
                surfacePoint.y = normal.y < 0.0f ? halfY : -halfY;
        }
        // 方向と接触位置を、元の回転した四角形の向きに戻す
        return WithPoint(MakeContact(Rotate(normal, obb.Rotation), depth),
                         Add(obb.Center, Rotate(surfacePoint, obb.Rotation)));
    }


}

namespace Utility
{
    namespace Collision
    {
        /*
         *  円と円の衝突情報
         */
        CollisionResult
        Check(const Circle& a, const Circle& b)
        {
            const Vec2 delta = Sub(b.Center, a.Center);
            const float distanceSq = Dot(delta, delta);
            const float radius = Radius(a.Radius) + Radius(b.Radius);
            // 2つの円の半径を合わせた距離より遠ければ当たっていない
            if (distanceSq > radius * radius) return CollisionResult{};

            // 中心が同じ場合は向きを決められないので、右方向を使う
            if (distanceSq <= EPSILON * EPSILON)
                return WithPoint(MakeContact(Vec2(1.0f, 0.0f), radius), a.Center);

            const float distance = std::sqrt(distanceSq);
            const Vec2 normal = Mul(delta, 1.0f / distance);
            // それぞれの円の表面から、当たった場所の目安を作る
            const Vec2 surfaceA = Add(a.Center, Mul(normal, Radius(a.Radius)));
            const Vec2 surfaceB = Sub(b.Center, Mul(normal, Radius(b.Radius)));
            return WithPoint(MakeContact(normal, radius - distance), Mul(Add(surfaceA, surfaceB), 0.5f));
        }

        /*
         *  円と矩形の衝突情報
         */
        CollisionResult
        Check(const Circle& circle, const AABB& aabb)
        {
            return CircleRectangleContact(circle, ToOBB(aabb));
        }

        CollisionResult
        Check(const Circle& circle, const OBB& obb)
        {
            return CircleRectangleContact(circle, obb);
        }

        /*
         *  矩形同士の衝突情報
         */
        CollisionResult
        Check(const AABB& a, const AABB& b)
        {
            return RectangleContact(ToOBB(a), ToOBB(b));
        }

        CollisionResult
        Check(const AABB& aabb, const OBB& obb)
        {
            return RectangleContact(ToOBB(aabb), obb);
        }

        CollisionResult
        Check(const OBB& a, const OBB& b)
        {
            return RectangleContact(a, b);
        }

        /*
         *  SegmentとSegmentの接触情報（押し戻し情報は未対応）
         */
        CollisionResult
        Check(const Segment& a, const Segment& b)
        {
            return HitOnly(Hit(a, b));
        }

        /*
         *  SegmentとCircleの接触情報（押し戻し情報は未対応）
         */
        CollisionResult
        Check(const Segment& a, const Circle& b)
        {
            return HitOnly(Hit(a, b));
        }

        /*
         *  SegmentとCapsuleの接触情報（押し戻し情報は未対応）
         */
        CollisionResult
        Check(const Segment& a, const Capsule& b)
        {
            return HitOnly(Hit(a, b));
        }

        /*
         *  SegmentとAABBの接触情報（押し戻し情報は未対応）
         */
        CollisionResult
        Check(const Segment& a, const AABB& b)
        {
            return HitOnly(Hit(a, b));
        }

        /*
         *  SegmentとOBBの接触情報（押し戻し情報は未対応）
         */
        CollisionResult
        Check(const Segment& a, const OBB& b)
        {
            return HitOnly(Hit(a, b));
        }

        /*
         *  円とカプセルの衝突情報
         */
        CollisionResult
        Check(const Circle& a, const Capsule& b)
        {
            const Segment axis{ b.Start, b.End };
            // カプセルの中心線から、円に一番近い点を探す
            const Vec2 nearest = ClosestPoint(axis, a.Center);
            const Vec2 delta = Sub(nearest, a.Center);
            const float distanceSq = Dot(delta, delta);
            const float radiusA = Radius(a.Radius);
            const float radiusB = Radius(b.Radius);
            const float radius = radiusA + radiusB;
            if (distanceSq > radius * radius) return CollisionResult{};

            const float distance = std::sqrt(distanceSq);
            Vec2 normal(1.0f, 0.0f);
            // 2つの中心が離れていれば、その間の方向を使う
            if (distance > EPSILON)
                normal = Mul(delta, 1.0f / distance);
            else
            {
                // 同じ場所に重なる場合は、カプセルの線に直角な方向を使う
                const Vec2 direction = Sub(b.End, b.Start);
                const float lengthSq = Dot(direction, direction);
                if (lengthSq > EPSILON * EPSILON)
                    normal = Mul(Vec2(-direction.y, direction.x), 1.0f / std::sqrt(lengthSq));
            }

            const Vec2 surfaceA = Add(a.Center, Mul(normal, radiusA));
            const Vec2 surfaceB = Sub(nearest, Mul(normal, radiusB));
            return WithPoint(MakeContact(normal, radius - distance), Mul(Add(surfaceA, surfaceB), 0.5f));
        }

        /*
         *  カプセル同士の衝突情報
         */
        CollisionResult
        Check(const Capsule& a, const Capsule& b)
        {
            const Segment axisA{ a.Start, a.End };
            const Segment axisB{ b.Start, b.End };
            Vec2 nearestA;
            Vec2 nearestB;
            // 2本の中心線で、一番近い場所をそれぞれ探す
            ClosestPoints(axisA, axisB, nearestA, nearestB);
            const Vec2 delta = Sub(nearestB, nearestA);
            const float distanceSq = Dot(delta, delta);
            const float radiusA = Radius(a.Radius);
            const float radiusB = Radius(b.Radius);
            const float radius = radiusA + radiusB;
            if (distanceSq > radius * radius) return CollisionResult{};

            const float distance = std::sqrt(distanceSq);
            Vec2 normal(1.0f, 0.0f);
            if (distance > EPSILON)
                normal = Mul(delta, 1.0f / distance);
            else
            {
                // 2本の線が重なる場合は、Aの線に直角な方向を使う
                const Vec2 axis = Sub(a.End, a.Start);
                const float lengthSq = Dot(axis, axis);
                if (lengthSq > EPSILON * EPSILON)
                    normal = Mul(Vec2(-axis.y, axis.x), 1.0f / std::sqrt(lengthSq));
            }
            const Vec2 surfaceA = Add(nearestA, Mul(normal, radiusA));
            const Vec2 surfaceB = Sub(nearestB, Mul(normal, radiusB));
            return WithPoint(MakeContact(normal, radius - distance), Mul(Add(surfaceA, surfaceB), 0.5f));
        }

        /*
         *  CapsuleとAABBの接触情報（押し戻し情報は未対応）
         */
        CollisionResult
        Check(const Capsule& a, const AABB& b)
        {
            return HitOnly(Hit(a, b));
        }

        /*
         *  CapsuleとOBBの接触情報（押し戻し情報は未対応）
         */
        CollisionResult
        Check(const Capsule& a, const OBB& b)
        {
            return HitOnly(Hit(a, b));
        }

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
