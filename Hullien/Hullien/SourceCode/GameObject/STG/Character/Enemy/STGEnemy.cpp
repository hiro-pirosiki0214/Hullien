#include "STGEnemy.h"
#include "..\..\Bullet\STGBullet.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Common\D3DX\D3DX11.h"
#include "..\..\..\..\Common\Font\Font.h"
#include "..\..\..\..\Utility\XInput\XInput.h"

STG::CEnemy::CEnemy()
	: CEnemy	( STG::SEnemyParam() )
{
}

STG::CEnemy::CEnemy( const STG::SEnemyParam& param )
	: PARAMETER				( param )
	, m_pFont				( nullptr )
	, m_FontRotation		( FONT_ROTATION )
	, m_NowState			( STG::EEnemyState::Spawn )
	, m_MoveSpeed			( 0.0f )
	, m_MoveingDistance		( 0.0f )
	, m_MoveingDistanceMax	( 0.0f )
	, m_SpawnCount			( 0 )
	, m_ShotAngle			( 0.0f )
	, m_NowShotBulletCount	( 0 )
	, m_IsHitShake			( false )
	, m_ShakeCount			( SHAKE_COUNT_MAX )
	, m_EscapeCount			( ESCAPE_COUNT_MAX )
{
	m_pFont			= std::make_unique<CFont>();
	m_pCollManager	= std::make_shared<CCollisionManager>();
	m_vPosition.z	= INIT_POSITION_Z;
	m_vPosition.x	= PARAMETER.PositionX;
	m_vRotation		= { 0.0f, 0.0f, static_cast<float>(D3DXToRadian(90)) };
	m_MoveSpeed		= PARAMETER.MoveSpeed;
	m_LifePoint		= PARAMETER.LifePoint;
	m_vScale		= { PARAMETER.TextSize, PARAMETER.TextSize, PARAMETER.TextSize };
}

STG::CEnemy::~CEnemy()
{
}

// 初期化関数.
bool STG::CEnemy::Init()
{
	if( CollisionInit()											== false ) return false;
	if( BulletInit( m_pBullets, PARAMETER.BulletCountMax, BULLET_MODEL_NAME ) == false )	return false;
	if( FAILED( m_pFont->Init( CDirectX11::GetDevice(), CDirectX11::GetContext() ) ))	return false;
	// 衝突時、弾を消すかどうか設定.
	if( PARAMETER.BulletCollDisappear == 1 ) for( auto& b : m_pBullets ) b->SetCollDisappear();
	return true;
}

// 更新関数.
void STG::CEnemy::Update()
{
	BulletUpdate();	// 弾の更新.

	switch( m_NowState )
	{
	case STG::EEnemyState::Spawn:	Spawn();	break;	// スポーン.
	case STG::EEnemyState::Move:	Move();		break;	// 移動.
	case STG::EEnemyState::Shot:	Shot();		break;	// 弾を撃つ.
	case STG::EEnemyState::Escape:	Escape();	break;	// 逃げる.
	case STG::EEnemyState::Dead:	Dead();		break;	// 死亡.
	default:									break;
	}

	HitShake();		// ヒット時の揺れ.
}

// 描画関数.
void STG::CEnemy::Render()
{
	BulletRender( 
		{ 
			1.0f,
			PARAMETER.BulletCollDisappear*BULLET_COLOR, 
			PARAMETER.BulletCollDisappear*BULLET_COLOR
		} );	// 弾の描画.

	m_pFont->SetColor( { 1.0f, 1.0f, 1.0f, 1.0f } );
	m_pFont->SetPosition( m_vPosition );
	m_pFont->SetRotation( m_FontRotation );
	m_pFont->SetScale( m_vScale );
	m_pFont->Render( PARAMETER.Text );

#ifdef _DEBUG
	m_pCollManager->DebugRender();
#endif	// #ifdef _DEBUG.
}

// 当たり判定.
void STG::CEnemy::Collision( STG::CActor* pActor )
{
	if( pActor->GetActive()	== false ) return;	// 相手が動作してなければ終了.
	// 弾の数だけあたり判定を行う.
	for( auto& b : m_pBullets ) b->Collision( pActor );

	if( m_IsActive			== false ) return;	// 自分が動作してなければ終了.
	// カプセルの当たり判定.
	if( m_pCollManager->IsCapsuleToCapsule( pActor->GetColl() ) == false ) return;
}

// スポーン.
void STG::CEnemy::Spawn()
{
	m_SpawnCount++;
	if( m_SpawnCount < PARAMETER.SpawnTime ) return;
	m_NowState = STG::EEnemyState::Move;
	m_IsActive = true;
}


// 移動関数.
void STG::CEnemy::Move()
{
	m_vPosition.z += m_MoveSpeed;
	if( m_vPosition.z >= MOVE_SUB_POSITION_Z ) m_MoveSpeed -= MOVE_SUB_VALUE;

	if( m_MoveSpeed > 0.0f ) return;

	m_NowState = STG::EEnemyState::Shot;

	// 相手の角度を取得.
	m_ShotAngle = atan2(
		m_vPosition.x - m_TargetPositon.x,
		m_vPosition.z - m_TargetPositon.z );
}

