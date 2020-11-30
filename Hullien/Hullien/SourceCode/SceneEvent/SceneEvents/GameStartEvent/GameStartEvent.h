#ifndef GAMESTART_EVENT_H
#define GAMESTART_EVENT_H

#include "..\..\EventBase\EventBase.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventCharacter.h"
#include "..\..\..\Camera\EvevtCamera\EvevtCamera.h"

class CGroundStage;				// �n�ʃN���X.
class CSpawnUFO;				// �C�x���g�pUFO�N���X.
class CEventPlayer;				// �C�x���g�p�v���C���[�N���X.
class CEventGirl;				// �C�x���g�p���̎q�N���X.
class CEventAlienA;				// �C�x���g�p�F���lA�N���X.
class CBarrier;					// �o���A�N���X.
class CMotherShipUFO;			// �}�U�[�V�b�v�N���X.
class CGameStartEventWidget;	// �C�x���g�pUI�N���X.
class CEventCamera;				// �C�x���g�p�J�����N���X.
class CEventManager;			// �C�x���g�Ǘ��N���X.
class CSkyDome;					// �w�i.

/***********************************
*	�X�^�[�g�C�x���g�N���X.
**/
class CGameStartEvent : public CEventBase
{
	const D3DXVECTOR3 SCALE_MAX = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	//�傫���ő�l.
	const float TWO			= 2.0f;	// ��.
	const int   WAIT_COUNT	= 200;	// �ҋ@����.

	// �J�����֌W.
	const D3DXVECTOR3 CAMERA_INITPOSITION = D3DXVECTOR3(-23.0f, 5.5f, 20.0f);			// �J���������ʒu.
	const D3DXVECTOR3 CAMERA_POSITION_MOVEUFO = D3DXVECTOR3(-3.5f, 7.0f, -10.5f);		// MoveUFO�̎��̃J�����ʒu.
	const D3DXVECTOR3 CAMERA_POSITION_APP_ALIEN = D3DXVECTOR3(0.0f, 8.5f, 15.0f);		// �F���l�o�ꎞ�̃J�����ʒu.
	const D3DXVECTOR3 CAMERA_POSITION_CAUGHT_GIRL = D3DXVECTOR3(20.0f, 8.0f, 10.0f);	// ���̎q�ߊl���̃J�����ʒu.
	const D3DXVECTOR3 CAMERA_POSITION_ORDER_BARRIER = D3DXVECTOR3(6.5f, 8.4f, 10.0f);	// �o���A�������̃J�����ʒu.
	const D3DXVECTOR3 CAMERA_POSITION_PLAYER_UP		= D3DXVECTOR3(0.0f, 8.0f, -7.0f);	// �v���C���[�̃A�b�v���̃J�����ʒu.
	const D3DXVECTOR3 CAMERA_GAMEPOSITION			= D3DXVECTOR3(0.0f, 12.0f, 29.0f);	// ���̎q�̈ʒu.

	const float AMPLITUDE_COUNT						= 50.0f;							// �J�����̗h��p�J�E���g.
	const float CAMERA_CORRECTION_PLAYERPOS_Y		= 8.0f;							// �v���C���[�ɑ΂���J����y���W�␳�l.
	const float CAMERA_CORRECTION_UFOPOS_Y			= 7.0f;								// UFO�ɑ΂���J����y���W�␳�l.
	const float CAMERA_CORRECTION_UFOPOS_Z			= 5.0f;								// UFO�ɑ΂���J����z���W�␳�l.
	const float CAMERA_CORRECTIONALIENPOS_Y			= 4.0f;								// �F���l�ɑ΂���J����y���W�␳�l.
	const float CAMERA_CORRECTION_PLAYERLOOK_Z		= 1.5f;								// �v���C���[�ɑ΂���J�����̒����ʒuz���W�␳�l.
	const float CAMERASWITCHING_POS_Z				= -40.0f;							// �J�����؂�ւ�z���W,
	const float CAMERA_ROTAION_MOVING_LIMIT_X = static_cast<float>(D3DXToRadian(-150)); // �J������x���W��]�l�̌��E.
	const float CAMERA_ROTAION_DEC_START = static_cast<float>(D3DXToRadian(-100));		// �J�����̌����J�n�l.
	const float CAMERA_DECELERATION_SPEED			= 0.001f;							// �J�����̌����x.
	const float CAMERA_ROTATION_SPEED				= 0.01f;							// �J�����̉�]���x.
	const float CAMERA_MOVE_SPEED					= 0.1f;								// �J�����ړ����x.
	const float CAMERA_MOVE_SPEED_Y					= 0.2f;								// �J�����ړ����x.
	const float CAMERA_MOVE_SPEED_Z					= 1.0f;								// �J�����ړ����x.
	const float CAMERA_LENGHT_Z						= 7.2f;								// �J�����ƃv���C���[�̋���z���W.
	const float CAMERA_LOOKPOS_Z_PLAYER_UP			= -9.5f;							// �v���C���[�A�b�v���̃J�����̒����ʒu.
	const float FREQUENCY_LOOKPOS					= 7.0f;								// �J���������ʒu�̎��g��.
	const float AMPLITUDE_LOOKPOS_X					= 0.008f;							// �J���������ʒu�̐U��.
	const float AMPLITUDE_LOOKPOS_Y					= 0.01f;							// �J���������ʒu�̐U��.
	const float AMPLITUDE_SPEED						= 0.5f;								// �J���������ʒu�̗h�ꑬ�x.

