/*!
 *  @file           stationery.cpp
 *  @brief          文房具共通基底・剛体物理（地形との接触）
 *  @date           2026/11/10
 */

#include "stationery.h"
#include "../../stage_manager/stage_manager.h"
#include "../../../../utility/utility.h"

#include <array>
#include <cmath>
#include <vector>

namespace
{
    struct SVector
    {
        float x;
        float y;
    };

    struct SContact
    {
        SVector Point;
        SVector Normal;       // 定規から地形へ向かう単位法線
        float Depth;
    };

    SVector operator+(SVector a, SVector b) { return {a.x + b.x, a.y + b.y}; }
    SVector operator-(SVector a, SVector b) { return {a.x - b.x, a.y - b.y}; }
    SVector operator*(SVector a, float s) { return {a.x * s, a.y * s}; }
    float Dot(SVector a, SVector b) { return a.x * b.x + a.y * b.y; }
    float Cross(SVector a, SVector b) { return a.x * b.y - a.y * b.x; }
    SVector Perp(SVector a) { return {-a.y, a.x}; }
    float LengthSq(SVector a) { return Dot(a, a); }
    SVector ToLocal(const vivid::Vector2& v) { return {v.x, v.y}; }

    constexpr float GRAVITY = 0.35f;             // px / frame^2
    constexpr float MAX_FALL_SPEED = 12.0f;      // px / frame
    constexpr float STATIC_FRICTION = 0.72f;
    constexpr float DYNAMIC_FRICTION = 0.48f;
    constexpr float LINEAR_DAMPING = 0.999f;
    constexpr float ANGULAR_DAMPING = 0.998f;
    constexpr float CONTACT_SLOP = 0.03f;       // 許容めり込み量(px)
    constexpr float CORRECTION_RATE = 0.22f;    // めり込みの位置補正率
    constexpr int SUB_STEPS = 8;
    constexpr int SOLVER_ITERATIONS = 12;
    constexpr int POSITION_ITERATIONS = 3;
    constexpr float SLEEP_SPEED_SQ = 0.015f * 0.015f;
    constexpr float SLEEP_ANGULAR_SPEED = 0.00016f;
    constexpr int SLEEP_FRAMES = 90;

    /*
     *  回転した矩形の4頂点を求める
     */
    std::array<SVector, 4>
    GetCorners(SVector center, SVector size, float rotation)
    {
        const float c = std::cos(rotation);
        const float s = std::sin(rotation);
        const float hx = size.x * 0.5f;
        const float hy = size.y * 0.5f;
        const std::array<SVector, 4> local = {{{-hx,-hy},{hx,-hy},{hx,hy},{-hx,hy}}};
        std::array<SVector, 4> result{};
        for (int i = 0; i < 4; ++i)
        {
            result[i] = {center.x + local[i].x * c - local[i].y * s,
                         center.y + local[i].x * s + local[i].y * c};
        }
        return result;
    }

    /*
     *  多角形をタイルの4辺で切り抜き、重なり領域を求める
     */
    std::vector<SVector>
    ClipPolygon(const std::array<SVector, 4>& corners,
                float left, float top, float right, float bottom)
    {
        std::vector<SVector> vertices(corners.begin(), corners.end());
        const auto clip = [&](SVector normal, float limit)
        {
            std::vector<SVector> out;
            if (vertices.empty()) return;
            SVector previous = vertices.back();
            float before = Dot(previous, normal) - limit;
            for (const SVector current : vertices)
            {
                const float after = Dot(current, normal) - limit;
                if ((before <= 0.0f) != (after <= 0.0f))
                {
                    const float denom = before - after;
                    if (std::fabs(denom) > 0.000001f)
                    {
                        out.push_back(previous + (current - previous) * (before / denom));
                    }
                }
                if (after <= 0.0f) out.push_back(current);
                previous = current;
                before = after;
            }
            vertices.swap(out);
        };
        clip({-1.0f,0.0f}, -left);
        clip({1.0f,0.0f}, right);
        clip({0.0f,-1.0f}, -top);
        clip({0.0f,1.0f}, bottom);
        return vertices;
    }

