#include "Dx9StaticMesh.h"
#include "..\..\..\Camera\Camera.h"
#include "..\..\..\Light\LightManager\LightManager.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\D3DX\D3DX11.h"
#include "..\..\Shader\ShadowMap\ShadowMap.h"
#include "..\..\Shader\TranslucentShader\TranslucentShader.h"
#include "..\..\SceneTexRenderer\SceneTexRenderer.h"
#include "..\..\Fog\Fog.h"
#include <crtdbg.h>	//_ASSERTϸۂŕK�v.

// �V�F�[�_�[��.
const char SHADER_NAME[] = "Data\\Shader\\Mesh.hlsl";

CDX9StaticMesh::CDX9StaticMesh()
	: m_hWnd				( nullptr )
	, m_pDevice9			( nullptr )
	, m_pVertexShader		( nullptr )
	, m_pVertexLayout		( nullptr )
	, m_pPixelShader		( nullptr )
	, m_pCBufferPerMesh		( nullptr )
	, m_pCBufferPerMaterial	( nullptr )
	, m_pCBufferPerFrame	( nullptr )
	, m_pVertexBuffer		( nullptr )
	, m_ppIndexBuffer		( nullptr )
	, m_pSampleLinear		( nullptr )
	, m_pToonSampleLinear	( nullptr )
	, m_pShadowMapSampler	( nullptr )
	, m_pMesh				( nullptr )
	, m_pMeshForRay			( nullptr )
	, m_NumMaterials		( 0 )
	, m_pToonTexture		( nullptr )
	, m_pFogTexture			( nullptr )
	, m_pMaterials			( nullptr )
	, m_NumAttr				( 0 )
	, m_AttrID				()
	, m_EnableTexture		( false )
	, m_IsShadow			( false )
{
}

CDX9StaticMesh::~CDX9StaticMesh()
{
	// �������.
	Release();
}


HRESULT CDX9StaticMesh::Init(
	HWND hWnd, 
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11, 
	LPDIRECT3DDEVICE9 pDevice9,
	const char* fileName )
{
	m_hWnd = hWnd;
	m_pDevice9 = pDevice9;
	if( FAILED( InitPram( pDevice11, pContext11 )) )	return E_FAIL;
	if( FAILED( LoadXMesh( fileName )) )				return E_FAIL; 
	if( FAILED( InitShader()) )							return E_FAIL;
	if( FAILED( CreateCBuffer( &m_pCBufferPerMesh,		sizeof(CBUFFER_PER_MESH) ) ))		return E_FAIL;
	if( FAILED( CreateCBuffer( &m_pCBufferPerMaterial,	sizeof(CBUFFER_PER_MATERIAL) ) ))	return E_FAIL;
	if( FAILED( CreateCBuffer( &m_pCBufferPerFrame,		sizeof(CBUFFER_PER_FRAME) ) ))		return E_FAIL;

	return S_OK;
}

