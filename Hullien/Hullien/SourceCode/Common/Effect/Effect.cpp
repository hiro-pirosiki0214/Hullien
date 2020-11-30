#include "Effect.h"
#include "..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\Common\SceneTexRenderer\SceneTexRenderer.h"

//�萔�錾.
//�`��p�ݽ�ݽ(���ײ�)�ő吔.
const int RENDER_SPRITE_MAX = 2000;
//�̪�ĊǗ��p�ݽ�ݽ�ő吔.
const int EFFECT_INSTANCE_MAX = 1000;

//�ݽ�׸�.
CEffect::CEffect()
	: m_pManager	( nullptr )
	, m_pRenderer	( nullptr )
	, m_pEffect		( nullptr )
#ifdef ENABLE_XAUDIO2
	, m_pSound(nullptr)
	, m_pXA2(nullptr)
	, m_pXA2Master(nullptr)
#endif//#ifdef ENABLE_XAUDIO2
{
}

CEffect::CEffect( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, const std::string& fileName )
	: CEffect	()
{
	if( FAILED( Init( pDevice11, pContext11,  fileName )) ){
		_ASSERT_EXPR( false, L"Effekseer���������s" );
	}
}

//�޽�׸�.
CEffect::~CEffect()
{
	Destroy();
}

//������.
HRESULT CEffect::Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, const std::string& fileName )
{
	if( FAILED( Create( pDevice11, pContext11 )) ){
		_ASSERT_EXPR( false, L"Effekseer�\�z���s" );
		return E_FAIL;
	}
	if( FAILED( LoadData( fileName )) ){
		_ASSERT_EXPR( false, L"Effekseer�ǂݍ��ݎ��s" );
		return E_FAIL;
	}
	return S_OK;
}

//�\�z.
HRESULT CEffect::Create( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
#ifdef ENABLE_XAUDIO2
	//XAudio2�̏��������s��.
	if (FAILED(
		XAudio2Create(&m_pXA2)))
	{
		_ASSERT_EXPR(false, "XAudio2�쐬���s");
		return E_FAIL;
	}
	if (FAILED(
		m_pXA2->CreateMasteringVoice(&m_pXA2Master)))
	{
		_ASSERT_EXPR(false, "MasteringVoice�쐬���s");
		return E_FAIL;
	}
#endif	//#ifdef ENABLE_XAUDIO2

	//�`��p�ݽ�ݽ�̐���.
	m_pRenderer
		= ::EffekseerRendererDX11::Renderer::Create(
			pDevice11, pContext11, RENDER_SPRITE_MAX );
	//�̪�ĊǗ��p�ݽ�ݽ�̐���.
	m_pManager
		= ::Effekseer::Manager::Create(EFFECT_INSTANCE_MAX);

	//�`��p�ݽ�ݽ����`��@�\��ݒ�.
	m_pManager->SetSpriteRenderer(m_pRenderer->CreateSpriteRenderer());
	m_pManager->SetRibbonRenderer(m_pRenderer->CreateRibbonRenderer());
	m_pManager->SetRingRenderer(m_pRenderer->CreateRingRenderer());
	m_pManager->SetModelRenderer(m_pRenderer->CreateModelRenderer());
	m_pManager->SetTrackRenderer(m_pRenderer->CreateTrackRenderer());

	//�`��p�ݽ�ݽ����ø����̓ǂݍ��݋@�\��ݒ�.
	//�Ǝ��g���\�A���݂�̧�ق���ǂݍ���ł���.
	m_pManager->SetTextureLoader(m_pRenderer->CreateTextureLoader());
	m_pManager->SetModelLoader(m_pRenderer->CreateModelLoader());
	m_pManager->SetMaterialLoader(m_pRenderer->CreateMaterialLoader());

#ifdef ENABLE_XAUDIO2
	//���̍Đ��p�ݽ�ݽ�̐���.
	m_pSound = ::EffekseerSound::Sound::Create(m_pXA2, 16, 16);

	//���̍Đ��p�ݽ�ݽ����Đ��@�\��ݒ�.
	m_pManager->SetSoundPlayer(m_pSound->CreateSoundPlayer());

	//���̍Đ��p�ݽ�ݽ���绳����ް��̓ǂݍ��݋@�\��ݒ�.
	//�Ǝ��g���\�A���݂�̧�ق���ǂݍ���ł���.
	m_pManager->SetSoundLoader(m_pSound->CreateSoundLoader());
#endif//#ifdef ENABLE_XAUDIO2

	return S_OK;
}


