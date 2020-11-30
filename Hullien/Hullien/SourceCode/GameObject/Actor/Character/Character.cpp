#include "Character.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\XAudio2\SoundManager.h"

CCharacter::CCharacter()
	: m_pSkinMesh				( nullptr )
	, m_AnimFrameList			()
	, m_NowAnimNo				( 0 )
	, m_OldAnimNo				( 0 )
	, m_AnimSpeed				( DEFAULT_ANIM_SPEED )
	, m_MoveVector				( 0.0f, 0.0f, 0.0f )
	, m_InvincibleCount			( 0 )
	, m_HasFinishedParamSetting	( false )
	, m_pFootCollision			()
	, m_pGroundCollision		( nullptr )
	, m_vGroundPosition			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, m_vRightPosition			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, m_vLeftPosition			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
{
}

CCharacter::~CCharacter()
{
}

// ���b�V���̕\��.
void CCharacter::MeshRender()
{
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vScale );
	m_pSkinMesh->SetAnimSpeed( m_AnimSpeed );
	m_pSkinMesh->Render();
}

// ���G���Ԃ��ǂ���.
bool CCharacter::IsInvincibleTime( const int& invincibleTime )
{
	return m_InvincibleCount > invincibleTime * FPS;
}

// ���f���̎擾.
bool CCharacter::GetModel( const char* modelName )
{
	// ���ɓǂݍ��߂Ă�����I��.
	if( m_pSkinMesh != nullptr ) return true;
	// ���f���̎擾.
	CMeshResorce::GetSkin( m_pSkinMesh, modelName );
	// ���f�����ǂݍ��߂ĂȂ���� false.
	if( m_pSkinMesh == nullptr ) return false;
	return true;
}

// �ړI�̍��W�։�].
bool CCharacter::TargetRotation( const D3DXVECTOR3& targetVec, const float& rotSpeed, const float& ToleranceRadian )
{
	// ���g�̃x�N�g����p��.
	const D3DXVECTOR3 myVector = { sinf(m_vRotation.y), 0.0f, cosf(m_vRotation.y) };

	// �x�N�g���̒��������߂�.
	const float myLenght = sqrtf(myVector.x*myVector.x + myVector.z*myVector.z);
	const float targetLenght = sqrtf(targetVec.x*targetVec.x + targetVec.z*targetVec.z);

	// ���ς����߂�.
	float dot = myVector.x*targetVec.x + myVector.z*targetVec.z;
	dot = acosf( ( myLenght * targetLenght ) * dot );

	if( ( -ToleranceRadian < dot && dot < ToleranceRadian ) ||	// ���ς����e�͈͂Ȃ�.
		( std::isfinite( dot ) ) == false ){					// ���ς̒l���v�Z�ł��Ȃ��l�Ȃ�.
		return true;	// ��]�I��.
	} else {
		// �ړI�̃x�N�g���ƁA�����̃x�N�g���̊O�ς����߂�.
		const float cross = myVector.x*targetVec.z - myVector.z*targetVec.x;
		// �O�ς�0.0��菭�Ȃ���� ���v��� : �����v��� �ɉ�]����.
		m_vRotation.y += cross < 0.0f ? rotSpeed : -rotSpeed;

		return false;	// ��]��.
	}
}

// �A�j���[�V�����ݒ�.
void CCharacter::SetAnimation( const int& animNo, LPD3DXANIMATIONCONTROLLER	pAc )
{
	if( m_pSkinMesh == nullptr ) return;
	if( m_NowAnimNo == animNo ) return;
	m_OldAnimNo = m_NowAnimNo;
	m_NowAnimNo = animNo;
	m_pSkinMesh->ChangeAnimSet( m_NowAnimNo, pAc );
	m_AnimFrameList.at(animNo).NowFrame = 0.0;
}

// �A�j���[�V�������u�����h���Đݒ�.
void CCharacter::SetAnimationBlend( const int& animNo )
{
	if( m_pSkinMesh == nullptr ) return;
	if( m_NowAnimNo == animNo ) return;
	m_OldAnimNo = m_NowAnimNo;
	m_NowAnimNo = animNo;
	m_pSkinMesh->ChangeAnimBlend( m_NowAnimNo, m_OldAnimNo );
	m_AnimFrameList.at(animNo).NowFrame = 0.0;
}

// ����.
void CCharacter::FootStep(const char * rightfoot, const char * leftfoot)
{
	m_vGroundPosition = m_vPosition;
	m_pSkinMesh->GetPosFromBone(leftfoot, &m_vLeftPosition);
	m_pSkinMesh->GetPosFromBone(rightfoot, &m_vRightPosition);

	if (m_pFootCollision[0]->IsShereToShere(m_pGroundCollision.get()) == true
		|| m_pFootCollision[1]->IsShereToShere(m_pGroundCollision.get()) == true)
	{
		CSoundManager::NoMultipleSEPlay("Walk");
	}

	m_pGroundCollision->DebugRender();
	m_pFootCollision[0]->DebugRender();
	m_pFootCollision[1]->DebugRender();
}

// �����p�����蔻��̐ݒ�.
bool CCharacter::FootStepCollisionSetting()
{
	// �n�ʂ̓����蔻��.
	if (m_pGroundCollision != nullptr) return true;
	if (m_pGroundCollision == nullptr)
	{
		m_pGroundCollision = std::make_shared<CCollisionManager>();
	}
	if (FAILED(m_pGroundCollision->InitSphere(
		&m_vGroundPosition,
		&m_vRotation,
		&m_vScale.x,
		{0.0f,0.0f,0.0f},
		1.0f))) return false;

	// ���̓����蔻��.
	if (m_pFootCollision.size() != 0) return true;
	m_pFootCollision.emplace_back(std::make_shared<CCollisionManager>());
	m_pFootCollision.emplace_back(std::make_shared<CCollisionManager>());
	if (FAILED(m_pFootCollision[0]->InitSphere(
		&m_vRightPosition,
		&m_vRotation,
		&m_vScale.x,
		{ 0.0f,0.0f,0.0f },
		0.5f))) return false;
	if (FAILED(m_pFootCollision[1]->InitSphere(
		&m_vLeftPosition,
		&m_vRotation,
		&m_vScale.x,
		{ 0.0f,0.0f,0.0f },
		0.5f))) return false;
	return true;
}
