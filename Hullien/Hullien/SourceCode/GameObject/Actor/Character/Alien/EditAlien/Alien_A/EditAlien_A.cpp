#include "EditAlien_A.h"
#include "..\..\..\..\..\Arm\Arm.h"
#include "..\..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"

CEditAlienA::CEditAlienA()
{
	m_vScale = { 1.0f, 1.0f, 1.0f };
	m_ObjectTag = EObjectTag::Alien_A;
}

CEditAlienA::~CEditAlienA()
{}

// �������֐�.
bool CEditAlienA::Init()
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
void CEditAlienA::Update()
{
	// �A�j���[�V�����t���[���̍X�V.
	m_AnimFrameList[m_NowAnimNo].UpdateFrame( m_AnimSpeed );
	CurrentStateUpdate();				// ���݂̏�Ԃ̍X�V.
}

// �`��֐�.
void CEditAlienA::Render()
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
void CEditAlienA::Collision( CActor* pActor )
{}

// �X�|�[��.
void CEditAlienA::Spawning()
{
	CEditAlien::Spawning();
}

// �ړ�.
void CEditAlienA::Move()
{
	CEditAlien::Move();
}

// ����.
void CEditAlienA::Abduct()
{
	CEditAlien::Abduct();
}

// ����.
void CEditAlienA::Fright()
{
	CEditAlien::Fright();
}

// ���S.
void CEditAlienA::Death()
{
	CEditAlien::Death();
}

// ������.
void CEditAlienA::Escape()
{
	CEditAlien::Escape();
}

// �����蔻��̐ݒ�.
bool CEditAlienA::ColliderSetting()
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