#include "ColliderBase.h"

CColliderBase::CColliderBase()
	: m_pvPosition		( nullptr )
	, m_vAdjPosition	{ 0.0f, 0.0f, 0.0f }
	, m_pvRotation		( nullptr )
	, m_pScale			( nullptr )
{
}

CColliderBase::~CColliderBase()
{
}

// メッシュの頂点の最大・最小を取得する.
HRESULT CColliderBase::GetBoundingBox( LPD3DXMESH pInMesh, D3DXVECTOR3& vOutMax, D3DXVECTOR3& vOutMin )
{
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
	VOID* pVertices = nullptr;
	D3DXVECTOR3	Max, Min;

	if( FAILED( pInMesh->GetVertexBuffer( &pVB ) ) ){
		_ASSERT_EXPR( false, L"頂点バッファ取得失敗" );
		return E_FAIL;
	}
	// メッシュの頂点バッファをロックする.
	if( FAILED( pVB->Lock( 0, 0, &pVertices, 0 ) ) ){
		_ASSERT_EXPR( false, L"頂点バッファのロック失敗" );
		SAFE_RELEASE( pVB );
		return E_FAIL;
	}
	// メッシュ内の頂点位置の最大と最小を検索する.
	D3DXComputeBoundingBox(
		static_cast<D3DXVECTOR3*>( pVertices ),
		pInMesh->GetNumVertices(),
		D3DXGetFVFVertexSize( pInMesh->GetFVF() ),
		&Min, &Max );	// (out)最小,最大頂点.

	vOutMax = Max;
	vOutMin = Min;

	pVB->Unlock();		// アンロック.
	SAFE_RELEASE( pVB );	// 使用済み頂点バッファの解放.

	return S_OK;
}

// メッシュの頂点の最大・最小を取得する.
HRESULT CColliderBase::GetBoundingSphere( LPD3DXMESH pInMesh, float& fOutRadius )
{
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr; // 頂点バッファ.
	void* pVertices = nullptr;	// 頂点.
	D3DXVECTOR3 vCenter;		// 中心.

								// 頂点バッファを取得.
	if( FAILED( pInMesh->GetVertexBuffer( &pVB ) ) ){
		ERROR_MESSAGE("Failed to get mesh vertex buffer");
		return E_FAIL;
	}
	// メッシュの頂点バッファをロックする.
	if( FAILED( pVB->Lock( 0, 0, &pVertices, 0 ) ) ){
		SAFE_RELEASE( pVB );
		ERROR_MESSAGE("Lock failure of mesh vertex buffer");
		return E_FAIL;
	}
	// メッシュの外接円の中心と半径を計算する.
	D3DXComputeBoundingSphere(
		static_cast<D3DXVECTOR3*>( pVertices ),
		pInMesh->GetNumVertices(),				// 頂点の数.
		D3DXGetFVFVertexSize( pInMesh->GetFVF() ),// 頂点の情報.
		&vCenter,								// (out)中心座標.
		&fOutRadius );							// (out)半径.

	// アンロック.
	pVB->Unlock();
	SAFE_RELEASE( pVB );

	return S_OK;
}