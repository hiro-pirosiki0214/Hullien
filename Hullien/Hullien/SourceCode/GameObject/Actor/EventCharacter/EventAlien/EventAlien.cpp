#include "EventAlien.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"

/****************************************
*	イベント用宇宙人クラス.
**/
CEventAlien::CEventAlien()
	: m_pAbductUFOPosition		( nullptr )
	, m_NowState				( EEventAlienState::None )
{
}

CEventAlien::~CEventAlien()
{
}

// 相手座標の設定.
void CEventAlien::SetTargetPos(CActor & actor)
{
	SetGirlPos(actor);
}

// 現在の状態の更新関数.
void CEventAlien::CurrentStateUpdate()
{
	switch (m_NowState)
	{
	case EEventAlienState::Spawn:
		this->Spawning();
		break;
	case EEventAlienState::Move:
		this->Move();
		break;
	case EEventAlienState::Abduct:
		this->Abduct();
		break;
	case EEventAlienState::Escape:
		this->Escape();
		break;
	case EEventAlienState::BlowAway:
		this->BlowAway();
		break;
	default:
		break;
	}
}

// 女の子の座標を設定.
void CEventAlien::SetGirlPos(CActor& actor)
{
	if (m_NowMoveState == EMoveState::Move) return;
	// 女の子じゃなければ終了.
	if (actor.GetObjectTag() != EObjectTag::Girl) return;

}

// スポーン中.
void CEventAlien::Spawning()
{
	if (m_Parameter.IsDisp == false)
	{
		m_Parameter.IsDisp = true;	
		m_Parameter.vScale = {0.0f,0.0f,0.0f};
	}

	// モデルのアルファ値を足していく.
	if (m_Parameter.ModelAlpha >= MODEL_ALPHA_MAX) return;
	m_Parameter.ModelAlpha += m_Parameter.AlphaSpeed;

	// モデルのスケール値を足していく.
	if (m_Parameter.vScale.x >= MODEL_SCALE_MAX) return;
	m_vSclae.x += m_Parameter.ScaleSpeed;
	m_vSclae.y += m_Parameter.ScaleSpeed;
	m_vSclae.z += m_Parameter.ScaleSpeed;
	if (m_Parameter.vScale.x > MODEL_SCALE_MAX)
	{
		m_Parameter.vScale.x = MODEL_SCALE_MAX;
		m_Parameter.vScale.y = MODEL_SCALE_MAX;
		m_Parameter.vScale.z = MODEL_SCALE_MAX;
	}

}

// 移動.
void CEventAlien::Move()
{
}

// 拐う.
void CEventAlien::Abduct()
{
	if (m_IsBarrierHit == true) return;
}

// 逃げる.
void CEventAlien::Escape()
{
	if (m_IsBarrierHit == true) return;
}

// 吹き飛ぶ.
void CEventAlien::BlowAway()
{

}

// 女の子との当たり判定.
void CEventAlien::GirlCollision(CActor* pActor)
{
	// オブジェクトのタグが女の子じゃなければ終了.
	if (pActor->GetObjectTag() != EObjectTag::Girl) return;
	if (m_IsBarrierHit == true) return;
	if (m_NowState == EEventAlienState::Spawn) return;	// スポーン状態なら終了.

	bool isAbduct = false;
	if (m_NowState == EEventAlienState::Abduct) {
		isAbduct = true;
	}
	else {
		isAbduct = true;
	}

	if (isAbduct == false) return;
	// 球体の当たり判定.
	if (m_pCollManager->IsShereToShere(pActor->GetCollManager()) == false) return;
	pActor->SetTargetPos(*this);

	if (m_NowState == EEventAlienState::Abduct) return;
	m_NowState = EEventAlienState::Abduct;
	m_NowMoveState = EMoveState::Rotation;
}

// バリアとの当たり判定.
void CEventAlien::BarrierCollision(CActor* pActor)
{
	// オブジェクトのタグが女の子じゃなければ終了.
	if (pActor->GetObjectTag() != EObjectTag::Bariier) return;
	const float moveSpeed = -2.0f;
	// 球体の当たり判定.
	if (m_pCollManager->IsShereToShere(pActor->GetCollManager()) == false) {
		m_Speed  = m_Parameter.MoveSpeed;
		m_IsBarrierHit = false;

	}
	else {
		m_Speed = moveSpeed;
		m_IsBarrierHit = true;
		m_NowState = EEventAlienState::Move;
		m_NowMoveState = EMoveState::Move;
	}
}
