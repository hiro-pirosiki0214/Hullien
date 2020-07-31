#include "ItemBase.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"

CItemBase::CItemBase()
	: m_pStaticMesh			( nullptr )
	, m_NowState			( ENowState::None )
	, m_Scale				( 0.0f )
	, m_ModelAlpha			( INIT_MODEL_ALPHA )
	, m_AccelerationValue	( DEFAULT_ACCELERATION_VALUE )
	, m_Gravity				( DEFAULT_GRAVITY )
	, m_BoundCount			( 0 )
	, m_ActiveCount			( 0 )
	, m_FlashingCount		( 0.0f )
	, m_DisappearCount		( 0.0f )
	, m_FlashingAccValue	( 0.0f )
	, pPRAMETER				( nullptr )
{
	m_vSclae = { 1.0f, 1.0f, 1.0f };
}

CItemBase::CItemBase( const SParameter* pParam )
	: m_pStaticMesh			( nullptr )
	, m_NowState			( ENowState::None )
	, m_Scale				( 0.0f )
	, m_ModelAlpha			( INIT_MODEL_ALPHA )
	, m_AccelerationValue	( pParam->InitAccelerationValue )
	, m_Gravity				( pParam->InitGravity )
	, m_BoundCount			( 0 )
	, m_ActiveCount			( 0 )
	, m_FlashingCount		( 0.0f )
	, m_DisappearCount		( 0.0f )
	, m_FlashingAccValue	( 0.0f )
	, pPRAMETER				( pParam )
{
	m_vSclae = { pPRAMETER->ModelScaleMax, pPRAMETER->ModelScaleMax, pPRAMETER->ModelScaleMax };
}

CItemBase::~CItemBase()
{
}

// 更新関数.
void CItemBase::Update()
{
	switch( m_NowState )
	{
	case ENowState::Drop:
		Drop();
		break;
	case ENowState::Active:
		Active();
		break;
	case ENowState::HitDisappear:
		HitDisappear();
		break;
	case ENowState::TimeDisappear:
		TimeDisappear();
		break;
	case ENowState::Delete:
		break;
	default:
		break;
	}
}

// 描画関数.
void CItemBase::Render()
{
	if( m_NowState == ENowState::Delete ) return;
	if( m_NowState == ENowState::None ) return;
	if( m_NowState == ENowState::Max ) return;
	if( m_ModelAlpha < 1.0f ) return;

	if( m_pStaticMesh == nullptr ) return;
	m_pStaticMesh->SetPosition( m_vPosition );
	m_pStaticMesh->SetRotation( m_vRotation );
	m_pStaticMesh->SetScale( m_vSclae );
	m_pStaticMesh->SetAlpha( m_ModelAlpha );
	m_pStaticMesh->SetBlend( true );
	m_pStaticMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pStaticMesh->Render();
	m_pStaticMesh->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pStaticMesh->SetBlend( false );

#if _DEBUG
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// 当たり判定関数.
void CItemBase::Collision( CActor* pActor )
{
	// 対象オブジェクトじゃなければ終了.
	if( pActor->GetObjectTag() != EObjectTag::Player ) return;
	if( m_NowState == ENowState::Delete ) return;
	if( m_NowState == ENowState::None ) return;
	if( m_NowState == ENowState::HitDisappear ) return;
	if( m_NowState == ENowState::Drop ) return;

	// 球体の当たり判定.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;

	// 当たった際の効果を与える.
	this->GiveEffect( pActor );

	m_NowState = ENowState::HitDisappear;	// 現在の状態を当たった時に消す状態に変更.
}

// 出現処理.
void CItemBase::Drop( const D3DXVECTOR3& vPos )
{
	this->Init();
	m_NowState = ENowState::Drop;
	this->m_vPosition = vPos;
}

// 出現.
void CItemBase::Drop()
{
	// モデルサイズを大きくする.
	if( m_Scale <= pPRAMETER->ModelScaleMax ){
		m_Scale += pPRAMETER->ModelScaleAddValue;
		m_vSclae = { m_Scale, m_Scale, m_Scale };
	}

	m_AccelerationValue += m_Gravity;		// 重力の加算.
	m_vPosition.y -= m_AccelerationValue;	// 座標を加算値で引く.

	// 高さが0.0fより低くなった場合.
	if( m_vPosition.y <= 0.0f ){
		m_AccelerationValue = -m_AccelerationValue;	// 加算値を反転する.
		m_Gravity += m_Gravity;	// 重力を加算する.
		m_BoundCount++;		// バウンドの回数を加算する.

		if( m_BoundCount <= pPRAMETER->BoundCountMax ) return;
		// バウンド数がバウンド最大数以上なら.
		m_NowState = ENowState::Active;	// 現在の状態をアクティブに変更.
	}
}

// アクティブ.
void CItemBase::Active()
{
	// 回転処理.
	Rotate();

	m_ActiveCount++;	// アクティブカウントの加算.
	if( m_ActiveCount < pPRAMETER->ActiveTime*FPS ) return;
	m_NowState = ENowState::TimeDisappear;	// 現在の状態を時間経過で消す状態に変更.
}

// 当たった際に消える処理.
void CItemBase::HitDisappear()
{
	m_NowState = ENowState::Delete;	// 現在の状態を消す状態に変更.
}

// 時間が経った特に消える.
void CItemBase::TimeDisappear()
{
	// 回転処理.
	Rotate();

	m_FlashingCount += m_FlashingAccValue;		// 点滅カウントを加算.
	m_FlashingAccValue += pPRAMETER->FlashingAddValue;	// 点滅加算値を加算.
	m_ModelAlpha = sinf( m_FlashingCount ) + pPRAMETER->ModelAlphaMax;

	m_DisappearCount++;
	if( m_DisappearCount < pPRAMETER->DisappearTime*FPS ) return;
	m_ModelAlpha = 0.0f;
	m_NowState = ENowState::Delete;	// 現在の状態を消す状態に変更.
}

// 回転関数.
void CItemBase::Rotate()
{
	// 回転処理.
	m_vRotation.y += pPRAMETER->RotationSpeed;
	// オーバーフロー対策.
	if( m_vRotation.y >= D3DX_PI*2.0 ) m_vRotation.y = 0.0f;
}

// モデルの取得.
bool CItemBase::GetModel( const char* modelName )
{
	// 既に読み込めていたら終了.
	if( m_pStaticMesh != nullptr ) return true;
	// モデルの取得.
	CMeshResorce::GetStatic( m_pStaticMesh, modelName );
	// モデルが読み込めてなければ false.
	if( m_pStaticMesh == nullptr ) return false;

	return true;
}

// 当たり判定の設定.
bool CItemBase::ColliderSetting()
{
	if( m_pStaticMesh == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere( 
		m_pStaticMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		{ 0.0f, 0.0f, 0.0f },
		0.0f ) )) return false;
	return true;
}