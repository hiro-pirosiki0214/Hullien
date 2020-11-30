#ifndef D3DX9_H
#define D3DX9_H

#include "..\..\Global.h"

/*********************************
*	DirectX9
*/
class CDirectX9
{
public:
	CDirectX9();
	~CDirectX9();

	// �C���X�^���X�̎擾.
	static CDirectX9* GetInstance();

	// DirectX9�̍\�z.
	static HRESULT Create( HWND hWnd );
	// DirectX9�̉��.
	static HRESULT Release();

	// �f�o�C�X�̎擾�֐�.
	static LPDIRECT3DDEVICE9 GetDevice(){ return GetInstance()->m_pDevice9; }

private:
	// �f�o�C�X9�̍\�z.
	HRESULT CreateDevice9();

private:
	HWND				m_hWnd;		// �E�B���h�E�n���h��.
	LPDIRECT3DDEVICE9	m_pDevice9;	// �f�o�C�X9.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CDirectX9( const CDirectX9 & )				= delete;
	CDirectX9& operator = ( const CDirectX9 & )	= delete;
	CDirectX9( CDirectX9 && )					= delete;
	CDirectX9& operator = ( CDirectX9 && )		= delete;
};

#endif	// #ifndef D3DX9_H.