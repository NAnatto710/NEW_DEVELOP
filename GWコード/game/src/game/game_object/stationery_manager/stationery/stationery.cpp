/*!
 *  @file  stationery.cpp
 *  @brief  文房具共通基底・剛体物理（地形との接触）
 *  @date  2026/11/10
 */

#include "stationery.h"
#include "../../stage_manager/stage_manager.h"
#include "../../../../utility/utility.h"

#include <array>
#include <cmath>
#include <vector>

// クラス共通定数（既存の物理・操作パラメータを維持）
const float IStationery::m_gravity = 0.35f;  //!< 重力加速度（px / frame^2）
const float IStationery::m_max_fall_speed = 12.0f;  //!< 最大落下速度（px / frame）
const float IStationery::m_static_friction = 0.72f;  //!< 静止摩擦係数
const float IStationery::m_dynamic_friction = 0.48f;  //!< 動摩擦係数
const float IStationery::m_linear_damping = 0.999f;  //!< 並進速度の減衰率
const float IStationery::m_angular_damping = 0.998f;  //!< 角速度の減衰率
const float IStationery::m_contact_slop = 0.03f;  //!< 許容めり込み量（px）
const float IStationery::m_correction_rate = 0.22f;  //!< 地形とのめり込み補正率
const int IStationery::m_sub_steps = 8;  //!< 物理更新の分割数
const int IStationery::m_solver_iterations = 12;  //!< 衝突速度の解決回数
const int IStationery::m_position_iterations = 3;  //!< 位置補正の反復回数
const float IStationery::m_sleep_speed_sq = 0.015f * 0.015f;  //!< Sleep判定の速度二乗しきい値
const float IStationery::m_sleep_angular_speed = 0.00016f;  //!< Sleep判定の角速度しきい値
const int IStationery::m_sleep_frames = 90;  //!< Sleep移行までの静止フレーム数

/*
 *  法線から接線を求めるため、ベクトルを反時計回りに90度回転する
 *  物理計算で使用する座標・速度は vivid::Vector2 に統一する
 */
vivid::Vector2
IStationery::
Perp(const vivid::Vector2& vector)
{
    return vivid::Vector2(-vector.y, vector.x);
}

/*
 *  平方根を求めずに長さの二乗を計算する
 *  静止判定や接触点の比較で使用する
 */
float
IStationery::
LengthSq(const vivid::Vector2& vector)
{
    return vivid::Vector2::Dot(vector, vector);
}


/*
 *  回転した矩形の4頂点を求める
 */
std::array<vivid::Vector2, 4>
IStationery::
GetCorners(vivid::Vector2 center, vivid::Vector2 size, float rotation)
{
    // 回転角から cos / sin を計算し、中心からの頂点位置を回転する。
    const float c = std::cos(rotation);
    const float s = std::sin(rotation);
    const float hx = size.x * 0.5f;
    const float hy = size.y * 0.5f;
    const std::array<vivid::Vector2, 4> local = {{
        vivid::Vector2(-hx, -hy), vivid::Vector2(hx, -hy),
        vivid::Vector2(hx, hy), vivid::Vector2(-hx, hy)
    }};
    std::array<vivid::Vector2, 4> result{};
    for (int i = 0; i < 4; ++i)
    {
        result[i] = vivid::Vector2(center.x + local[i].x * c - local[i].y * s,
                                   center.y + local[i].x * s + local[i].y * c);
    }
    return result;
}

/*
 *  多角形をタイルの4辺で切り抜き、重なり領域を求める
 */
