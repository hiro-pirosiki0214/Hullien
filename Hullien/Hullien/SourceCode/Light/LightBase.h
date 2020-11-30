#ifndef LIGHT_BASE_H
#define LIGHT_BASE_H

#include "..\Global.h"

class CLightBase
{
	const float FLOAT_WND_W = static_cast<float>(WND_W);	// (float)��ʕ�.
	const float FLOAT_WND_H = static_cast<float>(WND_H);	// (float)��ʕ�.
	const float INITIAL_VIEWING_ANGLE	= static_cast<float>(3.141519/4.0);	// ��������p.
	const float SCREEN_ASPECT_RATIO		= FLOAT_WND_W/FLOAT_WND_H;			// ��ʃA�X�y�N�g��.
	const float INITIAL_MIN_CLIPPING_DISTANCE = 0.1f;			// �����ŏ��`�拗��.
	const float INITIAL_MAX_CLIPPING_DISTANCE = 1000.f;			// �����ő�`�拗��.
	const D3DXVECTOR3 VIEW_UP_VECTOR	= { 0.0f, 1.0f, 0.0f };	// ������x�N�g��.

	static const int MAX_CASCADE = 4;	// �ő�J�X�P�[�h��.

	// �o�E���f�B���O�{�b�N�X(�J�X�P�[�h�V���h�E�Ɏg�p).
	struct stBBox
	{
		D3DXVECTOR3 mini;
		D3DXVECTOR3 maxi;

		stBBox( D3DXVECTOR3 min, D3DXVECTOR3 max )
			: mini ( min )
			, maxi ( max )
		{}

	} typedef SBBox;

public:
	CLightBase();
	virtual ~CLightBase();

	// �J�X�P�[�h���̃V���h�E�s����擾����.
	D3DXMATRIX* GetShadowVP();

	// �J�X�P�[�h�������W���擾.
	float* GetSpritPos(){ return m_SplitPos; }

	// ���W�̐ݒ�.
	void SetPosition( const D3DXVECTOR3& pos ){ m_vPosition = pos; }
	// ���W�̎擾.
	D3DXVECTOR3 GetPosition() const { return m_vPosition; }

	// ���_���W�̐ݒ�.
	void SetLookPosition( const D3DXVECTOR3& lookPos ){ m_vLookPosition = lookPos; }

	// �����̐ݒ�.
	void SetDirection( const D3DXVECTOR3& dir ){ m_vDirection = dir; }
	// �����̎擾.
	D3DXVECTOR3 GetDirection() const { return m_vDirection; }

	// ��]�s��̐ݒ�.
	void SetRorarionMatrix( const D3DXMATRIX& rot ){ m_mRotation = rot; }
	// ��]�s��̎擾.
	D3DXMATRIX GetRorarionMatrix() const { return m_mRotation; }

	// �����̐ݒ�.
	void SetIntensity( const float& intensity ){ m_fIntensity = intensity; }
	// �����̎擾.
	float GetIntensity() const { return m_fIntensity; }

private:
	// ���s��������.
	void ComputeSplitPositions(
		const int splitCount, const float lamda, 
		const float nearClip, const float farClip, 
		float* positions );
	// ���������������8�p�����Ƃ߂āC���C�g�̃r���[�ˉe��Ԃ�AABB�����߂�.
	SBBox CalculateFrustum( float nearClip, float farClip, D3DXMATRIX& viewProj );
	// �N���b�v�s������߂�.
	D3DXMATRIX CreateCropMatrix( SBBox& box );

protected:
	D3DXMATRIX	m_ShadowMatrix[MAX_CASCADE];	// �V���h�E�p�s��(�J�X�P�[�h���p��).
	float		m_SplitPos[MAX_CASCADE];		// �J�X�P�[�h�̋�؂���W.
	D3DXMATRIX	m_ProjMatrix;		// �v���W�F�N�V����.
	D3DXMATRIX	m_ViewMatrix;		// �r���[.
	D3DXVECTOR3	m_vPosition;		// �ʒu.
	D3DXVECTOR3 m_vLookPosition;	// �Vt�N���W.
	D3DXVECTOR3	m_vDirection;		// ����.
	D3DXMATRIX	m_mRotation;		// ��]�s��.
	float		m_fIntensity;		// ���x(���邳).

	float m_ViewingAngle;			// ����p.
	float m_MaxClippingDistance;	// �ő�N���b�s���O����.
	float m_MinClippingDistance;	// �ŏ��N���b�s���O����.
};

#endif	// #ifndef LIGHT_BASE_H.