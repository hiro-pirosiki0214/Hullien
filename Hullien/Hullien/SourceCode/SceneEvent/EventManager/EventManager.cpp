#include "..\EventList.h"
#include "..\..\GameObject\SkyDome\SkyDome.h"

#include "..\..\Common\D3DX\D3DX11.h"
#include "..\..\Common\Shader\ShadowMap\ShadowMap.h"
#include "..\..\Common\SceneTexRenderer\SceneTexRenderer.h"

/************************************
*	イベントシーン管理クラス.
**/
CEventManager::CEventManager()
	: m_pEventBase		( nullptr )
	, m_pSkyDome		( nullptr )
	, m_NowEvent		( EEvent::Start )
	, m_NextEvent		( EEvent::Start )
	, m_IsLoadEnd		( false )
	, m_IsSkip			( false )
	, m_IsGameOver		( false )
	, m_IsEventEnd		( false ) 
{
	NextEventMove();
	m_pSkyDome = std::make_shared<CSkyDome>();
}

CEventManager::~CEventManager()
{
}

// 更新関数.
void CEventManager::Update()
{
	if( m_pEventBase == nullptr ) return;

	if (m_IsLoadEnd == false)
	{
		m_IsLoadEnd = m_pEventBase->Load();
		m_pSkyDome->Init();
	}
	else
	{
		// 読み込みが終了したら.
		m_IsEventEnd = m_pEventBase->GetIsEventEnd();
		m_pEventBase->Update();		//イベントの更新.
	}
}

// 描画関数.
void CEventManager::Render()
{
	if (m_IsLoadEnd == false) return;
	ModelRender();	
	m_pEventBase->SpriteRender();
}

// 次のイベントに移動.
void CEventManager::NextEventMove()
{
	m_IsLoadEnd = false;
	switch (m_NextEvent)
	{
	case EEvent::GameStart:
		m_pEventBase	= std::make_shared<CGameStartEvent>();
		m_IsGameOver	= false;
		m_IsEventEnd	= false;
		m_NowEvent		= m_NextEvent;
		m_NextEvent		= EEvent::ResultCheck;
		break;
	case EEvent::ResultCheck:
		if (m_IsGameOver == true)
		{
			m_NextEvent = EEvent::GameOver;
		}
		else {
			m_NextEvent = EEvent::GameClear;
		}
		NextEventMove();
		break;
	case EEvent::GameClear:
		m_pEventBase = std::make_shared<CGameClearEvent>();
		m_IsEventEnd = false;
		m_NowEvent = m_NextEvent;
		m_NextEvent = EEvent::GameStart;
		break;
	case EEvent::GameOver:
		m_pEventBase = std::make_shared<CGameOverEvent>();
		m_IsEventEnd = false;
		m_NowEvent = m_NextEvent;
		m_NextEvent = EEvent::GameStart;
		break;
	default:
		break;
	}
}

// モデルの描画.
void CEventManager::ModelRender()
{
	//--------------------------------------------.
	// 描画パス1.
	//--------------------------------------------.
	// 深度テクスチャに影用の深度を書き込む.
	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::Shadow );
	m_pEventBase->Render();

	//--------------------------------------------.
	// 描画パス2.
	//--------------------------------------------.
	// G-Bufferにcolor, normal, depthを書き込む.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::Trans );
	CSceneTexRenderer::SetTransBuffer();
	m_pSkyDome->Render();
	m_pEventBase->Render();

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::GBuffer );
	CSceneTexRenderer::SetGBuffer();
	m_pSkyDome->Render();
	m_pEventBase->Render();

	//--------------------------------------------.
	// 最終描画.
	//--------------------------------------------.
	// G-Bufferを使用して、画面に描画する.

	CDirectX11::SetBackBuffer();
	CSceneTexRenderer::Render();
}
