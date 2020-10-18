#include "STGManager.h"
#include "..\..\..\Camera\Camera.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\Character\Player\STGPlayer.h"

CSTGManager::CSTGManager()
	: m_pCamera	( nullptr )
	, m_pPlayer	( nullptr )
{
	m_pCamera = std::make_shared<CCamera>();
	m_pPlayer = std::make_shared<STG::CPlayer>();
}

CSTGManager::~CSTGManager()
{
}

// ‰Šú‰»ŠÖ”.
bool CSTGManager::Init()
{
	if( m_pPlayer->Init() == false ) return false;
	return true;
}

// XVŠÖ”.
void CSTGManager::Update()
{
	m_pPlayer->Controller();
	m_pPlayer->Update();


	m_pCamera->SetPosition( { 0.0f, 100.0f, 10.0f } );
	m_pCamera->SetLookPosition( { 0.0f, 0.0f, 0.0f } );

	CCameraManager::SetCamera( m_pCamera );
}

// •`‰æŠÖ”.
void CSTGManager::Render()
{
	m_pPlayer->Render();
}