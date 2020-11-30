#ifndef GAMEOVER_EVENT_H
#define GAMEOVER_EVENT_H

#include "..\..\EventBase\EventBase.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventCharacter.h"
#include "..\..\..\Camera\EvevtCamera\EvevtCamera.h"

class CGroundStage;				// �n�ʃN���X.
class CMotherShipUFO;			// �C�x���g�pUFO�N���X.
class CEventGirl;				// �C�x���g�p���̎q�N���X.
class CEventCamera;				// �C�x���g�p�J�����N���X.
class CEventManager;			// �C�x���g�Ǘ��N���X.
class CEventWidget;				// �C�x���g��UI�N���X.
class CSkyDome;					// �w�i.

/************************************
*	�Q�[���I�[�o�[�C�x���g�N���X.
**/
class CGameOverEvent : public CEventBase
{
	const int ONE				= 1;	//��.
	const int WAITCOUNT_WAIT	= 150;	//�ҋ@���̑ҋ@����.
	const int WAITCOUNT_DEFAULT	= 30;	//�W���ҋ@����.
	const float SCALE_MAX		= 1.0f;	// �傫���̍ő�.

	// �J����.
	const D3DXVECTOR3 CAMERA_INITPOSITION	= D3DXVECTOR3(0.3f, 5.0f, -30.0f);	// �J���������ʒu.
	const float LOOKPOS_Y			= 6.0f;										// �����ʒu.
	const float CAMERA_RISE_SPEED	= 0.027f;									//�J�����̏㏸���x.

	// UFO.
	const D3DXVECTOR3 UFO_INITPOSITION			= D3DXVECTOR3(0.0f, 20.0f, 0.0f);
	const D3DXVECTOR3 DESTINATION_RUGHTFIRST	= D3DXVECTOR3(20.0f, 10.0f, 10.0f);		// �E�ړ�Part1���̖ړI�n.
	const D3DXVECTOR3 DESTINATION_LEFT			= D3DXVECTOR3(-35.0f, 25.0f, 50.0f);	// ���ړ����̖ړI�n.
	const D3DXVECTOR3 DESTINATION_RUGHTSECOND	= D3DXVECTOR3(50.0f, 50.0f, 100.0f);	// �E�ړ�Part2���̖ړI�n.
	const D3DXVECTOR3 DESTINATION_BACK			= D3DXVECTOR3(0.0f, 200.0f, 500.0f);	// ���ړ����̖ړI�n.
	const float CORRECTION_UFOPOSITION_Y		= 3.0f;									// UFO�ʒu�␳.
	const float UFO_MOVE_SPEED_RUGHTFIRST		= 0.5f;									// �E�ړ�Part1����UFO�ړ����x.
	const float UFO_MOVE_SPEED_LEFT				= 1.0f;									// ���ړ�����UFO�ړ����x.
	const float UFO_MOVE_SPEED_RUGHTSECOND		= 2.5f;									// �E�ړ�Part2����UFO�ړ����x.
	const float UFO_MOVE_SPEED_BACK				= 8.0f;									// ���ړ�����UFO�ړ����x.

	// ���̎q.
	const D3DXVECTOR3 GIRL_INITPOSITION = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	const float GIRL_RISE_SPEED			= 0.06f;								//���̎q�̏㏸���x.
	const float GIRL_SCALEDOWN_SPEED	= 0.005f;								//���̎q�̏k�����x.
	const float GIRL_SCALEDOWN_STARTPOS	= 7.0f;									//���̎q�̏k���J�n�ʒu.

private:
	// �C�x���g�X�e�b�v.
	enum class enEventStep
	{
		Wait = 0,				// �ҋ@.
		Sucked_Girl,			// ���̎q���z�����܂��.
		MoveRight_UFO_First,	// UFO�E�ړ�Part1.
		MoveLeft_UFO,			// UFO���ړ�.
		MoveRight_UFO_Second,	// UFO�E�ړ�Part2.
		Move_Back_UFO,			// UFO���Ɉړ�.
		EventEnd,				// �C�x���g�I��.

		Start = Wait,
		Skip = EventEnd - 1,	
	}typedef EEventStep;

public:
	CGameOverEvent();
	virtual ~CGameOverEvent();

	// �Ǎ��֐�.
	virtual bool Load() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �X�v���C�g�`��֐�.
	virtual void SpriteRender() override;

private:
	// �J�����������֐�.
	bool CameraInit();
	// UFO�������֐�.
	bool SpawnUFOInit();
	// ���̎q�������֐�.
	bool GirlInit();
	// �A�N�^�̍X�V�֐�.
	void ActorUpdate();
	// �J�����̍X�V�֐�.
	void CameraUpdate();

	// �V�[���̐ݒ�.
	void SceneSetting();
	// ���̃V�[���ɐi�߂�.
	virtual void NextStep() override;
	// �X�L�b�v.
	virtual void Skip() override;
	
	// �ȉ��C�x���g�X�e�b�v.
	void Wait();				// �ҋ@.
	void SuckedGirl();			// ���̎q���z�����܂��.
	void MoveRightUFOFirst();	// UFO�̉E�ړ�Part1.
	void MoveLeftUFO();			// UFO�̍��ړ�.
	void MoveRightUFOSecond();	// UFO�̉E�ړ�Part1.
	void MoveBackUFO();			// UFO���Ɉړ�.
	void EventEnd();			// �C�x���g�I��.

	// �ȉ��f�o�b�O�p.
	void DebugRender();
	void DebugOperation();

private:
	std::shared_ptr<CGroundStage>			m_pGroundStage;
	std::shared_ptr<CMotherShipUFO>			m_pUFO;
	std::shared_ptr<CEventGirl>				m_pGirl;
	std::shared_ptr<CEventCamera>			m_pEventCamera;
	std::shared_ptr<CEventWidget>			m_pEventWidget;
	std::shared_ptr<CSkyDome>				m_pSkyDome;
	CEventCharacter::SOptionalState			m_stGirl;			// ���̎q�̏��.
	CEventCamera::SCameraState				m_stCamera;			// �J�����̏��.
	D3DXVECTOR3								m_vUFOPosition;		// UFO�̈ʒu.
	D3DXVECTOR3								m_vUFOScale;		// UFO�̑傫��.
	EEventStep								m_EventStep;		// �C�x���g�X�e�b�v.
	int										m_WaitCount;		// �ҋ@�J�E���g.
	float									m_MoveCount;		// �ړ��J�E���g.

private:
	float m_Speed;
};

#endif	//#ifndef GAMEOVER_EVENT_H.