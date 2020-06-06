#include "Player.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include "..\..\..\..\Camera\CameraManager\CameraManager.h"

CPlayer::CPlayer()	
	: m_pSkinMesh	( nullptr )
	, m_pCamera		( nullptr )
{
	m_pCamera = std::make_shared<CRotLookAtCenter>();
	m_vSclae = { 0.03f, 0.03f, 0.03f };
}

CPlayer::~CPlayer()
{
}

// 初期化関数.
bool CPlayer::Init()
{
	if( m_pSkinMesh != nullptr ) return true;
	CMeshResorce::GetSkin( m_pSkinMesh, MODEL_NAME );

	if( m_pSkinMesh == nullptr ) return false;
	return true;
}

// 更新関数.
void CPlayer::Update()
{
	Controller();	// 操作.
	Move();			// 移動.

	m_pCamera->SetLength( 7.0f );	// 中心との距離を設定.
	m_pCamera->SetHeight( 3.0 );	// 高さの設定.
	// プレイヤーを注視して回転.
	m_pCamera->RotationLookAtObject( m_vPosition );

	// カメラをマネージャーに設定.
	CCameraManager::SetCamera( m_pCamera );
}

// 描画関数.
void CPlayer::Render()
{
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->Render();
}

// 操作関数.
void CPlayer::Controller()
{
	// コントローラーのLスティックの傾きを取得.
	m_MoveVector.x = static_cast<float>(CXInput::LThumbX_Axis());
	m_MoveVector.z = static_cast<float>(CXInput::LThumbY_Axis());

	// カメラの回転移動.
	// 横方向.
	if( CXInput::RThumbX_Axis() >= IDLE_THUMB_MAX ) 
		m_pCamera->DegreeHorizontalMove(  0.05f );	// 右方向.
	if( CXInput::RThumbX_Axis() <= IDLE_THUMB_MIN ) 
		m_pCamera->DegreeHorizontalMove( -0.05f );	// 左方向.
}

// 移動関数.
void CPlayer::Move()
{
	// 各値が有効範囲外なら終了.
	if( m_MoveVector.x < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.x &&
		m_MoveVector.z < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.z ) return;
	// スティックの傾いた方向に向く.
	m_vRotation.y = atan2f( m_MoveVector.x, m_MoveVector.z );
	// カメラの角度と足し合わせる.
	m_vRotation.y += m_pCamera->GetRadianX();
	// 回転軸で移動.
	m_vPosition.x -= sinf( m_vRotation.y ) * m_MoveSpeed;
	m_vPosition.z -= cosf( m_vRotation.y ) * m_MoveSpeed;
}