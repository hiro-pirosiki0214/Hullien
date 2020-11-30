#include "Barrier.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\XAudio2\SoundManager.h"

#include "..\..\..\Common\Effect\EffectManager.h"

CBarrier::CBarrier()
	: m_StaticMesh			( nullptr )
	, m_pEffect				( nullptr )
	, m_IsEffectPlay		( false )
	, m_IsActive			( false )
	, m_ActiveCount			( 0.0f )
	, m_CollSphereRadius	( 0.0f )
#if _DEBUG
	, m_ResizeCollTime		( 0.0f )
#endif	// #if _DEBUG.
{
	m_ObjectTag = EObjectTag::Bariier;
	m_pEffect = std::make_shared<CEffectManager>();

}

CBarrier::~CBarrier()
{
}

// �������֐�.
bool CBarrier::Init()
{
	if( m_IsActive == true ) return false;
	if( GetModel() == false ) return false;
	if( ColliderSetting() == false ) return false;
	if (m_pEffect->SetEffect(EFFECT_NAME) == false) return false;
	m_IsActive			= true;
	m_CollSphereRadius	= 0.0f;
	m_ActiveCount		= 0.0f;
	m_IsEffectPlay = false;

	return true;
}

// �X�V�֐�.
void CBarrier::Update()
{
	if( m_IsActive == false ) return;	// ���삵�ĂȂ���ΏI��.
	if( m_CollSphereRadius <= COLLISION_SPHERE_RDIUS_MAX ){
		CSoundManager::NoMultipleSEPlay("BarrierInvocating");
		m_CollSphereRadius += COLLISION_SPHERE_RDIUS_ADD_VALUE;	// �����蔼�a�̉��Z.
		return;
	}

	m_ActiveCount++;	// ����J�E���g�̉��Z.
	if( m_ActiveCount < ACTIVE_TIME*FPS ) return;
	m_IsActive = false;	// ���삵�Ȃ�.
	m_CollSphereRadius	= 0.0f;
	m_ActiveCount		= 0.0f;
	// �����蔻��̃T�C�Y��������.
	m_pCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_CollSphereRadius,
		{ 0.0f, 0.0f, 0.0f },
		m_CollSphereRadius );
}

// �`��֐�.
void CBarrier::Render()
{
	if( m_IsActive == false ) return;		// ���삵�ĂȂ���ΏI��.
	if( m_StaticMesh == nullptr ) return;
	if( m_pCollManager == nullptr ) return;

	//m_StaticMesh->SetPosition( m_vPosition );
	//m_StaticMesh->SetRotation( m_vRotation );
	//m_StaticMesh->SetScale( m_CollSphereRadius );
	//m_StaticMesh->SetColor( { 0.2f, 0.2f, 1.0f, 0.3f } );
	//m_StaticMesh->SetBlend( true );
	//m_StaticMesh->Render( true );
	//m_StaticMesh->SetBlend( false );

#if _DEBUG
	m_pCollManager->DebugRender();
	m_ResizeCollTime++;
	if( m_ResizeCollTime < 15.0f ) return;
	m_ResizeCollTime = 0.0f;
#endif	// #if _DEBUG.
	// �����蔻��̃T�C�Y��ύX.
	m_pCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_CollSphereRadius,
		{ 0.0f, 0.0f, 0.0f },
		m_CollSphereRadius );
}

// �G�t�F�N�g�`��֐�.
void CBarrier::EffectRender()
{
	if( m_IsActive == false ) return;		// ���삵�ĂȂ���ΏI��.
	if( m_StaticMesh == nullptr ) return;
	if( m_pCollManager == nullptr ) return;

	// �G�t�F�N�g��`��.
	m_pEffect->SetLocation( m_vPosition );
	m_pEffect->SetScale( 0.3f );
	m_pEffect->Render();

#if _DEBUG
	m_pCollManager->DebugRender();
	m_ResizeCollTime++;
	if( m_ResizeCollTime < 15.0f ) return;
	m_ResizeCollTime = 0.0f;
#endif	// #if _DEBUG.
	// �����蔻��̃T�C�Y��ύX.
	m_pCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_CollSphereRadius,
		{ 0.0f, 0.0f, 0.0f },
		m_CollSphereRadius );
}

// �����蔻��֐�.
void CBarrier::Collision( CActor* pActor )
{
	if( m_IsActive == false ) return;
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	// �ΏۃI�u�W�F�N�g����Ȃ���ΏI��.
	if( ( pActor->GetObjectTag() != EObjectTag::Alien_A ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_B ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_C ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_D )) return;

	// ���̂̓����蔻��.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;
	pActor->SetTargetPos( *this );
	
}

// ������W�̐ݒ�֐�.
void CBarrier::SetTargetPos( CActor& pActor )
{
	if( m_IsActive == false ) return;
	m_vPosition = pActor.GetPosition();
	// ���łɃG�t�F�N�g���Đ����Ă���ΏI��.
	if (m_IsEffectPlay == true) return;
	m_pEffect->Play(m_vPosition);	// �G�t�F�N�g���Đ�.
	m_IsEffectPlay = true;

}

// ���f���̎擾.
bool CBarrier::GetModel()
{
	// ���ɓǂݍ��߂Ă�����I��.
	if( m_StaticMesh != nullptr ) return true;
	// ���f���̎擾.
	CMeshResorce::GetStatic( m_StaticMesh, MODEL_NAME );
	// ���f�����ǂݍ��߂ĂȂ���� false.
	if( m_StaticMesh == nullptr ) return false;
	return true;
}

// �����蔻��̐ݒ�.
bool CBarrier::ColliderSetting()
{
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	return true;
}