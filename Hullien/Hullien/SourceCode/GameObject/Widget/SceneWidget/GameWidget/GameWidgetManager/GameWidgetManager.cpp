#include "GameWidgetManager.h"
#include "..\LimitTime\LimitTime.h"
#include "..\MiniMap\MiniMap.h"
#include "..\UltemateSign\UltemateSign.h"

/******************************************
*	ゲームUI管理クラス.
**/
CGameWidgetManager::CGameWidgetManager()
	: m_pLimitTime		(nullptr)
	, m_pMiniMap			(nullptr)
	, m_pUltemateSing	(nullptr)
{
	m_pLimitTime = std::make_unique<CLimitTime>();
	m_pMiniMap = std::make_unique<CMiniMap>();
	m_pUltemateSing = std::make_unique<CUltemateSing>();
}

CGameWidgetManager::~CGameWidgetManager()
{
}

// 初期化関数,
bool CGameWidgetManager::Init()
{
	if ( m_pLimitTime->Init() == false ) return false;
	if ( m_pMiniMap->Init() == false ) return false;
	if ( m_pUltemateSing->Init() == false ) return false;

	return true;
}

// 更新関数.
void CGameWidgetManager::Update(CGameActorManager* pActorManager)
{
	m_pLimitTime->Update();
	m_pMiniMap->Update();
}

// 描画関数.
void CGameWidgetManager::Render()
{
}
