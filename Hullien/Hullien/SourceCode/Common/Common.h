#ifndef COMMON_H
#define COMMON_H

#include "..\Global.h"
#include "D3DX/D3DX11.h"

class CCommon
{
public:
	CCommon();
	virtual ~CCommon();

	// ���W�̐ݒ�.
	void SetPosition( const D3DXVECTOR3& vPos ){ m_vPos = vPos; }
	// ��]���̐ݒ�.
	void SetRotation( const D3DXVECTOR3& vRot ){ m_vRot = vRot; }
	// �T�C�Y�̐ݒ�.
	void SetScale( const D3DXVECTOR3& vScale ){ m_vScale = vScale; }
	void SetScale( const float& fScale ){ m_vScale = { fScale, fScale, fScale }; }
	// �F�̐ݒ�.
	void SetColor( const D3DXVECTOR4& vColor ){ m_vColor = vColor; }
	// �A���t�@�l�̐ݒ�.
	void SetAlpha( const float& fAlpha ){ m_vColor.w = fAlpha; }

	// �u�����h��L��:�����ɐݒ肷��.
	void SetBlend( bool EnableAlpha );
	// �A���t�@�J�o���[�W��L��:�����ɐݒ肷��.
	void SetCoverage( bool EnableCoverage );
	// �[�x�e�X�g��L��:�����ɐݒ肷��.
	void SetDeprh( bool flag );
	// ���X�^���C�U�X�e�[�g�ݒ�.
	void SetRasterizerState( const ERS_STATE& rsState );

protected:
	// �f�o�C�X�̎擾�A�e���̏�����.
	HRESULT InitPram( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );

protected:
	ID3D11Device*			m_pDevice11;	// �f�o�C�X�I�u�W�F�N�g.
	ID3D11DeviceContext*	m_pContext11;	// �f�o�C�X�R���e�L�X�g.

	D3DXVECTOR3 m_vPos;		// ���W���.
	D3DXVECTOR3 m_vRot;		// ��]���.
	D3DXVECTOR3	m_vScale;	// �X�P�[�����.
	D3DXVECTOR4 m_vColor;	// �F.
};

#endif	// #ifndef COMMON_H.