// ���b�V���ǂݍ���.
HRESULT CDX9StaticMesh::LoadXMesh(const char* fileName)
{
	// �}�e���A���o�b�t�@.
	LPD3DXBUFFER pD3DXMtrlBuffer = nullptr;

	// X�t�@�C���̓ǂݍ���.
	if( FAILED( D3DXLoadMeshFromX(
		fileName,			// �t�@�C����.
		D3DXMESH_SYSTEMMEM	// �V�X�e���������ɓǂݍ���.
		| D3DXMESH_32BIT,	// 32bit.
		m_pDevice9,
		nullptr,
		&pD3DXMtrlBuffer,	// (out)�}�e���A�����.
		nullptr,
		&m_NumMaterials,	// (out)�}�e���A����.
		&m_pMesh ))){		// (out)���b�V���I�u�W�F�N�g.
		_ASSERT_EXPR(false, L"X̧�ٓǍ����s");
		return E_FAIL;
	}

	//X̧�ق�۰��(ڲ�Ƃ̔���p�ɕʐݒ�œǂݍ���).
	if( FAILED( D3DXLoadMeshFromX(
		fileName,			// �t�@�C����.
		D3DXMESH_SYSTEMMEM,	// �V�X�e���������ɓǂݍ���.
		m_pDevice9,
		nullptr,
		&pD3DXMtrlBuffer,	// (out)�}�e���A�����.
		nullptr,
		&m_NumMaterials,	// (out)�}�e���A����.
		&m_pMeshForRay ))){	// (out)���b�V���I�u�W�F�N�g.
		_ASSERT_EXPR(false, L"X̧�ٓǍ����s");
		return E_FAIL;
	}

	D3D11_BUFFER_DESC		bd;			// Dx11�o�b�t�@�\����.
	D3D11_SUBRESOURCE_DATA	InitData;	// �������f�[�^.
	// �ǂݍ��񂾏�񂩂�K�v�ȏ��𔲂��o��.
	D3DXMATERIAL* d3dxMaterials
		= static_cast<D3DXMATERIAL*>(pD3DXMtrlBuffer->GetBufferPointer());
	// �}�e���A�������̗̈���m��.
	m_pMaterials = new MY_MATERIAL[m_NumMaterials]();
	m_ppIndexBuffer = new ID3D11Buffer*[m_NumMaterials]();
	// �}�e���A�������J��Ԃ�.
	for( DWORD No = 0; No < m_NumMaterials; No++ ){
		// �C���f�b�N�X�o�b�t�@�̏�����.
		m_ppIndexBuffer[No] = nullptr;

		// �}�e���A�����̃R�s�[.
		// �A���r�G���g.
		m_pMaterials[No].Ambient.x = d3dxMaterials[No].MatD3D.Ambient.r;
		m_pMaterials[No].Ambient.y = d3dxMaterials[No].MatD3D.Ambient.g;
		m_pMaterials[No].Ambient.z = d3dxMaterials[No].MatD3D.Ambient.b;
		m_pMaterials[No].Ambient.w = d3dxMaterials[No].MatD3D.Ambient.a;
		// �f�B�t���[�Y.
		m_pMaterials[No].Diffuse.x = d3dxMaterials[No].MatD3D.Diffuse.r;
		m_pMaterials[No].Diffuse.y = d3dxMaterials[No].MatD3D.Diffuse.g;
		m_pMaterials[No].Diffuse.z = d3dxMaterials[No].MatD3D.Diffuse.b;
		m_pMaterials[No].Diffuse.w = d3dxMaterials[No].MatD3D.Diffuse.a;
		// �X�y�L����.
		m_pMaterials[No].Specular.x = d3dxMaterials[No].MatD3D.Specular.r;
		m_pMaterials[No].Specular.y = d3dxMaterials[No].MatD3D.Specular.g;
		m_pMaterials[No].Specular.z = d3dxMaterials[No].MatD3D.Specular.b;
		m_pMaterials[No].Specular.w = d3dxMaterials[No].MatD3D.Specular.a;

		// (���̖ʂ�)�e�N�X�`�����\���Ă��邩�H.
		if( d3dxMaterials[No].pTextureFilename != nullptr &&
			lstrlen(d3dxMaterials[No].pTextureFilename) > 0 ){
			// �e�N�X�`������̃t���O�𗧂Ă�.
			m_EnableTexture = true;

			char path[128] = "";
			int path_count = lstrlen(fileName);
			for( int k = path_count; k >= 0; k-- ){
				if( fileName[k] == '\\' ){
					for( int j = 0; j <= k; j++ ){
						path[j] = fileName[j];
					}
					path[k + 1] = '\0';
					break;
				}
			}
			// �p�X�ƃe�N�X�`������A��.
			strcat_s( path, sizeof(path), d3dxMaterials[No].pTextureFilename );

			//�@�e�N�X�`�������R�s�[.
			strcpy_s( m_pMaterials[No].szTextureName, sizeof(m_pMaterials[No].szTextureName), path );

			// �e�N�X�`���쐬.
			if( FAILED( D3DX11CreateShaderResourceViewFromFile(
				m_pDevice11,
				m_pMaterials[No].szTextureName,	// �e�N�X�`���t�@�C����.
				nullptr,
				nullptr,
				&m_pMaterials[No].pTexture,		// (out)�e�N�X�`���I�u�W�F�N�g.
				nullptr ))){
				_ASSERT_EXPR(false, L"ø����쐬���s");
				return E_FAIL;
			}
		}
	}
	// �g�D�[���e�N�X�`���쐬.
	if( FAILED( D3DX11CreateShaderResourceViewFromFile(
		m_pDevice11,
		"Data\\Mesh\\toon.png",	// �e�N�X�`���t�@�C����.
		nullptr,
		nullptr,
		&m_pToonTexture,		// (out)�e�N�X�`���I�u�W�F�N�g.
		nullptr ))){
		_ASSERT_EXPR(false, L"ø����쐬���s");
		return E_FAIL;
	}
	// �t�H�O�e�N�X�`���쐬.
	if( FAILED( D3DX11CreateShaderResourceViewFromFile(
		m_pDevice11, 
		"Data\\Mesh\\Fog.png",	// �e�N�X�`���t�@�C����.
		nullptr,
		nullptr,
		&m_pFogTexture,			// (out)�e�N�X�`���I�u�W�F�N�g.
		nullptr ))){
		_ASSERT_EXPR(false, L"ø����쐬���s");
		return E_FAIL;
	}
	//------------------------------------------------
	//	�C���f�b�N�X�o�b�t�@�쐬.
	//------------------------------------------------
	// ���b�V���̑������𓾂�.
	//	�������ŃC���f�b�N�X�o�b�t�@����ׂ����}�e���A�����Ƃ̃C���f�b�N�X�o�b�t�@�𕪗��ł���.
	D3DXATTRIBUTERANGE* pAttrTable = nullptr;

	// ���b�V���̖ʂ���ђ��_�̏��ԕύX�𐧌䂵�A�p�t�H�[�}���X���œK������.
	//	D3DXMESHOPT_COMPACT : �ʂ̏��Ԃ�ύX���A�g�p����Ă��Ȃ����_�Ɩʂ��폜����.
	//	D3DXMESHOPT_ATTRSORT : �p�t�H�[�}���X���グ��ׁA�ʂ̏��Ԃ�ύX���čœK�����s��.
	m_pMesh->OptimizeInplace( D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT,
		nullptr, nullptr, nullptr, nullptr );
	// �����e�[�u���̎擾.
	m_pMesh->GetAttributeTable( nullptr, &m_NumAttr );
	pAttrTable = new D3DXATTRIBUTERANGE[m_NumAttr];
	if( FAILED( m_pMesh->GetAttributeTable( pAttrTable, &m_NumAttr )) ){
		_ASSERT_EXPR(false, L"����ð��َ擾���s");
		return E_FAIL;
	}

	// ������Lock���Ȃ��Ǝ��o���Ȃ�.
	int* pIndex = nullptr;
	m_pMesh->LockIndexBuffer( D3DLOCK_READONLY, (void**)&pIndex );
	// �������Ƃ̃C���f�b�N�X�o�b�t�@���쐬.
	for( DWORD No = 0; No < m_NumAttr; No++ ){
		m_AttrID[No] = pAttrTable[No].AttribId;
		// Dx9�̃C���f�b�N�X�o�b�t�@����̏��ŁA
		//	Dx11�̃C���f�b�N�X�o�b�t�@���쐬.
		bd.Usage			= D3D11_USAGE_DEFAULT;
		bd.ByteWidth		= sizeof(int)*pAttrTable[No].FaceCount*3;//�ʐ��~3�Œ��_��.
		bd.BindFlags		= D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags	= 0;
		bd.MiscFlags		= 0;
		// �傫���C���f�b�N�X�o�b�t�@���̃I�t�Z�b�g(�~3����).
		InitData.pSysMem = &pIndex[pAttrTable[No].FaceStart*3];

		// �C���f�b�N�X�o�b�t�@�̍쐬.
		if( FAILED( m_pDevice11->CreateBuffer(
			&bd, 
			&InitData,
			&m_ppIndexBuffer[No] ))){
			_ASSERT_EXPR(false, L"���ޯ���ޯ̧�쐬���s");
			return E_FAIL;
		}
		// �ʂ̐����R�s�[.
		m_pMaterials[m_AttrID[No]].dwNumFace = pAttrTable[No].FaceCount;
	}
	// �����e�[�u���̍폜.
	SAFE_DELETE( pAttrTable );
	// �g�p�ς݂̃C���f�b�N�X�o�b�t�@�̉��.
	m_pMesh->UnlockIndexBuffer();
	// �s�v�ɂȂ����}�e���A���o�b�t�@�����.
	SAFE_RELEASE( pD3DXMtrlBuffer );

	//------------------------------------------------
	//	���_�o�b�t�@�̍쐬.
	//------------------------------------------------
	// Dx9�̏ꍇ�Amap�ł͂Ȃ�Lock�Œ��_�o�b�t�@����f�[�^�����o��.
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
	m_pMesh->GetVertexBuffer( &pVB );
	DWORD dwStride = m_pMesh->GetNumBytesPerVertex();
	BYTE* pVertices = nullptr;
	VERTEX* pVertex = nullptr;
	if( SUCCEEDED( pVB->Lock(0, 0, (VOID**)&pVertices, 0 ))){
		pVertex				= (VERTEX*)pVertices;
		// Dx9�̒��_�o�b�t�@����̏��ŁADx11���_�o�b�t�@���쐬.
		bd.Usage			= D3D11_USAGE_DEFAULT;
		// ���_���i�[����̂ɕK�v�ȃo�C�g��.
		bd.ByteWidth		= m_pMesh->GetNumBytesPerVertex()*m_pMesh->GetNumVertices();
		bd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags	= 0;
		bd.MiscFlags		= 0;
		InitData.pSysMem = pVertex;
		if( FAILED( m_pDevice11->CreateBuffer(
			&bd,
			&InitData,
			&m_pVertexBuffer ))){
			_ASSERT_EXPR(false, L"���_�ޯ̧�쐬���s");
			return E_FAIL;
		}
		pVB->Unlock();
	}
	SAFE_RELEASE( pVB );	// ���_�o�b�t�@���.


	// �e�N�X�`���p�̃T���v���\����.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter		= D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// ���j�A�t�B���^(���`���).
															// POINT:���������e��.
	// ���b�s���O���[�h.
	samDesc.AddressU	= D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressV	= D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW	= D3D11_TEXTURE_ADDRESS_WRAP;
	// WRAP		: �J��Ԃ�.
	// MIRROR	: ���]�J��Ԃ�.
	// CLAMP	: �[�̖͗l�������L�΂�.
	// BORDE	: �ʓr���E�F�����߂�.
	// �T���v���쐬.
	if( FAILED( m_pDevice11->CreateSamplerState(
		&samDesc,
		&m_pSampleLinear ))){	// (out)�T���v��.
		_ASSERT_EXPR(false, L"����׍쐬���s");
		return E_FAIL;
	}

	// �g�D�[���e�N�X�`���p.
	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	if( FAILED( m_pDevice11->CreateSamplerState(
		&samDesc,
		&m_pToonSampleLinear))){
		_ASSERT_EXPR(false, L"����׍쐬���s");
		return E_FAIL;
	}

	// �V���h�E�}�b�v�p.
	samDesc.Filter		= D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU	= D3D11_TEXTURE_ADDRESS_BORDER;
	samDesc.AddressV	= D3D11_TEXTURE_ADDRESS_BORDER;
	samDesc.AddressW	= D3D11_TEXTURE_ADDRESS_BORDER;
	samDesc.BorderColor[0] = 1.0f;
	samDesc.BorderColor[1] = 1.0f;
	samDesc.BorderColor[2] = 1.0f;
	samDesc.BorderColor[3] = 1.0f;
	samDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	samDesc.MaxAnisotropy = 1;
	samDesc.MipLODBias = 0.0f;
	samDesc.MinLOD = 0.0f;
	samDesc.MinLOD = D3D11_FLOAT32_MAX;
	// �T���v���쐬.
	if( FAILED( m_pDevice11->CreateSamplerState(
		&samDesc, &m_pShadowMapSampler ))){
		_ASSERT_EXPR( false, "�T���v���[�쐬���s" );
		MessageBox( nullptr, "�T���v���[�쐬���s", "Warning", MB_OK );
		return E_FAIL;
	}

	return S_OK;
}

