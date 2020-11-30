#include "MoveSpeedUp.h"
#include "..\..\..\..\..\Common\Effect\EffectManager.h"

CMoveSpeedUpItem::CMoveSpeedUpItem()
	: CItemBase		( nullptr )
	, pMOVE_VALUE	( nullptr )
	, pMOVE_UP_TIME	( nullptr )
{
}

CMoveSpeedUpItem::CMoveSpeedUpItem( 
	const float* pMoveValue,
	const float* pMoveTime,
	const CItemBase::SParameter* pParam )
	: CItemBase		( pParam )
	, pMOVE_VALUE	( pMoveValue )
	, pMOVE_UP_TIME	( pMoveTime )
{
	m_ObjectTag = EObjectTag::MoveSpeedUpItem;
}

CMoveSpeedUpItem::~CMoveSpeedUpItem()
{
}

// �������֐�.
bool CMoveSpeedUpItem::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( EffectSetting() == false ) return false;
	if( ColliderSetting() == false ) return false;
	m_vScale = { 0.0f, 0.0f, 0.0f };
	m_ObjectTag = EObjectTag::MoveSpeedUpItem;
	return true;
}

// �X�V�֐�.
void CMoveSpeedUpItem::Update()
{
	CItemBase::Update();
}

// �`��֐�.
void CMoveSpeedUpItem::Render()
{
	CItemBase::Render();
}

// �G�t�F�N�g�̕`��.
void CMoveSpeedUpItem::EffectRender()
{
	CItemBase::EffectRender();
}

// �����蔻��֐�.
void CMoveSpeedUpItem::Collision( CActor* pActor )
{
	CItemBase::Collision( pActor );
}

// ������W�̐ݒ�֐�.
void CMoveSpeedUpItem::SetTargetPos( CActor& pActor )
{
	m_vPosition = pActor.GetPosition();
}

// ���������ۂ̌��ʂ�^����.
void CMoveSpeedUpItem::GiveEffect( CActor* pActor )
{
	if( pMOVE_VALUE == nullptr ) return;
	if( pMOVE_UP_TIME == nullptr ) return;

	// �񕜊֐�.
	auto moveSpeedUp = [&]( float& moveValue, float& moveUpTime )
	{
		moveValue = *pMOVE_VALUE;
		moveUpTime = *pMOVE_UP_TIME;
	};
	// �R�[���o�b�N�֐�.
	pActor->SetMoveSpeedEffectTime( moveSpeedUp );	// �Ώۂ̗̑͂��񕜂���.
}

// �G�t�F�N�g�̐ݒ�.
bool CMoveSpeedUpItem::EffectSetting()
{
	const char* effectNames[] =
	{
		DROP_AND_AVTIVE_EFFECT_NAME,
		HIT_EFFECT_NAME,
	};
	int SpriteMax = sizeof(effectNames) / sizeof(effectNames[0]);

	for( int i = 0; i < SpriteMax; i++ ){
		m_pEffects[i]->SetEffect(effectNames[i]);
		if (m_pEffects[i] == nullptr) return false;
	}

	return true;
}