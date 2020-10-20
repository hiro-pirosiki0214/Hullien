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

// èâä˙âªä÷êî.
bool STG::CEnemy::Init()
{
	if( CollisionInit()											== false ) return false;
	if( BulletInit( m_pBullets, PARAMETER.BulletCountMax, BULLET_MODEL_NAME ) == false )	return false;
	if( FAILED( m_pFont->Init( CDirectX11::GetDevice(), CDirectX11::GetContext() ) ))	return false;
	// è’ìÀéûÅAíeÇè¡Ç∑Ç©Ç«Ç§Ç©ê›íË.
	if( PARAMETER.BulletCollDisappear == 1 ) for( auto& b : m_pBullets ) b->SetCollDisappear();
	return true;
}

// çXêVä÷êî.
void STG::CEnemy::Update()
{
	switch( m_NowState )
	{
	case EState_Spawn:	Spawn();	break;	// ÉXÉ|Å[Éì.
	case EState_Move:	Move();		break;	// à⁄ìÆ.
	case EState_Shot:	Shot();		break;	// íeÇåÇÇ¬.
	case EState_Escape: Escape();	break;	// ì¶Ç∞ÇÈ.
	case EState_Dead:	Dead();		break;	// éÄñS.
	default:						break;
	}

	BulletUpdate();
}

// ï`âÊä÷êî.
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
	if( m_SpawnCount < PARAMETER.SpawnTime*FPS ) return;
	m_NowState = EState_Move;
}


// à⁄ìÆä÷êî.
void STG::CEnemy::Move()
{
	m_vPosition.z += m_MoveSpeed;
	if( m_vPosition.z >= MOVE_SUB_POSITION_Z ) m_MoveSpeed -= MOVE_SUB_VALUE;
	if( m_MoveSpeed > 0.0f ) return;
	m_IsActive = true;
	m_NowState = EState_Shot;
	// íeÇÃêîÇ∆â~ÇÃä‘äuÇ≈èâä˙ìxÇéÊìæÇ∑ÇÈ.
	const float startDegree =
		static_cast<float>(PARAMETER.ShotBulletCount*D3DXToDegree(PARAMETER.BulletAngle)) -
		static_cast<float>((PARAMETER.ShotBulletCount+1)*(D3DXToDegree(PARAMETER.BulletAngle)/2));
	// ëäéËÇÃäpìxÇéÊìæ.
	m_Angle = atan2(
		m_vPosition.x - m_TargetPositon.x,
		m_vPosition.z - m_TargetPositon.z );
	m_Angle -= static_cast<float>(D3DXToRadian(startDegree));
}

// íeÇåÇÇ¬.
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
		m_Angle += PARAMETER.ShotAngle;	// äpìxÇÃâ¡éZ.

		break;
	case 2:
		if( m_NowShotBulletCount == PARAMETER.AnyBulletCountMax ) m_NowState = EState_Escape;

		BulletShotAnyWay( m_Angle, PARAMETER.BulletAngle, PARAMETER.BulletSpeed, PARAMETER.ShotBulletCount );
		m_Angle += PARAMETER.ShotAngle;	// äpìxÇÃâ¡éZ.

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
	if( m_MoveSpeed < PARAMETER.MoveSpeed ) m_MoveSpeed += MOVE_SUB_VALUE;
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
	}
}

// ìñÇΩÇËîªíËÇÃçÏê¨.
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