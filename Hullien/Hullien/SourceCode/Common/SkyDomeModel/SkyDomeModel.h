#ifndef SKY_DOME_MODEL_H
#define SKY_DOME_MODEL_H

#include "..\Common.h"

class CSkyDomeModel : public CCommon
{
	// ���b�V���P�ʂœn�����.
	struct CBUFFER_PER_MESH
	{
		D3DXMATRIX	mWVP;	// ���[���h,�r���[,�v���W�F�N�V�����̍����ϊ��s��.	
	};
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
								// �R���X�g���N�^.
		MY_MATERIAL()
			: Ambient(),Diffuse(),Specular(),szTextureName()
			, pTexture(nullptr),dwNumFace(0)
		{
		}
		// �f�X�g���N�^.
		~MY_MATERIAL() 
		{
			SAFE_RELEASE(pTexture);
		}
	};

	const char* SHADER_NAME = "Data\\Shader\\SkyDome.hlsl";

public:
	CSkyDomeModel();
	virtual ~CSkyDomeModel();

	// ������.
	HRESULT Init( const char* fileName );
	// ����֐�.
	void Release();

	// �`��.
	void Render();


private:
	// ���b�V���ǂݍ���.
	HRESULT LoadXMesh( const char* fileName );
	// �V�F�[�_�쐬.
	HRESULT InitShader();
	// �T���v���̍쐬.
	HRESULT InitSampler();

private:
	LPDIRECT3DDEVICE9		m_pDevice9;			//Dx9���޲���޼ު��.

	ID3D11VertexShader*		m_pVertexShader;	//���_�����.
	ID3D11InputLayout*		m_pVertexLayout;	//���_ڲ���.
	ID3D11PixelShader*		m_pPixelShader;		//�߸�ټ����.
	ID3D11Buffer*			m_pCBufferPerMesh;	//�ݽ����ޯ̧(ү����).
	ID3D11Buffer*			m_pVertexBuffer;	//���_�ޯ̧.
	ID3D11Buffer**			m_ppIndexBuffer;	//���ޯ���ޯ̧.
	ID3D11SamplerState*		m_pSampleLinear;	//�����:ø����Ɋe��̨����������.


	LPD3DXMESH				m_pMesh;			//ү����޼ު��.
	DWORD					m_NumMaterials;		//��رِ�.
	MY_MATERIAL*			m_pMaterials;		//��رٍ\����.
	DWORD					m_NumAttr;			//������.
	DWORD					m_AttrID[300];		//����ID ��300�����܂�.

};

#endif	// #ifndef SKY_DOME_MODEL_H.