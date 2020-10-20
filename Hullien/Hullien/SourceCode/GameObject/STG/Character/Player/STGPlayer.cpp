#include "STGPlayer.h"
#include "..\..\Bullet\STGBullet.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Utility\XInput\XInput.h"

STG::CPlayer::CPlayer()
	: m_Direction	( 0.0f, 0.0f, 0.0f )
{
	m_IsActive		= true;
	m_pCollManager	= std::make_shared<CCollisionManager>();
}

STG::CPlayer::~CPlayer()
{
}

// 初期化関数.
bool STG::CPlayer::Init()
{
	if( CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME )	== false ) return false;
	if( CollisionInit()											== false ) return false;
	if( BulletInit( m_pBullets,BULLET_COUNT_MAX, BULLET_MODEL_NAME )	== false ) return false;
	for( auto& b : m_pBullets ) b->SetCollDisappear();	// 当たった弾が消えるかどうかの設定.
	return true;
}

// 更新関数.
void STG::CPlayer::Update()
{
	Move();			// 移動.
	BulletUpdate();	// 弾の更新.
}

// 描画関数.
void STG::CPlayer::Render()
{
	if( m_pStaticMesh == nullptr ) return;
	MeshRender();	// メッシュの描画.
	BulletRender();	// 弾の描画.

#ifdef _DEBUG
	m_pCollManager->DebugRender();
#endif	// #ifdef _DEBUG.
}

// 当たり判定.
void STG::CPlayer::Collision( STG::CActor* pActor )
{
	if( m_IsActive			== false ) return;	// 自分が動作してなければ終了.
	if( pActor->GetActive()	== false ) return;	// 相手が動作してなければ終了.

	// 弾の数だけあたり判定を行う.
	for( auto& b : m_pBullets ) b->Collision( pActor );
	// カプセルの当たり判定.
	if( m_pCollManager->IsCapsuleToCapsule( pActor->GetColl() ) == false ) return;

}

// 移動関数.
void STG::CPlayer::Move()
{
	m_vPosition.x -= m_MoveVector.x * MOVE_SPEED;
	m_vPosition.z -= m_MoveVector.z * MOVE_SPEED;
}

// 操作関数.
void STG::CPlayer::Controller()
{
	if( m_IsActive == false ) return;
	// コントローラーのLスティックの傾きを取得.
	m_MoveVector.x = static_cast<float>(CXInput::LThumbX_Axis());
	m_MoveVector.z = static_cast<float>(CXInput::LThumbY_Axis());
	// コントローラーのRスティックの傾きを取得.
	m_Direction.x = static_cast<float>(CXInput::RThumbX_Axis());
	m_Direction.z = static_cast<float>(CXInput::RThumbY_Axis());

	if( GetAsyncKeyState('W') & 0x8000 )	m_MoveVector.z = IDLE_THUMB_MAX;
	if( GetAsyncKeyState('S') & 0x8000 )	m_MoveVector.z = IDLE_THUMB_MIN;
	if( GetAsyncKeyState('D') & 0x8000 )	m_MoveVector.x = IDLE_THUMB_MAX;
	if( GetAsyncKeyState('A') & 0x8000 )	m_MoveVector.x = IDLE_THUMB_MIN;

	if( GetAsyncKeyState(VK_UP) & 0x8000 )		m_Direction.z = IDLE_THUMB_MAX;
	if( GetAsyncKeyState(VK_DOWN) & 0x8000 )	m_Direction.z = IDLE_THUMB_MIN;
	if( GetAsyncKeyState(VK_RIGHT) & 0x8000 )	m_Direction.x = IDLE_THUMB_MAX;
	if( GetAsyncKeyState(VK_LEFT) & 0x8000 )	m_Direction.x = IDLE_THUMB_MIN;

	// 各値が有効範囲外なら終了.
	if( m_MoveVector.x < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.x &&
		m_MoveVector.z < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.z ){
		m_MoveVector = { 0.0f, 0.0f, 0.0f };
	} else {
		// ノーマライズ.
		D3DXVec3Normalize( &m_MoveVector, &m_MoveVector );
	}
	// 各値が有効範囲内なら.
	if( m_Direction.x >= IDLE_THUMB_MAX || IDLE_THUMB_MIN >= m_Direction.x ||
		m_Direction.z >= IDLE_THUMB_MAX || IDLE_THUMB_MIN >= m_Direction.z ){
		m_vRotation.y = atan2( m_Direction.x, m_Direction.z );	// 回転値を取得.
	}
	// 弾を撃つ操作.
	ShotController();
}

// 弾を撃つ操作関数.
void STG::CPlayer::ShotController()
{
	// 押した瞬間にShotCount初期化・弾を撃つ.
	if( CXInput::R_Button() == CXInput::enPRESSED_MOMENT ){
		m_ShotCount = 0;
		BulletShot( m_vRotation.y, BULLET_MOVE_SPEED );
	}
	// 長押しの場合弾を撃つ・ShotCountの加算.
	if( CXInput::R_Button() == CXInput::enPRESS_AND_HOLD || ( GetAsyncKeyState('R') & 0x8000 )){
		m_ShotCount++;
		if( m_ShotCount == SHOT_INTERVAL_FRAME ){
			BulletShot( m_vRotation.y, BULLET_MOVE_SPEED );
			m_ShotCount = 0;
		}
	}
}

// 当たり判定の作成.
bool STG::CPlayer::CollisionInit()
{
	if( FAILED( m_pCollManager->InitCapsule(
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		{0.0f, 0.0f, 0.0f},
		2.0f,
		2.0f ))) return false;
	return true;
}