#include "AttackUp.h"
#include "..\..\..\..\..\Common\Effect\EffectManager.h"

CAttackUpItem::CAttackUpItem()
	: CItemBase			( nullptr )
	, pATTACK_UP_POWER	( nullptr )
	, pATTACK_UP_TIME	( nullptr )
{
}

CAttackUpItem::CAttackUpItem( 
	const float* pAttackUpPower,
	const float* pAttackUpTime,
	const CItemBase::SParameter* pParam )
	: CItemBase			( pParam )
	, pATTACK_UP_POWER	( pAttackUpPower )
	, pATTACK_UP_TIME	( pAttackUpTime )
{
	m_ObjectTag = EObjectTag::AttackUpItem;
}

CAttackUpItem::~CAttackUpItem()
{
}

// �������֐�.
bool CAttackUpItem::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( EffectSetting() == false ) return false;
	if( ColliderSetting() == false ) return false;
	m_vScale = { 0.0f, 0.0f, 0.0f };
	m_ObjectTag = EObjectTag::AttackUpItem;
	return true;
}

// �X�V�֐�.
void CAttackUpItem::Update()
{
	CItemBase::Update();
}

// �`��֐�.
void CAttackUpItem::Render()
{
	CItemBase::Render();
}

// �G�t�F�N�g�̕`��.
void CAttackUpItem::EffectRender()
{
	CItemBase::EffectRender();
}

// �����蔻��֐�.
void CAttackUpItem::Collision( CActor* pActor )
{
	CItemBase::Collision( pActor );
}

// ������W�̐ݒ�֐�.
void CAttackUpItem::SetTargetPos( CActor& pActor )
{
	m_vPosition = pActor.GetPosition();
}

// ���������ۂ̌��ʂ�^����.
void CAttackUpItem::GiveEffect( CActor* pActor )
{
	if( pATTACK_UP_POWER == nullptr ) return;
	if( pATTACK_UP_TIME == nullptr ) return;

	// �񕜊֐�.
	auto attackUp = [&]( float& attackPower, float& attackUpTime )
	{
		attackPower = *pATTACK_UP_POWER;
		attackUpTime = *pATTACK_UP_TIME;
	};
	// �R�[���o�b�N�֐�.
	pActor->SetAttackEffectTime( attackUp );	// �Ώۂ̗̑͂��񕜂���.
}

// �G�t�F�N�g�̐ݒ�.
bool CAttackUpItem::EffectSetting()
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