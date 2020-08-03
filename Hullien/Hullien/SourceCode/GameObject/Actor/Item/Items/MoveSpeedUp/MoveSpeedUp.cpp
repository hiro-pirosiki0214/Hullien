#include "MoveSpeedUp.h"

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
}

CMoveSpeedUpItem::~CMoveSpeedUpItem()
{
}

// 初期化関数.
bool CMoveSpeedUpItem::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( ColliderSetting() == false ) return false;
	m_vSclae = { 0.0f, 0.0f, 0.0f };
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