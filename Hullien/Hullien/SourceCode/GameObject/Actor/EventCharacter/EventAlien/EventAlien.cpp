#include "EventAlien.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\XAudio2\SoundManager.h"	
#include "..\..\..\Arm\Arm.h"

/****************************************
*	�C�x���g�p�F���l�N���X.
**/
CEventAlien::CEventAlien()
	: m_pArm					( nullptr )
	, m_pAbductUFOPosition		( nullptr )
	, m_NowState				( EEventAlienState::None )
	, m_vTargetPosition			( D3DXVECTOR3(0.0f, 0.0f, 0.0f) )
	, m_IsBarrierHit			( false )
	, m_Speed					( 0.0f )
{
	m_AnimFrameList.resize( EAnimNo_Max );
}


CEventAlien::~CEventAlien()
{
}

// ������W�̐ݒ�.
void CEventAlien::SetTargetPos(CActor & actor)
{
	SetGirlPos(actor);
}

bool CEventAlien::IsGrab() const
{
	return m_pArm->IsGrab();
}

// ���݂̏�Ԃ̍X�V�֐�.
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

// ���̎q�̍��W��ݒ�.
void CEventAlien::SetGirlPos(CActor& actor)
{
	// ���̎q����Ȃ���ΏI��.
	if (actor.GetObjectTag() != EObjectTag::Girl) return;
	m_vTargetPosition = actor.GetPosition();	// ���̎q�̍��W���擾.

	// �ړI�̉�]�����擾.
	float TargetRotationY = atan2f(
		m_vTargetPosition.x - m_vPosition.x,
		m_vTargetPosition.z - m_vPosition.z);

	// �ړ��p�x�N�g�����擾.
	m_vTargetPosition.x -= sinf(TargetRotationY) * CArm::GRAB_DISTANCE;
	m_vTargetPosition.z -= cosf(TargetRotationY) * CArm::GRAB_DISTANCE;
}


// �X�|�[����.
void CEventAlien::Spawning()
{
	// ���f���̃X�P�[���l�𑫂��Ă���.
	if (m_vScale.x >= MODEL_SCALE_MAX) return;
	m_vScale.x += m_Parameter.ScaleSpeed;
	m_vScale.y += m_Parameter.ScaleSpeed;
	m_vScale.z += m_Parameter.ScaleSpeed;
	if (m_vScale.x > MODEL_SCALE_MAX)
	{
		m_vScale.x = MODEL_SCALE_MAX;
		m_vScale.y = MODEL_SCALE_MAX;
		m_vScale.z = MODEL_SCALE_MAX;
		m_NowState = EEventAlienState::Move;
	}

	// ���f���̃A���t�@�l�𑫂��Ă���.
	if (m_Parameter.ModelAlpha >= MODEL_ALPHA_MAX) return;
	m_Parameter.ModelAlpha += m_Parameter.AlphaSpeed;
}

// ���̎q�Ƃ̓����蔻��.
void CEventAlien::GirlCollision(CActor* pActor)
{
	// �I�u�W�F�N�g�̃^�O�����̎q����Ȃ���ΏI��.
	if (pActor->GetObjectTag() != EObjectTag::Girl) return;
	if (m_IsBarrierHit == true) return;
	if (m_NowState == EEventAlienState::Spawn)		return;	// �X�|�[����ԂȂ�I��.

	bool isAbduct = false;
	if (m_NowState == EEventAlienState::Abduct) {
		isAbduct = true;
		pActor->SetPosition( {m_pArm->GetGrabPosition().x, m_pArm->GetGrabPosition().y-5.5f, m_pArm->GetGrabPosition().z} );
		return;
	}
	else {
		isAbduct = true;
	}

	if (isAbduct == false) return;
	// ���̂̓����蔻��.
	if (m_pCollManager->IsShereToShere(pActor->GetCollManager()) == false) return;

	if (m_pArm->IsGrab() == false) {
		m_pArm->SetAppearancePreparation();
		return;
	}

	if (m_NowState == EEventAlienState::Abduct) return;
	m_NowState = EEventAlienState::Abduct;
}

// �o���A�Ƃ̓����蔻��.
void CEventAlien::BarrierCollision(CActor* pActor)
{
	// �I�u�W�F�N�g�̃^�O�����̎q����Ȃ���ΏI��.
	if (pActor->GetObjectTag() != EObjectTag::Bariier) return;
	const float moveSpeed = -2.0f;
	// ���̂̓����蔻��.
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