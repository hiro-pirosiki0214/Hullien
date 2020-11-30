#ifndef PERA_RENDERER_H
#define PERA_RENDERER_H

#include "..\Common.h"
#include "..\Shader\SpriteShader\SpriteShader.h"

#include <vector>

class CSceneTexRenderer
{
public:
	static const int MAX_CASCADE = 4;	// �J�X�P�[�h�̍ő吔.
	// G-Buffer�̎��.
	enum enGBufferNo
	{
		enGBufferNo_COLOR,		// �F���.
		enGBufferNo_NORMAL,		// �@�����.
		enGBufferNo_Z_DEPTH,	// �[�x�l���.

		enGBufferNo_MAX,	// �ő�l.

	} typedef EGBufferNo;

	// �����_�[�p�X.
	enum class enRenderPass
	{
		Shadow,		// ����.
		Trans,		// ����.
		GBuffer,	// �O���.

		Max,	// �ő�.
	} typedef ERenderPass;
private:
	// �V�F�[�_�[��.
	const char* SHADER_NAME = "Data\\Shader\\SceneTexRenderer.hlsl";
	const float CLEAR_BACK_COLOR[4] = { 0.6f, 0.6f, 0.6f, 1.0f };	// �o�b�N�J���[.

public:
	//======================================
	//	�\����.
	//======================================
	// �R���X�^���g�o�b�t�@.
	struct C_BUFFER
	{
		ALIGN16 D3DXMATRIX	mW;				// ���[���h�s��.
		ALIGN16 D3DXVECTOR2	vViewPort;		// UV���W.
	};
	// ���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// ���_���W.
		D3DXVECTOR2 Tex;	// �e�N�X�`�����W.
	};

public:
	CSceneTexRenderer();
	~CSceneTexRenderer();

	// �C���X�^���X�̎擾.
	static CSceneTexRenderer* GetInstance();

	// ������.
	static HRESULT Init();

	// ���.
	static void Release();

	// �`��֐�.
	static void Render();

	// �o�b�t�@�̃N���A.
	static void ClearBuffer();

	// �����_�����O�p�X��ݒ�.
	static void SetRenderPass( const ERenderPass& pass ){ GetInstance()->m_NowRenderPass = pass; }
	// ���݂̃����_�����O�p�X���擾.
	static ERenderPass GetRenderPass() { return GetInstance()->m_NowRenderPass; }

	// Shadow�o�b�t�@�̐ݒ�.
	static void SetShadowBuffer( int i );
	// G-Buffer�̐ݒ�.
	static void SetGBuffer();
	// TransBuffer�̐ݒ�.
	static void SetTransBuffer();

	// �[�x�o�b�t�@�e�N�X�`�����擾.
	static std::vector<ID3D11ShaderResourceView*> GetShadowBuffer(){ return GetInstance()->m_pShadowBufferSRV; }
	// G-Buffer�e�N�X�`�����擾.
	static std::vector<ID3D11ShaderResourceView*> GetGBuffer(){ return GetInstance()->m_pGBufferSRV; }
	// TransBuffer�e�N�X�`�����擾.
	static ID3D11ShaderResourceView* GetTransBaffer(){ return GetInstance()->m_pTransBufferSRV; }

private:
	// ShadowBuffer�̍쐬.
	HRESULT InitShadowBufferTex();
	// G-Buffer�̍쐬.
	HRESULT InitGBufferTex();
	// TransBuffer�̍쐬.
	HRESULT InitTransBufferTex();
	// Antialiasing�̍쐬.
	HRESULT InitAntialiasingTex();
	// �V�F�[�_�쐬.
	HRESULT CreateShader();
	// �T���v���̍쐬.
	HRESULT InitSample();
	// �R���X�^���g�o�b�t�@�̍쐬.
	HRESULT CreateConstantBuffer();
	// ���f���쐬.
	HRESULT CreateModel();

	// �o�b�t�@�̍쐬.
	HRESULT CreateBufferTex(
		const D3D11_TEXTURE2D_DESC	texDesc,
		ID3D11RenderTargetView**	ppRTV,
		ID3D11ShaderResourceView**	ppSRV,
		ID3D11Texture2D**			ppTex );
private:
	ID3D11Device*			m_pDevice11;	// �f�o�C�X�I�u�W�F�N�g.
	ID3D11DeviceContext*	m_pContext11;	// �f�o�C�X�R���e�L�X�g.

	std::vector<ID3D11RenderTargetView*>	m_pShadowBufferRTV;	// �V���h�E�}�b�v�p�o�b�t�@�̃����_�[�^�[�Q�b�g�r���[.
	std::vector<ID3D11ShaderResourceView*>	m_pShadowBufferSRV;	// �V���h�E�}�b�v�p�o�b�t�@�̃X�e���V���r���[.
	std::vector<ID3D11Texture2D*>			m_pShadowBufferTex;	// �V���h�E�}�b�v�p�o�b�t�@�̃e�N�X�`���[2D.

	std::vector<ID3D11RenderTargetView*>	m_pGBufferRTV;	// G-Buffer�̃����_�[�^�[�Q�b�g�r���[.
	std::vector<ID3D11ShaderResourceView*>	m_pGBufferSRV;	// G-Buffer�̃V�F�[�_�[���\�[�X�r���[.
	std::vector<ID3D11Texture2D*>			m_pGBufferTex;	// G-Buffer�̃e�N�X�`��2D.

	ID3D11RenderTargetView*		m_pTransBufferRTV;	// �������`��p�̃����_�[�^�[�Q�b�g�r���[.
	ID3D11ShaderResourceView*	m_pTransBufferSRV;	// �������`��p�̃V�F�[�_�[���\�[�X�r���[.
	ID3D11Texture2D*			m_pTransBufferTex;	// �������`��p�̃e�N�X�`��2D.

	ID3D11RenderTargetView*		m_pAntialiasingRTV;	// �A���`�G�C���A�X�̃����_�[�^�[�Q�b�g�r���[.
	ID3D11ShaderResourceView*	m_pAntialiasingSRV;	// �A���`�G�C���A�X�̃V�F�[�_�[���\�[�X�r���[.
	ID3D11Texture2D*			m_pAntialiasingTex;	// �A���`�G�C���A�X�̃e�N�X�`��2D.

	ID3D11VertexShader*		m_pVertexShader;	// ���_�V�F�[�_�[.
	ID3D11PixelShader*		m_pPixelShader;		// �s�N�Z���V�F�[�_�[.
	ID3D11PixelShader*		m_pLastPixelShader;	// �s�N�Z���V�F�[�_�[.
	ID3D11InputLayout*		m_pVertexLayout;	// ���_���C�A�E�g.
	ID3D11Buffer*			m_pConstantBuffer;	// �R���X�^���g�o�b�t�@.
	ID3D11Buffer*			m_pVertexBuffer;	// ���_�o�b�t�@.
	ID3D11SamplerState*		m_pSampleLinear;	// �T���v��:�e�N�X�`���Ɋe��t�B���^��������.

	ERenderPass				m_NowRenderPass;	// ���݂̃����_�����O�p�X.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CSceneTexRenderer( const CSceneTexRenderer & )				= delete;
	CSceneTexRenderer& operator = ( const CSceneTexRenderer & )	= delete;
	CSceneTexRenderer( CSceneTexRenderer && )					= delete;
	CSceneTexRenderer& operator = ( CSceneTexRenderer && )		= delete;
};

#endif	// #ifndef PERA_RENDERER_H.