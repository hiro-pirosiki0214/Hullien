#include "LifeRecovery.h"

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
}

CLifeRecoveryItem::~CLifeRecoveryItem()
{
	pLIFE_RECOVERY_VALUE = nullptr ;
}

// 初期化関数.
bool CLifeRecoveryItem::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( ColliderSetting() == false ) return false;
	m_vSclae = { 0.0f, 0.0f, 0.0f };
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
	// 回復関数.
	auto lifeRecovery = [&]( float& life )
	{
		life += *pLIFE_RECOVERY_VALUE;
	};
	// コールバック関数.
	pActor->LifeCalculation( lifeRecovery );	// 対象の体力を回復する.
}