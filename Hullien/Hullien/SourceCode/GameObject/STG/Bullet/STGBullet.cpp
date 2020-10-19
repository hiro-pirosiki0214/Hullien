#include "STGBullet.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Collider\CollsionManager\CollsionManager.h"

namespace STG
{
	CBullet::CBullet()
		: CBullet	( "" )
	{
	}

	CBullet::CBullet( const char* modelName )
		: MODEL_NAME					( modelName )
		, m_MoveSpeed					( 0.0f )
		, m_IsBulletToBulletCollJudge	( false )
	{
		m_pCollManager	= std::make_shared<CCollisionManager>();
		m_ObjectTag		= EObjectTag::STG_Bullet;
	}

	CBullet::~CBullet()
	{
	}

	// ‰Šú‰»ŠÖ”.
	bool CBullet::Init()
	{
		if( CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME ) == false ) return false;
		if( CollisionInit() == false ) return false;
		return true;
	}

	// XVŠÖ”.
	void CBullet::Update()
	{
		if( m_IsActive == false ) return;
		m_vPosition.x -= m_MoveVector.x * m_MoveSpeed;
		m_vPosition.z -= m_MoveVector.z * m_MoveSpeed;

		if( IsDisplayOut() == true ) m_IsActive = false;
	}

	// •`‰æŠÖ”.
	void CBullet::Render()
	{
		if( m_IsActive == false ) return;
		MeshRender();

#ifdef _DEBUG
		m_pCollManager->DebugRender();
#endif	// #ifdef _DEBUG.
	}

	// “–‚½‚è”»’è.
	void CBullet::Collision( STG::CActor* pActor )
	{
		if( m_IsActive			== false ) return;	// Ž©•ª‚ª“®ì‚µ‚Ä‚È‚¯‚ê‚ÎI—¹.
		if( pActor->GetActive()	== false ) return;	// ‘ŠŽè‚ª“®ì‚µ‚Ä‚È‚¯‚ê‚ÎI—¹.

		// ‘ŠŽè‚ª’e‚©‚Ç‚¤‚©.
		bool targetIsBullet = false;
		if( pActor->GetObjectTag() == EObjectTag::STG_Bullet ){
			if( m_IsBulletToBulletCollJudge == false ) return;	// ’e“¯Žm‚Ì“–‚½‚è”»’è‚ðs‚¤‚©.
			targetIsBullet = true;
		}

		// ƒJƒvƒZƒ‹‚Ì“–‚½‚è”»’è.
		if( m_pCollManager->IsCapsuleToCapsule( pActor->GetColl() ) == false ) return;

		// ‘ŠŽè‚ª’e‚È‚ç‘ŠŽè‚Ì’e‚Ì“®ì‚ð‚È‚­‚·.
		if( targetIsBullet == true ) pActor->SetActive( false );
		m_IsActive = false;	// Ž©•ª‚Ì“®ì‚ð‚È‚­‚·.
	}

	// Œ‚‚Â.
	bool CBullet::Shoot( 
		const D3DXVECTOR3& pos, 
		const float& rot,
		const float& moveSpeed )
	{
		if( m_IsActive == true ) return false;
		m_vPosition		= pos;
		m_vRotation.y	= rot;
		m_MoveVector	= { sinf(rot), 0.0f, cosf(rot) };
		m_MoveSpeed		= moveSpeed;
		m_IsActive		= true;
		return true;
	}

	// “–‚½‚è”»’è‚Ìì¬.
	bool CBullet::CollisionInit()
	{
		if( FAILED( m_pCollManager->InitCapsule(
			&m_vPosition,
			&m_vRotation,
			&m_vSclae.x,
			{0.0f, 0.0f, 0.0f},
			1.0f,
			1.0f ))) return false;
		return true;
	}
};