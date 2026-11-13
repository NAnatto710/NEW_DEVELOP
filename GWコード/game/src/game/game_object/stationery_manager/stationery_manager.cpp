/*!
 *  @file           stationery_manager.cpp
 *  @brief          文房具管理
 *  @date           2026/11/05
 */

#include "stationery_manager.h"
#include "stationery/stationery.h"
#include "stationery/ruler/ruler.h"

#include <cmath>
#include <array>
#include "../../../utility/utility.h"

/*
 *  インスタンスの取得
 */
CStationeryManager&
CStationeryManager::
GetInstance(void)
{
    static CStationeryManager instance;
    return instance;
}

/*
 *  デストラクタ
 */
CStationeryManager::
~CStationeryManager(void) = default;

/*
 *  初期化
 */
void
CStationeryManager::
Initialize(void)
{
    // シーン再入場時に前回の文房具を残さない
    Finalize();
}

/*
 *  更新
 */
void
CStationeryManager::
Update(void)
{
    m_Controller.Update(m_Stationeries);

    for (const auto& stationery : m_Stationeries)
    {
        stationery->Update();

        // ドラッグ中は移動と回転をユーザーの操作に任せる
        if (!m_Controller.IsDragging(stationery.get()))
        {
            stationery->UpdatePhysics();
        }
    }

    // 地形との物理更新後に文房具同士を解決する
    ResolveStationeryPairs();
}

/*
 *  描画
 */
void
CStationeryManager::
Draw(void)
{
    for (const auto& stationery : m_Stationeries)
    {
        stationery->Draw();
    }
}

/*
 *  解放
 */
void
CStationeryManager::
Finalize(void)
{
    // 選択中のポインタが解放済みオブジェクトを指さないようにする
    m_Controller.Reset();
    m_Stationeries.clear();
}

/*
 *  定規生成
 */
CRuler*
CStationeryManager::
CreateRuler(const vivid::Vector2& position)
{
    auto ruler = std::make_unique<CRuler>();
    ruler->Initialize(position);

    // 所有権の移動後も実体のアドレスは変わらない
    CRuler* result = ruler.get();
    m_Stationeries.emplace_back(std::move(ruler));

    return result;
}

/*
 *  文房具の個数取得
 */
std::size_t
CStationeryManager::
GetCount(void) const
{
    return m_Stationeries.size();
}

/*
 *  定規を掴んでいるか
 */
bool
CStationeryManager::
IsDragging(void) const
{
    return m_Controller.IsDragging();
}

/*
 *  配置済み文房具との当たり判定
 */
bool
CStationeryManager::
CheckCollision(const Utility::AABB& player,
               Utility::CollisionResult& result,
               IStationery*& stationery) const
{
    result = Utility::CollisionResult{};
    stationery = nullptr;

    // 最も深く重なった文房具を返す（複数接触時の暫定処理）
    for (const auto& item : m_Stationeries)
    {
        IStationery* target = item.get();

        // 配置前およびマウスで移動中の文房具は衝突対象外
        if (target->GetState() == STATIONERY_STATE::HELD ||
            m_Controller.IsDragging(target))
        {
            continue;
        }

        const vivid::Vector2& center = target->GetPosition();
        const vivid::Vector2& size = target->GetSize();
        const Utility::OBB collider = {
            center,
            {size.x * 0.5f, size.y * 0.5f},
            target->GetRotation()
        };

        const Utility::CollisionResult hit =
            Utility::Collision::Check(player, collider);

        if (hit.IsHit && hit.HasSeparation &&
            (stationery == nullptr ||
             hit.PenetrationDepth > result.PenetrationDepth))
        {
            result = hit;
            stationery = target;
        }
    }

    return stationery != nullptr;
}


namespace
{
    struct SPairVector
    {
        float x;
        float y;
    };

    SPairVector operator+(SPairVector a, SPairVector b)
    {
        return {a.x + b.x, a.y + b.y};
    }

    SPairVector operator-(SPairVector a, SPairVector b)
    {
        return {a.x - b.x, a.y - b.y};
    }

    SPairVector operator*(SPairVector a, float s)
    {
        return {a.x * s, a.y * s};
    }

