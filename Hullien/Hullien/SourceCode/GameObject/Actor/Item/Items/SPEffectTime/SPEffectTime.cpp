#include "SPEffectTime.h"
#include "..\..\..\..\..\Common\Effect\EffectManager.h"

CSPEffectTimeItem::CSPEffectTimeItem()
	: CItemBase				( nullptr )
	, pSP_RECOVERY_VALUE	( nullptr )
	, pSP_RECOVERY_TIME		( nullptr )
{
}

CSPEffectTimeItem::CSPEffectTimeItem( 
	const float* pSPRecoveryValue,
	const float* pSPRecoveryTime,
	const CItemBase::SParameter* pParam )
	: CItemBase				( pParam )
	, pSP_RECOVERY_VALUE	( pSPRecoveryValue )
	, pSP_RECOVERY_TIME		( pSPRecoveryTime )
{
	m_ObjectTag = EObjectTag::SPEffectTimeItem;
}

CSPEffectTimeItem::~CSPEffectTimeItem()
{
	pSP_RECOVERY_VALUE = nullptr;
	pSP_RECOVERY_TIME = nullptr;
}

// �������֐�.
bool CSPEffectTimeItem::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( EffectSetting() == false ) return false;
	if( ColliderSetting() == false ) return false;
	m_vScale = { 0.0f, 0.0f, 0.0f };
	m_ObjectTag = EObjectTag::SPEffectTimeItem;
	return true;
}

// �X�V�֐�.
void CSPEffectTimeItem::Update()
{
	CItemBase::Update();
}

// �`��֐�.
void CSPEffectTimeItem::Render()
{
	CItemBase::Render();
}

// �G�t�F�N�g�̕`��.
void CSPEffectTimeItem::EffectRender()
{
	CItemBase::EffectRender();
}

// �����蔻��֐�.
void CSPEffectTimeItem::Collision( CActor* pActor )
{
	CItemBase::Collision( pActor );
}

// ������W�̐ݒ�֐�.
void CSPEffectTimeItem::SetTargetPos( CActor& pActor )
{
	m_vPosition = pActor.GetPosition();
}

// ���������ۂ̌��ʂ�^����.
void CSPEffectTimeItem::GiveEffect( CActor* pActor )
{
	if( pSP_RECOVERY_VALUE == nullptr ) return;
	if( pSP_RECOVERY_TIME == nullptr ) return;

	// �񕜊֐�.
	auto spEffectTime = [&]( float& recoveryValue, float& recoveryTime )
	{
		recoveryValue = *pSP_RECOVERY_VALUE;
		recoveryTime = *pSP_RECOVERY_TIME;
	};
	// �R�[���o�b�N�֐�.
	pActor->SetSPEffectTime( spEffectTime );	// �Ώۂ̗̑͂��񕜂���.
}

// �G�t�F�N�g�̐ݒ�.
bool CSPEffectTimeItem::EffectSetting()
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