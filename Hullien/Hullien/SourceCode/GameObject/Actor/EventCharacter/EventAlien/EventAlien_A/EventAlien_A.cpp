#include "EventAlien_A.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"

/*************************************
*	イベント用宇宙人A.
**/
CEventAlienA::CEventAlienA()
{
	m_ObjectTag = EObjectTag::Alien_A;
}

CEventAlienA::~CEventAlienA()
{
}

// 初期化関数.
bool CEventAlienA::Init()
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
void CEventAlienA::Update()
{
	if (m_Parameter.IsDisp == false) return;
	CurrentStateUpdate();	// 現在の状態の更新.
}

// 描画関数.
void CEventAlienA::Render()
{
	if (m_Parameter.IsDisp == false) return;

#ifndef IS_TEMP_MODEL_RENDER
	if (m_pSkinMesh == nullptr) return;

	m_pSkinMesh->SetPosition(m_vPosition);
	D3DXVECTOR3 rot = m_vRotation;
	rot.y += static_cast<float>(D3DX_PI);
	m_pSkinMesh->SetRotation(rot);
	m_pSkinMesh->SetScale(m_vSclae);
	m_pSkinMesh->SetColor({ 0.5f, 0.8f, 0.5f, m_Parameter.ModelAlpha });
	//	m_pSkinMesh->SetBlend( true );
	m_pSkinMesh->SetRasterizerState(CCommon::enRS_STATE::Back);
	m_pSkinMesh->Render();
	m_pSkinMesh->SetRasterizerState(CCommon::enRS_STATE::None);
	//	m_pSkinMesh->SetBlend( false );
#else
	if (m_pTempStaticMesh == nullptr) return;
	m_pTempStaticMesh->SetPosition(m_vPosition);
	m_pTempStaticMesh->SetRotation(m_vRotation);
	m_pTempStaticMesh->SetScale(m_vSclae);
	m_pTempStaticMesh->SetColor({ 0.5f, 0.8f, 0.5f, m_Parameter.ModelAlpha });
	//	m_pTempStaticMesh->SetBlend( true );
	m_pTempStaticMesh->SetRasterizerState(CCommon::enRS_STATE::Back);
	m_pTempStaticMesh->Render();
	m_pTempStaticMesh->SetRasterizerState(CCommon::enRS_STATE::None);
	//	m_pTempStaticMesh->SetBlend( false );
#endif	// #ifdef IS_TEMP_MODEL_RENDER.

#if _DEBUG
	if (m_pCollManager == nullptr) return;
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// 当たり判定関数.
void CEventAlienA::Collision(CActor * pActor)
{
	if (m_Parameter.IsDisp == false) return;
	if (pActor == nullptr) return;
	if (m_pCollManager == nullptr) return;
	if (m_pCollManager->GetSphere() == nullptr) return;

	GirlCollision(pActor);
	BarrierCollision(pActor);
}

// スポーン.
bool CEventAlienA::Spawn(const D3DXVECTOR3& spawnPos)
{
	// 既にスポーン済みなら終了.
	if (m_NowState != EEventAlienState::None) return true;
	// 初期化に失敗したら終了.
	if (Init() == false) return false;

	Spawning();
	return true;
}

// スポーン.
void CEventAlienA::Spawning()
{
	CEventAlien::Spawning();
}

// 移動.
void CEventAlienA::Move()
{
	CEventAlien::Move();
}

// 拐う.
void CEventAlienA::Abduct()
{
	CEventAlien::Abduct();
}

// 逃げる.
void CEventAlienA::Escape()
{
	CEventAlien::Escape();
}

// 吹き飛ぶ.
void CEventAlienA::BlowAway()
{
	CEventAlien::BlowAway();
}

// 当たり判定の設定
bool CEventAlienA::ColliderSetting()
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
	if (FAILED(m_pCollManager->InitSphere(
		m_pTempStaticMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius))) return false;
	return true;
#endif	// #ifndef IS_MODEL_RENDER.
}