std::vector<vivid::Vector2>
IStationery::
ClipPolygon(const std::array<vivid::Vector2, 4>& corners,
            float left, float top, float right, float bottom)
{
    // OBB の4頂点を多角形として扱い、タイルの各辺で切り抜く。
    std::vector<vivid::Vector2> vertices(corners.begin(), corners.end());
    const auto clip = [&](vivid::Vector2 normal, float limit)
    {
        std::vector<vivid::Vector2> out;
        if (vertices.empty()) return;
        // 凸四角形を矩形で切り抜いた結果は最大8頂点。再確保を抑える。
        out.reserve(8);
        // 各辺について境界の内外を調べ、境界をまたぐ線分には交点を追加する。
        vivid::Vector2 previous = vertices.back();
        float before = vivid::Vector2::Dot(previous, normal) - limit;
        for (const vivid::Vector2 current : vertices)
        {
            const float after = vivid::Vector2::Dot(current, normal) - limit;
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
    // 左右上下の4本の境界を順に適用し、重なり領域だけを残す。
    clip(vivid::Vector2(-1.0f, 0.0f), -left);
    clip(vivid::Vector2(1.0f, 0.0f), right);
    clip(vivid::Vector2(0.0f, -1.0f), -top);
    clip(vivid::Vector2(0.0f, 1.0f), bottom);
    return vertices;
}

/*
 *  文房具のOBBと地形タイルの接触を列挙する
 *  多角形の重なり領域の地形側の辺を接触点とする
 */
std::vector<IStationery::SContact>
IStationery::
FindContacts(const CStageManager& stage, vivid::Vector2 position,
             vivid::Vector2 size, float rotation, int block_size)
{
    std::vector<SContact> result;
    result.reserve(8);
    // 回転矩形の頂点から範囲を求め、接触候補の地形タイルを絞る。
    const auto corners = GetCorners(position, size, rotation);
    float min_x = corners[0].x, max_x = corners[0].x;
    float min_y = corners[0].y, max_y = corners[0].y;
    for (const vivid::Vector2 p : corners)
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
            // OBB と地形の AABB で衝突し、分離法線とめり込み量を調べる。
            const Utility::CollisionResult hit = Utility::Collision::Check(box, tile);
            if (!hit.IsHit || !hit.HasSeparation || hit.PenetrationDepth <= 0.0f)
            {
                continue;
            }
            vivid::Vector2 normal = hit.Normal;
            const float normal_sq = LengthSq(normal);
            if (normal_sq < 0.5f) continue;
            normal = normal * (1.0f / std::sqrt(normal_sq));

            // 接触点を安定させるため、OBB とタイルの重なり多角形を作成する。
            const auto overlap = ClipPolygon(corners, bx, by,
                bx + block_size, by + block_size);
            if (overlap.empty()) continue;

            // 法線に対して最も浅い位置の2点を接触として採用する
            float shallowest = vivid::Vector2::Dot(overlap[0], normal);
            for (const vivid::Vector2 p : overlap)
            {
                shallowest = Utility::Min(shallowest, vivid::Vector2::Dot(p, normal));
            }
            std::vector<vivid::Vector2> candidates;
            candidates.reserve(overlap.size());
            for (const vivid::Vector2 p : overlap)
            {
                if (vivid::Vector2::Dot(p, normal) - shallowest < 0.15f)
                {
                    bool duplicate = false;
                    for (const vivid::Vector2 q : candidates)
                    {
                        if (LengthSq(p - q) < 0.01f) duplicate = true;
                    }
                    if (!duplicate) candidates.push_back(p);
                }
            }
            if (candidates.empty()) candidates.push_back(overlap[0]);

            // 候補点を接線方向へ投影し、端点を最大2つの接触点として採用する。
            const vivid::Vector2 tangent = Perp(normal);
            vivid::Vector2 first = candidates[0], last = candidates[0];
            for (const vivid::Vector2 p : candidates)
            {
                if (vivid::Vector2::Dot(p, tangent) < vivid::Vector2::Dot(first, tangent)) first = p;
                if (vivid::Vector2::Dot(p, tangent) > vivid::Vector2::Dot(last, tangent)) last = p;
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
    // 地形との接触を安定させるためのサブステップ・減衰・Sleep判定用の定数


    const CStageManager& stage = CStageManager::GetInstance();
    const int block_size = stage.GetBlockSize();
    if (block_size <= 0 || m_Size.x <= 0.0f || m_Size.y <= 0.0f) return;

    vivid::Vector2 position = m_Position;
    vivid::Vector2 velocity = m_PhysicsVelocity;
    const vivid::Vector2 size = m_Size;
    float angular = m_PhysicsAngularVelocity;
    float rotation = m_Rotation;

    // 質量1の一様な矩形として慣性モーメントを計算する
    const float inv_mass = 1.0f;
    const float inv_inertia = 12.0f / (size.x * size.x + size.y * size.y);
    const float dt = 1.0f / static_cast<float>(m_sub_steps);
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

    // フレームを分割し、積分と衝突解決を交互に実行してすり抜けを抑える
    for (int step = 0; step < m_sub_steps; ++step)
    {
        // 重力は常に重心へ作用。片端支持時の回転は接触インパルスから自然に生じる
        velocity.y = Utility::Min(velocity.y + m_gravity * dt, m_max_fall_speed);
        velocity.x *= std::pow(m_linear_damping, dt);
        angular *= std::pow(m_angular_damping, dt);

        position = position + velocity * dt;
        rotation += angular * dt;

        // 速度拘束：接触点の法線方向速度を0以上に保つ
        // 法線方向の速度と接線方向の摩擦を接触点ごとに反復補正する
        // 接触インパルスを複数回反復し、複数点での接触を安定させる。
        // 速度だけを更新する反復中は、位置・角度が変わらない。
        // 同じ地形との接触点を何度も算出せず、このサブステップ内で再利用する。
        // 位置補正後は座標が変わるため、下の位置補正ループで再取得する。
        const std::vector<SContact> velocity_contacts =
            FindContacts(stage, position, size, rotation, block_size);

        for (int iteration = 0;
             iteration < m_solver_iterations && !velocity_contacts.empty();
             ++iteration)
        {
            for (const SContact& contact : velocity_contacts)
            {
                const vivid::Vector2 r = contact.Point - position;
                const vivid::Vector2 surface_velocity = velocity + Perp(r) * angular;
                const float vn = vivid::Vector2::Dot(surface_velocity, contact.Normal);
                if (vn <= 0.0f) continue;

                const float rn = vivid::Vector2::Cross(r, contact.Normal);
                const float effective_mass = inv_mass + rn * rn * inv_inertia;
                if (effective_mass <= 0.0f) continue;
                const float impulse = vn / effective_mass;
                const vivid::Vector2 applied = contact.Normal * impulse;
                velocity = velocity - applied * inv_mass;
                angular -= vivid::Vector2::Cross(r, applied) * inv_inertia;

                // 接触後の接線方向の速度を摩擦で減らす
                const vivid::Vector2 tangent = Perp(contact.Normal);
                const vivid::Vector2 next_surface_velocity = velocity + Perp(r) * angular;
                const float vt = vivid::Vector2::Dot(next_surface_velocity, tangent);
                const float rt = vivid::Vector2::Cross(r, tangent);
                const float tangent_mass = inv_mass + rt * rt * inv_inertia;
                if (tangent_mass <= 0.0f) continue;
                const float desired = vt / tangent_mass;
                const float max_static = m_static_friction * impulse;
                const float max_dynamic = m_dynamic_friction * impulse;
                const float friction = (std::fabs(desired) < max_static)
                    ? desired : Utility::Max(-max_dynamic, Utility::Min(desired, max_dynamic));
                const vivid::Vector2 friction_impulse = tangent * friction;
                velocity = velocity - friction_impulse * inv_mass;
                angular -= vivid::Vector2::Cross(r, friction_impulse) * inv_inertia;
            }
        }

        // 位置だけの緩やかなめり込み解消。角度を直接変えず跳ね上がりを抑える
        // 速度の補正とは別に、残っためり込み量を位置補正で解消する
        // 速度の解決後、残っためり込みを位置補正で減らす。
        for (int iteration = 0; iteration < m_position_iterations; ++iteration)
        {
            const auto contacts = FindContacts(stage, position, size, rotation, block_size);
            if (contacts.empty()) break;
            vivid::Vector2 sum(0.0f, 0.0f);
            int count = 0;
            for (const SContact& contact : contacts)
            {
                const float depth = Utility::Max(0.0f, contact.Depth - m_contact_slop);
                if (depth <= 0.0f) continue;
                sum = sum - contact.Normal * (depth * m_correction_rate);
                ++count;
            }
            if (count == 0) break;
            position = position + sum * (1.0f / static_cast<float>(count));
        }

        // 接地状態は途中のサブステップも考慮する（従来の判定を維持）。
        const std::vector<SContact> contacts =
            FindContacts(stage, position, size, rotation, block_size);
        for (const SContact& contact : contacts)
        {
            if (contact.Normal.y > 0.65f) has_ground_contact = true;
        }
    }

    // Sleepは「重心が支持範囲内」にある安定状態のみ許可
    // 最終接触を使って支持範囲を調べ、安定時のみSleepへ移行する
    const auto final_contacts = FindContacts(stage, position, size, rotation, block_size);
    float min_x = 1e9f, max_x = -1e9f;
    for (const SContact& contact : final_contacts)
    {
        if (contact.Normal.y <= 0.65f) continue;
        has_ground_contact = true;
        min_x = Utility::Min(min_x, contact.Point.x);
        max_x = Utility::Max(max_x, contact.Point.x);
    }
    const bool stable_support = min_x < position.x + 1.0f &&
                                max_x > position.x - 1.0f;
    if (stable_support && LengthSq(velocity) < m_sleep_speed_sq &&
        std::fabs(angular) < m_sleep_angular_speed)
    {
        ++m_PhysicsStillFrames;
        if (m_PhysicsStillFrames >= m_sleep_frames)
        {
            m_IsPhysicsSleeping = true;
            velocity = vivid::Vector2(0.0f, 0.0f);
            angular = 0.0f;
        }
    }
    else
    {
        m_PhysicsStillFrames = 0;
    }

    m_Position = position;
    m_Rotation = rotation;
    m_PhysicsVelocity = velocity;
    m_PhysicsAngularVelocity = angular;
    m_IsPhysicsGrounded = has_ground_contact || stable_support;
}
