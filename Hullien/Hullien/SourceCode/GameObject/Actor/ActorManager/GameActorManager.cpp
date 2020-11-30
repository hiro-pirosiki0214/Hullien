#include "GameActorManager.h"
#include "..\Character\Player\Player.h"
#include "..\Character\Girl\Girl.h"
#include "..\Character\Alien\AlienManager\AlienManager.h"
#include "..\Barrier\Barrier.h"
#include "..\Item\ItemManager\ItemManager.h"
#include "..\..\GroundStage\GroundStage.h"
#include "..\..\SkyDome\SkyDome.h"
#include "..\..\MotherShipUFO\MotherShipUFO.h"
#include "..\..\Widget\SceneWidget\GameWidget\Warning\Warning.h"
#include "..\..\InvisibleWall\InvisibleWall.h"

CGameActorManager::CGameActorManager()
	: m_pSkyDome			( nullptr )
	, m_pGroundStage		( nullptr )
	, m_pPlayer				( nullptr )
	, m_pGirl				( nullptr )
	, m_pMotherShipUFO		( nullptr )
	, m_pAlienManager		( nullptr )
	, m_pItemManager		( nullptr )
	, m_pBarrier			( nullptr )
	, m_pInvisibleWall		( nullptr )
	, m_ObjPositionList		()
	, m_ObjPosListCount		( 0 )
	, m_IsOllAnimationStop	( false )
{
	m_pSkyDome			= std::make_unique<CSkyDome>();
	m_pGroundStage		= std::make_shared<CGroundStage>();
	m_pPlayer			= std::make_shared<CPlayer>();
	m_pGirl				= std::make_shared<CGirl>();
	m_pMotherShipUFO	= std::make_unique<CMotherShipUFO>();
	m_pAlienManager		= std::make_shared<CAlienManager>();
	m_pItemManager		= std::make_shared<CItemManager>();
	m_pBarrier			= std::make_shared<CBarrier>();
	m_pInvisibleWall	= std::make_unique<CInvisibleWall>();
}

CGameActorManager::~CGameActorManager()
{
}

// �������֐�.
bool CGameActorManager::Init()
{
	if( m_pSkyDome->Init()			== false ) return false;	// �w�i�̏�����.
	if( m_pGroundStage->Init()		== false ) return false;	// �n�ʂ̏�����.
	if( m_pInvisibleWall->Init()	== false ) return false;	// �����Ȃ��ǂ̏�����.
	if( m_pPlayer->Init()			== false ) return false;	// �v���C���[�̏�����.
	if( m_pGirl->Init()				== false ) return false;	// ���̎q�̏�����.
	if( m_pMotherShipUFO->Init()	== false ) return false;	// �}�U�[�V�b�v�̏�����.
	if( m_pAlienManager->Init()		== false ) return false;	// �F���l�Ǘ��̏�����.
	if( m_pItemManager->Init()		== false ) return false;	// �A�C�e���Ǘ��̏�����.
	// �}�U�[�V�b�v�̍��W��擾.
	m_pAlienManager->SetMotherShipUFOPos( m_pMotherShipUFO->GetPosition() );
	m_pMotherShipUFO->DischargePreparation();
	m_pPlayer->SetBoxWall( m_pInvisibleWall->GetBoxWall() );
	m_pGirl->SetBoxWall( m_pInvisibleWall->GetBoxWall() );

	return true;
}

