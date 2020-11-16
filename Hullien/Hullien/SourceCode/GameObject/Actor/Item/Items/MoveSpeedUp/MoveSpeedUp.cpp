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

// 初期化関数.
bool CMoveSpeedUpItem::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( EffectSetting() == false ) return false;
	if( ColliderSetting() == false ) return false;
	m_vScale = { 0.0f, 0.0f, 0.0f };
	m_ObjectTag = EObjectTag::MoveSpeedUpItem;
	return true;
}

// 更新関数.
void CMoveSpeedUpItem::Update()
{
	CItemBase::Update();
}

// 描画関数.
void CMoveSpeedUpItem::Render()
{
	CItemBase::Render();
}

// エフェクトの描画.
void CMoveSpeedUpItem::EffectRender()
{
	CItemBase::EffectRender();
}

// 当たり判定関数.
void CMoveSpeedUpItem::Collision( CActor* pActor )
{
	CItemBase::Collision( pActor );
}

// 相手座標の設定関数.
void CMoveSpeedUpItem::SetTargetPos( CActor& pActor )
{
	m_vPosition = pActor.GetPosition();
}

// 当たった際の効果を与える.
void CMoveSpeedUpItem::GiveEffect( CActor* pActor )
{
	if( pMOVE_VALUE == nullptr ) return;
	if( pMOVE_UP_TIME == nullptr ) return;

	// 回復関数.
	auto moveSpeedUp = [&]( float& moveValue, float& moveUpTime )
	{
		moveValue = *pMOVE_VALUE;
		moveUpTime = *pMOVE_UP_TIME;
	};
	// コールバック関数.
	pActor->SetMoveSpeedEffectTime( moveSpeedUp );	// 対象の体力を回復する.
}

// エフェクトの設定.
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