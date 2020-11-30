#ifndef EVENT_CHARACTER_H
#define EVENT_CHARACTER_H

#include "..\Actor.h"
#include "..\Character\AnimationParamInfo.h"

class CCollisionManager;

/***************************************
*	�C�x���g�p�L�����N�^�N���X.
**/
class CEventCharacter : public CActor
{
	const float ROTATIONAL_SPEED = 0.05f;	// ��]���x.
	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(10.0));	// ��]�̋��e�͈�.

protected:
	const float MODEL_ALPHA_MAX = 1.0f;	// ���f���A���t�@�̍ő�l.
	const float MODEL_SCALE_MAX = 1.0f;	// ���f���̑傫���ő�l.

public:
	// �C�ӂŐݒ肷����.
	struct stOptionalState
	{
		D3DXVECTOR3	vPosition;			// �ʒu.
		D3DXVECTOR3	vRotation;			// ��]�l.
		D3DXVECTOR3	vScale;				// �傫��.
		float		ModelAlpha;			// ���f���̓��ߒl.
		float		MoveSpeed;			// �ړ����x.
		float		RotationalSpeed;	// ��]���x.
		float		ScaleSpeed;			// �g�呬�x.
		float		AlphaSpeed;			// ���ߑ��x.
		bool		IsDisp;				// �`�悷�邩.

		stOptionalState()
			: vPosition			( {0.0f,0.0f,0.0f} )
			, vRotation			( {0.0f,0.0f,0.0f} )
			, vScale			( {1.0f,1.0f,1.0f} )
			, ModelAlpha		( 1.0f )
			, MoveSpeed			( 0.1f )
			, RotationalSpeed	( 0.01f )
			, ScaleSpeed		( 0.006f )
			, AlphaSpeed		( 0.03f )
			, IsDisp			( true )
		{}
	}typedef SOptionalState;

protected:

	// �L�����N�^�̏��.
	struct stCharacterParam : public SOptionalState
	{
		float			ResearchLenght;		// �Č�������ۂ̋���.
		D3DXVECTOR3		SphereAdjPos;		// �X�t�B�A�̒������W.
		float			SphereAdjRadius;	// �X�t�B�A�̒������a.

		stCharacterParam()
			: ResearchLenght(0.0f)
			, SphereAdjPos(0.0f, 0.0f, 0.0f)
			, SphereAdjRadius(0.0f)
		{}
	}typedef SCharacterParam;

public:
	CEventCharacter();
	virtual ~CEventCharacter();

	// �ړ��֐�.
	virtual void Move() = 0;

	// ���ݒ�֐�.
	virtual void SetOptionalState(const SOptionalState& state);

	// ���W�ʈʒu�ݒ�֐�.
	void SetPositionX(const float& vPos_x);
	void SetPositionY(const float& vPos_y);
	void SetPositionZ(const float& vPos_z);

	// ��]�l�擾�֐�.
	D3DXVECTOR3 GetRotation() const;
	// ��]�l�ݒ�֐�.
	void SetRotation(const D3DXVECTOR3& vRot);
	void SetRotationX(const float& vRot_x);
	void SetRotationY(const float& vRot_y);
	void SetRotationZ(const float& vRot_z);

	// �傫���擾�֐�.
	D3DXVECTOR3 GetScale() const;
	// �傫���ݒ�֐�.
	void SetScale(const D3DXVECTOR3& vScale);
	void SetScaleX(const float& vScale_x);
	void SetScaleY(const float& vScale_y);
	void SetScaleZ(const float& vScale_z);

	// �E��]�̐ݒ�.
	float RotationMoveRight(const float& rotValue, 
		const float& rotSpeed, bool IsRightRot = true);
	// �A�j���[�V�����ݒ�.
	void SetAnimation(const int& animNo);
	// �A�j���[�V�������u�����h���Đݒ�.
	void SetAnimationBlend(const int& animNo);
	// �A�j���[�V�������x�̐ݒ�.
	inline void SetAnimSpeed( const double& speed ){ m_AnimSpeed = speed; }

protected:
	// ���b�V���̕\���֐�.
	void MeshRender();
	// ���f���̎擾�֐�.
	bool GetModel(const char* modelName);
	// ����.
	void FootStep(const char* rightfoot, const char* leftfoot);
	// �����p�����蔻��̐ݒ�.
	bool FootStepCollisionSetting();

protected:
	std::shared_ptr<CDX9SkinMesh>	m_pSkinMesh;		// �X�L�����b�V��.
	anim::AAnimFrameList			m_AnimFrameList;	// �A�j���[�V�����t���[���̃��X�g.
	int				m_NowAnimNo;				// ���݂̃A�j���[�V�����ԍ�.
	int				m_OldAnimNo;				// �ߋ��̃A�j���[�V�����ԍ�.
	double			m_AnimSpeed;				// �A�j���[�V�������x.
	SCharacterParam	m_Parameter;				// �p�����[�^.

private:
	std::vector<std::shared_ptr<CCollisionManager>> m_pFootCollision;
	std::shared_ptr<CCollisionManager>				m_pGroundCollision;
	D3DXVECTOR3 m_vGroundPosition;	// �n�ʂ̈ʒu.
	D3DXVECTOR3 m_vRightPosition;	// �E���̈ʒu.
	D3DXVECTOR3 m_vLeftPosition;	// �����̈ʒu.
};

#endif	//#ifndef EVENT_CHARACTER_H.
