#include "STGEnemy.h"
#include "..\..\Bullet\STGBullet.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"

STG::CEnemy::CEnemy()
	: m_NowShotBulletCount	( 0 )
{
	m_pBullets.resize( BULLET_COUNT_MAX );
}

STG::CEnemy::~CEnemy()
{
}

// èâä˙âªä÷êî.
bool STG::CEnemy::Init()
{
	if( CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME ) == false ) return false;
	if( BulletInit( BULLET_MODEL_NAME ) == false ) return false;
	return true;
}

// çXêVä÷êî.
void STG::CEnemy::Update()
{
	static bool s = false;
	static float rot = 0.0f;
	if( GetAsyncKeyState('B') & 0x0001 ){
		s = true;
	}
	if( s == true ){
		m_ShotCount++;
		if( m_NowShotBulletCount < BULLET_COUNT_MAX ){
			if( m_ShotCount == SHOT_INTERVAL_FRAME ){
				BulletShotAnyWay( rot, 15.0f, BULLET_MOVE_SPEED, 3 );
				m_ShotCount = 0;
				rot += static_cast<float>(D3DXToRadian(5.0));	// äpìxÇÃâ¡éZ.
			}
		}
	}
	BulletUpdate();
}

// ï`âÊä÷êî.
void STG::CEnemy::Render()
{
	MeshRender();
	BulletRender();
}

// à⁄ìÆä÷êî.
void STG::CEnemy::Move()
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
	float addrot = rot;
	int count = 0;
	for( auto& b : m_pBullets ){
		if( count == bulletCount ) return;
		if( b->Shoot( m_vPosition, addrot, moveSpeed ) == false ) continue;
		addrot += static_cast<float>(D3DXToRadian(angle));	// äpìxÇÃâ¡éZ.
		count++;
	}
}