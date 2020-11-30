#ifndef GAMECLEAR_EVENT_H
#define GAMECLEAR_EVENT_H

#include "..\..\EventBase\EventBase.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventCharacter.h"
#include "..\..\..\Camera\EvevtCamera\EvevtCamera.h"

class CGroundStage;				// �n�ʃN���X.
class CSpawnUFO;				// �C�x���g�pUFO�N���X.
class CEventPlayer;				// �C�x���g�p�v���C���[�N���X.
class CEventGirl;				// �C�x���g�p���̎q�N���X.
class CEventAlienA;				// �C�x���g�p�F���lA�N���X.
class CEventCamera;				// �C�x���g�p�J�����N���X.
class CEventManager;			// �C�x���g�Ǘ��N���X.
class CEventWidget;				// �C�x���g��UI�N���X.
class CSkyDome;					// �w�i.


/************************************
*	�N���A�C�x���g�N���X.
**/
class CGameClearEvent : public CEventBase
{
	const D3DXVECTOR3 SCALE_DEFAULT				= D3DXVECTOR3(1.0f, 1.0f, 1.0f);		//�W���g��l.
	const float TWO								= 2.0f;		// ��.
	const float MOVE_SPEED						= 0.2f;		// �ړ����x.
	const float SCALEDOWN_SPEED					= 0.01f;	// �k�����x.
	const int WAIT_COUNT_FALL_ALIEN				= 150;		// �F���l�̗������̑ҋ@����.
	const int WAIT_COUNT_MOVE_UFO				= 100;		// UFO�ړ����̑ҋ@����.
	const int WAIT_COUNT_MOVE_UFO_MAX			= 350;		// UFO�ړ����̑ҋ@����.


	// �J����.
	const D3DXVECTOR3 INIT_CAMERAPOSITION		= D3DXVECTOR3(-27.0f, 7.5f, -65.5f);	// �J�����̏����ʒu.
	const D3DXVECTOR3 CAMERAPOSITION			= D3DXVECTOR3(-8.0f, 19.0f, -114.5f);	// �J�����̈ʒu.
	const float CORRECTION_PLAYER_LOOKPOS_Y		= 3.0f;									// �v���C���[�����ʒuy���W�␳.
	const float INIT_VIEWING_ANGLE				= 0.5f;									// ����p�̏����l.
	const float MOVE_LIMIT_Z_SUCKED_INTO_UFO	= 18.0f;								// UFO�ɋz�����܂��Ƃ��̈ړ����E.
	const float MOVESPEED_Z_SUCKED_INTO_UFO		= 0.15f;								// UFO�ɋz�����܂��Ƃ��̈ړ����x.
	const float VIEWING_ANGLE_MOVESPEED			= 0.01f;								// ����p�ړ����x.
	const float VIEWING_ANGLE_UP_UFO			= 0.34f;								// UFO�A�b�v���̎���p.
	const float CAMERA_LIMITPOS_Y_KICKEDOUT_ALIEN = 1.3f;	// �F���l���ǂ��o�����Ƃ��̃J�����̌��E�l.
	const float CAMERA_SPEED_Y_KICKEDOUT_ALIEN	= 0.3f;		// �F���l���ǂ��o�����Ƃ��̃J�����̈ړ����x.
	const float FREQUENCY_LOOKPOS_Y				= 10.0f;	// �J���������ʒu�̎��g��.
	const float AMPLITUDE_LOOKPOS				= 0.1f;		// �J���������ʒu�̐U��.
	const float LOOK_POSITION_Y_FALL_ALIEN		= 9.0f;		// �F���l�̗������̒����ʒu.
	const float LOOK_SPEED_Y_FALL_ALIEN			= 1.0f;		// �F���l�̗������̒����ʒu�ړ����x.

	// �v���C���[.
	const float INIT_PLAYERPOSITION_Z			= -30.0f;	//�v���C���[�̏���z���W.
	// ���̎q.
	const float INIT_GIRLPOSITION_Z				= 2.5f;	//���̎q�̏���z���W.

	// �F���l.
	const float INIT_ALIENPOSITION_Y			= 9.0f;	//�F���l�̏���y���W.
	const float ALIEN_ROTATION_Y				= static_cast<float>(D3DXToRadian(145)); // �F���l��y���W��]�l.
	const float ALIEN_FALL_POSITION_Y			= 7.0f;		//�F���l�̗����ʒu.
	const float ALIEN_FALL_SPEED_X				= 0.19f;	//�F���l�̗������x.
	const float ALIEN_FALL_SPEED_Y				= 0.4f;		//�F���l�̗������x.
	const float ALIEN_FALL_SPEED_Z				= 0.2f;		//�F���l�̗������x.
	const float ADD_GRAVITY						= 0.005f;	//�d�͂̉��Z�l.
	const float ALIEN_FALL_ROTATION				= static_cast<float>(D3DXToRadian(160));	//�F���l�̗������̉�]�l.
	const float ALIEN_FALL_ROTATION_SPEED		= static_cast<float>(D3DXToRadian(20));		//�F���l�̗������̉�]���x.