    /*
     *  定規のOBBと地形タイルの接触を列挙する
     *  多角形の重なり領域の地形側の辺を接触点とする
     */
    std::vector<SContact>
    FindContacts(const CStageManager& stage, SVector position,
                 SVector size, float rotation, int block_size)
    {
        std::vector<SContact> result;
        const auto corners = GetCorners(position, size, rotation);
        float min_x = corners[0].x, max_x = corners[0].x;
        float min_y = corners[0].y, max_y = corners[0].y;
        for (const SVector p : corners)
        {
            min_x = Utility::Min(min_x, p.x); max_x = Utility::Max(max_x, p.x);
            min_y = Utility::Min(min_y, p.y); max_y = Utility::Max(max_y, p.y);
        }
        const int left = static_cast<int>(std::floor(min_x / block_size));
        const int right = static_cast<int>(std::floor(max_x / block_size));
        const int top = static_cast<int>(std::floor(min_y / block_size));
        const int bottom = static_cast<int>(std::floor(max_y / block_size));

        const Utility::OBB box = {
            {position.x, position.y}, {size.x * 0.5f, size.y * 0.5f}, rotation
        };

        for (int ty = top; ty <= bottom; ++ty)
        {
            for (int tx = left; tx <= right; ++tx)
            {
                const float bx = static_cast<float>(tx * block_size);
                const float by = static_cast<float>(ty * block_size);
                if (!stage.IsHit(static_cast<int>(bx), static_cast<int>(by))) continue;

                const Utility::AABB tile = {{bx, by},
                    static_cast<float>(block_size), static_cast<float>(block_size)};
                const Utility::CollisionResult hit = Utility::Collision::Check(box, tile);
                if (!hit.IsHit || !hit.HasSeparation || hit.PenetrationDepth <= 0.0f)
                {
                    continue;
                }
                SVector normal = ToLocal(hit.Normal);
                const float normal_sq = LengthSq(normal);
                if (normal_sq < 0.5f) continue;
                normal = normal * (1.0f / std::sqrt(normal_sq));

                const auto overlap = ClipPolygon(corners, bx, by,
                    bx + block_size, by + block_size);
                if (overlap.empty()) continue;

                // 法線に対して最も浅い位置の2点を接触として採用する
                float shallowest = Dot(overlap[0], normal);
                for (const SVector p : overlap)
                {
                    shallowest = Utility::Min(shallowest, Dot(p, normal));
                }
                std::vector<SVector> candidates;
                for (const SVector p : overlap)
                {
                    if (Dot(p, normal) - shallowest < 0.15f)
                    {
                        bool duplicate = false;
                        for (const SVector q : candidates)
                        {
                            if (LengthSq(p - q) < 0.01f) duplicate = true;
                        }
                        if (!duplicate) candidates.push_back(p);
                    }
                }
                if (candidates.empty()) candidates.push_back(overlap[0]);

                const SVector tangent = Perp(normal);
                SVector first = candidates[0], last = candidates[0];
                for (const SVector p : candidates)
                {
                    if (Dot(p, tangent) < Dot(first, tangent)) first = p;
                    if (Dot(p, tangent) > Dot(last, tangent)) last = p;
                }
                const float depth = Utility::Min(hit.PenetrationDepth, 2.0f);
                result.push_back(SContact{first, normal, depth});
                if (LengthSq(last - first) > 1.0f)
                {
                    result.push_back(SContact{last, normal, depth});
                }
            }
        }
        return result;
    }
}

/*
 *  コンストラクタ
 */
IStationery::
IStationery(STATIONERY_TYPE type)
    : m_Type(type)
    , m_State(STATIONERY_STATE::HELD)
    , m_Position(0.0f, 0.0f)
    , m_Size(0.0f, 0.0f)
    , m_Rotation(0.0f)
    , m_PhysicsVelocity(0.0f, 0.0f)
{
}

/*
 *  初期化
 */
void
IStationery::
Initialize(const vivid::Vector2& position, const vivid::Vector2& size)
{
    m_Position = position;
    m_Size = size;
    m_Rotation = 0.0f;
    m_State = STATIONERY_STATE::HELD;
    ResetPhysicsVelocity();
}

