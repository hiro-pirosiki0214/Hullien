#include "Arm.h"
#include "..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\XAudio2\SoundManager.h"	

CArm::CArm()
	: m_pSkinMesh		( nullptr )
	, m_pAC				( nullptr )
	, m_GrabPosition	( 0.0f, 0.0f, 0.0f )
	, m_NowArmState		( EArmState::Start )
	, m_OldArmState		( EArmState::Start )
	, m_AppearanceCount	( 0.0f )
	, m_ScalingValue	( SCALING_VALUE )
	, m_AnimSpeed		( 0.0 )
{
	m_vScale = { 0.0f, 0.0f, 0.0f };
}

CArm::~CArm()
{
}

// �������֐�.
bool CArm::Init()
{
	if( CMeshResorce::GetSkin( m_pSkinMesh, MODEL_NAME ) == false ) return false;
	// �A�j���[�V�����R���g���[���[�̃N���[���쐬.
	if( FAILED( m_pSkinMesh->GetAnimController()->CloneAnimationController(
		m_pSkinMesh->GetAnimationController()->GetMaxNumAnimationOutputs(),
		m_pSkinMesh->GetAnimationController()->GetMaxNumAnimationSets(),
		m_pSkinMesh->GetAnimationController()->GetMaxNumTracks(),
		m_pSkinMesh->GetAnimationController()->GetMaxNumEvents(),
		&m_pAC ) )) return false;
	m_pSkinMesh->ChangeAnimSet( 0, m_pAC );
	m_AnimEndFrame = m_pSkinMesh->GetAnimPeriod(0)-0.01;	// �I���t���[���̎擾(+1.0)����.
	return true;
}

// �X�V�֐�.
void CArm::Update()
{
	switch( m_NowArmState )
	{
	case EArmState::Appearance:
		// �o��.
		Appearance();
		break;
	case EArmState::Grab:
		// �͂�.
		Grab();
		break;
	case EArmState::CleanUp:
		// �ЂÂ���.
		CleanUp();
		break;
	case EArmState::End:
		// �͂ގ��̍��W���v�Z.
		m_GrabPosition.x = m_vPosition.x - sinf( m_vRotation.y ) * GRAB_DISTANCE;
		m_GrabPosition.z = m_vPosition.z - cosf( m_vRotation.y ) * GRAB_DISTANCE;
		m_GrabPosition.y = m_vPosition.y;
		break;
	default:
		break;
	}
}

// �`��֐�.
void CArm::Render()
{
	if( m_pSkinMesh == nullptr ) return;

	m_vRotation.y += 0.001f;

	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vScale );
	m_pSkinMesh->SetAnimSpeed( m_AnimSpeed );
	m_pSkinMesh->Render( m_pAC );
}

// �͂�ł�����W�̎擾.
D3DXVECTOR3 CArm::GetGrabPosition()
{
	m_GrabPosition.x = m_vPosition.x - sinf( m_vRotation.y ) * GRAB_DISTANCE;
	m_GrabPosition.z = m_vPosition.z - cosf( m_vRotation.y ) * GRAB_DISTANCE;
	m_GrabPosition.y = m_vPosition.y;
	return m_GrabPosition;
}

// �o������p��.
void CArm::SetAppearancePreparation()
{
	if( m_NowArmState == EArmState::Appearance ) return;
	if( m_NowArmState == EArmState::Grab ) return;
	m_OldArmState = m_NowArmState;
	m_NowArmState = EArmState::Appearance;	// �o�������Ԃ�.
	if( m_OldArmState == EArmState::CleanUp ) return;
	// ������.
	m_AppearanceCount	= 0.0f;
	m_ScalingValue		= SCALING_VALUE;
	m_pAC->ResetTime();
	m_pSkinMesh->ChangeAnimSet_StartPos( 0, 0.0, m_pAC );
	CSoundManager::PlaySE("Arm");
}

