#include "STGPlayer.h"
#include "..\..\Bullet\STGBullet.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Utility\XInput\XInput.h"

STG::CPlayer::CPlayer()
	: m_Direction		( 0.0f, 0.0f, 0.0f )
	, m_SpawnMoveSpeed	( MOVE_SPEED )
	, m_IsDead			( false )
{
	m_pCollManager	= std::make_shared<CCollisionManager>();
	m_vPosition		= INIT_POSITION;
	m_LifePoint		= LIFE_POINT_MAX;
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
	SpawnMove();	// スポーン移動.
	Move();			// 移動.
	DeadUpdate();	// 死亡処理.
	BulletUpdate();	// 弾の更新.
}

// 描画関数.
void STG::CPlayer::Render()
{
	if( m_pStaticMesh == nullptr ) return;
	MeshRender();					// メッシュの描画.
	BulletRender( BULLET_COLOR );	// 弾の描画.

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

// スポーン移動.
void STG::CPlayer::SpawnMove()
{
	if( m_IsActive == true ) return;
	m_vPosition.z -= m_SpawnMoveSpeed;
	if( m_vPosition.z <= SPAWN_END_POS_Z ) m_SpawnMoveSpeed -= SPAWN_SPEED_SUB;
	if( m_SpawnMoveSpeed > 0.0f ) return;
	m_IsActive = true;
}

// 移動関数.
void STG::CPlayer::Move()
{
	if( m_IsActive == false ) return;
	if( m_IsDead == true ) return;
	m_vPosition.x -= m_MoveVector.x * MOVE_SPEED;
	m_vPosition.z -= m_MoveVector.z * MOVE_SPEED;
	OutDispMove();	// 画面外に行った際の処理.
}

// 死亡後処理.
void STG::CPlayer::DeadUpdate()
{
	if( m_IsDead == false ) return;

	m_vScale.x -= DEAD_SPEED;
	m_vScale.y -= DEAD_SPEED;
	m_vScale.z -= DEAD_SPEED;
	m_vRotation.y += DEAD_SPEED;

	if( m_vScale.x > 0.0f ) return;
	m_vRotation.y		= 0.0f;
	m_vScale			= { 1.0f, 1.0f, 1.0f };
	m_IsDead			= false;
	m_IsActive			= false;
	m_SpawnMoveSpeed	= MOVE_SPEED;
	m_LifePoint			= LIFE_POINT_MAX;
	m_vPosition			= INIT_POSITION;
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

	if( GetAsyncKeyState(VK_UP) & 0x8000 )		m_MoveVector.z = IDLE_THUMB_MAX;
	if( GetAsyncKeyState(VK_DOWN) & 0x8000 )	m_MoveVector.z = IDLE_THUMB_MIN;
	if( GetAsyncKeyState(VK_RIGHT) & 0x8000 )	m_MoveVector.x = IDLE_THUMB_MAX;
	if( GetAsyncKeyState(VK_LEFT) & 0x8000 )	m_MoveVector.x = IDLE_THUMB_MIN;

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
	if( CXInput::R_Button() == CXInput::enPRESS_AND_HOLD || ( GetAsyncKeyState('Z') & 0x8000 )){
		m_ShotCount++;
		if( m_ShotCount == SHOT_INTERVAL_FRAME ){
			BulletShot( m_vRotation.y, BULLET_MOVE_SPEED );
			m_ShotCount = 0;
		}
	}
}

// ライフ計算関数.
void STG::CPlayer::LifeCalculation( const std::function<void(float&)>& proc )
{
	proc( m_LifePoint );

	if( m_LifePoint > 0.0f ) return;
	m_IsDead = true;
}

// 画面外に行った時の処理.
void STG::CPlayer::OutDispMove()
{
	if( OUT_POSITION_X < m_vPosition.x || m_vPosition.x < -OUT_POSITION_X ){
		m_vPosition.x += m_MoveVector.x * MOVE_SPEED;
	}
	if( OUT_POSITION_Z < m_vPosition.z || m_vPosition.z < -OUT_POSITION_Z ){
		m_vPosition.z += m_MoveVector.z * MOVE_SPEED;
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