//����֐�.
void CDX9StaticMesh::Release()
{
	//���ޯ���ޯ̧���.
	if( m_ppIndexBuffer != nullptr ){
		for( int No = m_NumMaterials-1; No >= 0; No-- ){
			SAFE_RELEASE( m_ppIndexBuffer[No] );
		}
		delete[] m_ppIndexBuffer;
		m_ppIndexBuffer = nullptr;
	}
	SAFE_DELETE_ARRAY(m_pMaterials);
	SAFE_RELEASE( m_pMesh );
	SAFE_RELEASE( m_pShadowMapSampler );
	SAFE_RELEASE( m_pToonSampleLinear );
	SAFE_RELEASE( m_pFogTexture );
	SAFE_RELEASE( m_pToonTexture );
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pCBufferPerMaterial );
	SAFE_RELEASE( m_pCBufferPerMesh );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pVertexShader );

	m_pDevice9 = nullptr;
	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
	m_hWnd = nullptr;
}

//===========================================================
//	HLSĻ�ق�ǂݍ��ݼ���ނ��쐬����.
//	HLSL: High Level Shading Language �̗�.
//===========================================================
HRESULT CDX9StaticMesh::InitShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif	// #ifdef _DEBUG.

	// ���_�V�F�[�_�[�̃u���u�쐬.
	if( m_EnableTexture == true ){
		if( FAILED( D3DX11CompileFromFile(
			SHADER_NAME,
			nullptr,
			nullptr,
			"VS_Main",
			"vs_5_0",
			uCompileFlag,
			0,
			nullptr,
			&pCompiledShader,
			&pErrors,
			nullptr ))){
			_ASSERT_EXPR(false, L"hlsl�ǂݍ��ݎ��s");
			return E_FAIL;
		}
	} else {
		if( FAILED( D3DX11CompileFromFile(
			SHADER_NAME,
			nullptr,
			nullptr,
			"VS_NoTex",
			"vs_5_0",
			uCompileFlag,
			0,
			nullptr,
			&pCompiledShader,
			&pErrors, nullptr ))){
			_ASSERT_EXPR(false, L"hlsl�ǂݍ��ݎ��s");
			return E_FAIL;
		}
	}
	
	// ��L�ō쐬�����u���u����u���_�V�F�[�_�[�v���쐬.
	if( FAILED(
		m_pDevice11->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pVertexShader))){	// (out)���_�V�F�[�_�[.
		_ASSERT_EXPR(false, L"�ްï������ލ쐬���s");
		return E_FAIL;
	}

	// ���_�C���v�b�g���C�A�E�g���`.
	D3D11_INPUT_ELEMENT_DESC layout[3];
	// ���_�C���v�b�g�̔z��v�f�����Z�o.
	UINT numElements = 0;
	if( m_EnableTexture == true ){
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0, 0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"NORMAL",  0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,   0,24,D3D11_INPUT_PER_VERTEX_DATA,0}
		};
		numElements = sizeof(tmp) / sizeof(tmp[0]);	// �v�f���Z�o.
		memcpy_s( layout, sizeof(layout), tmp, sizeof(D3D11_INPUT_ELEMENT_DESC)*numElements );
	} else {
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0, 0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"NORMAL",  0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0}
		};
		numElements = sizeof(tmp) / sizeof(tmp[0]);	//�v�f���Z�o.
		memcpy_s( layout, sizeof(layout), tmp, sizeof(D3D11_INPUT_ELEMENT_DESC)*numElements );
	}

	// ���_�C���v�b�g���C�A�E�g���쐬.
	if( FAILED( m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout ))){	//(out)���_�C���v�b�g���C�A�E�g.
		_ASSERT_EXPR(false, L"���_���߯�ڲ��č쐬���s");
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );

	// HLSL����s�N�Z���V�F�[�_�[�̃u���u���쐬.
	if( m_EnableTexture == true ){
		if( FAILED( D3DX11CompileFromFile(
			SHADER_NAME,
			nullptr,
			nullptr,
			"PS_Main",
			"ps_5_0",
			uCompileFlag,
			0,
			nullptr,
			&pCompiledShader,
			&pErrors,
			nullptr ))){
			_ASSERT_EXPR(false, L"hlsl�ǂݍ��ݎ��s");
			return E_FAIL;
		}
	} else {
		if( FAILED( D3DX11CompileFromFile(
			SHADER_NAME,
			nullptr,
			nullptr,
			"PS_NoTex",
			"ps_5_0",
			uCompileFlag,
			0,
			nullptr,
			&pCompiledShader,
			&pErrors,
			nullptr ))){
			_ASSERT_EXPR(false, L"hlsl�ǂݍ��ݎ��s");
			return E_FAIL;
		}
	}
	SAFE_RELEASE(pErrors);

	// ��L�ō쐬�����u���u����u�s�N�Z���V�F�[�_�[�v���쐬.
	if( FAILED( m_pDevice11->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pPixelShader ))){	// (out)�s�N�Z���V�F�[�_�[.
		_ASSERT_EXPR(false, L"�߸�ټ���ލ쐬���s");
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	return S_OK;
}

