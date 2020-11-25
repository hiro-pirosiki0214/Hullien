#include "..\SceneList.h"
#include "..\..\..\GameObject\Widget\SceneWidget\ClearWidget\ClearWidget.h"
#include "..\..\..\GameObject\STG\STGManager\STGManager.h"
#include "..\..\..\GameObject\Widget\Fade\Fade.h"
#include "..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\XAudio2\SoundManager.h"
#include "..\..\..\Common\SceneTexRenderer\SceneTexRenderer.h"
#include "..\..\..\Common\D3DX\D3DX11.h"

#define ENDING_STG // Clear時のSTGを入れるか.

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
//	読込関数.
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
//	更新関数.
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

	// ゲームクリアのスプライト表示が終了したら.
	if( m_pClearWidget->IsSpriteRenderEnd() == true ){
		// STGの更新.
		m_pSTGManager->Update();
	}

	// STGが終了したら.
	if( m_pSTGManager->IsSTGEnd() == true ){
		m_pClearWidget->SetIsSTGEnd();
		OnChangeScene();
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
//	描画関数.
//============================.
void CGameClear::Render()
{
	ModelRender();
	m_pClearWidget->Render();
}

//============================.
// モデルの描画.
//============================.
void CGameClear::ModelRender()
{
	//--------------------------------------------.
	// 描画パス1.
	//--------------------------------------------.
	// 深度テクスチャに影用の深度を書き込む.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::Shadow );
	m_pSTGManager->Render();

	//--------------------------------------------.
	// 描画パス2.
	//--------------------------------------------.
	// エフェクトなどの描画.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::Trans );
	CSceneTexRenderer::SetTransBuffer();
	m_pSTGManager->Render();

	//--------------------------------------------.
	// 描画パス3.
	//--------------------------------------------.
	// G-Bufferにcolor, normal, depthを書き込む.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::GBuffer );
	CSceneTexRenderer::SetGBuffer();
	m_pSTGManager->Render();

	//--------------------------------------------.
	// 最終描画.
	//--------------------------------------------.
	// G-Bufferを使用して、画面に描画する.
	CSceneTexRenderer::Render();
}

//============================.
// シーンを切り替える.
//============================.
void CGameClear::OnChangeScene()
{
	if(m_IsChangeScene == true) return;
	CFade::SetFadeIn();
	CSoundManager::PlaySE("Determination");
	CSoundManager::FadeOutBGM("ClearBGM");
	m_IsChangeScene = true;
}

//============================.
// シーン切り替え関数.
//============================.
void CGameClear::ChangeScene()
{
	if(m_IsChangeScene == false) return;

	// フェードイン状態かつフェード中なら処理しない.
	if(CFade::GetFadeState() != CFade::EFadeState::In) return;
	if(CFade::GetIsFade() == true) return;
	if(CSoundManager::GetBGMVolume("ClearBGM") > 0.0f) return;
	while(CSoundManager::StopBGMThread("ClearBGM")== false);
	m_pSceneManager->NextSceneMove();
}

//============================.
// スキップの更新関数.
//============================.
void CGameClear::SkipUpdate()
{
	// スキップ.
	if (GetAsyncKeyState(VK_RETURN) & 0x8000
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD) {
		m_SkipWaitCount++;

	}
	else {
		if (m_SkipWaitCount < SKIP_WAIT_COUNT) m_SkipWaitCount = 0;
	}
	if (m_SkipWaitCount < SKIP_WAIT_COUNT) return;
	OnChangeScene();
}