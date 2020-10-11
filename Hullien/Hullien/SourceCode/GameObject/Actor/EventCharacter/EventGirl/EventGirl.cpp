#include "EventGirl.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"

#include "..\..\..\Actor\Actor.h"

/****************************************
*	�C�x���g�p���̎q�N���X.
**/
CEventGirl::CEventGirl()
	: m_Parameter			()
	, m_pSearchCollManager	( nullptr )
	, m_OldPosition			( 0.0f, 0.0f, 0.0f )
	, m_NowState			( ENowState::None )
	, m_IsDanger			( false )
{
	m_ObjectTag = EObjectTag::Girl;
	m_NowState = ENowState::Move;
	m_NowMoveState = EMoveState::Move;
	m_pSearchCollManager = std::make_shared<CCollisionManager>();
}

CEventGirl::~CEventGirl()
{
}

// �������֐�.
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

// �X�V�֐�.
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

// �`��֐�
void CEventGirl::Render()
{
	if (m_Parameter.IsDisp == false) return;
	MeshRender();	// ���b�V���̕`��.

#if _DEBUG
	if (m_pCollManager == nullptr) return;
	m_pCollManager->DebugRender();
	if (m_pSearchCollManager == nullptr) return;
	m_pSearchCollManager->DebugRender();
#endif	// #if _DEBUG.

}

// �����蔻��֐�.
void CEventGirl::Collision(CActor * pActor)
{
	if (m_Parameter.IsDisp == false) return;
	if (pActor == nullptr) return;
	SearchCollision(pActor);
	if (m_pCollManager == nullptr) return;
	if (m_pCollManager->GetSphere() == nullptr) return;
}

// ������W�̐ݒ�֐�
void CEventGirl::SetTargetPos(CActor& actor)
{
	m_vPosition = actor.GetPosition();
	m_NowState = ENowState::Abduct;
}

// �ړ��֐�.
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

// ���G�̓����蔻��.
void CEventGirl::SearchCollision(CActor * pActor)
{
	if (m_Parameter.IsDisp == false) return;
	if (pActor == nullptr) return;
	if (m_pSearchCollManager == nullptr) return;
	if (m_pSearchCollManager->GetSphere() == nullptr) return;

	// ���ɘA�ꋎ���Ă�����I��.
	if (m_NowState == ENowState::Abduct) return;

	// �ΏۃI�u�W�F�N�g����Ȃ���ΏI��.
	if ((pActor->GetObjectTag() != EObjectTag::Alien_A) &&
		(pActor->GetObjectTag() != EObjectTag::Alien_B) &&
		(pActor->GetObjectTag() != EObjectTag::Alien_C) &&
		(pActor->GetObjectTag() != EObjectTag::Alien_D)) return;

	// ���̂̓����蔻��.
	if (m_pSearchCollManager->IsShereToShere(pActor->GetCollManager()) == false) return;
	m_IsDanger = true;

}

// �����蔻��̍쐬.
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
	// ���̎q�̓����蔻��.
	if (FAILED(m_pCollManager->InitSphere(
		m_pTempStaticMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius))) return false;
	// ���G�̓����蔻��.
	if (FAILED(m_pSearchCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SearchCollRadius))) return false;

	return true;
#endif	// #ifndef IS_MODEL_RENDER.
}