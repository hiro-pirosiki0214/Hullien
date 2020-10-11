#include "Arm.h"
#include "..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"

CArm::CArm()
	: m_pSkinMesh		( nullptr )
	, m_NowArmState		( EArmState::None )
	, m_AppearanceCount	( 0.0f )
	, m_ScalingValue	( SCALING_VALUE )
	, m_IsScaleDown		( false )
{
	m_vPosition = { 0.0f, 4.0f, 5.0f };
	m_vSclae = { 0.0f, 0.0f, 0.0f };
}

CArm::~CArm()
{
}

// 初期化関数.
bool CArm::Init()
{
	if( CMeshResorce::GetStatic( m_pSkinMesh, MODEL_NAME ) == false ) return false;
	if( CMeshResorce::GetStatic( m_pAMesh, "a" ) == false ) return false;
	return true;
}

// 更新関数.
void CArm::Update()
{
	if( GetAsyncKeyState('V') & 0x8000 ) m_NowArmState = EArmState::Start;
	switch( m_NowArmState )
	{
	case EArmState::Appearance:
		Appearance();	// 出現.
		break;
	default:
		break;
	}
}

// 描画関数.
void CArm::Render()
{
	if( m_pSkinMesh == nullptr ) return;

	m_vRotation.y += 0.001f;

	D3DXVECTOR3 pos = m_vPosition;
	pos.x -= sinf( m_vRotation.y ) * 2.0f;
	pos.z -= cosf( m_vRotation.y ) * 2.0f;

	m_pSkinMesh->SetPosition( pos );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->Render();

	m_pAMesh->SetPosition( m_vPosition );
	m_pAMesh->SetRotation( m_vRotation );
	m_pAMesh->SetScale( 1.0f );
	m_pAMesh->Render();
}

// 出現.
void CArm::Appearance()
{
	m_AppearanceCount += APPEARANCE_ADD_VALUE;	// 出現カウントの加算.
	if( m_IsScaleDown == false ){
		// 拡大中.
		m_ScalingValue += SCALING_ADD_VALUE;	// 拡縮値を加算.
		if( m_ScalingValue >= SCALING_VALUE_MAX ) m_IsScaleDown = true;
	} else {
		// 縮小中.
		m_ScalingValue -= SCALING_ADD_VALUE;	// 拡縮値を減算.
		if( m_ScalingValue <= SCALING_VALUE ) m_IsScaleDown = false;
	}
	// 出現カウントが最大値を超えたら.
	if( m_AppearanceCount >= APPEARANCE_COUNT_MAX ){
		// 各値が一定より大きくならないようにする.
		m_AppearanceCount	= APPEARANCE_COUNT_MAX;
		m_ScalingValue		= SCALING_VALUE_MAX;
		m_NowArmState		= EArmState::Max;	// 次の状態へ移動.
	}
	// スケール値を計算.
	const float scale = fabsf(sinf( static_cast<float>(D3DX_PI)*0.5f * m_AppearanceCount ));
	m_vSclae = { scale * m_ScalingValue, scale, scale };
}