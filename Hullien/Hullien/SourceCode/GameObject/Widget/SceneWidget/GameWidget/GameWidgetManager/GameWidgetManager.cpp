#include "GameWidgetManager.h"
#include "..\LimitTime\LimitTime.h"
#include "..\MiniMap\MiniMap.h"
#include "..\UltemateSign\UltemateSign.h"

/******************************************
*	�Q�[��UI�Ǘ��N���X.
**/
CGameWidgetManager::CGameWidgetManager()
	: m_pLimitTime		(nullptr)
	, m_pMiniMap		(nullptr)
//	, m_pUltemateSing	(nullptr)
{
	m_pLimitTime = std::make_unique<CLimitTime>();
	m_pMiniMap = std::make_unique<CMiniMap>();
//	m_pUltemateSing = std::make_unique<CUltemateSing>();
}

CGameWidgetManager::~CGameWidgetManager()
{
}

// �������֐�,
bool CGameWidgetManager::Init()
{
	if ( m_pLimitTime->Init() == false ) return false;
	if ( m_pMiniMap->Init() == false ) return false;
//	if ( m_pUltemateSing->Init() == false ) return false;

	return true;
}

// �X�V�֐�.
void CGameWidgetManager::Update(CGameActorManager* pActorManager)
{
	m_pLimitTime->Update();
	m_pMiniMap->SetObjPosition( pActorManager );
	m_pMiniMap->Update();
//	m_pUltemateSing->IsAppUltemate(pActorManager);
//	m_pUltemateSing->Update();
}

// �`��֐�.
void CGameWidgetManager::Render()
{
	m_pLimitTime->Render();
	m_pMiniMap->Render();
//	m_pUltemateSing->Render();
}

// �Q�[�����I��������.
bool CGameWidgetManager::IsGameFinish()
{

	return m_pLimitTime->IsFinish();
}
