#include "EventBase.h"

/***********************************
*	イベントシーン元クラス.
**/
CEventBase::CEventBase()
	: m_IsEventEnd	( false )
	, m_IsSkip		( false )
{
}

CEventBase::~CEventBase()
{
}

// 目的地へのの移動.
bool CEventBase::MoveDestination(D3DXVECTOR3& vMyPosition, const D3DXVECTOR3 & vDestination, const float & speed)
{
	// 移動距離の算出.
	float Distance = sqrtf((vMyPosition.x - vDestination.x) * (vMyPosition.x - vDestination.x)
		+ (vMyPosition.y - vDestination.y) * (vMyPosition.y - vDestination.y)
		+ (vMyPosition.z - vDestination.z) * (vMyPosition.z - vDestination.z));

	// 移動回数の算出.
	float MoveCount = Distance / speed;
	if (MoveCount < 1) return true;

	// 移動量を座標に足す.
	vMyPosition.x += (vDestination.x - vMyPosition.x) / MoveCount;
	vMyPosition.y += (vDestination.y - vMyPosition.y) / MoveCount;
	vMyPosition.z += (vDestination.z - vMyPosition.z) / MoveCount;

	if (MoveCount >= 1) return false;
	return true;
}