// 弾を撃つ.
void STG::CEnemy::Shot()
{
	m_ShotCount++;
	if( m_ShotCount != PARAMETER.ShotIntervalFrame ) return;

	float angle = m_ShotAngle;
	for( int i = 0; i < PARAMETER.AnyBulletCountMax; i++ ){
		if( m_NowShotBulletCount < PARAMETER.BulletCountMax ){
			STG::CCharacter::BulletShot( angle, PARAMETER.BulletSpeed );
			angle += PARAMETER.ShotAngle;
			m_NowShotBulletCount++;
		}
	}
	if( m_NowShotBulletCount == PARAMETER.BulletCountMax ){
		m_NowState = STG::EEnemyState::Escape;
	}
	m_ShotAngle += PARAMETER.BulletAngle;	// 角度の加算.
	m_ShotCount = 0;
}

// 逃げる.
void STG::CEnemy::Escape()
{
	// 移動速度を一定速度になるまで加算.
	if( m_MoveSpeed < PARAMETER.MoveSpeed ) m_MoveSpeed += MOVE_SUB_VALUE;

	// ベクトルを使用して移動.
	m_vPosition.x += m_MoveVector.x * m_MoveSpeed;
	m_vPosition.z += m_MoveVector.z * m_MoveSpeed;

	m_EscapeCount--;	// 逃げるカウントの減算.
	// 逃げるカウントが 0.0 より大きければ.
	if( m_EscapeCount > 0.0f ){
		m_MoveingDistance += m_MoveSpeed;	// 距離を加算.
	}
	// 移動距離が一定距離を超得たら.
	if( m_MoveingDistance >= m_MoveingDistanceMax ){
		SearchRandomMoveVector();	// 移動ベクトルを検索する.
	}
	// 画面外に出たら.
	if( IsDisplayOut( E_WND_OUT_ADJ_SIZE ) == true ){
		m_NowState = STG::EEnemyState::Dead;	// 死亡.
	}
}

// 死亡.
void STG::CEnemy::Dead()
{
	m_IsActive	= false;		// 動作終了.
	m_vScale.x -= DEAD_SCALE_SUB_VALUE;	// スケールを加算.
	m_vScale.y -= DEAD_SCALE_SUB_VALUE;	// スケールを加算.
	m_vScale.z -= DEAD_SCALE_SUB_VALUE;	// スケールを加算.
	m_FontRotation.z += DEAD_ROTATION_SPEED;	// 回転させる.

	if( m_vScale.x > 0.0f ) return;
	// スケールが 0.0 以下になれば.
	m_NowState	= STG::EEnemyState::None;	// 何もない状態へ遷移.
	// 座標を画面外へ.
	m_vPosition	= { INIT_POSITION_Z, 0.0f, INIT_POSITION_Z };
}

// 当たった時の揺れ.
void STG::CEnemy::HitShake()
{
	if( m_IsHitShake == false ) return;

	m_ShakeCount -= SHAKE_SUB_VALUE;	// 揺れカウントを減算.
	m_vPosition.x += sinf( static_cast<float>(D3DX_PI)		* m_ShakeCount ) * SHAKE_SPEED;
	m_vPosition.z += sinf( static_cast<float>(D3DX_PI)*0.5f * m_ShakeCount ) * SHAKE_SPEED;

	if( m_ShakeCount > 0.0f ) return;
	// 揺れカウントが 0.0 以下になれば.
	m_ShakeCount = SHAKE_COUNT_MAX;	// 揺れカウントを初期化.
	m_IsHitShake = false;			// ヒットフラグを下す.
}

// ライフ計算関数.
void STG::CEnemy::LifeCalculation( const std::function<void(float&)>& proc )
{
	// 逃げる状態のみ攻撃を受ける.
	if( m_NowState != STG::EEnemyState::Escape ){
		m_IsHitShake = true;	// ヒットフラグを立てる.
		return;
	}

	proc( m_LifePoint );
	m_IsHitShake = true;	// ヒットフラグを立てる.

	if( m_LifePoint > 0.0f ) return;
	// ライフが0以下になれば
	m_NowState = STG::EEnemyState::Dead;	// 死亡状態へ遷移.
}

// ランダムで移動ベクトルを検索.
void STG::CEnemy::SearchRandomMoveVector()
{
	// 移動ベクトルをランダムから取得.
	m_MoveVector.x = sinf( static_cast<float>(rand()) );
	m_MoveVector.z = cosf( static_cast<float>(rand()) );

	// 最大移動距離をランダムから取得.
	m_MoveingDistanceMax = fabsf(cosf( static_cast<float>(rand()) )) * ESCAPE_MOVE_DISTANCE;
	m_MoveingDistance = 0.0f;
	BulletShot( m_ShotAngle, PARAMETER.BulletSpeed );
	m_ShotAngle += PARAMETER.ShotAngle;	// 角度の加算.
}

// 当たり判定の作成.
bool STG::CEnemy::CollisionInit()
{
	if( FAILED( m_pCollManager->InitCapsule(
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		{0.0f, 0.0f, 0.0f},
		PARAMETER.TextSize,
		PARAMETER.TextSize*2.0f*static_cast<float>(PARAMETER.Text.length()) ))) return false;
	return true;
}