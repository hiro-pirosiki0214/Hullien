#include "..\SceneList.h"
#include "..\..\..\GameObject\Widget\SceneWidget\ClearWidget\ClearWidget.h"
#include "..\..\..\GameObject\STG\STGManager\STGManager.h"
#include "..\..\..\GameObject\Widget\Fade\Fade.h"
#include "..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\XAudio2\SoundManager.h"
#include "..\..\..\Common\SceneTexRenderer\SceneTexRenderer.h"
#include "..\..\..\Common\D3DX\D3DX11.h"

#define ENDING_STG // Clear����STG�����邩.

CGameClear::CGameClear( CSceneManager* pSceneManager )
	: CSceneBase			( pSceneManager )
	, m_pClearWidget		( nullptr )
	, m_pSTGManager			( nullptr )
	, m_IsChangeScene		( false )
	, m_SkipWaitCount		( 0 )
{
	m_pClearWidget	= std::make_unique<CClearWidget>();
	m_pSTGManager	= std::make_unique<CSTGManager>();

	CFade::SetFadeOut();
}

CGameClear::~CGameClear()
{
}

//============================.
//	�Ǎ��֐�.
//============================.
bool CGameClear::Load()
{
	if ( m_pClearWidget->Init() == false ) return false;
	if( m_pSTGManager->Init() == false ) return false;

	CSoundManager::ThreadPlayBGM("ClearBGM");
	CSoundManager::FadeInBGM("ClearBGM");
	m_pSceneManager->SetNowBGMName("ClearBGM");
	return true;
}

//============================.
//	�X�V�֐�.
//============================.
void CGameClear::Update()
{

	if (CFade::GetFadeState() == CFade::EFadeState::Out 
		&& CFade::GetIsFade() == true) return;
#ifndef ENDING_STG
	m_pClearWidget->Update();
	ChangeScene();
#else
	m_pClearWidget->Update();

	// �Q�[���N���A�̃X�v���C�g�\�����I��������.
	if( m_pClearWidget->IsSpriteRenderEnd() == true ){
		// STG�̍X�V.
		m_pSTGManager->Update();
	}

	// STG���I��������.
	if( m_pSTGManager->IsSTGEnd() == true ){
		m_pClearWidget->SetIsSTGEnd();
		OnChangeScene( false );
	} else {
		SkipUpdate();
	}
	ChangeScene();

#ifdef _DEBUG
	if( GetAsyncKeyState(VK_F4) & 0x0001 ){
		m_pSceneManager->NextSceneMove();
	}
#endif	// #ifdef _DEBUG.
#endif	// #ifndef ENDING_STG
}

//============================.
//	�`��֐�.
//============================.
void CGameClear::Render()
{
	ModelRender();
	m_pClearWidget->Render();
}

//============================.
// ���f���̕`��.
//============================.
void CGameClear::ModelRender()
{
	//--------------------------------------------.
	// �`��p�X1.
	//--------------------------------------------.
	// �[�x�e�N�X�`���ɉe�p�̐[�x����������.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::Shadow );
	m_pSTGManager->Render();

	//--------------------------------------------.
	// �`��p�X2.
	//--------------------------------------------.
	// �G�t�F�N�g�Ȃǂ̕`��.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::Trans );
	CSceneTexRenderer::SetTransBuffer();
	m_pSTGManager->Render();

	//--------------------------------------------.
	// �`��p�X3.
	//--------------------------------------------.
	// G-Buffer��color, normal, depth����������.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::GBuffer );
	CSceneTexRenderer::SetGBuffer();
	m_pSTGManager->Render();

	//--------------------------------------------.
	// �ŏI�`��.
	//--------------------------------------------.
	// G-Buffer���g�p���āA��ʂɕ`�悷��.
	CSceneTexRenderer::Render();
}

//============================.
// �V�[����؂�ւ���.
//============================.
void CGameClear::OnChangeScene( const bool& isPlaySE )
{
	if(m_IsChangeScene == true) return;
	CFade::SetFadeIn();
	if( isPlaySE == true )CSoundManager::PlaySE("Determination");
	CSoundManager::FadeOutBGM("ClearBGM");
	m_IsChangeScene = true;
}

//============================.
// �V�[���؂�ւ��֐�.
//============================.
void CGameClear::ChangeScene()
{
	if(m_IsChangeScene == false) return;

	// �t�F�[�h�C����Ԃ��t�F�[�h���Ȃ珈�����Ȃ�.
	if(CFade::GetFadeState() != CFade::EFadeState::In) return;
	if(CFade::GetIsFade() == true) return;
	if(CSoundManager::GetBGMVolume("ClearBGM") > 0.0f) return;
	while(CSoundManager::StopBGMThread("ClearBGM")== false);
	m_pSceneManager->NextSceneMove();
}

//============================.
// �X�L�b�v�̍X�V�֐�.
//============================.
void CGameClear::SkipUpdate()
{
	// �X�L�b�v.
	if (GetAsyncKeyState(VK_RETURN) & 0x8000
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD) {
		m_SkipWaitCount++;

	}
	else {
		if (m_SkipWaitCount < SKIP_WAIT_COUNT) m_SkipWaitCount = 0;
	}
	if (m_SkipWaitCount < SKIP_WAIT_COUNT) return;
	OnChangeScene( true );
}