// �X�V�֐�.
void CGameActorManager::Update()
{
	m_ObjPosListCount = 0;		// �J�E���g�̏�����.
	m_ObjPositionList.clear();	// ���X�g�̏�����.

	// �v���C���[�̍X�V.
	m_pPlayer->SetTargetPos( *m_pGirl.get() );	// ���̎q�̍��W���擾.
	m_pPlayer->Update();						// �X�V.
	

	// �o���A���g���邩.
	if( m_pPlayer->IsSpecialAbility() == true ) m_pBarrier->Init();	// �o���A�̏�����.

	// ���̎q�̍X�V.
	m_pGirl->Update();
	m_pGirl->SetCameraRadianX(m_pPlayer->GetCameraRadianX());	//�J�����̃��W�A���l�擾.
	m_pBarrier->SetTargetPos( *m_pGirl.get() );	// ���̎q�̍��W���擾.

	// �o���A�̍X�V.
	m_pBarrier->Update();

	// �F���l�̍X�V.
	m_pAlienManager->Update( 
		[&]( CActor* pActor )
		{
			// ���W�̐ݒ�.
			pActor->SetTargetPos( *m_pPlayer.get() );
			pActor->SetTargetPos( *m_pGirl.get() );

			// �F���l�̍X�V����.
			pActor->Update();
			SetPositionList( pActor );	// ���W���X�g�̐ݒ�.

			// �v���C���[�A���̎q�̓����蔻��.
			m_pPlayer->Collision( pActor );
			m_pGirl->Collision( pActor );

			// �F���l�̓����蔻��.
			pActor->Collision( m_pPlayer.get() );
			pActor->Collision( m_pGirl.get() );
			pActor->Collision( m_pBarrier.get() );

			// �}�U�[�V�b�v�̓����蔻��.
			m_pMotherShipUFO->Collision( pActor );
		} );

	// �A�C�e�����X�g������΃A�C�e���𗎂Ƃ�.
	m_pItemManager->Drop( m_pAlienManager->GetDropItemList() );
	// �A�C�e���̍X�V.
	m_pItemManager->Update( 
		[&]( CActor* pActor )
		{
			pActor->Update();						// �A�C�e���̍X�V.
			SetPositionList( pActor );				// ���W���X�g�̐ݒ�.
			pActor->Collision( m_pPlayer.get() );	// �A�C�e���̓����蔻��.
		} );

	SetPositionList( m_pGirl.get() );			// ���̎q���W��ݒ�.
	SetPositionList( m_pPlayer.get() );			// �v���C���[���W��ݒ�.
	SetPositionList( m_pMotherShipUFO.get() );	// �}�U�[�V�b�v�̍��W��ݒ�.
}

// �`��֐�.
void CGameActorManager::Render()
{
	m_pSkyDome->Render();
	m_pGroundStage->Render();	// �X�e�[�W�̕`��.
	m_pPlayer->Render();		// �v���C���[�̕`��.
	m_pGirl->Render();			// ���̎q�̕`��.
	m_pAlienManager->Render();	// �F���l�B�̕`��.
	m_pMotherShipUFO->Render();	// �}�U�[�V�b�v�̕`��.
	m_pItemManager->Render();	// �A�C�e���̕`��.
	m_pBarrier->Render();		// �o���A�̕`��.
	m_pInvisibleWall->Render();	// �����Ȃ��ǂ̕`��.

	// �G�t�F�N�g�̕`��.
	m_pPlayer->EffectRender();			// �v���C���[�̃G�t�F�N�g�`��.
	m_pAlienManager->EffectRender();	// �F���l�̃G�t�F�N�g�`��.
	m_pItemManager->EffectRender();		// �A�C�e���̃G�t�F�N�g�`��.
	m_pBarrier->EffectRender();			// �o���A�G�t�F�N�g�`��.
	// ���@��ňړ�.
	m_pAlienManager->SpriteRender();	// �X�v���C�g�̕`��.
}

// �X�v���C�g�̕`��.
void CGameActorManager::SpriteRender()
{
	m_pGirl->SpriteRender();	//���̎q�̃X�v���C�g�`��.
	m_pPlayer->SpriteRender();	//�v���C���[�̃X�v���C�g�`��.
}

// �Q�[���I�[�o�[���ǂ���.
bool CGameActorManager::IsGameOver()
{
	if( m_pPlayer->IsDead() == false ) return false;
	AnimationStop();	// �A�j���[�V�������~�߂�.
	return true;
}

// ���̎q��A�ꋎ���Ă��邩.
bool CGameActorManager::IsGirlAbduct()
{
	return m_pAlienManager->IsGirlAbduct();
}

// ���̎q���댯�ȏ�Ԃ�.
bool CGameActorManager::IsDanger()
{
	return m_pGirl->IsDanger();
}

// �F���l���A�҂�����.
bool CGameActorManager::IsReturnAlien()
{
	if(m_pGirl->GetPosition().y < GIRL_LIMIT_POS_Y) return false;
	return m_pMotherShipUFO->IsReturnAlien();
}

// ���W���X�g�̐ݒ�.
void CGameActorManager::SetPositionList( CGameObject* pObj )
{
	if( pObj == nullptr ) return;
	m_ObjPosListCount++;	// �I�u�W�F�N�g���̉��Z.
	m_ObjPositionList.emplace_back( 
		pObj->GetObjectTag(),	// �^�O�̎擾.
		std::pair<D3DXVECTOR3, float>(
			pObj->GetPosition(),	// ���W�̎擾.
			pObj->GetRotatinY()));	// Y����]�l�̎擾.
}

// �A�j���[�V�������~�߂�.
void CGameActorManager::AnimationStop()
{
	if( m_IsOllAnimationStop == true ) return;

	m_pPlayer->StopAnimation();
	m_pGirl->StopAnimation();
	m_pAlienManager->StopAnimation();

	m_IsOllAnimationStop = true;
}