//�R���X�^���g�o�b�t�@�쐬�֐�.
HRESULT CDX9StaticMesh::CreateCBuffer( ID3D11Buffer** pConstantBuffer, UINT size )
{
	D3D11_BUFFER_DESC cb;

	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = size;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if( FAILED( m_pDevice11->CreateBuffer(
		&cb, 
		nullptr,
		pConstantBuffer ))){
		return E_FAIL;
	}
	return S_OK;
}

// �����_�����O�p.
void CDX9StaticMesh::Render( const bool& isTrans )
{
	// ���[���h�s��A�X�P�[���s��A��]�s��A���s�ړ��s��.
	D3DXMATRIX mWorld, mScale, mRot, mTran;
	D3DXMATRIX mYaw, mPitch, mRoll;

	// �g��k���s��쐬.
	D3DXMatrixScaling(
		&mScale,
		m_vScale.x, m_vScale.y, m_vScale.z );

	// Y����]�s��쐬.
	D3DXMatrixRotationY( &mYaw, m_vRot.y );
	// X����]�s��쐬.
	D3DXMatrixRotationX( &mPitch, m_vRot.x );
	// Z����]�s��쐬.
	D3DXMatrixRotationZ( &mRoll, m_vRot.z );
	// ��]�s����쐬.
	mRot = mYaw * mPitch * mRoll;

	// ���s�ړ��s��쐬.
	D3DXMatrixTranslation(
		&mTran,
		m_vPos.x, m_vPos.y, m_vPos.z );

	// ���[���h�s��쐬.
	// �g�k�~��]�~�ړ� �����Ԃ��ƂĂ���؁I�I.
	mWorld = mScale * mRot * mTran;

	// �e��`�悵����I��.
	if( ShadowRender( mWorld ) == true ) return;
	if( isTrans == false ){
		if( TranslucentRender( mWorld ) == true ) return;
	}

	// �g�p����V�F�[�_�[�̃Z�b�g.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	// ���_�V�F�[�_�[.
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	// �s�N�Z���V�F�[�_�[.
	// �T���v���̃Z�b�g.
	m_pContext11->PSSetSamplers( 1, 1, &m_pToonSampleLinear );
	m_pContext11->PSSetSamplers( 2, 1, &m_pShadowMapSampler );

	// �R���X�^���g�o�b�t�@.
	CBUFFER_PER_FRAME cb;

	// �J�X�P�[�h�̐��������[�v.
	for( int i = 0; i < CSceneTexRenderer::MAX_CASCADE; i++ ){
		const float f = isTrans == false ? 1.0f : 0.0f;
		// ���C�g�̍s���n��.
		cb.mLightWVP[i] = (mWorld * CLightManager::GetShadowVP()[i]) * f;
		D3DXMatrixTranspose( &cb.mLightWVP[i], &cb.mLightWVP[i] );
		// �e�e�N�X�`����n��.
		m_pContext11->PSSetShaderResources( i+1, 1, &CSceneTexRenderer::GetShadowBuffer()[i] );
	}
	// �V�F�[�_�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	// �o�b�t�@���̃f�[�^�̏��������J�n����Map.
	if( SUCCEEDED( m_pContext11->Map(
		m_pCBufferPerFrame, 0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&pData))){

		// �F�̃Z�b�g.
		cb.vColor = m_vColor;

		// �J�����ʒu.
		cb.vCamPos = { CCameraManager::GetPosition(), 0.0f };

		// ���C�g�ʒu.
		cb.vCamPos = { CLightManager::GetPosition(), 0.0f };
		// ���C�g����.
		cb.vLightDir = { CLightManager::GetDirection(), 0.0f };
		D3DXVec4Normalize(&cb.vLightDir, &cb.vLightDir);
		// ���C�g��]�s��.
		cb.mLightRot = CLightManager::GetRorarionMatrix();

		// �J�X�P�[�h�̊Ԋu����n��.
		cb.SpritPos.x = CLightManager::GetSpritPos()[0];
		cb.SpritPos.y = CLightManager::GetSpritPos()[1];
		cb.SpritPos.z = CLightManager::GetSpritPos()[2];
		cb.SpritPos.w = CLightManager::GetSpritPos()[3];

		// ���C�g���x(���邳).
		cb.fIntensity.x = CLightManager::GetIntensity();

		// �������g�ɉe�𗎂Ƃ���.
		cb.IsShadow.x = m_IsShadow;

		// �t�H�O�̃e�N�X�`�����W.
		cb.Fog = CFog::GetFog();

		memcpy_s(
			pData.pData,	// �R�s�[��̃o�t�@.
			pData.RowPitch,	// �R�s�[��̃o�t�@�T�C�Y.
			(void*)(&cb),	// �R�s�[���̃o�t�@.
			sizeof(cb));	// �R�s�[���̃o�t�@�T�C�Y.

		// �o�b�t�@���̃f�[�^�̏��������I������Unmap.
		m_pContext11->Unmap( m_pCBufferPerFrame, 0 );
	}

	// �R���X�^���g�o�b�t�@�̃Z�b�g.
	m_pContext11->VSSetConstantBuffers( 2, 1, &m_pCBufferPerFrame );
	m_pContext11->PSSetConstantBuffers( 2, 1, &m_pCBufferPerFrame );

	// �g�D�[���}�b�v�e�N�X�`����n��.
	m_pContext11->PSSetShaderResources( 5, 1, &m_pToonTexture );
	// �t�H�O�e�N�X�`����n��.
	m_pContext11->PSSetShaderResources( 6, 1, &m_pFogTexture );
	// ���b�V���̃����_�����O.
	RenderMesh( mWorld, CCameraManager::GetViewMatrix(), CCameraManager::GetProjMatrix(), isTrans );
}

