#ifndef ARM_H
#define ARM_H

#include "..\GameObject.h"

class CArm : public CGameObject
{
public:
	inline static const float GRAB_DISTANCE	= 5.0f;		// �͂ނƂ��̋���.

private:
	const char* MODEL_NAME = "armsandwich_s";

	const float APPEARANCE_ADD_VALUE	= 0.1f;		// �o�����̉��Z�l.
	const float APPEARANCE_COUNT_MAX	= 1.0f;		// �o�����̍ő吔.
	const float CLEAN_UP_SUB_VALUE		= 0.015f;	// �ЂÂ����̌��Z�l.
	const float CLEAN_UP_COUNT_MIN		= 0.0f;		// �ЂÂ����̍ŏ���.

	const float SCALING_VALUE			= 0.5f;		// �g�k�l.
	const float SCALING_VALUE_MAX		= 1.0f;		// �g�k�̍ő�l.
	const float SCALING_ADD_VALUE		= (1.0f-SCALING_VALUE)*0.04f;	// �g�k�̉��Z�l.

	enum class enArmState
	{
		None,

		Appearance,	// �o��.
		Grab,		// �͂�.
		CleanUp,	// �ЂÂ���.

		End,		// �I��.

		Start = None,

		Max,


	} typedef EArmState;
public:
	CArm();
	virtual ~CArm();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

	// Y���̐ݒ�.
	inline void SetRotationY( const float& rotY ){ m_vRotation.y = rotY; }

	// �͂�ł��邩.
	inline bool IsGrab(){ return m_NowArmState == EArmState::End; }
	// �ЂÂ��Ă��邩.
	inline bool IsCleanUp(){ return m_NowArmState == EArmState::Start; }
	// �͂�ł�����W�̎擾.
	D3DXVECTOR3 GetGrabPosition();

	// �o������p��.
	void SetAppearancePreparation();
	// �ЂÂ���p��.
	void SetCleanUpPreparation();

	// ���S�Ɏ��o��
	void SetAppearance();
	// ���S�ɕЂÂ���.
	void SetCleanUp();

	inline void SetCleanUpScale( const D3DXVECTOR3& scale ){ m_vScale = scale; }

private:
	// �o��.
	void Appearance();
	// �͂�.
	void Grab();
	// �ЂÂ���.
	void CleanUp();

private:
	std::shared_ptr<CDX9SkinMesh>	m_pSkinMesh;	// �X�L�����b�V��.
	LPD3DXANIMATIONCONTROLLER		m_pAC;			// �A�j���[�V�����R���g���[���[.
	D3DXVECTOR3						m_GrabPosition;	// �͂�ł��鎞�̍��W.
	EArmState						m_NowArmState;	// ���݂̃A�[���̏��.
	EArmState						m_OldArmState;	// �O��̃A�[���̏��.
	float	m_AppearanceCount;	// �o���J�E���g.
	float	m_ScalingValue;		// �g�k�l.
	double	m_AnimSpeed;		// �A�j���[�V�������x.
	double	m_AnimEndFrame;		// �A�j���[�V�����̏I���t���[��.

};

#endif	// #ifndef ARM_H.