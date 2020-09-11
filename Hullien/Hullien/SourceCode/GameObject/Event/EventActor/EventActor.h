#ifndef EVENT_ACTOR_MANAGER_H
#define EVENT_ACTOR_MANAGER_H

#include "..\..\GameObject.h"

class CCollisionManager;	// 当たり判定クラス.

/****************************************
*	イベント用アクタクラス.
**/
class CEventActor : public CGameObject
{
public:
	CEventActor();
	virtual ~CEventActor();

	// 当たり判定関数.
	virtual void Collision(CEventActor* pActor) = 0;
	// 相手座標の設定関数.
	virtual void SetTargetPos(CEventActor& actor) {}

	// 当たり判定取得関数.
	CCollisionManager* GetCollManager() { return m_pCollManager.get(); }

protected:
	std::shared_ptr<CCollisionManager>	m_pCollManager;	// 当たり判定クラス.
};

#endif//	#ifndef EVENT_ACTOR_MANAGER_H.