//�j��.
void CEffect::Destroy()
{
	//�̪���ް��̉��.
	ReleaseData();

	//��ɴ̪�ĊǗ��p�ݽ�ݽ��j��.
	if( m_pManager != nullptr ){
		m_pManager->Destroy();
		m_pManager = nullptr;
	}


#ifdef ENABLE_XAUDIO2
	//���ɉ��̍Đ��p�ݽ�ݽ��j��.
	m_pSound->Destroy();
#endif//#ifdef ENABLE_XAUDIO2

	//���ɕ`��p�ݽ�ݽ��j��.
	if( m_pRenderer != nullptr ){
		m_pRenderer->Destroy();
		m_pRenderer = nullptr;
	}

#ifdef ENABLE_XAUDIO2
	//XAudio2�̉��.
	if (m_pXA2Master != nullptr) {
		m_pXA2Master->DestroyVoice();
		m_pXA2Master = nullptr;
	}
	if( m_pXA2 != nullptr ){
		m_pXA2->Release();
		m_pXA2 = nullptr;
	}
#endif//#ifdef ENABLE_XAUDIO2
}


//�ް��ǂݍ���.
HRESULT CEffect::LoadData( const std::string& fileName )
{
	// �����ϊ�.
	const size_t charSize = fileName.length() + 1;	// ���͕����̃T�C�Y+1���擾.
	wchar_t* FileName = nullptr;
	FileName = new wchar_t[charSize];				// wchar_t�^��"charSize"���m��.
	size_t ret;										// �ϊ����wchar_t�̃T�C�Y�擾�p.
	mbstowcs_s( &ret, FileName, charSize, fileName.c_str(), _TRUNCATE );

	//�̪�Ă̓ǂݍ���.
	m_pEffect = Effekseer::Effect::Create( m_pManager, (const EFK_CHAR*)FileName );
	if (m_pEffect == nullptr) {
		_ASSERT_EXPR(false, L"�̪�ēǂݍ��ݎ��s");
		return E_FAIL;
	}

	delete[] FileName;

	return S_OK;
}

//�ް����.
void CEffect::ReleaseData()
{
	//�̪�Ẳ��.
	if( m_pEffect != nullptr ){
		m_pEffect->Release();
		m_pEffect = nullptr;
	}
}

//�`��֐�.
void CEffect::Render( const Effekseer::Handle& eHandle )
{
	if( eHandle == -1 ) return;

	D3DXMATRIX view = CCameraManager::GetViewMatrix();
	D3DXMATRIX Proj = CCameraManager::GetProjMatrix();

	//�ޭ��s���ݒ�.
	SetViewMatrix(view);

	//��ۼު���ݍs���ݒ�.
	SetProjectionMatrix(Proj);

	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::Trans ){
		//�̪�Ă̍X�V����.
		m_pManager->BeginUpdate();
		m_pManager->UpdateHandle( eHandle );
		m_pManager->EndUpdate();
	}
	if( CSceneTexRenderer::GetRenderPass() != CSceneTexRenderer::ERenderPass::Shadow ){
		//�̪�Ă̕`��J�n����.
		m_pRenderer->BeginRendering();
		m_pManager->DrawHandle( eHandle );
		m_pRenderer->EndRendering();
	}	
}


//DirectX D3DXVECTOR3 -> Effekseer Vector3 �ɕϊ�����.
Effekseer::Vector3D	CEffect::ConvertToVec3Efk( const D3DXVECTOR3& SrcVec3Dx ) const
{
	return Effekseer::Vector3D(
		SrcVec3Dx.x, SrcVec3Dx.y, SrcVec3Dx.z);
}

//Effekseer Vector3 -> DirectX D3DXVECTOR3 �ɕϊ�����.
D3DXVECTOR3 CEffect::ConvertToVec3Dx( const Effekseer::Vector3D& SrcVec3Efk) const
{
	return D3DXVECTOR3(SrcVec3Efk.X, SrcVec3Efk.Y, SrcVec3Efk.Z);
}

//DirectX D3DXMATRIX -> Effekseer Matrix44 �ɕϊ�����.
Effekseer::Matrix44 CEffect::ConvertToMatEfk( const D3DXMATRIX & SrcMatDx) const
{
	Effekseer::Matrix44 DestMatEfk;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			DestMatEfk.Values[i][j] = SrcMatDx.m[i][j];
		}
	}

	return DestMatEfk;
}

//Effekseer Matrix44 -> DirectX D3DXMATRIX �ɕϊ�����.
D3DXMATRIX CEffect::ConvertToMatDx( const Effekseer::Matrix44& SrcMatEfk ) const
{
	D3DXMATRIX DestMatDx;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			DestMatDx.m[i][j] = SrcMatEfk.Values[i][j];
		}
	}

	return DestMatDx;
}
//�ޭ��s���ݒ�.
void CEffect::SetViewMatrix(D3DXMATRIX& mView)
{
	Effekseer::Matrix44 mViewEfk = ConvertToMatEfk(mView);

	//��׍s���ݒ�.
	m_pRenderer->SetCameraMatrix(mViewEfk);
}

//��ۼު���ݍs���ݒ�.
void CEffect::SetProjectionMatrix(D3DXMATRIX& mProj)
{
	Effekseer::Matrix44 mProjEfk = ConvertToMatEfk(mProj);

	//��ۼު���ݍs���ݒ�.
	m_pRenderer->SetProjectionMatrix(mProjEfk);
}
