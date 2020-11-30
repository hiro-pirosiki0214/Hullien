#ifndef GIRL_H
#define GIRL_H

#include "..\Character.h"
#include "GirlParam.h"
#include "..\..\..\Widget\SceneWidget\GameWidget\Warning\Warning.h"

class CGirl : public CCharacter
{
	// �p�����[�^�̃t�@�C���p�X.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Girl\\Girl.bin";
	const char* MODEL_NAME			= "yuri-dy_s";	// ���f����.
	const float TOLERANCE_RADIAN		= static_cast<float>(D3DXToRadian(10.0));	// ��]�̋��e�͈�.
	const float THRESHOLD_VALUE_RADIAN	= static_cast<float>(D3DXToRadian(140.0));	// �������l.


	enum enAnimNo
	{
		EAnimNo_None = -1,

		EAnimNo_Wait,	// �ҋ@.
		EAnimNo_Move,	// �ړ�.
		EAnimNo_Abduct,	// �A�ꋎ���Ă���.

		EAnimNo_Max,

		EAnimNo_Begin	= EAnimNo_Wait,
		EAnimNo_End		= EAnimNo_Max,

		Abduct_Max,
	} typedef EAnimNo;

	// ���݂̏��.
	enum class enNowState
	{
		None,

		Protected,	// ����Ă���.
		Abduct,		// �A�ꋎ��Ă���.
		Move,		// �ړ�.

		Max,

	} typedef ENowState;

	// �ړ����.
	enum class enMoveState
	{
		None,

		Rotation,	// ��].
		Move,		// �ړ�.
		Wait,		// �ҋ@.

		Max,
	} typedef EMoveState;
public:
	CGirl();
	~CGirl();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;
	// ������W�̐ݒ�֐�.
	virtual void SetPosition( const D3DXVECTOR3& pos ) override;
	// �X�v���C�g�̕`��.
	virtual void SpriteRender() override;
	// �댯�ȏ�Ԃ�.
	inline bool IsDanger(){ return m_IsDanger; }
	// �J�����̃��W�A���l�̐ݒ�֐�.
	inline void SetCameraRadianX( const float& radian ) { m_CameraRadianX = radian; }

private:
	// �ړ��֐�.
	virtual void Move() override;
	// �A�ꋎ��Ă���.
	void Abduct();
	// �ړI�̏ꏊ�Ɍ����ĉ�].
	void TargetRotation();
	// �ړI�̏ꏊ�Ɍ����Ĉړ�.
	void TargetMove();

	// ���G�̓����蔻��.
	void SearchCollision( CActor* pActor );

	// �����蔻��̍쐬.
	bool ColliderSetting();

	// �A�j���[�V�����t���[���̐ݒ�.
	virtual bool SetAnimFrameList() override;

	// �T�E���h.
	void Sound();

	// �댯���̉�].
	void WarningRotation();
	// �f�o�b�O�`��֐�.
	void DebugRender();

private:
	SGirlParam m_Parameter;	// ���̎q�̃p�����[�^�[.
	std::shared_ptr<CCollisionManager>	m_pSearchCollManager;	// ���G�p�̓����蔻��.
	std::unique_ptr<CWarning>			m_pWarning;				// �x���N���X.
	D3DXVECTOR3	m_OldPosition;	// �O��̍��W.
	ENowState	m_NowState;		// ���݂̏��.
	EMoveState	m_NowMoveState;	// ���݂̈ړ����.
	float		m_CameraRadianX;// �J�����̃��W�A���l.
	bool		m_IsDanger;		// �댯���ǂ���.
	bool		m_IsOnlyFirst;	//��x�����������Ȃ�.
};

#endif	// #ifndef GIRL_H.