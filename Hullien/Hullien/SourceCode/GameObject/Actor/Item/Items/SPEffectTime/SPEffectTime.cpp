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
}

CSPEffectTimeItem::~CSPEffectTimeItem()
{
	pSP_RECOVERY_VALUE = nullptr;
	pSP_RECOVERY_TIME = nullptr;
}

// 初期化関数.
bool CSPEffectTimeItem::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( EffectSetting() == false ) return false;
	if( ColliderSetting() == false ) return false;
	m_vScale = { 0.0f, 0.0f, 0.0f };
	m_ObjectTag = EObjectTag::SPEffectTimeItem;
	return true;
}

// 更新関数.
void CSPEffectTimeItem::Update()
{
	CItemBase::Update();
}

// 描画関数.
void CSPEffectTimeItem::Render()
{
	CItemBase::Render();
}

// 当たり判定関数.
void CSPEffectTimeItem::Collision( CActor* pActor )
{
	CItemBase::Collision( pActor );
}

// 相手座標の設定関数.
void CSPEffectTimeItem::SetTargetPos( CActor& pActor )
{
	m_vPosition = pActor.GetPosition();
}

// 当たった際の効果を与える.
void CSPEffectTimeItem::GiveEffect( CActor* pActor )
{
	if( pSP_RECOVERY_VALUE == nullptr ) return;
	if( pSP_RECOVERY_TIME == nullptr ) return;

	// 回復関数.
	auto spEffectTime = [&]( float& recoveryValue, float& recoveryTime )
	{
		recoveryValue = *pSP_RECOVERY_VALUE;
		recoveryTime = *pSP_RECOVERY_TIME;
	};
	// コールバック関数.
	pActor->SetSPEffectTime( spEffectTime );	// 対象の体力を回復する.
}

// エフェクトの設定.
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