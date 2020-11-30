#include "STGManager.h"
#include "..\..\..\Camera\Camera.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\SkyDome\SkyDome.h"
#include "..\Character\Player\STGPlayer.h"
#include "..\Character\Enemy\STGEnemyManager\STGEnemyManager.h"
#include "..\Bullet\STGBullet.h"

CSTGManager::CSTGManager()
	: m_pCamera			( nullptr )
	, m_pSkyDome		( nullptr )
	, m_pPlayer			( nullptr )
	, m_pEnemyManager	( nullptr )
	, m_STGEndFrame		( 0.0f )
	, m_STGNowFrame		( 0.0f )
	, m_SkyDomeRotX		( 0.0f )
{
	m_pCamera		= std::make_shared<CCamera>();
	m_pSkyDome		= std::make_unique<CSkyDome>();
	m_pPlayer		= std::make_shared<STG::CPlayer>();
	m_pEnemyManager	= std::make_unique<STG::CEnemyManager>();
}

CSTGManager::~CSTGManager()
{
}

// �������֐�.
bool CSTGManager::Init()
{
	if( m_pSkyDome->Init()		== false ) return false;
	if( m_pPlayer->Init()		== false ) return false;
	if( m_pEnemyManager->Init()	== false ) return false;

	m_pCamera->SetPosition( CAMERA_POSITION );
	m_pCamera->SetLookPosition( CAMERA_LOOK_POSITION );
	CCameraManager::SetCamera( m_pCamera );

	return true;
}

// �X�V�֐�.
void CSTGManager::Update()
{
	m_STGNowFrame++;

	m_SkyDomeRotX += SKY_DOME_SCROLL_SPEED;
	m_pSkyDome->SetRotationX( m_SkyDomeRotX );
	m_pSkyDome->Update();

	m_pPlayer->Controller();// �v���C���[�̑���.
	m_pPlayer->Update();	// �v���C���[�̍X�V.

	m_pEnemyManager->Update( [&]( STG::CActor* pEnemy )
	{
		// �v���C���[�̍��W��ݒ�.
		pEnemy->SetTargetPos( m_pPlayer->GetPosition() );

		// �G�̍X�V.
		pEnemy->Update();

		// �v���C���[�A�G�̓����蔻��.
		m_pPlayer->Collision( pEnemy );
		pEnemy->Collision( m_pPlayer.get() );

		// �e���m�̓����蔻��.
		for( auto& pb : m_pPlayer->GetBullets() ){
			for( auto& eb : pEnemy->GetBullets() ){
				eb->Collision( pb.get() );
			}
		}
	});
}

// �`��֐�.
void CSTGManager::Render()
{
	m_pSkyDome->Render();		// �w�i�̕`��.
	m_pPlayer->Render();		// �v���C���[�̕`��.
	m_pEnemyManager->Render();	// �G�̕`��.
}

// �V���[�e�B���O���I��������.
bool CSTGManager::IsSTGEnd()
{
	return m_pEnemyManager->IsEndEnmeyDead();
}