	const float VIEWING_ANGLE_PLAYER_UP				= 1.6f;								// �v���C���[�̃A�b�v���̎���p.
	const float VIEWING_ANGLE_MOVING_LIMIT			= 0.7f;								// ����p�̈ړ����E�l.
	const float VIEWING_ANGLE_DEC_START_POS			= 1.2f;								// ����p�̌����J�n�ʒu.
	const float VIEWING_ANGLE_MOVE_SPEED			= 0.03f;							// ����p�̈ړ����x.
	const float VIEWING_ANGLE_DECELERATION_SPEED	= 0.0008f;							// ����p�̌����x.

	// UFO�֌W.
	const D3DXVECTOR3 UFO_INITPOSITION	= D3DXVECTOR3(0.0f, 10.0f, -120.0f);		// UFO�����ʒu.
	const D3DXVECTOR3 UFO_POSITION		= D3DXVECTOR3(0.0f, 10.0f, -100.0f);		// UFO�����ʒu.
	const float UFO_STOP_COUNT			= 80.0f;									// UFO��~�̃J�E���g.
	const float UFO_MOVE_SPEED			= 0.3f;										// UFO�̈ړ����x.
	const float UFO_STOP_DECELERATION_Y	= 0.001f;									// UFO�̒�~�����x.
	const float UFO_STOP_SPEED			= 0.01f;									// UFO��~���x.
	const float UFO_MOVEING_LIMIT_Y		= 11.5f;									// UFO�̈ړ����E�ly���W.
	const float UFO_MOVEING_LIMIT_Z		= 30.0f;									// UFO�̈ړ����E�lz���W.
	const float FREQUENCY_UFO_STOP		= 100.0f;									// UFO��~�̎��g��.
	const float AMPLITUDE_UFO_STOP		= 0.1f;										// UFO��~�̐U��.

	// �v���C���[�֌W.
	const float PLAYER_INITPOSITION_Z	= -150.0f;									// �v���C���[�����ʒu.
	const float RUN_SPEED				= 0.2f;										//���鑬�x.
	const float PLAYER_ROTATION_Y = static_cast<float>(D3DXToRadian(0));			// �v���C���[��y���W��]�l.
	const float PLAYER_DEFAULT_ROTATION_Y = static_cast<float>(D3DXToRadian(180));	// �v���C���[��y���W��]�l.

	// ���̎q�֌W.
	const float GIRL_DISTANCE_Z = -4.5f;											// �v���C���[�Ə��̎q�̊Ԃ̋���.
	const float CAMERASWITCHING_GIRLPOS_Z = -20.0f;									// �J�����؂�ւ��p�̏��̎q��z���W,
	const float PRESERVE_GIRL_DISP_POSITION = -15.0f;								// ���̎q�����w���̕\���ʒu.
	const float GIRL_DEFAULT_ROTATION_Y = static_cast<float>(D3DXToRadian(180));	// ���̎q��y���W��]�l.

