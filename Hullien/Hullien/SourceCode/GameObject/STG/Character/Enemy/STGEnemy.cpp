#include "STGEnemy.h"
#include "..\..\Bullet\STGBullet.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Common\D3DX\D3DX11.h"
#include "..\..\..\..\Common\Font\Font.h"

STG::CEnemy::CEnemy()
	: m_pFont				( nullptr )
	, m_NowState			( EState_Spawn )
	, m_MoveSpeed			( 0.0f )
	, m_SpawnCount			( 0 )
	, m_Angle				( 0.0f )
	, m_NowShotBulletCount	( 0 )
{
	m_pFont			= std::make_unique<CFont>();
	m_pCollManager	= std::make_shared<CCollisionManager>();
	m_vPosition.z	= INIT_POSITION_Z;
	m_vRotation	= { 0.0f, 0.0f, static_cast<float>(D3DXToRadian(90)) };
	m_IsActive	= true;
	m_MoveSpeed = MOVE_SPEED;
}

STG::CEnemy::~CEnemy()
{
}

// èâä˙âªä÷êî.
bool STG::CEnemy::Init()
{
	if( CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME )	== false ) return false;
	if( CollisionInit()											== false ) return false;
	if( BulletInit( m_pBullets,				BULLET_COUNT_MAX, BULLET_MODEL_NAME ) == false ) return false;
	if( FAILED( m_pFont->Init( CDirectX11::GetDevice(), CDirectX11::GetContext() ) )) return false;

	return true;
}

// çXêVä÷êî.
void STG::CEnemy::Update()
{
	if( m_IsActive == false ) return;

	switch( m_NowState )
	{
	case EState_Spawn:	Spawn();	break;
	case EState_Move:	Move();		break;
	case EState_Shot:	Shot();		break;
	case EState_Escape: Escape();	break;
	case EState_Dead:	Dead();		break;
	default:
		break;
	}

	// ëäéËÇÃäpìxÇéÊìæ.
	m_Angle = atan2(
		m_TargetPositon.x - m_vPosition.x,
		m_TargetPositon.z - m_vPosition.z );

	BulletUpdate();
}

// ï`âÊä÷êî.
void STG::CEnemy::Render()
{
	if( m_IsActive == false ) return;
	m_pFont->SetColor( { 0.0f, 0.0f, 0.0f, 1.0f } );
	m_pFont->SetPosition( m_vPosition );
	m_pFont->SetRotation( FONT_ROTATION );
	m_pFont->Render( RENDER_CHAR );

	BulletRender();

#ifdef _DEBUG
	m_pCollManager->DebugRender();
#endif	// #ifdef _DEBUG.
}

// ìñÇΩÇËîªíË.
void STG::CEnemy::Collision( STG::CActor* pActor )
{
	if( m_IsActive			== false ) return;	// é©ï™Ç™ìÆçÏÇµÇƒÇ»ÇØÇÍÇŒèIóπ.
	if( pActor->GetActive()	== false ) return;	// ëäéËÇ™ìÆçÏÇµÇƒÇ»ÇØÇÍÇŒèIóπ.

	// íeÇÃêîÇæÇØÇ†ÇΩÇËîªíËÇçsÇ§.
	for( auto& b : m_pBullets ) b->Collision( pActor );
	// ÉJÉvÉZÉãÇÃìñÇΩÇËîªíË.
	if( m_pCollManager->IsCapsuleToCapsule( pActor->GetColl() ) == false ) return;
}

// ÉXÉ|Å[Éì.
void STG::CEnemy::Spawn()
{
	m_SpawnCount++;
	if( m_SpawnCount <= SPAWN_TIME*FPS ) return;
	m_NowState = EState_Move;
}


// à⁄ìÆä÷êî.
void STG::CEnemy::Move()
{
	m_vPosition.z += m_MoveSpeed;
	if( m_vPosition.z >= MOVE_SUB_POSITION_Z ) m_MoveSpeed -= MOVE_SUB_VALUE;
	if( m_MoveSpeed > 0.0f ) return;
	m_NowState = EState_Shot;
}

// íeÇåÇÇ¬.
void STG::CEnemy::Shot()
{
	static float rot = 0.0f;

	m_ShotCount++;
	if( m_ShotCount != SHOT_INTERVAL_FRAME ) return;
	switch( SHOT_NUMBER )
	{
	case 0:
		break;
	case 1:
		if( m_NowShotBulletCount == BULLET_COUNT_MAX ) m_NowState = EState_Escape;

		BulletShot( rot, BULLET_ANGLE );
		rot += SHOT_ANGLE;	// äpìxÇÃâ¡éZ.
		if( rot >= ANGLE_MAX ) rot = ANGLE_MIN;

		break;
	case 2:
		if( m_NowShotBulletCount == ANY_BULLET_COUNT_MAX ) m_NowState = EState_Escape;

		BulletShotAnyWay( rot, BULLET_ANGLE, BULLET_MOVE_SPEED, SHOT_BULLET_COUNT );
		rot += SHOT_ANGLE;	// äpìxÇÃâ¡éZ.
		if( rot >= ANGLE_MAX ) rot = ANGLE_MIN;

		break;
	default:
		break;
	}
	m_ShotCount = 0;
	m_NowShotBulletCount++;
}

// ì¶Ç∞ÇÈ.
void STG::CEnemy::Escape()
{
	if( m_MoveSpeed < MOVE_SPEED ) m_MoveSpeed += MOVE_SUB_VALUE;
	m_vPosition.x += m_MoveSpeed;
}

// éÄñS.
void STG::CEnemy::Dead()
{
}

// íeÇåÇÇ¬.
void STG::CEnemy::BulletShot( const float& rot, const float& moveSpeed )
{
	STG::CCharacter::BulletShot( rot, moveSpeed );
}

// íeÇåÇÇ¬(ï°êî).
void STG::CEnemy::BulletShotAnyWay( 
	const float& rot, 
	const float& angle, 
	const float& moveSpeed,
	const int& bulletCount )
{
	float addrot = rot;	// åªç›ÇÃäpìxÇéÊìæ.
	int count = 0;		// åÇÇ¡ÇΩíeÇÃÉJÉEÉìÉg.
	for( auto& b : m_pBullets ){
		// åÇÇ¡ÇΩíeÇ™éwíËÇµÇΩíeÇ∆ìØÇ∂Ç…Ç»ÇÍÇŒèIóπ.
		if( count == bulletCount ) return;
		// íeÇ™åÇÇƒÇ»ÇØÇÍÇŒÇ‡Ç§àÍìx.
		if( b->Shoot( m_vPosition, addrot, moveSpeed ) == false ) continue;
		addrot += angle;	// äpìxÇÃâ¡éZ.
		count++;			// åÇÇ¡ÇΩíeÇÃâ¡éZ.
		if( addrot >= ANGLE_MAX ) addrot = ANGLE_MIN;
	}
}

// ìñÇΩÇËîªíËÇÃçÏê¨.
bool STG::CEnemy::CollisionInit()
{
	if( FAILED( m_pCollManager->InitCapsule(
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		{0.0f, 0.0f, 0.0f},
		1.0f,
		2.0f*static_cast<float>(strlen(RENDER_CHAR)) ))) return false;
	return true;
}