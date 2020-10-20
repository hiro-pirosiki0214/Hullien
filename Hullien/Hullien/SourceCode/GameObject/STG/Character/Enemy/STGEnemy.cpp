#include "STGEnemy.h"
#include "..\..\Bullet\STGBullet.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Common\D3DX\D3DX11.h"
#include "..\..\..\..\Common\Font\Font.h"

STG::CEnemy::CEnemy()
	: CEnemy	( STG::SEnemyParam() )
{
}

STG::CEnemy::CEnemy( const STG::SEnemyParam& param )
	: PARAMETER				( param )
	, m_pFont				( nullptr )
	, m_NowState			( EState_Spawn )
	, m_MoveSpeed			( 0.0f )
	, m_SpawnCount			( 0 )
	, m_Angle				( 0.0f )
	, m_NowShotBulletCount	( 0 )
{
	m_pFont			= std::make_unique<CFont>();
	m_pCollManager	= std::make_shared<CCollisionManager>();
	m_vPosition.z	= INIT_POSITION_Z;
	m_vPosition.x	= PARAMETER.PositionX;
	m_vRotation		= { 0.0f, 0.0f, static_cast<float>(D3DXToRadian(90)) };
	m_MoveSpeed		= PARAMETER.MoveSpeed;
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
	switch( m_NowState )
	{
	case EState_Spawn:	Spawn();	break;	// スポーン.
	case EState_Move:	Move();		break;	// 移動.
	case EState_Shot:	Shot();		break;	// 弾を撃つ.
	case EState_Escape: Escape();	break;	// 逃げる.
	case EState_Dead:	Dead();		break;	// 死亡.
	default:						break;
	}

	BulletUpdate();
}

// 描画関数.
void STG::CEnemy::Render()
{
	m_pFont->SetColor( { 0.0f, 0.0f, 0.0f, 1.0f } );
	m_pFont->SetPosition( m_vPosition );
	m_pFont->SetRotation( FONT_ROTATION );
	m_pFont->Render( PARAMETER.Text );

	BulletRender();

#ifdef _DEBUG
	m_pCollManager->DebugRender();
#endif	// #ifdef _DEBUG.
}

// 当たり判定.
void STG::CEnemy::Collision( STG::CActor* pActor )
{
	if( m_IsActive			== false ) return;	// 自分が動作してなければ終了.
	if( pActor->GetActive()	== false ) return;	// 相手が動作してなければ終了.

	// 弾の数だけあたり判定を行う.
	for( auto& b : m_pBullets ) b->Collision( pActor );
	// カプセルの当たり判定.
	if( m_pCollManager->IsCapsuleToCapsule( pActor->GetColl() ) == false ) return;
}

// スポーン.
void STG::CEnemy::Spawn()
{
	m_SpawnCount++;
	if( m_SpawnCount < PARAMETER.SpawnTime*FPS ) return;
	m_NowState = EState_Move;
}


// 移動関数.
void STG::CEnemy::Move()
{
	m_vPosition.z += m_MoveSpeed;
	if( m_vPosition.z >= MOVE_SUB_POSITION_Z ) m_MoveSpeed -= MOVE_SUB_VALUE;
	if( m_MoveSpeed > 0.0f ) return;
	m_IsActive = true;
	m_NowState = EState_Shot;
	// 弾の数と円の間隔で初期度を取得する.
	const float startDegree =
		static_cast<float>(PARAMETER.ShotBulletCount*D3DXToDegree(PARAMETER.BulletAngle)) -
		static_cast<float>((PARAMETER.ShotBulletCount+1)*(D3DXToDegree(PARAMETER.BulletAngle)/2));
	// 相手の角度を取得.
	m_Angle = atan2(
		m_vPosition.x - m_TargetPositon.x,
		m_vPosition.z - m_TargetPositon.z );
	m_Angle -= static_cast<float>(D3DXToRadian(startDegree));
}

// 弾を撃つ.
void STG::CEnemy::Shot()
{
	m_ShotCount++;
	if( m_ShotCount != PARAMETER.ShotIntervalFrame ) return;
	switch( PARAMETER.ShotNumber )
	{
	case 0:
		break;
	case 1:
		if( m_NowShotBulletCount == PARAMETER.BulletCountMax ) m_NowState = EState_Escape;

		BulletShot( m_Angle, PARAMETER.BulletAngle );
		m_Angle += PARAMETER.ShotAngle;	// 角度の加算.

		break;
	case 2:
		if( m_NowShotBulletCount == PARAMETER.AnyBulletCountMax ) m_NowState = EState_Escape;

		BulletShotAnyWay( m_Angle, PARAMETER.BulletAngle, PARAMETER.BulletSpeed, PARAMETER.ShotBulletCount );
		m_Angle += PARAMETER.ShotAngle;	// 角度の加算.

		break;
	default:
		break;
	}
	m_ShotCount = 0;
	m_NowShotBulletCount++;
}

// 逃げる.
void STG::CEnemy::Escape()
{
	if( m_MoveSpeed < PARAMETER.MoveSpeed ) m_MoveSpeed += MOVE_SUB_VALUE;
	m_vPosition.x += m_MoveSpeed;
}

// 死亡.
void STG::CEnemy::Dead()
{
}

// 弾を撃つ.
void STG::CEnemy::BulletShot( const float& rot, const float& moveSpeed )
{
	STG::CCharacter::BulletShot( rot, moveSpeed );
}

// 弾を撃つ(複数).
void STG::CEnemy::BulletShotAnyWay( 
	const float& rot, 
	const float& angle, 
	const float& moveSpeed,
	const int& bulletCount )
{
	float addrot = rot;	// 現在の角度を取得.
	int count = 0;		// 撃った弾のカウント.
	for( auto& b : m_pBullets ){
		// 撃った弾が指定した弾と同じになれば終了.
		if( count == bulletCount ) return;
		// 弾が撃てなければもう一度.
		if( b->Shoot( m_vPosition, addrot, moveSpeed ) == false ) continue;
		addrot += angle;	// 角度の加算.
		count++;			// 撃った弾の加算.
	}
}

// 当たり判定の作成.
bool STG::CEnemy::CollisionInit()
{
	if( FAILED( m_pCollManager->InitCapsule(
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		{0.0f, 0.0f, 0.0f},
		1.0f,
		2.0f*static_cast<float>(PARAMETER.Text.length()) ))) return false;
	return true;
}