	// �F���l�֌W.
	const float ALIEN_INITROTATION_Y = static_cast<float>(D3DXToRadian(0));		// �F���l��y���W��]�l.
	const float ALIEN_MOVEING_LIMIT_Y = 0.0f;										// �F���l�̈ړ����E�ly���W.
	const float ALIEN_MOVEING_LIMIT_Z = -50.0f;										// �F���l�̈ړ����E�lz���W.
	const float ALIEN_FALL_SPEED = 0.05f;											// �F���l�̍~�����x.
	const float ALIEN_MOVE_SPEED = 0.3f;											// �F���l�̈ړ����x.
	const float ALIEN_RUN_SPEED = 0.5f;												// �F���l�̈ړ����x.
	const float FREQUENCY_ALIEN_UPDOWN = 90.0f;										// �F���l�̎��g��.
	const float AMPLITUDE_ALIEN_UPDOWN = 0.01f;										// �F���l�̐U��.
	const float GET_EXPELLED_SPEED = 7.0f;											// �o���A����͂����o����鑬�x.

private:
	// �C�x���g�̊e�X�e�b�v.
	enum class enEventStep
	{
		FootUp = 0,					// ���̃A�b�v.
		Escape_PlayerAndGirl,		// ������v���C���[�Ə��̎q.
		Viewpoint_UFO,				// UFO�̎��_.
		Move_UFO,					// UFO��ʒu�܂ňړ�.
		Stop_UFO,					// UFO��~.
		Appearance_Alien,			// �F���l�o��.
		Move_Alien,					// �F���l�O�i.
		GetCaught_Girl,				// ���̎q���߂܂�.
		InvocatingOrder_Barrier,	// �o���A��������.
		Player_Up,					// �v���C���[�̃A�b�v.
		Invocating_Barrier,			// �o���A����.
		Return_Girl,				// ���̎q�A��.
		Disp_Preserve_Girl,			// ���̎q�����w���̕\��.
		GameStart,					// �Q�[���J�n.

		Max,
		EventStart = FootUp,
		Skip = InvocatingOrder_Barrier -1,
	} typedef EEventStep;

public:
	CGameStartEvent();
	virtual ~CGameStartEvent();

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
	// �v���C���[�������֐�.
	bool PlayerInit();
	// ���̎q�������֐�.
	bool GirlInit();
	// �F���l�������֐�.
	bool AlienInit();
	// �}�U�[�V�b�v�������֐�.
	bool MotherShipUFOInit();
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

	// �ȉ��C�x���g�̃X�e�b�v.
	void FootUp();
	void EscapePlayerAndGirl();
	void ViewpointUFO();
	void MoveUFO();
	void StopUFO();
	void AppearanceAlien();
	void MoveAlien();
	void GetCaughtGirl();
	void InvocatingOrderBarrier();
	void PlayerUp();
	void InvocatingBarrier();
	void ReturnGirl();
	void DispPreserveGirl();
	void GameStart();

	// �ȉ��f�o�b�O�p.
	void DebugRender();
	void DebugOperation();

private:
	std::shared_ptr<CGroundStage>			m_pGroundStage;
	std::shared_ptr<CSpawnUFO>				m_pSpawnUFO;
	std::shared_ptr<CEventPlayer>			m_pPlayer;
	std::shared_ptr<CEventGirl>				m_pGirl;
	std::shared_ptr<CEventAlienA>			m_pAlienA;
	std::shared_ptr<CBarrier>				m_pBarrier;
	std::shared_ptr<CMotherShipUFO>			m_pMotherShipUFO;
	std::unique_ptr<CGameStartEventWidget>	m_pWidget;
	std::shared_ptr<CEventCamera>			m_pEventCamera;
	std::shared_ptr<CEventManager>			m_pEventManager;
	std::shared_ptr<CSkyDome>				m_pSkyDome;
	D3DXVECTOR3								m_vUFOPosition;
	EEventStep								m_EventStep;
	int										m_NowStep;
	float									m_Speed;
	float									m_DecelerationY;	// y���W�����x.
	float									m_DecelerationZ;	// z���W�����x.
	float									m_Count;			// �J�E���g.

	CEventCharacter::SOptionalState			m_stPlayer;			//�v���C���[�̏��.
	CEventCharacter::SOptionalState			m_stGirl;			//���̎q�̏��.
	CEventCharacter::SOptionalState			m_stAlien;			//�F���l�̏��.
	CEventCamera::SCameraState				m_stCamera;			//�J�����̏��.
};

#endif //#ifndef START_EVENT_H.