#include "Alien_D.h"
#include "..\..\..\LaserBeam\LaserBeam.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\XAudio2\SoundManager.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"

#define IS_TEMP_MODEL_RENDER

CAlienD::CAlienD()
	: m_pAttackRangeSprite	( nullptr )
	, m_pLaserBeam			( nullptr )
	, m_ControlPositions	( 1 )
	, m_AttackCount			( 0.0f )
	, m_IsAttackStart		( false )
{
	m_ObjectTag = EObjectTag::Alien_D;
	m_pLaserBeam = std::make_unique<CLaserBeam>();
}

CAlienD::~CAlienD()
{
}

// 初期化関数.
bool CAlienD::Init()
{
#ifndef IS_TEMP_MODEL_RENDER
	if( GetModel( MODEL_NAME ) == false ) return false;
#else
	// 既に読み込めていたら終了.
	if( m_pTempStaticMesh != nullptr ) return true;
	// モデルの取得.
	CMeshResorce::GetStatic( m_pTempStaticMesh, MODEL_TEMP_NAME );
	// モデルが読み込めてなければ false.
	if( m_pTempStaticMesh == nullptr ) return false;
#endif	// #ifndef IS_TEMP_MODEL_RENDER.
	if( GetSprite( SPRITE_NAME ) == false ) return false;
	if( ColliderSetting() == false ) return false;
	if( m_pLaserBeam->Init() == false ) return false; 
	return true;
}

// 更新関数.
void CAlienD::Update()
{
	SetMoveVector( m_TargetPosition );	// 目的のベクトルを取得.
	m_pLaserBeam->Update();	// レーザーの更新.
	CurrentStateUpdate();	// 現在の状態の更新.
}

// 描画関数.
void CAlienD::Render()
{
	// 画面の外なら終了.
	if( IsDisplayOut() == true ) return;
	m_pLaserBeam->Render();	// レーザーの描画.
	ModelRender();			// モデルの描画.

#if _DEBUG
	if( m_pCollManager == nullptr ) return;
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// 当たり判定関数.
void CAlienD::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	AttackCollision( pActor );	// 攻撃(レーザー)の当たり判定.
	BarrierCollision( pActor );	// バリアの当たり判定.
}

// スポーン.
bool CAlienD::Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos )
{
	// 既にスポーン済みなら終了.
	if( m_NowState != EAlienState::None ) return true;
	m_Parameter = param;	// パラメータを設定.
	// 初期化に失敗したら終了.
	if( Init() == false ) return false;
	m_vPosition		= spawnPos;					// スポーン座標の設定.
	m_LifePoint		= m_Parameter.LifeMax;		// 体力の設定.
	m_NowState = EAlienState::Spawn;	// 現在の状態をスポーンに変更.
	
	// レーザーの移動速度の設定.
	m_pLaserBeam->SetMoveSpped( m_Parameter.LaserMoveSpeed );
	// レーザーの麻痺時間の設定.
	m_pLaserBeam->SetParalysisTime( m_Parameter.ParalysisTime );

	if (CSoundManager::GetIsPlaySE("AlienDApp", 0) == false)
	{
		CSoundManager::NoMultipleSEPlay("AlienDApp");
	}
	return true;
}

// スプライトの描画.
void CAlienD::SpriteRender()
{
	AttackRangeSpriteRender();
}

// モデルの描画.
void CAlienD::ModelRender()
{
#ifndef IS_TEMP_MODEL_RENDER
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->SetColor( { 0.5f, 0.8f, 0.5f, m_ModelAlpha } );
	AlphaBlendSetting();
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pSkinMesh->Render();
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pSkinMesh->SetBlend( false );
#else
	if( m_pTempStaticMesh == nullptr ) return;
	m_pTempStaticMesh->SetPosition( m_vPosition );
	m_pTempStaticMesh->SetRotation( m_vRotation );
	m_pTempStaticMesh->SetScale( m_vSclae );
	m_pTempStaticMesh->SetColor( { 0.8f, 0.8f, 0.0f, m_ModelAlpha } );
	AlphaBlendSetting();
	m_pTempStaticMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pTempStaticMesh->Render();
	m_pTempStaticMesh->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pTempStaticMesh->SetBlend( false );
#endif	// #ifdef IS_TEMP_MODEL_RENDER.
}

// 攻撃範囲のスプライト描画.
void CAlienD::AttackRangeSpriteRender()
{
	if( m_pAttackRangeSprite == nullptr ) return;
	if( m_NowMoveState != EMoveState::Attack ) return;

	D3DXVECTOR4 color;
	if( m_IsAttackStart == true ){
		// 攻撃の色 (黄).
		color	= ATTACK_RANGE_DANGER_COLOR;
		color.w = m_AttackCount;
	} else {
		// 攻撃の予告の色 (赤).
		color	= ATTACK_RANGE_COLOR;
		color.w = m_AttackCount;
	}

	// 攻撃範囲スプライトの描画.
	m_pAttackRangeSprite->SetPosition( { m_TargetPosition.x, m_Parameter.AttackRangeSpritePosY, m_TargetPosition.z } );
	m_pAttackRangeSprite->SetRotation( { static_cast<float>(D3DXToRadian(90)), 0.0f, 0.0f } );
	m_pAttackRangeSprite->SetScale( m_Parameter.AttackRangeSpriteScale );	
	m_pAttackRangeSprite->SetColor( color );
	m_pAttackRangeSprite->SetBlend( true );
	m_pAttackRangeSprite->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pAttackRangeSprite->Render();
	m_pAttackRangeSprite->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pAttackRangeSprite->SetBlend( false );
}

// スポーン.
void CAlienD::Spawning()
{
	CAlien::Spawning();
}