/*
 *  配置確定
 */
bool
IStationery::
Place(void)
{
    if (m_State != STATIONERY_STATE::HELD) return false;
    m_State = STATIONERY_STATE::PLACED;
    return true;
}

/*
 *  使用済みへの変更
 */
bool
IStationery::
MarkUsed(void)
{
    if (m_State != STATIONERY_STATE::PLACED) return false;
    m_State = STATIONERY_STATE::USED;
    return true;
}

/*
 *  プレイヤー操作による位置変更
 */
bool
IStationery::
SetPosition(const vivid::Vector2& position)
{
    if (!CanManipulate()) return false;
    m_Position = position;
    ResetPhysicsVelocity();
    return true;
}

/*
 *  プレイヤー操作による回転変更
 */
bool
IStationery::
SetRotation(float rotation)
{
    if (!CanManipulate()) return false;
    m_Rotation = rotation;
    ResetPhysicsVelocity();
    return true;
}

/*
 *  プレイヤー操作の可否
 */
bool
IStationery::
CanManipulate(void) const
{
    return m_State != STATIONERY_STATE::USED;
}

/*
 *  物理演算による位置変更
 */
void
IStationery::
SetPhysicsPosition(const vivid::Vector2& position)
{
    m_Position = position;
}

/*
 *  物理演算による回転変更
 */
void
IStationery::
SetPhysicsRotation(float rotation)
{
    m_Rotation = rotation;
}

/*
 *  速度・静止状態の初期化
 */
void
IStationery::
ResetPhysicsVelocity(void)
{
    m_PhysicsVelocity = vivid::Vector2(0.0f, 0.0f);
    m_PhysicsAngularVelocity = 0.0f;
    m_PhysicsStillFrames = 0;
    m_IsPhysicsSleeping = false;
    m_IsPhysicsGrounded = false;
}

/*
 *  文房具の剛体物理更新
 *  接触点の速度を考慮した反発なしの衝突解決を行う
 */
