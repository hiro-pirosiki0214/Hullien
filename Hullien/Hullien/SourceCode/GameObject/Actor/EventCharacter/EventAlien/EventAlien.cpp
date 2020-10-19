#include "EventAlien.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\XAudio2\SoundManager.h"	
#include "..\..\..\Arm\Arm.h"

/****************************************
*	イベント用宇宙人クラス.
**/
CEventAlien::CEventAlien()
	: m_pArm					( nullptr )
	, m_pAbductUFOPosition		( nullptr )
	, m_NowState				( EEventAlienState::None )
	, m_vTargetPosition			( D3DXVECTOR3(0.0f, 0.0f, 0.0f) )
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

bool CEventAlien::IsGrab() const
{
	return m_pArm->IsGrab();
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
		Move();
		break;
	default:
		break;
	}
}

// 女の子の座標を設定.
void CEventAlien::SetGirlPos(CActor& actor)
{
	// 女の子じゃなければ終了.
	if (actor.GetObjectTag() != EObjectTag::Girl) return;
	m_vTargetPosition = actor.GetPosition();	// 女の子の座標を取得.

	// 目的の回転軸を取得.
	float TargetRotationY = atan2f(
		m_vTargetPosition.x - m_vPosition.x,
		m_vTargetPosition.z - m_vPosition.z);

	// 移動用ベクトルを取得.
	m_vTargetPosition.x -= sinf(TargetRotationY) * CArm::GRAB_DISTANCE;
	m_vTargetPosition.z -= cosf(TargetRotationY) * CArm::GRAB_DISTANCE;
}


// スポーン中.
void CEventAlien::Spawning()
{
	// モデルのスケール値を足していく.
	if (m_vSclae.x >= MODEL_SCALE_MAX) return;
	m_vSclae.x += m_Parameter.ScaleSpeed;
	m_vSclae.y += m_Parameter.ScaleSpeed;
	m_vSclae.z += m_Parameter.ScaleSpeed;
	if (m_vSclae.x > MODEL_SCALE_MAX)
	{
		m_vSclae.x = MODEL_SCALE_MAX;
		m_vSclae.y = MODEL_SCALE_MAX;
		m_vSclae.z = MODEL_SCALE_MAX;
		m_NowState = EEventAlienState::Move;
	}

	// モデルのアルファ値を足していく.
	if (m_Parameter.ModelAlpha >= MODEL_ALPHA_MAX) return;
	m_Parameter.ModelAlpha += m_Parameter.AlphaSpeed;
}

// 女の子との当たり判定.
void CEventAlien::GirlCollision(CActor* pActor)
{
	// オブジェクトのタグが女の子じゃなければ終了.
	if (pActor->GetObjectTag() != EObjectTag::Girl) return;
	if (m_IsBarrierHit == true) return;
	if (m_NowState == EEventAlienState::Spawn)		return;	// スポーン状態なら終了.

	bool isAbduct = false;
	if (m_NowState == EEventAlienState::Abduct) {
		isAbduct = true;
		pActor->SetPosition(m_pArm->GetGrabPosition());
		return;
	}
	else {
		isAbduct = true;
	}

	if (isAbduct == false) return;
	// 球体の当たり判定.
	if (m_pCollManager->IsShereToShere(pActor->GetCollManager()) == false) return;

	if (m_pArm->IsGrab() == false) {
		m_pArm->SetAppearance();
		return;
	}
	pActor->SetPosition(m_pArm->GetGrabPosition());

	if (m_NowState == EEventAlienState::Abduct) return;
	m_NowState = EEventAlienState::Abduct;
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
	}
}
