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
}

CAttackUpItem::~CAttackUpItem()
{
}

// 初期化関数.
bool CAttackUpItem::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( EffectSetting() == false ) return false;
	if( ColliderSetting() == false ) return false;
	m_vSclae = { 0.0f, 0.0f, 0.0f };
	m_ObjectTag = EObjectTag::AttackUpItem;
	return true;
}

// 更新関数.
void CAttackUpItem::Update()
{
	CItemBase::Update();
}

// 描画関数.
void CAttackUpItem::Render()
{
	CItemBase::Render();
}

// 当たり判定関数.
void CAttackUpItem::Collision( CActor* pActor )
{
	CItemBase::Collision( pActor );
}

// 相手座標の設定関数.
void CAttackUpItem::SetTargetPos( CActor& pActor )
{
	m_vPosition = pActor.GetPosition();
}

// 当たった際の効果を与える.
void CAttackUpItem::GiveEffect( CActor* pActor )
{
	if( pATTACK_UP_POWER == nullptr ) return;
	if( pATTACK_UP_TIME == nullptr ) return;

	// 回復関数.
	auto attackUp = [&]( float& attackPower, float& attackUpTime )
	{
		attackPower = *pATTACK_UP_POWER;
		attackUpTime = *pATTACK_UP_TIME;
	};
	// コールバック関数.
	pActor->SetAttackEffectTime( attackUp );	// 対象の体力を回復する.
}

// エフェクトの設定.
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