
/*!
 *  @file           stationery_manager.h
 *  @brief          文房具管理
 *  @author         Ryusei Shimizu
 *  @date           2026/11/20
 */

#include "stationery_manager.h"
#include "stationery/stationery.h"
#include "stationery/ruler/ruler.h"
#include "../stage_manager/stage_manager.h"

#include <cmath>
#include <array>
#include <vector>
#include "../../../utility/utility.h"

// 物理の精度を調整する値はここにまとめる。
const int   CStationeryManager::m_sub_steps           = 6;
const int   CStationeryManager::m_velocity_iterations = 8;
const int   CStationeryManager::m_position_iterations = 4;
const float CStationeryManager::m_friction            = 0.60f;
const float CStationeryManager::m_contact_slop        = 0.02f;
const float CStationeryManager::m_position_rate       = 0.65f;

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
    // 入力による選択・ドラッグ・配置を先に反映する。
    m_Controller.Update(m_Stationeries);

    // ドラッグ中・配置前の文房具を物理の対象から外す。
    std::vector<IStationery*> active;
    active.reserve(m_Stationeries.size());

    for (const auto& stationery : m_Stationeries)
    {
        stationery->Update();

        if (stationery->GetState() != STATIONERY_STATE::HELD && !m_Controller.IsDragging(stationery.get()))
        {
            active.push_back(stationery.get());
        }
    }
    if (active.empty()) return;

    // 全文房具を同じ時間ずつ動かし、その都度まとめて衝突を解決する。
    // 個別に落下させてから文房具同士を押し出すと、床との補正が競合しやすい。
    const float dt = 1.0f / static_cast<float>(m_sub_steps);

    std::vector<Contact> contacts;
    contacts.reserve(active.size() * 8);

    for (int step = 0; step < m_sub_steps; ++step)
    {
        // 1. 重力・移動・回転
        for (IStationery* item : active) item->UpdatePhysics(dt);

        // 2. 地形と文房具同士を同じルールで押し返す。
        CollectContacts(active, contacts);
        for (int i = 0; i < m_velocity_iterations; ++i)
        {
            for (Contact& contact : contacts) ResolveVelocity(contact);
        }

        // 3. 速度を変更しても残っているめり込みを解消する。
        //    位置・角度が変わるため、毎回接触を調べ直す。
        for (int i = 0; i < m_position_iterations; ++i)
        {
            CollectContacts(active, contacts);
            if (contacts.empty()) break;
            for (Contact& contact : contacts) ResolvePosition(contact);
        }
    }
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
CheckCollision(const Utility::AABB& player, Utility::CollisionResult& result, IStationery*& stationery) const
{
    // 呼び出し元へ前回の衝突情報が残らないよう、出力引数を初期化する。
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

        // プレイヤーのAABBと、文房具の回転後の包囲AABBを先に比較する。
        // 離れている文房具はSATを実行しないため、文房具が増えた際の負荷を抑える。
        const float c = std::fabs(std::cos(target->GetRotation()));
        const float s = std::fabs(std::sin(target->GetRotation()));

        const float half_x = (size.x * c + size.y * s) * 0.5f;
        const float half_y = (size.x * s + size.y * c) * 0.5f;

        if (center.x + half_x < player.Position.x ||
            center.x - half_x > player.Position.x + player.Width ||
            center.y + half_y < player.Position.y ||
            center.y - half_y > player.Position.y + player.Height)
        {
            continue;
        }

        const Utility::OBB collider = {center, {size.x * 0.5f, size.y * 0.5f}, target->GetRotation()};

        const Utility::CollisionResult hit = Utility::Collision::Check(player, collider);

        // 最も深い接触を代表として返し、呼び出し側で押し戻しに利用する。
        if (hit.IsHit && hit.HasSeparation && (stationery == nullptr || hit.PenetrationDepth > result.PenetrationDepth))
        {
            result = hit;
            stationery = target;
        }
    }

    return stationery != nullptr;
}

/*
 *  文房具の矩形コライダーを作成する
 */
Utility::OBB
CStationeryManager::
GetCollider(const IStationery& item)
{
    return Utility::OBB
    {
        item.m_Position,
        vivid::Vector2(item.m_Size.x * 0.5f, item.m_Size.y * 0.5f),
        item.m_Rotation
    };
}

/*
 *  2つの矩形が実際に重なる領域の中央を接触点として使う。
 *  角が当たる場合も、足場の端だけで支えられる場合も同じ計算で処理できる。
 *  最大8頂点になる多角形の切り抜き処理。ヒープ確保は行わない。
 */