void
IStationery::
UpdatePhysics(void)
{
    if (m_State == STATIONERY_STATE::HELD) return;
    const CStageManager& stage = CStageManager::GetInstance();
    const int block_size = stage.GetBlockSize();
    if (block_size <= 0 || m_Size.x <= 0.0f || m_Size.y <= 0.0f) return;

    SVector position = ToLocal(m_Position);
    SVector velocity = ToLocal(m_PhysicsVelocity);
    const SVector size = ToLocal(m_Size);
    float angular = m_PhysicsAngularVelocity;
    float rotation = m_Rotation;

    // 質量1の一様な矩形として慣性モーメントを計算する
    const float inv_mass = 1.0f;
    const float inv_inertia = 12.0f / (size.x * size.x + size.y * size.y);
    const float dt = 1.0f / static_cast<float>(SUB_STEPS);
    bool has_ground_contact = false;

    // 睡眠中は、中心を支持しない片端接触なら必ず起こす
    if (m_IsPhysicsSleeping)
    {
        const auto contacts = FindContacts(stage, position, size, rotation, block_size);
        float min_support_x = 1e9f, max_support_x = -1e9f;
        for (const SContact& contact : contacts)
        {
            if (contact.Normal.y > 0.65f)
            {
                min_support_x = Utility::Min(min_support_x, contact.Point.x);
                max_support_x = Utility::Max(max_support_x, contact.Point.x);
            }
        }
        if (min_support_x < position.x + 1.0f &&
            max_support_x > position.x - 1.0f)
        {
            return;
        }
        m_IsPhysicsSleeping = false;
        m_PhysicsStillFrames = 0;
    }

    for (int step = 0; step < SUB_STEPS; ++step)
    {
        // 重力は常に重心へ作用。片端支持時の回転は接触インパルスから自然に生じる
        velocity.y = Utility::Min(velocity.y + GRAVITY * dt, MAX_FALL_SPEED);
        velocity.x *= std::pow(LINEAR_DAMPING, dt);
        angular *= std::pow(ANGULAR_DAMPING, dt);

        position = position + velocity * dt;
        rotation += angular * dt;

        // 速度拘束：接触点の法線方向速度を0以上に保つ
        for (int iteration = 0; iteration < SOLVER_ITERATIONS; ++iteration)
        {
            const auto contacts = FindContacts(stage, position, size, rotation, block_size);
            if (contacts.empty()) break;
            for (const SContact& contact : contacts)
            {
                const SVector r = contact.Point - position;
                const SVector surface_velocity = velocity + Perp(r) * angular;
                const float vn = Dot(surface_velocity, contact.Normal);
                if (vn <= 0.0f) continue;

                const float rn = Cross(r, contact.Normal);
                const float effective_mass = inv_mass + rn * rn * inv_inertia;
                if (effective_mass <= 0.0f) continue;
                const float impulse = vn / effective_mass;
                const SVector applied = contact.Normal * impulse;
                velocity = velocity - applied * inv_mass;
                angular -= Cross(r, applied) * inv_inertia;

                // 接触後の接線方向の速度を摩擦で減らす
                const SVector tangent = Perp(contact.Normal);
                const SVector next_surface_velocity = velocity + Perp(r) * angular;
                const float vt = Dot(next_surface_velocity, tangent);
                const float rt = Cross(r, tangent);
                const float tangent_mass = inv_mass + rt * rt * inv_inertia;
                if (tangent_mass <= 0.0f) continue;
                const float desired = vt / tangent_mass;
                const float max_static = STATIC_FRICTION * impulse;
                const float max_dynamic = DYNAMIC_FRICTION * impulse;
                const float friction = (std::fabs(desired) < max_static)
                    ? desired : Utility::Max(-max_dynamic, Utility::Min(desired, max_dynamic));
                const SVector friction_impulse = tangent * friction;
                velocity = velocity - friction_impulse * inv_mass;
                angular -= Cross(r, friction_impulse) * inv_inertia;
            }
        }

        // 位置だけの緩やかなめり込み解消。角度を直接変えず跳ね上がりを抑える
        for (int iteration = 0; iteration < POSITION_ITERATIONS; ++iteration)
        {
            const auto contacts = FindContacts(stage, position, size, rotation, block_size);
            if (contacts.empty()) break;
            SVector sum{0.0f, 0.0f};
            int count = 0;
            for (const SContact& contact : contacts)
            {
                const float depth = Utility::Max(0.0f, contact.Depth - CONTACT_SLOP);
                if (depth <= 0.0f) continue;
                sum = sum - contact.Normal * (depth * CORRECTION_RATE);
                ++count;
            }
            if (count == 0) break;
            position = position + sum * (1.0f / static_cast<float>(count));
        }

        const auto contacts = FindContacts(stage, position, size, rotation, block_size);
        for (const SContact& contact : contacts)
        {
            if (contact.Normal.y > 0.65f) has_ground_contact = true;
        }
    }

    // Sleepは「重心が支持範囲内」にある安定状態のみ許可
    const auto final_contacts = FindContacts(stage, position, size, rotation, block_size);
    float min_x = 1e9f, max_x = -1e9f;
    for (const SContact& contact : final_contacts)
    {
        if (contact.Normal.y <= 0.65f) continue;
        min_x = Utility::Min(min_x, contact.Point.x);
        max_x = Utility::Max(max_x, contact.Point.x);
    }
    const bool stable_support = min_x < position.x + 1.0f &&
                                max_x > position.x - 1.0f;
    if (stable_support && LengthSq(velocity) < SLEEP_SPEED_SQ &&
        std::fabs(angular) < SLEEP_ANGULAR_SPEED)
    {
        ++m_PhysicsStillFrames;
        if (m_PhysicsStillFrames >= SLEEP_FRAMES)
        {
            m_IsPhysicsSleeping = true;
            velocity = {0.0f, 0.0f};
            angular = 0.0f;
        }
    }
    else
    {
        m_PhysicsStillFrames = 0;
    }

    m_Position = vivid::Vector2(position.x, position.y);
    m_Rotation = rotation;
    m_PhysicsVelocity = vivid::Vector2(velocity.x, velocity.y);
    m_PhysicsAngularVelocity = angular;
    m_IsPhysicsGrounded = has_ground_contact || stable_support;
}