    float PairDot(SPairVector a, SPairVector b)
    {
        return a.x * b.x + a.y * b.y;
    }

    float PairCross(SPairVector a, SPairVector b)
    {
        return a.x * b.y - a.y * b.x;
    }

    SPairVector PairPerp(SPairVector a)
    {
        return {-a.y, a.x};
    }

    SPairVector AsPairVector(const vivid::Vector2& a)
    {
        return {a.x, a.y};
    }

    std::array<SPairVector, 4>
    GetPairCorners(const IStationery& item)
    {
        const vivid::Vector2& center = item.GetPosition();
        const vivid::Vector2& size = item.GetSize();
        const float c = std::cos(item.GetRotation());
        const float s = std::sin(item.GetRotation());
        const float hx = size.x * 0.5f;
        const float hy = size.y * 0.5f;
        const std::array<SPairVector, 4> local = {{{-hx, -hy},
            {hx, -hy}, {hx, hy}, {-hx, hy}}};
        std::array<SPairVector, 4> result{};

        for (int i = 0; i < 4; ++i)
        {
            result[i] = {center.x + local[i].x * c - local[i].y * s,
                         center.y + local[i].x * s + local[i].y * c};
        }
        return result;
    }

    /*
     *  向きに最も張り出した頂点を求める
     *  ほぼ同じ投影位置にある頂点は平均して接触辺の中央にする
     */
    SPairVector
    GetPairSupport(const IStationery& item, SPairVector direction)
    {
        const auto corners = GetPairCorners(item);
        float best = PairDot(corners[0], direction);
        for (const SPairVector corner : corners)
        {
            best = Utility::Max(best, PairDot(corner, direction));
        }
        SPairVector sum{0.0f, 0.0f};
        int count = 0;
        for (const SPairVector corner : corners)
        {
            if (best - PairDot(corner, direction) < 0.5f)
            {
                sum = sum + corner;
                ++count;
            }
        }
        if (count <= 0) return corners[0];
        return sum * (1.0f / static_cast<float>(count));
    }
}

/*
 *  文房具同士の衝突処理
 *  地形の既存ソルバは変更しない。2物体の速度と位置を補正する。
 */
