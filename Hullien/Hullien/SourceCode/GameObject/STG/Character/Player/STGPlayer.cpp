#include "STGPlayer.h"
#include "..\..\Bullet\STGBullet.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Utility\XInput\XInput.h"

STG::CPlayer::CPlayer()
	: m_Direction	( 0.0f, 0.0f, 0.0f )
{
	m_pBullets.resize( BULLET_COUNT_MAX );
}

STG::CPlayer::~CPlayer()
{
}

// 初期化関数.
bool STG::CPlayer::Init()
{
	if( CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME ) == false ) return false;
	if( BulletInit( BULLET_MODEL_NAME ) == false ) return false;
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
	m_pStaticMesh->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	MeshRender();
	BulletRender();
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
	// コントローラーのLスティックの傾きを取得.
	m_MoveVector.x = static_cast<float>(CXInput::LThumbX_Axis());
	m_MoveVector.z = static_cast<float>(CXInput::LThumbY_Axis());
	// コントローラーのRスティックの傾きを取得.
	m_Direction.x = static_cast<float>(CXInput::RThumbX_Axis());
	m_Direction.z = static_cast<float>(CXInput::RThumbY_Axis());
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
	if( CXInput::R_Button() == CXInput::enPRESS_AND_HOLD ){
		m_ShotCount++;
		if( m_ShotCount == SHOT_INTERVAL_FRAME ){
			BulletShot( m_vRotation.y, BULLET_MOVE_SPEED );
			m_ShotCount = 0;
		}
	}
}