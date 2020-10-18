#include "Bullet.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"

namespace STG
{
	CBullet::CBullet()
		: CBullet	( "" )
	{
	}

	CBullet::CBullet( const char* modelName )
		: MODEL_NAME	( modelName )
		, m_MoveSpeed	( 0.0f )
		, m_IsShot		( false )
	{
	}

	CBullet::~CBullet()
	{
	}

	// 初期化関数.
	bool CBullet::Init()
	{
		if( CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME ) == false ) return false;
		return true;
	}

	// 更新関数.
	void CBullet::Update()
	{
		if( m_IsShot == false ) return;
		m_vPosition.x -= m_MoveVector.x * m_MoveSpeed;
		m_vPosition.z -= m_MoveVector.z * m_MoveSpeed;

		if( IsDisplayOut() == true ) m_IsShot = false;
	}

	// 描画関数.
	void CBullet::Render()
	{
		if( m_IsShot == false ) return;
		MeshRender();
	}

	// 撃つ.
	bool CBullet::Shoot( 
		const D3DXVECTOR3& pos, 
		const float& rot,
		const float& moveSpeed )
	{
		if( m_IsShot == true ) return false;
		m_vPosition		= pos;
		m_vRotation.y	= rot;
		m_MoveVector	= { sinf(rot), 0.0f, cosf(rot) };
		m_MoveSpeed		= moveSpeed;
		m_IsShot		= true;
		return true;
	}
};