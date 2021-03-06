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
		, m_Color						( 0.0f, 0.0f, 0.0f, 1.0f )
		, m_MoveSpeed					( 0.0f )
		, m_IsBulletToBulletCollJudge	( false )
	{
		m_pCollManager	= std::make_shared<CCollisionManager>();
		m_ObjectTag		= EObjectTag::STG_Bullet;
	}

	CBullet::~CBullet()
	{
	}

	// 初期化関数.
	bool CBullet::Init()
	{
		if( CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME ) == false ) return false;
		if( CollisionInit() == false ) return false;
		return true;
	}

	// 更新関数.
	void CBullet::Update()
	{
		if( m_IsActive == false ) return;
		m_vPosition.x -= m_MoveVector.x * m_MoveSpeed;
		m_vPosition.z -= m_MoveVector.z * m_MoveSpeed;

		if( IsDisplayOut() == true ) m_IsActive = false;
	}

	// 描画関数.
	void CBullet::Render()
	{
		if( m_IsActive == false ) return;
		m_pStaticMesh->SetColor( m_Color );
		MeshRender();

#ifdef _DEBUG
		m_pCollManager->DebugRender();
#endif	// #ifdef _DEBUG.
	}

	// 当たり判定.
	void CBullet::Collision( STG::CActor* pActor )
	{
		if( m_IsActive			== false ) return;	// 自分が動作してなければ終了.
		if( pActor->GetActive()	== false ) return;	// 相手が動作してなければ終了.

		// 相手が弾かどうか.
		bool targetIsBullet = false;
		if( pActor->GetObjectTag() == EObjectTag::STG_Bullet ){
			if( m_IsBulletToBulletCollJudge == false ) return;	// 弾同士の当たり判定を行うか.
			targetIsBullet = true;
		}

		// カプセルの当たり判定.
		if( m_pCollManager->IsCapsuleToCapsule( pActor->GetColl() ) == false ) return;

		// 衝突相手のライフ計算関数.
		pActor->LifeCalculation( []( float& life )
		{
			life -= 1.0f;
		});

		// 相手が弾なら相手の弾の動作をなくす.
		if( targetIsBullet == true ) pActor->SetActive( false );
		m_IsActive = false;	// 自分の動作をなくす.
	}

	// 撃つ.
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

	// 当たり判定の作成.
	bool CBullet::CollisionInit()
	{
		if( FAILED( m_pCollManager->InitCapsule(
			&m_vPosition,
			&m_vRotation,
			&m_vScale.x,
			{0.0f, 0.0f, 0.0f},
			1.0f,
			1.0f ))) return false;
		return true;
	}
};