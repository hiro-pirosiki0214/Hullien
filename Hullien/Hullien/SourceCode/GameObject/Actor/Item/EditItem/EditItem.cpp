#include "EditItem.h"
#include "..\..\..\..\Common\Effect\EffectManager.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\XAudio2\SoundManager.h"

CEditItem::CEditItem()
	: CItemBase			( &m_Parameter )
	, m_pStaticMeshs	()
	, m_Parameter		()
	, m_NowItemNo		( 0 )
{
	m_NowState = ENowState::Delete;
}

CEditItem::~CEditItem()
{
}

// 初期化関数.
bool CEditItem::Init()
{
	if( ModelSetting()		== false ) return false;
	if( EffectSetting()		== false ) return false;
	if( ColliderSetting()	== false ) return false;
	m_vScale = { 0.0f, 0.0f, 0.0f };
	m_Gravity = m_Parameter.InitGravity;
	return true;
}

// 更新関数.
void CEditItem::Update()
{
	CItemBase::Update();

	if( m_NowState != ENowState::Delete ) return;
	m_RotatonY += m_Parameter.RotationSpeed;
	if( m_RotatonY >= static_cast<float>(D3DXToDegree( 360.0 )) ) m_RotatonY = 0.0f;
}

// 描画関数.
void CEditItem::Render()
{
	m_pStaticMesh = m_pStaticMeshs[m_NowItemNo];
	CItemBase::Render();

	if( m_NowState != ENowState::Delete ) return;
	if( m_pStaticMesh == nullptr ) return;
	m_pStaticMesh->SetPosition( { 0.0f, 5.0f, 0.0f } );
	m_pStaticMesh->SetRotation( { 0.0f, m_RotatonY, 0.0f } );
	m_pStaticMesh->SetScale( { 1.0f, 1.0f, 1.0f } );
	m_pStaticMesh->SetAlpha( 1.0f );
	m_pStaticMesh->SetRasterizerState( ERS_STATE::Back );
	m_pStaticMesh->Render();
	m_pStaticMesh->SetRasterizerState( ERS_STATE::None );
}

// エフェクトの描画.
void CEditItem::EffectRender()
{
	if( m_NowState == ENowState::HitDisappear ) return;
	if( m_NowState == ENowState::Delete ) return;
	if( m_NowState == ENowState::None ) return;
	if( m_NowState == ENowState::Max ) return;
	if( m_ModelAlpha < 1.0f ) return;

	// エフェクトの描画.
	m_pEffects[m_NowItemNo]->SetScale( 1.5f );
	m_pEffects[m_NowItemNo]->Render();
}

// 当たり判定関数.
void CEditItem::Collision( CActor* pActor )
{
	CItemBase::Collision( pActor );
}

// 相手座標の設定関数.
void CEditItem::SetTargetPos( CActor& pActor )
{
	m_vPosition = pActor.GetPosition();
}

// 出現処理.
void CEditItem::Drop( const D3DXVECTOR3& vPos )
{
	if( m_NowState == ENowState::Drop ) return;
	m_vScale = { 0.0f, 0.0f, 0.0f };
	m_Scale				= 0.0f;
	m_ModelAlpha		= 1.0f;
	m_AccelerationValue	= 0.0f;
	m_BoundCount		= 0;
	m_ActiveCount		= 0;
	m_HitEffectCount	= 0.0f;
	m_NowState = ENowState::Drop;
	this->m_vPosition = vPos;
	m_pEffects[m_NowItemNo]->Play( { vPos.x, 0.001f, vPos.z } );
	CSoundManager::PlaySE("DropItem");
}

// パラメータの設定.
void CEditItem::SetParamter( const SParameter& param )
{
	if( m_NowState == ENowState::Drop ) return;
	m_Parameter = param;
	m_AccelerationValue = m_Parameter.InitAccelerationValue;
	m_Gravity = m_Parameter.InitGravity;
}

// モデルの設定.
bool CEditItem::ModelSetting()
{
	const char* modelNamaes[] =
	{
		CURE_MODEL_NAME,
		ABILITY_MODEL_NAME,
		ATTACK_MODEL_NAME,
		SPEED_MODEL_NAME,
	};
	int modelMax = sizeof(modelNamaes) / sizeof(modelNamaes[0]);

	m_pStaticMeshs.resize( modelMax );
	for( int i = 0; i < modelMax; i++ ){
		CMeshResorce::GetStatic( m_pStaticMeshs[i], modelNamaes[i] );
		if (m_pStaticMeshs[i] == nullptr) return false;
	}
	return true;
}

// エフェクトの設定.
bool CEditItem::EffectSetting()
{
	const char* effectNames[] =
	{
		CURE_EFFECT_NAME,
		ABILITY_EFFECT_NAME,
		ATTACK_EFFECT_NAME,
		SPEED_EFFECT_NAME,
	};
	int SpriteMax = sizeof(effectNames) / sizeof(effectNames[0]);

	m_pEffects.resize( SpriteMax );
	for( int i = 0; i < SpriteMax; i++ ){
		if( m_pEffects[i] == nullptr ) m_pEffects[i] = std::make_shared<CEffectManager>();
		if( m_pEffects[i]->SetEffect(effectNames[i]) == false ) return false;
	}

	return true;
}

// 当たり判定の設定.
bool CEditItem::ColliderSetting()
{
	if( m_pStaticMeshs[0] == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitCapsule(
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		{ 0.0f, 0.0f, 0.0f },
		1.0f,
		1.0f ) )) return false;
	return true;
}
