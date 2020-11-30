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

// 初期化関数.
bool CLifeRecoveryItem::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( EffectSetting() == false ) return false;
	if( ColliderSetting() == false ) return false;
	m_vScale = { 0.0f, 0.0f, 0.0f };
	m_ObjectTag = EObjectTag::LifeRecoveryItem;
	return true;
}

// 更新関数.
void CLifeRecoveryItem::Update()
{
	CItemBase::Update();
}

// 描画関数.
void CLifeRecoveryItem::Render()
{
	CItemBase::Render();
}

// エフェクトの描画.
void CLifeRecoveryItem::EffectRender()
{
	CItemBase::EffectRender();
}

// 当たり判定関数.
void CLifeRecoveryItem::Collision( CActor* pActor )
{
	CItemBase::Collision( pActor );
}

// 相手座標の設定関数.
void CLifeRecoveryItem::SetTargetPos( CActor& pActor )
{
	m_vPosition = pActor.GetPosition();
}

// 当たった際の効果を与える.
void CLifeRecoveryItem::GiveEffect( CActor* pActor )
{
	if( pLIFE_RECOVERY_VALUE == nullptr ) return;
	// コールバック関数 : 対象の体力を回復する..
	pActor->LifeCalculation( [&]( float& life, bool& isAttack )
	{
		life += *pLIFE_RECOVERY_VALUE;
	});
}

// エフェクトの設定.
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