vivid::Vector2
CStationeryManager::
GetContactPoint(const Utility::OBB& a, const Utility::OBB& b)
{
    const float ca = std::cos(a.Rotation);
    const float sa = std::sin(a.Rotation);
    const float cb = std::cos(b.Rotation);
    const float sb = std::sin(b.Rotation);

    const vivid::Vector2 ax(ca * a.HalfSize.x, sa * a.HalfSize.x);
    const vivid::Vector2 ay(-sa * a.HalfSize.y, ca * a.HalfSize.y);
    const vivid::Vector2 bx(cb * b.HalfSize.x, sb * b.HalfSize.x);
    const vivid::Vector2 by(-sb * b.HalfSize.y, cb * b.HalfSize.y);

    // 各矩形の頂点を左上から時計回りに並べる（画面のY軸は下向き）。
    std::array<vivid::Vector2, 16> points =
    {{
        a.Center - ax - ay, a.Center + ax - ay,
        a.Center + ax + ay, a.Center - ax + ay
    }};

    const std::array<vivid::Vector2, 4> edges =
    {{
        b.Center - bx - by, b.Center + bx - by,
        b.Center + bx + by, b.Center - bx + by
    }};

    int count = 4;

    // Bの4辺でAの多角形を切り抜き、重なった領域だけを残す。
    for (int edge = 0; edge < 4 && count > 0; ++edge)
    {
        std::array<vivid::Vector2, 16> next{};
        int next_count = 0;
        const vivid::Vector2 start = edges[edge];
        const vivid::Vector2 direction = edges[(edge + 1) % 4] - start;
        vivid::Vector2 previous = points[count - 1];
        float old_side = vivid::Vector2::Cross(direction, previous - start);

        for (int i = 0; i < count; ++i)
        {
            const vivid::Vector2 current = points[i];
            const float new_side = vivid::Vector2::Cross(direction, current - start);
            const bool previous_inside = old_side >= 0.0f;
            const bool current_inside = new_side >= 0.0f;

            // 辺をまたぐ場合は交点を追加する。
            if (previous_inside != current_inside)
            {
                const float rate = old_side / (old_side - new_side);
                if (next_count < 16)
                {
                    next[next_count++] = previous + (current - previous) * rate;
                }
            }
            if (current_inside && next_count < 16) next[next_count++] = current;
            previous = current;
            old_side = new_side;
        }
        points = next;
        count = next_count;
    }

    // 交差の数値誤差で空になったときは中心の中点を使う。
    if (count == 0) return (a.Center + b.Center) * 0.5f;

    vivid::Vector2 point(0.0f, 0.0f);
    for (int i = 0; i < count; ++i) point = point + points[i];
    return point * (1.0f / static_cast<float>(count));
}

/*
 *  SATの法線・めり込み量と、実際の重なり位置をまとめる
 */
void
CStationeryManager::
AddContact(IStationery* a, IStationery* b, const Utility::OBB& box_a, const Utility::OBB& box_b, const Utility::CollisionResult& hit, std::vector<Contact>& contacts)
{
    if (!hit.IsHit || !hit.HasSeparation || hit.PenetrationDepth <= 0.0f) return;

    const float length_sq = vivid::Vector2::Dot(hit.Normal, hit.Normal);
    if (length_sq < 0.5f) return;

    Contact contact;
    contact.A = a;
    contact.B = b;
    contact.Normal = hit.Normal * (1.0f / std::sqrt(length_sq));
    contact.Point = GetContactPoint(box_a, box_b);
    contact.Depth = hit.PenetrationDepth;
    contacts.push_back(contact);
}

/*
 *  接触する地形と文房具のペアを探す
 *  高速化のためOBB同士を調べる前に包囲矩形を比較する。
 */