void
CStationeryManager::
ResolveStationeryPairs(void)
{
    constexpr float CONTACT_SLOP = 0.05f;
    constexpr float CORRECTION_RATE = 0.45f;
    constexpr float MAX_CORRECTION = 3.0f;
    constexpr float FRICTION = 0.50f;
    constexpr int PAIR_ITERATIONS = 6;

    // 物体数が2未満なら文房具同士の衝突はない
    if (m_Stationeries.size() < 2) return;

    for (int iteration = 0; iteration < PAIR_ITERATIONS; ++iteration)
    {
        for (std::size_t i = 0; i < m_Stationeries.size(); ++i)
        {
            IStationery* a = m_Stationeries[i].get();
            if (m_Controller.IsDragging(a) || a->GetState() == STATIONERY_STATE::HELD) continue;

            for (std::size_t j = i + 1; j < m_Stationeries.size(); ++j)
            {
                IStationery* b = m_Stationeries[j].get();
                if (m_Controller.IsDragging(b) || b->GetState() == STATIONERY_STATE::HELD) continue;

                const Utility::OBB box_a = {
                    a->m_Position, {a->m_Size.x * 0.5f, a->m_Size.y * 0.5f}, a->m_Rotation
                };
                const Utility::OBB box_b = {
                    b->m_Position, {b->m_Size.x * 0.5f, b->m_Size.y * 0.5f}, b->m_Rotation
                };
                const Utility::CollisionResult hit = Utility::Collision::Check(box_a, box_b);
                if (!hit.IsHit || !hit.HasSeparation || hit.PenetrationDepth <= 0.0f)
                {
                    continue;
                }

                // 法線はaからbへの向き
                SPairVector normal = AsPairVector(hit.Normal);
                const float normal_sq = PairDot(normal, normal);
                if (normal_sq < 0.5f) continue;
                normal = normal * (1.0f / std::sqrt(normal_sq));

                const SPairVector pa = AsPairVector(a->m_Position);
                const SPairVector pb = AsPairVector(b->m_Position);
                const SPairVector point_a = GetPairSupport(*a, normal);
                const SPairVector point_b = GetPairSupport(*b, normal * -1.0f);
                const SPairVector point = (point_a + point_b) * 0.5f;
                const SPairVector ra = point - pa;
                const SPairVector rb = point - pb;

                // 質量は両方1、慣性モーメントは一様な矩形として計算
                const float inertia_a = 12.0f / (a->m_Size.x * a->m_Size.x +
                                                 a->m_Size.y * a->m_Size.y);
                const float inertia_b = 12.0f / (b->m_Size.x * b->m_Size.x +
                                                 b->m_Size.y * b->m_Size.y);
                const SPairVector va = AsPairVector(a->m_PhysicsVelocity) +
                    PairPerp(ra) * a->m_PhysicsAngularVelocity;
                const SPairVector vb = AsPairVector(b->m_PhysicsVelocity) +
                    PairPerp(rb) * b->m_PhysicsAngularVelocity;
                const float vn = PairDot(vb - va, normal);

                if (vn < 0.0f)
                {
                    const float ca = PairCross(ra, normal);
                    const float cb = PairCross(rb, normal);
                    const float effective_mass = 2.0f + ca * ca * inertia_a + cb * cb * inertia_b;
                    if (effective_mass > 0.000001f)
                    {
                        const float impulse = -vn / effective_mass;
                        const SPairVector push = normal * impulse;
                        a->m_PhysicsVelocity.x -= push.x;
                        a->m_PhysicsVelocity.y -= push.y;
                        b->m_PhysicsVelocity.x += push.x;
                        b->m_PhysicsVelocity.y += push.y;
                        a->m_PhysicsAngularVelocity -= ca * impulse * inertia_a;
                        b->m_PhysicsAngularVelocity += cb * impulse * inertia_b;

                        // 接線方向の相対速度を簡易摩擦で減らす
                        const SPairVector tangent = PairPerp(normal);
                        const SPairVector va_next = AsPairVector(a->m_PhysicsVelocity) +
                            PairPerp(ra) * a->m_PhysicsAngularVelocity;
                        const SPairVector vb_next = AsPairVector(b->m_PhysicsVelocity) +
                            PairPerp(rb) * b->m_PhysicsAngularVelocity;
                        const float relative_t = PairDot(vb_next - va_next, tangent);
                        const float ta = PairCross(ra, tangent);
                        const float tb = PairCross(rb, tangent);
                        const float tangent_mass = 2.0f + ta * ta * inertia_a + tb * tb * inertia_b;
                        const float desired = -relative_t / tangent_mass;
                        const float limit = FRICTION * impulse;
                        const float friction_impulse = Utility::Max(-limit, Utility::Min(desired, limit));
                        const SPairVector friction = tangent * friction_impulse;
                        a->m_PhysicsVelocity.x -= friction.x;
                        a->m_PhysicsVelocity.y -= friction.y;
                        b->m_PhysicsVelocity.x += friction.x;
                        b->m_PhysicsVelocity.y += friction.y;
                        a->m_PhysicsAngularVelocity -= ta * friction_impulse * inertia_a;
                        b->m_PhysicsAngularVelocity += tb * friction_impulse * inertia_b;
                    }
                }

                // 小さなめり込みを許し、両方を半分ずつ押し離す
                const float depth = Utility::Max(0.0f, hit.PenetrationDepth - CONTACT_SLOP);
                const float correction = Utility::Min(depth * CORRECTION_RATE, MAX_CORRECTION);
                a->m_Position.x -= normal.x * correction * 0.5f;
                a->m_Position.y -= normal.y * correction * 0.5f;
                b->m_Position.x += normal.x * correction * 0.5f;
                b->m_Position.y += normal.y * correction * 0.5f;

                // 静止中の微小な接触ではSleepを維持する
                // 強い接近または大きなめり込みのときだけ両方を起こす
                if (vn < -0.05f || depth > 0.5f)
                {
                    a->m_IsPhysicsSleeping = false;
                    b->m_IsPhysicsSleeping = false;
                    a->m_PhysicsStillFrames = 0;
                    b->m_PhysicsStillFrames = 0;
                }
            }
        }
    }
}