// �����_�����O�֐�(�׽���ł̂ݎg�p����).
void CDX9StaticMesh::RenderMesh(
	D3DXMATRIX& mWorld, 
	const D3DXMATRIX& mView, 
	const D3DXMATRIX& mProj,
	const bool& isTrans )
{
	// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	// �o�b�t�@���̃f�[�^�̏��������J�n����Map.
	if( SUCCEEDED( m_pContext11->Map(
		m_pCBufferPerMesh,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&pData ))){
		CBUFFER_PER_MESH cb;	// �R���X�^���g�o�b�t�@.

		// ���[���h�s���n��.
		const float f = isTrans == false ? 1.0f : 0.0f;
		cb.mW = mWorld * f;
		D3DXMatrixTranspose( &cb.mW, &cb.mW );

		// ���[���h, �r���[, �v���W�F�N�V�����s���n��.
		D3DXMATRIX mWVP = mWorld * mView * mProj;
		D3DXMatrixTranspose( &mWVP, &mWVP );	// �s���]�u����.
		// ���s��̌v�Z���@��DirectX��GPU�ňقȂ邽�ߓ]�u���K�v.
		cb.mWVP = mWVP;

		memcpy_s(
			pData.pData,	// �R�s�[��̃o�b�t�@.
			pData.RowPitch,	// �R�s�[��̃o�b�t�@�T�C�Y.
			(void*)(&cb),	// �R�s�[���̃o�b�t�@.
			sizeof(cb));	// �R�s�[���̃o�b�t�@�T�C�Y.

		// �o�b�t�@���̃f�[�^�̏��������I������Unmap.
		m_pContext11->Unmap(m_pCBufferPerMesh, 0);
	}

	// �R���X�^���g�o�b�t�@�̐ݒ�.
	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pCBufferPerMesh );
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pCBufferPerMesh );

	// ���_�C���v�b�g���C�A�E�g���Z�b�g.
	m_pContext11->IASetInputLayout( m_pVertexLayout );

	// �v���~�e�B�u�E�g�|���W�[���Z�b�g.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// ���_�o�b�t�@���Z�b�g.
	UINT stride = m_pMesh->GetNumBytesPerVertex();
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer, &stride, &offset);

	// �����̐������A���ꂼ��̑����̃C���f�b�N�X�o�b�t�@��`��.
	for( DWORD No = 0; No < m_NumAttr; No++ ){
		// �g�p����Ă��Ȃ��}�e���A���΍�.
		if( m_pMaterials[m_AttrID[No]].dwNumFace == 0 ) continue;

		// �C���f�b�N�X�o�b�t�@���Z�b�g.
		m_pContext11->IASetIndexBuffer( m_ppIndexBuffer[No], DXGI_FORMAT_R32_UINT, 0 );
		// �}�e���A���̊e�v�f���V�F�[�_�[�ɓn��.
		D3D11_MAPPED_SUBRESOURCE pDataMat;
		if( SUCCEEDED( m_pContext11->Map(
			m_pCBufferPerMaterial,
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&pDataMat ))){
			CBUFFER_PER_MATERIAL cb;
			// �A���r�G���g, �f�B�q���[�Y, �X�y�L�������V�F�[�_�[�ɓn��.
			cb.vAmbient		= m_pMaterials[m_AttrID[No]].Ambient;
			cb.vDiffuse		= m_pMaterials[m_AttrID[No]].Diffuse;
			cb.vSpecular	= m_pMaterials[m_AttrID[No]].Specular;

			memcpy_s( pDataMat.pData, pDataMat.RowPitch, (void*)&cb, sizeof(cb) );
			m_pContext11->Unmap( m_pCBufferPerMaterial, 0 );
		}

		// �R���X�^���g�o�b�t�@�̃Z�b�g.
		m_pContext11->VSSetConstantBuffers(1, 1, &m_pCBufferPerMaterial);
		m_pContext11->PSSetConstantBuffers(1, 1, &m_pCBufferPerMaterial);

		// �e�N�X�`�����V�F�[�_�[�ɓn��.
		if( m_pMaterials[m_AttrID[No]].pTexture != nullptr ){
			// �e�N�X�`��������Ƃ�.
			m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );
			m_pContext11->PSSetShaderResources( 0, 1, &m_pMaterials[m_AttrID[No]].pTexture );
		} else {
			// �e�N�X�`�����Ȃ��Ƃ�.
			ID3D11ShaderResourceView* pNothing[1] = { 0 };
			m_pContext11->PSSetShaderResources( 0, 1, pNothing );
		}

		// �|���S���������_�C�����O.
		m_pContext11->DrawIndexed( m_pMaterials[m_AttrID[No]].dwNumFace*3, 0, 0);
	}
}

