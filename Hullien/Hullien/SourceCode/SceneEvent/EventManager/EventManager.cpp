#include "..\EventList.h"
#include "..\..\GameObject\SkyDome\SkyDome.h"

#include "..\..\Common\D3DX\D3DX11.h"
#include "..\..\Common\Shader\ShadowMap\ShadowMap.h"
#include "..\..\Common\SceneTexRenderer\SceneTexRenderer.h"

/************************************
*	�C�x���g�V�[���Ǘ��N���X.
**/
CEventManager::CEventManager()
	: m_pEventBase		( nullptr )
	, m_NowEvent		( EEvent::Start )
	, m_NextEvent		( EEvent::Start )
	, m_IsLoadEnd		( false )
	, m_IsSkip			( false )
	, m_IsGameOver		( false )
	, m_IsEventEnd		( false ) 
{
	NextEventMove();
}

CEventManager::~CEventManager()
{
}

// �X�V�֐�.
void CEventManager::Update()
{
	if( m_pEventBase == nullptr ) return;

	if (m_IsLoadEnd == false)
	{
		m_IsLoadEnd = m_pEventBase->Load();
	}
	else
	{
		// �ǂݍ��݂��I��������.
		m_IsEventEnd = m_pEventBase->GetIsEventEnd();
		m_pEventBase->Update();		//�C�x���g�̍X�V.
	}
}

// �`��֐�.
void CEventManager::Render()
{
	if (m_IsLoadEnd == false) return;
	ModelRender();	
	m_pEventBase->SpriteRender();
}

// ���̃C�x���g�Ɉړ�.
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

// ���f���̕`��.
void CEventManager::ModelRender()
{
	//--------------------------------------------.
	// �`��p�X1.
	//--------------------------------------------.
	// �[�x�e�N�X�`���ɉe�p�̐[�x����������.
	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::Shadow );
	m_pEventBase->Render();

	//--------------------------------------------.
	// �`��p�X2.
	//--------------------------------------------.
	// �G�t�F�N�g�p�̃X�v���C�g�Ȃǂ���������.
	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::Trans );
	CSceneTexRenderer::SetTransBuffer();
	m_pEventBase->Render();

	//--------------------------------------------.
	// �`��p�X3.
	//--------------------------------------------.
	// G-Buffer��color, normal, depth����������.
	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::GBuffer );
	CSceneTexRenderer::SetGBuffer();
	m_pEventBase->Render();

	//--------------------------------------------.
	// �ŏI�`��.
	//--------------------------------------------.
	// G-Buffer���g�p���āA��ʂɕ`�悷��.
	CSceneTexRenderer::Render();
}
