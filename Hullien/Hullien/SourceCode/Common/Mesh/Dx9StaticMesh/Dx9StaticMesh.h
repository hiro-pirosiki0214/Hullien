#ifndef DX9_STATIC_MESH_H
#define DX9_STATIC_MESH_H

#include "..\..\Common.h"
#include "..\MeshStruct.h"

// �X�^�e�B�b�N���b�V���N���X.
class CDX9StaticMesh : public CCommon
{
	// ���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// ���_���W.
		D3DXVECTOR3	Normal;	// �@��(�A�e�v�Z�ɕK�{).
		D3DXVECTOR2	Tex;	// �e�N�X�`�����W.
	};
	// �}�e���A���\����.
	struct MY_MATERIAL
	{
		D3DXVECTOR4	Ambient;	// �A���r�G���g.
		D3DXVECTOR4	Diffuse;	// �f�B�q���[�Y.
		D3DXVECTOR4	Specular;	// �X�y�L����.
		char szTextureName[64];	// �e�N�X�`���t�@�C����.
		ID3D11ShaderResourceView* pTexture;	// �e�N�X�`��.
		DWORD dwNumFace;		// ���̃}�e���A���̃|���S����.
		MY_MATERIAL()
			: Ambient		( 0.0f, 0.0f, 0.0f, 0.0f )
			, Diffuse		( 0.0f, 0.0f, 0.0f, 0.0f )
			, Specular		( 0.0f, 0.0f, 0.0f, 0.0f )
			, szTextureName	()
			, pTexture		( nullptr )
			, dwNumFace		( 0 )
		{
		}
		// �f�X�g���N�^.
		~MY_MATERIAL() { SAFE_RELEASE(pTexture); }
	};
public:
	CDX9StaticMesh( 
		HWND hWnd, 
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, 
		LPDIRECT3DDEVICE9 pDevice9,
		const char* fileName )
		: CDX9StaticMesh()
	{
		Init( hWnd, pDevice11, pContext11, pDevice9, fileName );
	}

	CDX9StaticMesh();			// �R���X�g���N�^.
	virtual ~CDX9StaticMesh();	// �f�X�g���N�^.

	// ������.
	HRESULT Init(
		HWND hWnd, 
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, 
		LPDIRECT3DDEVICE9 pDevice9,
		const char* fileName );

	// ����֐�.
	void Release();

	// �����_�����O�p.
	void Render( const bool& isTrans = false );

	// ���b�V�����擾.
	inline LPD3DXMESH GetMesh() const { return m_pMeshForRay; }

	// �e��`�悷�邩.
	inline void SetIsShadow( const bool isShadow ){ m_IsShadow = isShadow; }

private:
	// ���b�V���ǂݍ���.
	HRESULT LoadXMesh( const char* fileName );
	// �V�F�[�_�쐬.
	HRESULT InitShader();
	//�R���X�^���g�o�b�t�@�쐬�֐�.
	HRESULT CreateCBuffer( ID3D11Buffer** pConstantBuffer, UINT size );

	// �����_�����O�֐�(�N���X���ł̂ݎg�p����).
	void RenderMesh(
		D3DXMATRIX& mWorld, 
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj,
		const bool& isTrans );
	// �e�̕`��.
	bool ShadowRender( const D3DXMATRIX& mWorld );
	// �������̕`��,
	bool TranslucentRender( const D3DXMATRIX& mWorld );

private:
	HWND				m_hWnd;		// �E�B���h�E�n���h��.

	LPDIRECT3DDEVICE9	m_pDevice9;	// Dx9�f�o�C�X�I�u�W�F�N�g.

	ID3D11VertexShader*		m_pVertexShader;		// ���_�V�F�[�_.
	ID3D11InputLayout*		m_pVertexLayout;		// ���_���C�A�E�g.
	ID3D11PixelShader*		m_pPixelShader;			// �s�N�Z���V�F�[�_�[.
	ID3D11Buffer*			m_pCBufferPerMesh;		// �R���X�^���g�o�b�t�@(���b�V����).
	ID3D11Buffer*			m_pCBufferPerMaterial;	// �R���X�^���g�o�b�t�@(�}�e���A����).
	ID3D11Buffer*			m_pCBufferPerFrame;		// �R���X�^���g�o�b�t�@(�t���[����).

	ID3D11Buffer*				m_pVertexBuffer;	// ���_�o�b�t�@.
	ID3D11Buffer**				m_ppIndexBuffer;	// �C���f�b�N�X�o�b�t�@.
	ID3D11SamplerState*			m_pSampleLinear;	// �T���v��:�e�N�X�`���Ɋe��t�B���^��������.
	ID3D11SamplerState*			m_pToonSampleLinear;// �g�D�[���}�b�v�p�T���v���[.
	ID3D11SamplerState*			m_pShadowMapSampler;// �V���h�E�}�b�v�p�T���v���[.

	LPD3DXMESH			m_pMesh;		// ���b�V���I�u�W�F�N�g.
	LPD3DXMESH			m_pMeshForRay;	// ���C�ƃ��b�V���p.
	DWORD				m_NumMaterials;	// �}�e���A����.

	ID3D11ShaderResourceView*	m_pToonTexture;	// �g�D�[���e�N�X�`��.
	ID3D11ShaderResourceView*	m_pFogTexture;	// �t�H�O�e�N�X�`��.

	MY_MATERIAL*	m_pMaterials;	// �}�e���A���\����.
	DWORD			m_NumAttr;		// ������.
	DWORD			m_AttrID[300];	// ����ID ��300�����܂�.

	bool			m_EnableTexture;//�e�N�X�`������.
	bool			m_IsShadow;		// �����ɃV���h�E�������邩.
};

#endif	// #ifndef DX9_STATIC_MESH_H.