// �e�̕`��.
bool CDX9StaticMesh::ShadowRender( const D3DXMATRIX& mWorld )
{
	if( CSceneTexRenderer::GetRenderPass() != CSceneTexRenderer::ERenderPass::Shadow ) return false;

	// �J�X�P�[�h�����[�v.
	for( int i = 0; i < CSceneTexRenderer::MAX_CASCADE; i++ ){
		CSceneTexRenderer::SetShadowBuffer( i );
		CShadowMap::SetConstantBufferData( mWorld*CLightManager::GetShadowVP()[i] );
		// ���_�o�b�t�@���Z�b�g.
		UINT stride = m_pMesh->GetNumBytesPerVertex();
		UINT offset = 0;
		m_pContext11->IASetVertexBuffers(
			0, 1, &m_pVertexBuffer, &stride, &offset);
		// �v���~�e�B�u�E�g�|���W�[���Z�b�g.
		m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		// �����̐������A���ꂼ��̑����̃C���f�b�N�X�o�b�t�@��`��.
		for( DWORD No = 0; No < m_NumAttr; No++ ){
			// �C���f�b�N�X�o�b�t�@���Z�b�g.
			m_pContext11->IASetIndexBuffer( m_ppIndexBuffer[No], DXGI_FORMAT_R32_UINT, 0 );
			// �|���S���������_�����O.
			m_pContext11->DrawIndexed( m_pMaterials[m_AttrID[No]].dwNumFace*3, 0, 0 );
		}
	}

	return true;
}

// �������̕`��,
bool CDX9StaticMesh::TranslucentRender( const D3DXMATRIX& mWorld )
{
	if( CSceneTexRenderer::GetRenderPass() != CSceneTexRenderer::ERenderPass::Trans ) return false;

	CTranslucentShader::SetConstantBufferData( mWorld*CCameraManager::GetViewMatrix()*CCameraManager::GetProjMatrix() );
	// ���_�o�b�t�@���Z�b�g.
	UINT stride = m_pMesh->GetNumBytesPerVertex();
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	// �v���~�e�B�u�E�g�|���W�[���Z�b�g.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	// �����̐������A���ꂼ��̑����̃C���f�b�N�X�o�b�t�@��`��.
	for( DWORD No = 0; No < m_NumAttr; No++ ){
		// �C���f�b�N�X�o�b�t�@���Z�b�g.
		m_pContext11->IASetIndexBuffer( m_ppIndexBuffer[No], DXGI_FORMAT_R32_UINT, 0 );
		// �|���S���������_�����O.
		m_pContext11->DrawIndexed( m_pMaterials[m_AttrID[No]].dwNumFace*3, 0, 0 );
	}
	return true;
}