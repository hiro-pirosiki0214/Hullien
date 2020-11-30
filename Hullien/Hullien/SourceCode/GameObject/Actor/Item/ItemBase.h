#ifndef ITEM_BASE_H
#define ITEM_BASE_H

#include "..\Actor.h"

class CEffectManager;	// �G�t�F�N�g�N���X.

class CItemBase : public CActor
{
	const float INIT_MODEL_ALPHA			= 1.0f;		// �������f�����ߒl.
	const float DEFAULT_ACCELERATION_VALUE	= -0.05f;	// �f�t�H���g�����l.
	const float DEFAULT_GRAVITY				= 0.001f;	// �f�t�H���g�d��.
public:
	// �p�����[�^.
	struct stParameter
	{
		float	InitAccelerationValue;	// ���������l.
		float	InitGravity;			// �����d��.

		int		BoundCountMax;			// �o�E���h�ő吔.
		float	FinalRenderHeight;		// �ŏI�I�ȕ`��̍���.
		float	FinalMoveSpeed;			// �ŏI�I�Ȉړ����x.
		float	ModelScaleMax;			// ���f���T�C�Y�ő�.
		float	ModelScaleAddValue;		// ���f���T�C�Y���Z�l.
		float	RotationSpeed;			// ��]���x.
		float	ActiveTime;				// �A�N�e�B�u����.
		float	DisappearTime;			// �����鎞��.
		float	FlashingAddValue;		// �_�ŉ��Z�l.
		float	ModelAlphaMax;			// ���f�����ߒl�ő�.
		float	HitEffectTime;			// �q�b�g���̃G�t�F�N�g����.
		stParameter()
			: InitAccelerationValue	( 0.0f )
			, InitGravity			( 0.0f )
			, BoundCountMax			( 0 )
			, FinalRenderHeight		( 0.0f )
			, FinalMoveSpeed		( 0.0f )
			, ModelScaleMax			( 0.0f )
			, ModelScaleAddValue	( 0.0f )
			, RotationSpeed			( 0.0f )
			, ActiveTime			( 0.0f )
			, DisappearTime			( 0.0f )
			, FlashingAddValue		( 0.0f )
			, ModelAlphaMax			( 0.0f )
			, HitEffectTime			( 0.0f )
		{}
	} typedef SParameter;

private:
	const SParameter* pPRAMETER;	// �p�����[�^(�O������|�C���^�Ŏ擾����).

protected:
	// ���݂̏��.
	enum class enNowState
	{
		None,

		Drop,			// �o��.
		Active,			// �A�N�e�B�u.
		HitDisappear,	// ���������ۂɏ�����.
		TimeDisappear,	// ���Ԃ��o�������ɏ�����.
		Delete,			// ������.

		Max,
	} typedef ENowState;

	// �g�p����G�t�F�N�g�ԍ�.
	enum class enEffectNumber
	{
		None = -1,

		DropAndActive,	// �h���b�v �A�N�e�B�u��.
		Hit,			// ����������.

		Max,	// �ő�.
	} typedef EEffectNumber;

public:
	CItemBase();
	CItemBase( const SParameter* pParam );
	virtual ~CItemBase();

	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �G�t�F�N�g�̕`��.
	virtual void EffectRender() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;
	// �o������.
	virtual void Drop( const D3DXVECTOR3& vPos );

	// ���������ǂ���.
	inline bool IsDelete(){ return m_NowState == ENowState::Delete; }
	// �������ď�������.
	inline bool IsHitDisappear(){ return m_NowState == ENowState::HitDisappear; }

protected:
	// ���������ۂ̌��ʂ�^����.
	virtual void GiveEffect( CActor* pActor ) = 0;
	// �o��.
	void Drop();
	// �A�N�e�B�u.
	void Active();
	// ���������ۂɏ����鏈��.
	void HitDisappear();
	// ���Ԃ��o�������ɏ�����.
	void TimeDisappear();

	// ��]�֐�.
	void Rotate();

	// �G�t�F�N�g�̐ݒ�.
	virtual bool EffectSetting() = 0;
	// ���f���̎擾.
	bool GetModel( const char* modelName );
	// �����蔻��̐ݒ�.
	virtual bool ColliderSetting();

private:
	// �h���b�v�@�A�N�e�B�u���̕`��.
	void DropAndActiveRender();
	// �h���b�v�@�A�N�e�B�u���̃G�t�F�N�g�̕`��.
	void DropAndActiveEffectRender();
	// �q�b�g���̃G�t�F�N�g�̕`��.
	void HitEffectRender();
	// �A���t�@�u�����h�̐ݒ�.
	void AlphaBlendSetting();

protected:
	std::shared_ptr<CDX9StaticMesh> m_pStaticMesh;	// �X�^�e�B�b�N���b�V��.
	std::vector<std::shared_ptr<CEffectManager>> m_pEffects; // �G�t�F�N�g.
	ENowState m_NowState;	// ���݂̏��.
	float m_Scale;
	float m_ModelAlpha;

	float m_AccelerationValue;	// �����l.
	float m_Gravity;			// �d��.
	int m_BoundCount;			// �o�E���h��.

	int m_ActiveCount;		// �A�N�e�B�u�J�E���g.

	float m_FlashingCount;		// �_�ŃJ�E���g.
	float m_DisappearCount;		// �����鎞��.
	float m_FlashingAccValue;	// �_�ŉ����l.

	float m_HitEffectCount;	// �q�b�g���̃G�t�F�N�g�J�E���g.
};

#endif	// #ifndef ITEM_BASE_H.