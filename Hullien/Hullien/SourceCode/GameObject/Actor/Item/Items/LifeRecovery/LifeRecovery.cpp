#include "LifeRecovery.h"
#include "..\..\..\..\..\Common\Effect\EffectManager.h"

CLifeRecoveryItem::CLifeRecoveryItem()
	: CItemBase				( nullptr )
	, pLIFE_RECOVERY_VALUE	( nullptr )
{
}

CLifeRecoveryItem::CLifeRecoveryItem( 
	const float* pLifeRecoveryValue, 
	const CItemBase::SParameter* pParam )
	: CItemBase				( pParam )
	, pLIFE_RECOVERY_VALUE	( pLifeRecoveryValue)
{
	m_ObjectTag = EObjectTag::LifeRecoveryItem;
}

CLifeRecoveryItem::~CLifeRecoveryItem()
{
	pLIFE_RECOVERY_VALUE = nullptr ;
}

// �������֐�.
bool CLifeRecoveryItem::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( EffectSetting() == false ) return false;
	if( ColliderSetting() == false ) return false;
	m_vScale = { 0.0f, 0.0f, 0.0f };
	m_ObjectTag = EObjectTag::LifeRecoveryItem;
	return true;
}

// �X�V�֐�.
void CLifeRecoveryItem::Update()
{
	CItemBase::Update();
}

// �`��֐�.
void CLifeRecoveryItem::Render()
{
	CItemBase::Render();
}

// �G�t�F�N�g�̕`��.
void CLifeRecoveryItem::EffectRender()
{
	CItemBase::EffectRender();
}

// �����蔻��֐�.
void CLifeRecoveryItem::Collision( CActor* pActor )
{
	CItemBase::Collision( pActor );
}

// ������W�̐ݒ�֐�.
void CLifeRecoveryItem::SetTargetPos( CActor& pActor )
{
	m_vPosition = pActor.GetPosition();
}

// ���������ۂ̌��ʂ�^����.
void CLifeRecoveryItem::GiveEffect( CActor* pActor )
{
	if( pLIFE_RECOVERY_VALUE == nullptr ) return;
	// �R�[���o�b�N�֐� : �Ώۂ̗̑͂��񕜂���..
	pActor->LifeCalculation( [&]( float& life, bool& isAttack )
	{
		life += *pLIFE_RECOVERY_VALUE;
	});
}

// �G�t�F�N�g�̐ݒ�.
bool CLifeRecoveryItem::EffectSetting()
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