void
CStationeryManager::
CollectContacts(const std::vector<IStationery*>& active, std::vector<Contact>& contacts) const
{
    contacts.clear();
    const CStageManager& stage = CStageManager::GetInstance();
    const int block_size = stage.GetBlockSize();
    if (block_size <= 0) return;

    // 地形との接触。回転後の矩形が占めるタイルだけを調べる。
    for (IStationery* item : active)
    {
        const Utility::OBB box = GetCollider(*item);
        const float c = std::fabs(std::cos(item->m_Rotation));
        const float s = std::fabs(std::sin(item->m_Rotation));
        const float hx = item->m_Size.x * c * 0.5f + item->m_Size.y * s * 0.5f;
        const float hy = item->m_Size.x * s * 0.5f + item->m_Size.y * c * 0.5f;
        const int left = static_cast<int>(std::floor((item->m_Position.x - hx) / block_size));
        const int right = static_cast<int>(std::floor((item->m_Position.x + hx) / block_size));
        const int top = static_cast<int>(std::floor((item->m_Position.y - hy) / block_size));
        const int bottom = static_cast<int>(std::floor((item->m_Position.y + hy) / block_size));

        for (int y = top; y <= bottom; ++y)
        {
            for (int x = left; x <= right; ++x)
            {
                const int tile_x = x * block_size;
                const int tile_y = y * block_size;
                if (!stage.IsHit(tile_x, tile_y)) continue;

                const Utility::OBB tile =
                {
                    vivid::Vector2(tile_x + block_size * 0.5f,
                                   tile_y + block_size * 0.5f),
                    vivid::Vector2(block_size * 0.5f, block_size * 0.5f),
                    0.0f
                };

                // 隣接するブロックの内部の面は、衝突面として使わない。
                const Utility::CollisionResult hit = Utility::Collision::Check(box, tile);
                if (!hit.IsHit || !hit.HasSeparation || hit.PenetrationDepth <= 0.0f) continue;
                const vivid::Vector2 normal = hit.Normal;
                if (normal.y > 0.75f && stage.IsHit(tile_x, tile_y - block_size)) continue;
                if (normal.y < -0.75f && stage.IsHit(tile_x, tile_y + block_size)) continue;
                if (normal.x > 0.75f && stage.IsHit(tile_x - block_size, tile_y)) continue;
                if (normal.x < -0.75f && stage.IsHit(tile_x + block_size, tile_y)) continue;

                AddContact(item, nullptr, box, tile, hit, contacts);
            }
        }
    }

    // 文房具同士の接触。同じペアは1回だけ調べる。
    for (std::size_t i = 0; i < active.size(); ++i)
    {
        IStationery* a = active[i];
        const float ca = std::fabs(std::cos(a->m_Rotation));
        const float sa = std::fabs(std::sin(a->m_Rotation));
        const float ax = (a->m_Size.x * ca + a->m_Size.y * sa) * 0.5f;
        const float ay = (a->m_Size.x * sa + a->m_Size.y * ca) * 0.5f;

        for (std::size_t j = i + 1; j < active.size(); ++j)
        {
            IStationery* b = active[j];
            const float cb = std::fabs(std::cos(b->m_Rotation));
            const float sb = std::fabs(std::sin(b->m_Rotation));
            const float bx = (b->m_Size.x * cb + b->m_Size.y * sb) * 0.5f;
            const float by = (b->m_Size.x * sb + b->m_Size.y * cb) * 0.5f;
            if (std::fabs(a->m_Position.x - b->m_Position.x) > ax + bx ||
                std::fabs(a->m_Position.y - b->m_Position.y) > ay + by)
            {
                continue;
            }
            const Utility::OBB box_a = GetCollider(*a);
            const Utility::OBB box_b = GetCollider(*b);
            const Utility::CollisionResult hit = Utility::Collision::Check(box_a, box_b);
            AddContact(a, b, box_a, box_b, hit, contacts);
        }
    }
}

/*
 *  衝突時の速度を変更する
 *  接触点から中心までの距離で回転のしやすさを変える。
 *  法線方向は押し戻し、接線方向は摩擦として扱う。
 */
