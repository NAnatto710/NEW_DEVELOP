/*!
 *  @file  stationery_manager.cpp
 *  @brief  文房具管理
 *  @date  2026/11/05
 */

#include "stationery_manager.h"
#include "stationery/stationery.h"
#include "stationery/ruler/ruler.h"

#include <cmath>
#include <array>
#include "../../../utility/utility.h"

// 衝突補正量は従来値を維持する。
const float CStationeryManager::m_pair_contact_slop      = 0.05f;
const float CStationeryManager::m_pair_correction_rate   = 0.45f;
const float CStationeryManager::m_pair_max_correction    = 3.0f;
const float CStationeryManager::m_pair_friction          = 0.50f;
const int   CStationeryManager::m_pair_iterations        = 6;

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
    // 入力による選択・ドラッグ・配置を先に反映する。
    m_Controller.Update(m_Stationeries);

    // 文房具固有の更新後に、操作されていない物体の物理だけ進める。
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

        const Utility::OBB collider = {
            center,
            {size.x * 0.5f, size.y * 0.5f},
            target->GetRotation()
        };

        const Utility::CollisionResult hit =
            Utility::Collision::Check(player, collider);

        // 最も深い接触を代表として返し、呼び出し側で押し戻しに利用する。
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

/*!
 *  @brief  接触法線を90度回転し、摩擦計算用の接線を取得する
 *
 *  @param[in]  vector  法線などの元ベクトル
 *  @return  左回りに90度回転したベクトル
 */
vivid::Vector2
CStationeryManager::
PairPerp(const vivid::Vector2& vector)
{
    return vivid::Vector2(-vector.y, vector.x);
}

/*!
 *  @brief  文房具の回転矩形についてワールド座標の4頂点を求める
 */
std::array<vivid::Vector2, 4>
CStationeryManager::
GetPairCorners(const IStationery& item)
{
    const vivid::Vector2& center = item.GetPosition();
    const vivid::Vector2& size = item.GetSize();
    const float c = std::cos(item.GetRotation());
    const float s = std::sin(item.GetRotation());
    const float hx = size.x * 0.5f;
    const float hy = size.y * 0.5f;
    const std::array<vivid::Vector2, 4> local = {{
        vivid::Vector2(-hx, -hy), vivid::Vector2(hx, -hy),
        vivid::Vector2(hx, hy), vivid::Vector2(-hx, hy)
    }};
    std::array<vivid::Vector2, 4> result{};

    // ローカル4頂点を中心座標に対して回転させ、ワールド座標へ変換。
    for (int i = 0; i < 4; ++i)
    {
        result[i] = vivid::Vector2(
            center.x + local[i].x * c - local[i].y * s,
            center.y + local[i].x * s + local[i].y * c);
    }
    return result;
}

/*
 *  向きに最も張り出した頂点を求める
 *  ほぼ同じ投影位置にある頂点は平均して接触辺の中央にする
 */
vivid::Vector2
CStationeryManager::
GetPairSupport(const IStationery& item, vivid::Vector2 direction)
{
    const auto corners = GetPairCorners(item);
    // 指定方向への内積が最大の点が、最も外側にある頂点。
    float best = vivid::Vector2::Dot(corners[0], direction);
    for (const vivid::Vector2 corner : corners)
    {
        best = Utility::Max(best, vivid::Vector2::Dot(corner, direction));
    }
    // ほぼ同じ位置にある辺の2頂点は平均化して接触点を安定させる。
    vivid::Vector2 sum(0.0f, 0.0f);
    int count = 0;
    for (const vivid::Vector2 corner : corners)
    {
        if (best - vivid::Vector2::Dot(corner, direction) < 0.5f)
        {
            sum = sum + corner;
            ++count;
        }
    }
    if (count <= 0) return corners[0];
    return sum * (1.0f / static_cast<float>(count));
}

/*
 *  文房具同士の衝突処理
 *  地形の既存ソルバは変更しない。2物体の速度と位置を補正する。
 */
