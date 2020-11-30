#include "EditAlien_C.h"
#include "..\..\..\..\..\Arm\Arm.h"
#include "..\..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"

CEditAlienC::CEditAlienC()
{
	m_vScale = { 1.0f, 1.0f, 1.0f };
	m_ObjectTag = EObjectTag::Alien_B;
}

CEditAlienC::~CEditAlienC()
{}

// �������֐�.
bool CEditAlienC::Init()
{
	if( GetModel( MODEL_NAME )		== false ) return false;
	if( GetAnimationController()	== false ) return false;
	if( SetAnimFrameList()			== false ) return false;
	if( ColliderSetting()			== false ) return false;
	if( EffectSetting()				== false ) return false;
	m_pSkinMesh->ChangeAnimSet_StartPos( alien::EAnimNo_Move, 0.0f, m_pAC );
	return true;
}

// �X�V�֐�.
void CEditAlienC::Update()
{
	// �A�j���[�V�����t���[���̍X�V.
	m_AnimFrameList[m_NowAnimNo].UpdateFrame( m_AnimSpeed );
	CurrentStateUpdate();				// ���݂̏�Ԃ̍X�V.
}

// �`��֐�.
void CEditAlienC::Render()
{
	// ��ʂ̊O�Ȃ�I��.
	if( IsDisplayOut() == true ) return;
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vScale );
	m_pSkinMesh->SetColor( { 0.2f, 0.8f, 0.2f, 1.0f } );
	m_pSkinMesh->SetAnimSpeed( m_AnimSpeed );
	m_pSkinMesh->SetRasterizerState( ERS_STATE::Back );
	m_pSkinMesh->Render( m_pAC );
	m_pSkinMesh->SetRasterizerState( ERS_STATE::None );

#if _DEBUG
	if( m_pCollManager == nullptr ) return;
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// �����蔻��֐�.
void CEditAlienC::Collision( CActor* pActor )
{}

// �X�|�[��.
void CEditAlienC::Spawning()
{
	CEditAlien::Spawning();
}

// �ړ�.
void CEditAlienC::Move()
{
	CEditAlien::Move();
}

// ����.
void CEditAlienC::Abduct()
{
	CEditAlien::Abduct();
}

// ����.
void CEditAlienC::Fright()
{
	CEditAlien::Fright();
}

// ���S.
void CEditAlienC::Death()
{
	CEditAlien::Death();
}

// ������.
void CEditAlienC::Escape()
{
	CEditAlien::Escape();
}

// �����蔻��̐ݒ�.
bool CEditAlienC::ColliderSetting()
{
	if( m_pSkinMesh == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere( 
		m_pSkinMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		m_Paramter.SphereAdjPos,
		m_Paramter.SphereAdjRadius ) )) return false;
	return true;
}