void
CStationeryManager::
ResolveVelocity(Contact& contact)
{
    IStationery* a = contact.A;
    IStationery* b = contact.B;
    const vivid::Vector2 ra = contact.Point - a->m_Position;
    const vivid::Vector2 rb = b != nullptr ? contact.Point - b->m_Position : vivid::Vector2(0.0f, 0.0f);

    // 質量は全て1。地形は質量無限大なので動かさない。
    const float mass_b = b != nullptr ? 1.0f : 0.0f;
    const float inertia_a = 12.0f / (a->m_Size.x * a->m_Size.x + a->m_Size.y * a->m_Size.y);
    const float inertia_b = b != nullptr ? 12.0f / (b->m_Size.x * b->m_Size.x + b->m_Size.y * b->m_Size.y) : 0.0f;

    const auto point_velocity = [](const IStationery* item, const vivid::Vector2& arm)
    {
        if (item == nullptr) return vivid::Vector2(0.0f, 0.0f);

        return item->m_PhysicsVelocity + vivid::Vector2(-arm.y, arm.x) * item->m_PhysicsAngularSpeed;
    };
    const auto impulse_to_bodies = [&](const vivid::Vector2& impulse)
    {
        a->m_PhysicsVelocity = a->m_PhysicsVelocity - impulse;
        a->m_PhysicsAngularSpeed -= vivid::Vector2::Cross(ra, impulse) * inertia_a;

        if (b != nullptr)
        {
            b->m_PhysicsVelocity = b->m_PhysicsVelocity + impulse;
            b->m_PhysicsAngularSpeed += vivid::Vector2::Cross(rb, impulse) * inertia_b;
        }
    };

    const vivid::Vector2 relative = point_velocity(b, rb) - point_velocity(a, ra);
    const float speed = vivid::Vector2::Dot(relative, contact.Normal);
    const float cross_a = vivid::Vector2::Cross(ra, contact.Normal);
    const float cross_b = vivid::Vector2::Cross(rb, contact.Normal);

    const float effective_mass = 1.0f + mass_b + cross_a * cross_a * inertia_a + cross_b * cross_b * inertia_b;

    // 衝突力を繰り返しの中で累積し、次の反復で力が急に消えないようにする。
    const float next_impulse = Utility::Max(0.0f, contact.NormalImpulse - speed / effective_mass);
    const float change = next_impulse - contact.NormalImpulse;
    contact.NormalImpulse = next_impulse;
    impulse_to_bodies(contact.Normal * change);

    // 摩擦は接触法線に垂直な方向の速度を抑える。
    const vivid::Vector2 tangent(-contact.Normal.y, contact.Normal.x);
    const vivid::Vector2 after = point_velocity(b, rb) - point_velocity(a, ra);
    const float tangent_speed = vivid::Vector2::Dot(after, tangent);

    const float ta = vivid::Vector2::Cross(ra, tangent);
    const float tb = vivid::Vector2::Cross(rb, tangent);
    const float tangent_mass = 1.0f + mass_b + ta * ta * inertia_a + tb * tb * inertia_b;

    const float limit = m_friction * contact.NormalImpulse;

    const float next_friction = Utility::Clamp(contact.TangentImpulse - tangent_speed / tangent_mass, -limit, limit);
    const float friction_change = next_friction - contact.TangentImpulse;
    contact.TangentImpulse = next_friction;

    impulse_to_bodies(tangent * friction_change);
}

/*
 *  位置補正。速度とは別に残った重なりだけを少しずつ解消する。
 *  回転角も接触位置に応じて動かすため、端で支えた定規が自然に倒れる。
 */
void
CStationeryManager::
ResolvePosition(Contact& contact)
{
    const float depth = Utility::Max(0.0f, contact.Depth - m_contact_slop);
    if (depth <= 0.0f) return;

    IStationery* a = contact.A;
    IStationery* b = contact.B;

    const vivid::Vector2 ra = contact.Point - a->m_Position;
    const vivid::Vector2 rb = b != nullptr ? contact.Point - b->m_Position : vivid::Vector2(0.0f, 0.0f);

    const float mass_b = b != nullptr ? 1.0f : 0.0f;

    const float inertia_a = 12.0f / (a->m_Size.x * a->m_Size.x + a->m_Size.y * a->m_Size.y);
    const float inertia_b = b != nullptr ? 12.0f / (b->m_Size.x * b->m_Size.x + b->m_Size.y * b->m_Size.y) : 0.0f;

    const float ca = vivid::Vector2::Cross(ra, contact.Normal);
    const float cb = vivid::Vector2::Cross(rb, contact.Normal);

    const float effective_mass = 1.0f + mass_b + ca * ca * inertia_a + cb * cb * inertia_b;
    if (effective_mass <= 0.000001f) return;

    // 1回で大きく押し戻すと跳ね上がるため、各反復で最大2pxずつ修正する。
    const float impulse = Utility::Min(depth * m_position_rate, 2.0f) / effective_mass;

    a->m_Position = a->m_Position - contact.Normal * impulse;
    a->m_Rotation -= Utility::Clamp(ca * impulse * inertia_a, -0.03f, 0.03f);

    if (b != nullptr)
    {
        b->m_Position = b->m_Position + contact.Normal * impulse;
        b->m_Rotation += Utility::Clamp(cb * impulse * inertia_b, -0.03f, 0.03f);
    }
}