	// UFO.
	const D3DXVECTOR3 INIT_UFOPOSITION			= D3DXVECTOR3(0.0f, 10.0f, -100.0f);	// UFO�̏����ʒu.
	const int WAITCOUNT_UP_UFO					= 50;									// UFO�A�b�v���̑ҋ@����.
	const float UFO_SCALE_SPEED_DEFAULT			= 0.1f;		// UFO�̊g�呬�x�̕W��.
	const float UFO_SCALE_MIN					= 0.5f;		// UFO�̊g��l�ŏ�.
	const float UFO_SCALE_MAX_X					= 3.0f;		// UFO�̊g��l�ő�x���W.
	const float UFO_SCALE_MAX_Y					= 2.5f;		// UFO�̊g��l�ő�y���W.
	const float UFO_LIMIT_SCALE_Y				= 2.0f;		// UFO�̊g��l�ړ����E.
	const float UFO_SCALE_SPEED_X				= 0.01f;	// x���W�g�呬�x.
	const float UFO_SCALE_SPEEDMAX_X			= 0.05f;	// x���W�g�呬�x�ő�.
	const float UFO_SCALE_SPEED_Y				= 0.05f;	// y���W�g�呬�x.
	const float UFO_SCALE_SPEEDMAX_Y			= 0.08f;	// y���W�g�呬�x�ő�.
	const float UFO_SURFACING_SPEED				= 0.1f;		// UFO���㑬�x.
	const float UFO_SURFACING_POSITION_Y		= 25.0f;	// UFO����ʒuy���W.
	const float UFO_MOVE_SPEED_X				= 3.0f;		// UFO�ړ����x.
	const float UFO_MOVE_SPEED_Y				= 0.5f;		// UFO�ړ����x.

private:
	// �C�x���g�̃X�e�b�v.
	enum class enEventStep
	{
		RunTowards_UFO,		// UFO�Ɍ������đ���.
		SuckedInto_UFO,		// UFO�ɋz�����܂��.
		Up_UFO,				// UFO�̃A�b�v.
		Fuss_UFO,			// UFO�̑呛��.
		KickedOut_Alien,	// �F���l�ǂ��o�����.
		Fall_Alien,			// �F���l����.
		Move_UFO,			// UFO�̈ړ�.
		NextScene,			// ���̃V�[���Ɉړ�.

		Max,
		Start = RunTowards_UFO,
		Skip = NextScene - 1,

	}typedef EEventStep;

public:
	CGameClearEvent();
	virtual ~CGameClearEvent();

	// �ǂݍ��݊֐�.
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

	// �A�N�^�̏k��.
	void ScaleDownActor(D3DXVECTOR3& scale, const float& speed);

	// �ȉ��C�x���g�X�e�b�v�֐�.
	void RunTowardsUFO();		// UFO�Ɍ������đ���.
	void SuckedIntoUFO();		// UFO�ɋz�����܂��.
	void UpUFO();				// UFO�̃A�b�v.
	void FussUFO();				// UFO�̑呛��.
	void KickedOutAlien();		// �F���l�ǂ��o�����.
	void FallAlien();			// �F���l����.
	void MoveUFO();				// UFO�̈ړ�.
	void NextScene();			// ���̃V�[���Ɉړ�.


	// �ȉ��f�o�b�O�p.
	void DebugRender();
	void DebugOperation();

private:
	std::shared_ptr<CGroundStage>			m_pGroundStage;
	std::shared_ptr<CSpawnUFO>				m_pSpawnUFO;
	std::shared_ptr<CEventPlayer>			m_pPlayer;
	std::shared_ptr<CEventGirl>				m_pGirl;
	std::shared_ptr<CEventAlienA>			m_pAlienA;
	std::shared_ptr<CEventCamera>			m_pEventCamera;
	std::shared_ptr<CSkyDome>				m_pSkyDome;
	std::shared_ptr<CEventWidget>			m_pEventWidget;
	CEventCharacter::SOptionalState			m_stPlayer;			//�v���C���[�̏��.
	CEventCharacter::SOptionalState			m_stGirl;			//���̎q�̏��.
	CEventCharacter::SOptionalState			m_stAlien;			//�F���l�̏��.
	CEventCamera::SCameraState				m_stCamera;			//�J�����̏��.

	D3DXVECTOR3								m_vUFOPosition;		// UFO�̈ʒu.
	D3DXVECTOR3								m_vUFOScale;		// UFO�̑傫��.
	D3DXVECTOR3								m_vAlienOldPosition;// �F���l�̑O���W.
	EEventStep								m_EventStep;		// �C�x���g�̃X�e�b�v.
	int										m_WaitCount;		// �ҋ@�J�E���g.
	int										m_SwingCameraCount;	// �J������h�炷�J�E���g.
	float									m_Gravity;			// �d��.

	float									m_Speed;
	int										m_UFOStep;			// UFO�̃X�e�b�v.

};


#endif	//#ifndef GAMECLEAR_EVENT_H