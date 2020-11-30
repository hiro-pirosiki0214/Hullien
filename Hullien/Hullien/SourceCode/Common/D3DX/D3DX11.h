#ifndef D3DX_H
#define D3DX_H

#include "..\..\Global.h"
#include <vector>

// ���X�^���C�U�X�e�[�g.
enum class enRS_STATE
{
	None,	// ���w�ʕ`��.
	Back,	// �w�ʂ�`�悵�Ȃ�.
	Front,	// ���ʂ�`�悵�Ȃ�.
	Wire,	// ���C���[�t���[���`��.

	Max,
} typedef ERS_STATE;

class CDirectX11
{
private: //----- �萔 ------.
	const float CLEAR_BACK_COLOR[4] = { 0.6f, 0.6f, 0.6f, 1.0f };	// �o�b�N�J���[.

public: //----- �֐� ------.
	CDirectX11();
	~CDirectX11();

	// �C���X�^���X�̎擾.
	static CDirectX11* GetInstance();

	// DirectX11�\�z�֐�.
	static HRESULT Create( HWND hWnd );
	// DirectX11����֐�.
	static HRESULT Release();

	// �N���A�o�b�N�o�b�t�@.
	static void ClearBackBuffer();
	// �X���b�v�`�F�[���v���[���g.
	static void SwapChainPresent();

	// �f�o�C�X11�̎擾�֐�.
	static ID3D11Device* GetDevice() { return GetInstance()->m_pDevice11; }
	// �R���e�L�X�g11�̎擾�֐�.
	static ID3D11DeviceContext* GetContext() { return GetInstance()->m_pContext11; }
	// �f�v�X�X�e���V���r���[�̎擾.
	static ID3D11DepthStencilView* GetDepthSV(){ return GetInstance()->m_pBackBuffer_DSTexDSV; }
	// BackBuffer�̐ݒ�.
	static void SetBackBuffer();

	// �u�����h��L��:�����ɐݒ肷��.
	static void SetBlend( bool EnableAlpha );
	// �A���t�@�J�o���[�W��L��:�����ɐݒ肷��.
	static void SetCoverage( bool EnableCoverage );
	// �[�x�e�X�g��L��:�����ɐݒ肷��.
	static void SetDeprh( bool flag );
	// ���X�^���C�U�X�e�[�g�ݒ�.
	static void SetRasterizerState( const ERS_STATE& rsState );

	// �t���X�N���[���̐ݒ�.
	static bool SetFullScreen( const bool& isOn );

private: //----- �֐� ------.
		 // �f�o�C�X11�̍쐬.
	HRESULT InitDevice11();
	// �����_�[�^�[�Q�b�g�r���[�̍쐬.
	HRESULT InitTexRTV();
	// �X�e���V���r���[�̍쐬.
	HRESULT InitDSTex();
	// �r���[�|�[�g�̍쐬.
	HRESULT InitViewports();
	// ���X�^���C�U�̍쐬.
	HRESULT InitRasterizer();

	// �u�����h�쐬.
	HRESULT InitBlend();
	// �[�x�e�X�g�쐬.
	HRESULT InitDeprh();
	// ���X�^���C�U�X�e�[�g�쐬.
	HRESULT InitRasterizerState();

private: //----- �ϐ� ------.
	HWND	m_hWnd;	// �E�B���h�E�n���h��.

	ID3D11Device*			m_pDevice11;	// �f�o�C�X11.
	ID3D11DeviceContext*	m_pContext11;	// �R���e�L�X�g11.

	IDXGISwapChain*			m_pSwapChain;			// �X���b�v�`�F�[��.

	ID3D11RenderTargetView*	m_pBackBuffer_TexRTV;	// �����_�[�^�[�Q�b�g�r���[.
	ID3D11DepthStencilView*	m_pBackBuffer_DSTexDSV;	// �X�e���V���r���[.
	ID3D11Texture2D*		m_pBackBuffer_DSTex;	// �e�N�X�`���[2D.


	ID3D11DepthStencilState*	m_pDepthStencilState;		// �f�v�X�X�e���V���L��.
	ID3D11DepthStencilState*	m_pDepthStencilStateOff;	// �f�v�X�X�e���V������.

	ID3D11BlendState*			m_pAlphaBlend;				// �A���t�@�u�����h�L��.
	ID3D11BlendState*			m_pNoAlphaBlend;			// �A���t�@�u�����h����.
	ID3D11BlendState*			m_pAlphaToCoverage;			// �A���t�@�J�o���[�W�L��.

	ID3D11RasterizerState*		m_pRsSoldAndNone;	// �\���b�hAnd���w�ʕ`��.
	ID3D11RasterizerState*		m_pRsSoldAndBack;	// �\���b�hAnd�w�ʂ�`�悵�Ȃ�.
	ID3D11RasterizerState*		m_pRsSoldAndFront;	// �\���b�hand���ʂ�`�悵�Ȃ�.
	ID3D11RasterizerState*		m_pRsWireFrame;		// ���C���[�t���[���`��.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CDirectX11( const CDirectX11 & )				= delete;
	CDirectX11& operator = ( const CDirectX11 & )	= delete;
	CDirectX11( CDirectX11 && )						= delete;
	CDirectX11& operator = ( CDirectX11 && )		= delete;
};

#endif	// #ifndef D3DX_H.