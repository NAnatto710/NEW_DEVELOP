#pragma once

enum class EFFECT_ID
{
	ATTACK,				// 未
	HIT,				// ヒットエフェクト
	IMPACT,				// ヒットエフェクトと同時につける
	CHARGE,				// 未
	GUARD,				// ガード状態
	BLOCK_PARTICLE,		// 未
	JAMP,				// ジャンプエフェクト
	LANDING,			// 着地エフェクト
	SMOKE,				// ダッシュ時の煙
	RESPAWN,			// 未
	RESPAWN_AURA,		// リスポーン時のオーラ
	RESPAWN_PARTICLE,
	SALIGIA_AURA,		// 選択時の背景
	HP_PARTICLE,		// 未
	TITLE_PARTICLE,		// タイトル画面でのパーティクル
	SIRCLE,				// 円の拡大エフェクト
	TWINKLE,
	RANKING_TWINKLE,
	ICON_DROP,
	FRAME_FLASH,
	AURA_PARTICLE,
	IMPACT_FLASH,
};

enum class DIRECTION
{
	TOP,
	BOTTOM,
	RIGHT,
	LEFT,
};
