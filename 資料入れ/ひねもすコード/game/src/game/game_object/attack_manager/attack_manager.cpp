
/*!
 *  @file       attack_manager.cpp
 *  @brief      攻撃管理クラス
 *  @author     Misaki Kawada
 *  @date       2026/2/2
 */

#include "attack_manager.h"

/*
 *  インスタンスの取得
 */
CAttackManager&
CAttackManager::
GetInstance(void)
{
	static CAttackManager instance;

	return instance;
}

/*
 *	初期化
 */
void
CAttackManager::
Initialize(void)
{
}

/*
 *	解放
 */
void
CAttackManager::
Finalize(void)
{

}

/*
 *	突進
 */
vivid::Vector2
CAttackManager::
Rush(vivid::Vector2 velocity, float speed, float direction, float val)
{
	return m_Rush.Rush(velocity, speed, direction, val);
}

/*
 *	コンストラクタ
 */
CAttackManager::
CAttackManager(void)
{
}

/*
 *	コピーコンストラクタ
 */
CAttackManager::
CAttackManager(const CAttackManager& rhs)
{
	(void)rhs;
}

/*
 *	デストラクタ
 */
CAttackManager::
~CAttackManager(void)
{
}

/*
 *	代入演算子
 */
CAttackManager&
CAttackManager::
operator=(const CAttackManager& rhs)
{
	(void)rhs;

	return *this;
}

