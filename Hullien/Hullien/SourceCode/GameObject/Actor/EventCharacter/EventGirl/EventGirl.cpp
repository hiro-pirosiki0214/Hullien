#include "EventGirl.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"

#include "..\..\..\Actor\Actor.h"

/****************************************
*	イベント用女の子クラス.
**/
CEventGirl::CEventGirl()
	: m_Parameter			()
	, m_pSearchCollManager	( nullptr )
	, m_OldPosition			( 0.0f, 0.0f, 0.0f )
	, m_NowState			( ENowState::None )
	, m_IsDanger			( false )
{
	m_ObjectTag		= EObjectTag::Girl;
	m_NowState		= ENowState::Move;
	m_NowMoveState	= EMoveState::Move;
	m_pSearchCollManager = std::make_shared<CCollisionManager>();
}

CEventGirl::~CEventGirl()
{
}

// 初期化関数.
bool CEventGirl::Init()
{
#ifndef IS_TEMP_MODEL_RENDER
	if (GetModel(MODEL_NAME) == false) return false;
#else
	if (GetModel(MODEL_TEMP_NAME) == false) return false;
#endif	// #ifndef IS_TEMP_MODEL_RENDER.
	if (ColliderSetting() == false) return false;

	return true;
}

// 更新関数.
void CEventGirl::Update()
{
	if (m_Parameter.IsDisp == false) return;
	switch (m_NowState)
	{
	case ENowState::None:
		break;
	case ENowState::Protected:
		break;
	case ENowState::Abduct:
		break;
	case ENowState::Move:
		Move();
		break;
	case ENowState::Wait:
		break;
	default:
		break;
	}
}

// 描画関数
void CEventGirl::Render()
{
	if (m_Parameter.IsDisp == false) return;
	MeshRender();	// メッシュの描画.

#if _DEBUG
	if (m_pCollManager == nullptr) return;
	m_pCollManager->DebugRender();
	if (m_pSearchCollManager == nullptr) return;
	m_pSearchCollManager->DebugRender();
#endif	// #if _DEBUG.

}

// 当たり判定関数.
void CEventGirl::Collision(CActor * pActor)
{
	if (m_Parameter.IsDisp == false) return;
	if (pActor == nullptr) return;
	SearchCollision(pActor);
	if (m_pCollManager == nullptr) return;
	if (m_pCollManager->GetSphere() == nullptr) return;
}

// 相手座標の設定関数
void CEventGirl::SetTargetPos(CActor& actor)
{
	m_vPosition = actor.GetPosition();
	m_NowState = ENowState::Abduct;
}

// 情報設定関数.
void CEventGirl::SetOptionalState(const SOptionalState & state)
{
	m_vPosition = state.vPosition;
	m_vRotation = state.vRotation;
	m_vSclae = state.vScale;
	m_Parameter.ModelAlpha = state.ModelAlpha;
	m_Parameter.MoveSpeed = state.MoveSpeed;
	m_Parameter.RotationalSpeed = state.RotationalSpeed;
	m_Parameter.ScaleSpeed = state.ScaleSpeed;
	m_Parameter.AlphaSpeed = state.AlphaSpeed;
	m_Parameter.IsDisp = state.IsDisp;
}

// 移動関数.
void CEventGirl::Move()
{
	switch (m_NowMoveState)
	{
	case EMoveState::None:
		break;
	case EMoveState::Rotation:
		break;
	case EMoveState::Move:
		break;
	default:
		break;
	}
}

// 索敵の当たり判定.
void CEventGirl::SearchCollision(CActor * pActor)
{
	if (m_Parameter.IsDisp == false) return;
	if (pActor == nullptr) return;
	if (m_pSearchCollManager == nullptr) return;
	if (m_pSearchCollManager->GetSphere() == nullptr) return;

	// 既に連れ去られていたら終了.
	if (m_NowState == ENowState::Abduct) return;

	// 対象オブジェクトじゃなければ終了.
	if ((pActor->GetObjectTag() != EObjectTag::Alien_A) &&
		(pActor->GetObjectTag() != EObjectTag::Alien_B) &&
		(pActor->GetObjectTag() != EObjectTag::Alien_C) &&
		(pActor->GetObjectTag() != EObjectTag::Alien_D)) return;

	// 球体の当たり判定.
	if (m_pSearchCollManager->IsShereToShere(pActor->GetCollManager()) == false) return;
	m_IsDanger = true;

}

// 当たり判定の作成.
bool CEventGirl::ColliderSetting()
{
#ifndef IS_TEMP_MODEL_RENDER
	if (m_pSkinMesh == nullptr) return false;
	if (m_pCollManager == nullptr) {
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if (FAILED(m_pCollManager->InitSphere(
		m_pSkinMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius))) return false;
	return true;
#else
	if (m_pTempStaticMesh == nullptr) return false;
	if (m_pCollManager == nullptr) {
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	// 女の子の当たり判定.
	if (FAILED(m_pCollManager->InitSphere(
		m_pTempStaticMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius))) return false;
	// 索敵の当たり判定.
	if (FAILED(m_pSearchCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SearchCollRadius))) return false;

	return true;
#endif	// #ifndef IS_MODEL_RENDER.
}