// 移動.
void CAlienD::Move()
{
	TargetRotation();		// 回転.
	CAlienD::VectorMove( m_MoveSpeed );	// 移動.
	Attack();				// 攻撃.
	CAlien::WaitMove();		// 待機.

	if( *m_pIsAlienOtherAbduct == false ) return;
	if( m_NowState == EAlienState::Abduct ) return;
	m_NowState		= EAlienState::Escape;	// 逃げる状態へ遷移.
	m_NowMoveState	= EMoveState::Rotation;	// 移動状態を回転する.
}

// 拐う.
void CAlienD::Abduct()
{
	// 拐わないので処理がいらない.
}

// 怯み.
void CAlienD::Fright()
{
	CAlien::Fright();
}

// 死亡.
void CAlienD::Death()
{
	CAlien::Death();
}

// 逃げる.
void CAlienD::Escape()
{
	CAlien::Escape();
}

// 攻撃関数.
void CAlienD::Attack()
{
	if( m_NowMoveState != EMoveState::Attack ) return;

	const float attackSpeed = m_IsAttackStart == false ? 
		m_Parameter.AttackRangeAddValue : m_Parameter.AttackRangeSubValue;
	m_AttackCount += attackSpeed;	// 攻撃カウントの追加.

	// 攻撃カウントが攻撃時間より多くなれば攻撃を始める.
	if( m_AttackCount >= ATTACK_TIME ){
		m_IsAttackStart = true;

		// 相手への向きを取得.
		const float radius = atan2f(
			m_TargetPosition.x - m_vPosition.x,
			m_TargetPosition.z - m_vPosition.z );

		// 相手との距離を測る.
		const float lenght = D3DXVec3Length(&D3DXVECTOR3(m_TargetPosition-m_vPosition)) - m_Parameter.ControlPointTwoLenght;

		// 上向き少し後ろに設定..
		m_ControlPositions[0].x = m_vPosition.x + sinf( radius ) * m_Parameter.ControlPointOneLenght;
		m_ControlPositions[0].y = m_vPosition.y + m_Parameter.ControlPointOneLenghtY;
		m_ControlPositions[0].z = m_vPosition.z + cosf( radius ) * m_Parameter.ControlPointOneLenght;

		// 上で設定したコントロールポジションを設定.
		m_pLaserBeam->SetControlPointList( m_ControlPositions );

		m_pLaserBeam->Shot( m_vPosition );	// ビームを打つ.
		CSoundManager::NoMultipleSEPlay("AlienDAttack");
	}

	if( m_AttackCount >= 0.0f ) return;
	m_NowMoveState = EMoveState::Wait;	// 待機状態へ遷移.
}

// 移動関数.
void CAlienD::VectorMove( const float& moveSpeed )
{
	if( m_NowMoveState != EMoveState::Move ) return;

	// ベクトルを使用して移動.
	m_vPosition.x -= m_MoveVector.x * moveSpeed;
	m_vPosition.z -= m_MoveVector.z * moveSpeed;

	// 再度座標を検索し、回転するか比較.
	if( D3DXVec3Length(&D3DXVECTOR3(m_BeforeMoveingPosition-m_vPosition)) >= m_Parameter.ResearchLenght ){
		m_NowMoveState	= EMoveState::Rotation;	// 回転状態へ遷移.
		m_IsAttackStart = false;	// 攻撃が始まるフラグを下す.
		return;
	}

	// プレイヤーとの距離が一定値より低いか比較.
	if( D3DXVec3Length(&D3DXVECTOR3(m_TargetPosition-m_vPosition)) >= m_Parameter.AttackLenght ) return;
	if( m_pLaserBeam->IsEndAttack() == false ) return;
	m_IsAttackStart	= false;	// 攻撃が始まるフラグを下す.
	m_AttackCount	= 0.0f;		// 攻撃カウントを初期化,
	m_NowMoveState	= EMoveState::Attack;	// 攻撃状態へ遷移.
}

// 相手座標の設定.
void CAlienD::SetTargetPos( CActor& actor )
{
	SetPlayerPos( actor );	// プレイヤー座標の設定.
}

// プレイヤー座標の設定.
void CAlienD::SetPlayerPos( CActor& actor )
{
	// プレイヤーじゃなければ終了.
	if( actor.GetObjectTag() != EObjectTag::Player ) return;
	m_pLaserBeam->SetTargetPos( actor );	// プレイヤーの座標を取得.

	// 攻撃が始まっている場合終了.
	if( m_IsAttackStart == true ) return;
	m_TargetPosition = actor.GetPosition();	// プレイヤーの座標を取得.
}

// 攻撃の当たり判定.
void CAlienD::AttackCollision( CActor* pActor  )
{
	if( m_pLaserBeam == nullptr ) return;
	m_pLaserBeam->Collision( pActor );	// レーザーの当たり判定.
}

// スプライトの取得.
bool CAlienD::GetSprite( const char* spriteName )
{
	// 既に読み込めていたら終了.
	if( m_pAttackRangeSprite != nullptr ) return true;
	// スプライトの取得.
	m_pAttackRangeSprite = CSpriteResource::GetSprite( spriteName );
	// スプライトが読み込めてなければ false.
	if( m_pAttackRangeSprite == nullptr ) return false;
	return true;
}

// 当たり判定の設定.
bool CAlienD::ColliderSetting()
{
#ifndef IS_TEMP_MODEL_RENDER
	if( m_pSkinMesh == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere( 
		m_pSkinMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius ) )) return false;
	return true;
#else
	if( m_pTempStaticMesh == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere( 
		m_pTempStaticMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius ) )) return false;
	return true;
#endif	// #ifndef IS_MODEL_RENDER.
}