void
CStationeryManager::
ResolveStationeryPairs(void)
{
    // 接触・摩擦・位置補正の定数はクラス共通定数として宣言済み。

    // 物体数が2未満なら文房具同士の衝突はない
    if (m_Stationeries.size() < 2) return;

    // 速度拘束と位置補正を分離する。
    // 速度を解決している途中で接触位置を変更すると、同じ接触面の
    // 法線や作用点が反復ごとに揺れ、回転・滑りが増幅される場合がある。
    //
    // 第1段階：速度・角速度・摩擦だけを解決する。
    // 第2段階：従来と同じ回数と補正量でめり込みだけを解消する。
    // 位置補正を1回に減らすと沈み込むため、反復回数は維持する。
    for (int phase = 0; phase < 2; ++phase)
    {
        const bool solve_velocity = (phase == 0);
        for (int iteration = 0; iteration < m_pair_iterations; ++iteration)
        {
            bool has_contact = false;
            // 同じ組を二重に判定しないよう、i より後ろの要素のみを調べる。
            for (std::size_t i = 0; i < m_Stationeries.size(); ++i)
            {
                IStationery* a = m_Stationeries[i].get();
                // つかんでいる物体はマウスの位置・角度を優先するため除外する。
                if (m_Controller.IsDragging(a) || a->GetState() == STATIONERY_STATE::HELD) continue;

                for (std::size_t j = i + 1; j < m_Stationeries.size(); ++j)
                {
                    IStationery* b = m_Stationeries[j].get();
                    // もう一方も配置前・ドラッグ中なら衝突による補正を行わない。
                    if (m_Controller.IsDragging(b) || b->GetState() == STATIONERY_STATE::HELD) continue;

                    // 回転後の包囲AABBで明らかに離れている組を除外する。
                    // 狭い範囲だけ精密なOBB衝突を行い、非接触ペアの負荷を下げる。
                    const float ca = std::fabs(std::cos(a->m_Rotation));
                    const float sa = std::fabs(std::sin(a->m_Rotation));
                    const float cb = std::fabs(std::cos(b->m_Rotation));
                    const float sb = std::fabs(std::sin(b->m_Rotation));
                    const float ax = (a->m_Size.x * ca + a->m_Size.y * sa) * 0.5f;
                    const float ay = (a->m_Size.x * sa + a->m_Size.y * ca) * 0.5f;
                    const float bx = (b->m_Size.x * cb + b->m_Size.y * sb) * 0.5f;
                    const float by = (b->m_Size.x * sb + b->m_Size.y * cb) * 0.5f;
                    if (std::fabs(a->m_Position.x - b->m_Position.x) > ax + bx ||
                        std::fabs(a->m_Position.y - b->m_Position.y) > ay + by)
                    {
                        continue;
                    }

                    // 各文房具の中心・半サイズ・回転角から OBB を作成する。
                    // OBB なら斜めに置いた定規同士でも当たり判定が可能。
                    const Utility::OBB box_a = {
                        a->m_Position, {a->m_Size.x * 0.5f, a->m_Size.y * 0.5f}, a->m_Rotation
                    };
                    const Utility::OBB box_b = {
                        b->m_Position, {b->m_Size.x * 0.5f, b->m_Size.y * 0.5f}, b->m_Rotation
                    };
                    // 重なりの有無に加え、衝突法線とめり込み量も取得する。
                    const Utility::CollisionResult hit = Utility::Collision::Check(box_a, box_b);
                    if (!hit.IsHit || !hit.HasSeparation || hit.PenetrationDepth <= 0.0f)
                    {
                        continue;
                    }

                    // 衝突法線は a → b 方向として扱い、左右に押し離す基準にする。
                    // 異常に短い法線は正規化時に不安定になるため無視する。
                    has_contact = true;
                    vivid::Vector2 normal = hit.Normal;
                    const float normal_sq = vivid::Vector2::Dot(normal, normal);
                    if (normal_sq < 0.5f) continue;
                    normal = normal * (1.0f / std::sqrt(normal_sq));

                    // 位置補正フェーズでは速度・接触点の計算は不要。
                    // 衝突判定後はめり込み量だけを使うため、FPSへの影響を抑える。
                    bool wake_by_velocity = false;
                    if (solve_velocity)
                    {
                        // それぞれの矩形で接触法線側に張り出した点を探す。
                        // 2点の中点を近似接触位置とし、中心からの距離 ra / rb を求める。
                        // 中心からずれた位置に力が加わると、並進だけでなく回転も起こる。
                        const vivid::Vector2 pa = a->m_Position;
                        const vivid::Vector2 pb = b->m_Position;
                        const vivid::Vector2 point_a = GetPairSupport(*a, normal);
                        const vivid::Vector2 point_b = GetPairSupport(*b, normal * -1.0f);
                        const vivid::Vector2 point = (point_a + point_b) * 0.5f;
                        const vivid::Vector2 ra = point - pa;
                        const vivid::Vector2 rb = point - pb;

                        // 両物体の質量を 1 とした矩形の「逆慣性モーメント」を計算する。
                        // 値が大きいほど接触の力で回転しやすい。ここでは逆数の形で利用。
                        // 幅と高さを使い、回転のしやすさを物体の寸法に反映する。
                        const float inertia_a = 12.0f / (a->m_Size.x * a->m_Size.x +
                                                         a->m_Size.y * a->m_Size.y);
                        const float inertia_b = 12.0f / (b->m_Size.x * b->m_Size.x +
                                                         b->m_Size.y * b->m_Size.y);
                        // 接触点の速度 = 中心の並進速度 + 角速度による接線速度。
                        // 物体中心が静止していても、回っていれば接触点は動いている。
                        const vivid::Vector2 va = a->m_PhysicsVelocity +
                            PairPerp(ra) * a->m_PhysicsAngularVelocity;
                        const vivid::Vector2 vb = b->m_PhysicsVelocity +
                            PairPerp(rb) * b->m_PhysicsAngularVelocity;
                        // 接触点の相対速度を法線へ投影する。
                        // vn < 0 は互いに接近中、vn >= 0 は離れる方向なので衝撃を加えない。
                        const float vn = vivid::Vector2::Dot(vb - va, normal);
                        wake_by_velocity = (vn < -0.05f);

                        if (vn < 0.0f)
                        {
                            // 力の作用点が中心からずれている分を外積で計算する。
                            // 有効質量には 2 物体の並進抵抗(1+1)と回転抵抗を含める。
                            const float ca = vivid::Vector2::Cross(ra, normal);
                            const float cb = vivid::Vector2::Cross(rb, normal);
                            const float effective_mass = 2.0f + ca * ca * inertia_a + cb * cb * inertia_b;
                            if (effective_mass > 0.000001f)
                            {
                                // 接近速度を打ち消す衝突インパルスを求める（反発係数は加えない）。
                                // a と b に同じ大きさ・逆向きの速度変化を与える。
                                const float impulse = -vn / effective_mass;
                                const vivid::Vector2 push = normal * impulse;
                                a->m_PhysicsVelocity.x -= push.x;
                                a->m_PhysicsVelocity.y -= push.y;
                                b->m_PhysicsVelocity.x += push.x;
                                b->m_PhysicsVelocity.y += push.y;
                                // 法線方向の力が重心から離れて作用するため角速度も更新する。
                                a->m_PhysicsAngularVelocity -= ca * impulse * inertia_a;
                                b->m_PhysicsAngularVelocity += cb * impulse * inertia_b;

                                // 法線への衝突応答後に接触点の速度をもう一度求める。
                                // 法線と直交する tangent 方向のすべりを摩擦で抑える。
                                // 衝突前の速度のまま計算すると摩擦が過不足になりやすい。
                                const vivid::Vector2 tangent = PairPerp(normal);
                                const vivid::Vector2 va_next = a->m_PhysicsVelocity +
                                    PairPerp(ra) * a->m_PhysicsAngularVelocity;
                                const vivid::Vector2 vb_next = b->m_PhysicsVelocity +
                                    PairPerp(rb) * b->m_PhysicsAngularVelocity;
                                const float relative_t = vivid::Vector2::Dot(vb_next - va_next, tangent);
                                const float ta = vivid::Vector2::Cross(ra, tangent);
                                const float tb = vivid::Vector2::Cross(rb, tangent);
                                const float tangent_mass = 2.0f + ta * ta * inertia_a + tb * tb * inertia_b;
                                // 接線方向のすべりを止めるための理想インパルスを計算する。
                                // 摩擦の大きさは FRICTION × 法線インパルス以内に制限する。
                                // これにより摩擦だけが過剰な力を生むことを防ぐ。
                                const float desired = -relative_t / tangent_mass;
                                const float limit = m_pair_friction * impulse;
                                const float friction_impulse = Utility::Max(-limit, Utility::Min(desired, limit));
                                const vivid::Vector2 friction = tangent * friction_impulse;
                                // 摩擦も両物体へ逆向きに適用し、並進・回転をともに更新する。
                                a->m_PhysicsVelocity.x -= friction.x;
                                a->m_PhysicsVelocity.y -= friction.y;
                                b->m_PhysicsVelocity.x += friction.x;
                                b->m_PhysicsVelocity.y += friction.y;
                                a->m_PhysicsAngularVelocity -= ta * friction_impulse * inertia_a;
                                b->m_PhysicsAngularVelocity += tb * friction_impulse * inertia_b;
                            }
                        }

                    }

                    // 速度補正だけでは、既に重なった位置が直ちに解消しない。
                    // 許容めり込み m_pair_contact_slop を引いた深さだけ位置を補正する。
                    // 補正量は m_pair_correction_rate で緩和し、m_pair_max_correction で上限を設定。
                    // 質量を同じと仮定しているので、a と b を半分ずつ反対へ動かす。
                    const float depth = Utility::Max(0.0f, hit.PenetrationDepth - m_pair_contact_slop);
                    if (!solve_velocity)
                    {
                        const float correction = Utility::Min(depth * m_pair_correction_rate, m_pair_max_correction);
                        a->m_Position.x -= normal.x * correction * 0.5f;
                        a->m_Position.y -= normal.y * correction * 0.5f;
                        b->m_Position.x += normal.x * correction * 0.5f;
                        b->m_Position.y += normal.y * correction * 0.5f;
                    }

                    // Sleep 中の文房具は小さな接触では起こさない。
                    // 一定以上の接近速度またはめり込みがあれば、両物体を再起動する。
                    // 静止フレーム数も 0 に戻し、次の更新で物理演算を再開可能にする。
                    if (wake_by_velocity || depth > 0.5f)
                    {
                        a->m_IsPhysicsSleeping = false;
                        b->m_IsPhysicsSleeping = false;
                        a->m_PhysicsStillFrames = 0;
                        b->m_PhysicsStillFrames = 0;
                    }
                }
            }

            // この段階で接触がなければ、無駄な残りの反復を終了する。
            if (!has_contact) break;
        }
    }
}