// �ЂÂ���p��.
void CArm::SetCleanUpPreparation()
{
	if( m_NowArmState == EArmState::CleanUp ) return;
	if( m_NowArmState == EArmState::Appearance ) return;
	if( m_NowArmState == EArmState::Grab ) return;
	m_OldArmState = m_NowArmState;
	m_NowArmState = EArmState::CleanUp;	// �ЂÂ����Ԃ�.
	if( m_OldArmState == EArmState::Appearance ) return;
	// ������.
	m_AppearanceCount	= APPEARANCE_COUNT_MAX;
	m_pAC->ResetTime();
	m_pSkinMesh->ChangeAnimSet_StartPos( 0, 0.0, m_pAC );
}

// ���S�Ɏ��o��
void CArm::SetAppearance()
{
	m_vScale = { 1.0f, 1.0f, 1.0f };
	m_pSkinMesh->ChangeAnimSet_StartPos( 0, m_AnimEndFrame, m_pAC );
	m_NowArmState  = EArmState::End;	// �I��.
}

// ���S�ɕЂÂ���.
void CArm::SetCleanUp()
{
	m_vScale = { 0.0f, 0.0f, 0.0f };
	m_pSkinMesh->ChangeAnimSet_StartPos( 0, 0.0, m_pAC );
	m_AnimSpeed			= 0.0;				// �A�j���[�V�������x�ύX.
	m_NowArmState		= EArmState::Start;	// ���̏�Ԃֈړ�.
}

// �o��.
void CArm::Appearance()
{
	m_AppearanceCount += APPEARANCE_ADD_VALUE;	// �o���J�E���g�̉��Z.
	if( m_AppearanceCount >= SCALING_VALUE ) m_ScalingValue += APPEARANCE_ADD_VALUE;	// �g�k�l�����Z.
	
	// �o���J�E���g���ő�l�𒴂�����.
	if( m_AppearanceCount >= APPEARANCE_COUNT_MAX ){
		// �e�l�������傫���Ȃ�Ȃ��悤�ɂ���.
		m_AppearanceCount	= APPEARANCE_COUNT_MAX;
		m_ScalingValue		= SCALING_VALUE_MAX;
		m_AnimSpeed			= 0.01;				// �A�j���[�V�������x�ύX.
		CSoundManager::PlaySE("ArmGrab");		// ���񂾎���SE�Đ�.
		m_NowArmState		= EArmState::Grab;	// ���̏�Ԃֈړ�.
	}
	if( m_ScalingValue >= SCALING_VALUE_MAX ) m_ScalingValue = SCALING_VALUE_MAX;
	// �X�P�[���l���v�Z.
	const float scale = fabsf(sinf( static_cast<float>(D3DX_PI)*0.5f * m_AppearanceCount ));
	m_vScale = { scale * m_ScalingValue, scale*m_ScalingValue, scale };
}

// �͂�.
void CArm::Grab()
{
	if( m_pAC->GetTime() <= m_AnimEndFrame ) return;
	m_AnimSpeed = 0.0;
	m_NowArmState  = EArmState::End;	// �I��.
}

// �ЂÂ���.
void CArm::CleanUp()
{
	m_AppearanceCount -= CLEAN_UP_SUB_VALUE;	// �o���J�E���g�̉��Z.

	// �o���J�E���g���ŏ��l�𒴂�����.
	if( m_AppearanceCount <= CLEAN_UP_COUNT_MIN ){
		// �e�l�������傫���Ȃ�Ȃ��悤�ɂ���.
		m_AppearanceCount	= CLEAN_UP_COUNT_MIN;
		m_AnimSpeed			= 0.0;				// �A�j���[�V�������x�ύX.
		m_NowArmState		= EArmState::Start;	// ���̏�Ԃֈړ�.
	}
	// �X�P�[���l���v�Z.
	const float scale = fabsf(sinf( static_cast<float>(D3DX_PI)*0.5f * m_AppearanceCount ));
	m_vScale = { scale, scale, scale };
}