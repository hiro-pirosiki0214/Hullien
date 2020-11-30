#include "Explosion.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Common\Effect\EffectManager.h"
#include "..\..\..\Resource\EffectResource\EffectResource.h"

CExplosion::CExplosion()
	: m_pEffect				( nullptr )
	, m_Param				()
	, m_CollSphereRadius	( 0.0f )
	, m_IsEffectPlay		( false )
#if _DEBUG
	, m_ResizeCollTime		( 0.0f )
#endif	// #if _DEBUG.
{
	m_pEffect = std::make_shared<CEffectManager>();
}

CExplosion::~CExplosion()
{
}

// �������֐�.
bool CExplosion::Init()
{
	if( EffectSetting() == false ) return false;
	if( ColliderSetting() == false ) return false;
	m_CollSphereRadius = 0.0f;
	m_IsEffectPlay = false;
	return true;
}

// �X�V�֐�.
void CExplosion::Update()
{
}

// �`��֐�.
void CExplosion::Render()
{
	// �G�t�F�N�g�̍Đ��I����Ă���ΏI��,
	if( m_IsEffectPlay == false ) return;

	// �����蔻�肪�ő哖���蔻����傫����ΏI��.
	if( m_CollSphereRadius >= m_Param.SphereMaxRadius )return;
	if( m_pCollManager == nullptr ) return;
	m_CollSphereRadius += m_Param.ExplosionSpeed;	// �����蔻���傫������.

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
		&m_vScale.x,
		m_Param.SphereAdjPos,
		m_CollSphereRadius );
}

// �G�t�F�N�g�̕`��.
void CExplosion::EffectRender()
{
	// �G�t�F�N�g�̍Đ��I����Ă���ΏI��,
	if( m_IsEffectPlay == false ) return;
	// �G�t�F�N�g��`��.
	m_pEffect->SetScale( 1.0f );
	m_pEffect->Render();
}

// �����蔻��֐�.
void CExplosion::Collision( CActor* pActor )
{
	if( m_CollSphereRadius >= m_Param.SphereMaxRadius )return;
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

	// �Ώۂ̗̑͂����炷.
	pActor->LifeCalculation( [&]( float& life, bool& isAttack )
	{ 
		life -= m_Param.AttackPower; 
		isAttack = true;
	});
}

// ������W�̐ݒ�֐�.
void CExplosion::SetTargetPos( CActor& pActor )
{
	m_vPosition = pActor.GetPosition();	// ���W��ݒ�.
	// ���łɃG�t�F�N�g���Đ����Ă���ΏI��.
	if( m_IsEffectPlay == true ) return;
	m_pEffect->Play( m_vPosition );	// �G�t�F�N�g���Đ�.
	m_IsEffectPlay = true;
}

void CExplosion::SetPosition( const D3DXVECTOR3& vPos )
{
	m_vPosition = vPos;	// ���W��ݒ�.
	// ���łɃG�t�F�N�g���Đ����Ă���ΏI��.
	if( m_IsEffectPlay == true ) return;
	m_pEffect->Play( m_vPosition );	// �G�t�F�N�g���Đ�.
	m_IsEffectPlay = true;
}

// �~�܂��Ă��邩�ǂ���.
bool CExplosion::IsStop()
{
	if( m_CollSphereRadius < m_Param.SphereMaxRadius ) return false;
	if( m_pEffect->IsExists() == true ) return false;
	m_IsEffectPlay = false;
	return true;
}

// �G�t�F�N�g�̐ݒ�.
bool CExplosion::EffectSetting()
{
	if( m_pEffect->SetEffect( EFFECT_NAME ) == false ) return false;
	return true;
}

// �����蔻��̐ݒ�.
bool CExplosion::ColliderSetting()
{
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	m_pCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		m_Param.SphereAdjPos,
		0.0f );
	return true;
}