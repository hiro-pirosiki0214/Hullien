#include "Arm.h"
#include "..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\XAudio2\SoundManager.h"	

CArm::CArm()
	: m_pSkinMesh		( nullptr )
	, m_pAC				( nullptr )
	, m_GrabPosition	( 0.0f, 0.0f, 0.0f )
	, m_NowArmState		( EArmState::Start )
	, m_OldArmState		( EArmState::Start )
	, m_AppearanceCount	( 0.0f )
	, m_ScalingValue	( SCALING_VALUE )
	, m_AnimSpeed		( 0.0 )
{
	m_vSclae = { 0.0f, 0.0f, 0.0f };
}

CArm::~CArm()
{
}

// 初期化関数.
bool CArm::Init()
{
	if( CMeshResorce::GetSkin( m_pSkinMesh, MODEL_NAME ) == false ) return false;
	if( CMeshResorce::GetStatic( m_pAMesh, "a" ) == false ) return false;
	if( CMeshResorce::GetStatic( m_pGirlMesh, "yuri-dy" ) == false ) return false;
	// アニメーションコントローラーのクローン作成.
	if( FAILED( m_pSkinMesh->GetAnimController()->CloneAnimationController(
		m_pSkinMesh->GetAnimationController()->GetMaxNumAnimationOutputs(),
		m_pSkinMesh->GetAnimationController()->GetMaxNumAnimationSets(),
		m_pSkinMesh->GetAnimationController()->GetMaxNumTracks(),
		m_pSkinMesh->GetAnimationController()->GetMaxNumEvents(),
		&m_pAC ) )) return false;
	m_pSkinMesh->ChangeAnimSet( 0, m_pAC );
	m_AnimEndFrame = m_pSkinMesh->GetAnimPeriod(0)-0.01;	// 終了フレームの取得(+1.0)調整.
	return true;
}

// 更新関数.
void CArm::Update()
{
	switch( m_NowArmState )
	{
	case EArmState::Appearance:
		// 出現.
		Appearance();
		break;
	case EArmState::Grab:
		// 掴む.
		Grab();
		break;
	case EArmState::CleanUp:
		// 片づける.
		CleanUp();
		break;
	case EArmState::End:
		// 掴む時の座標を計算.
		m_GrabPosition.x = m_vPosition.x - sinf( m_vRotation.y ) * GRAB_DISTANCE;
		m_GrabPosition.z = m_vPosition.z - cosf( m_vRotation.y ) * GRAB_DISTANCE;
		m_GrabPosition.y = m_vPosition.y;
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

	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->SetAnimSpeed( m_AnimSpeed );
	m_pSkinMesh->Render( m_pAC );

	//m_pAMesh->SetPosition( m_vPosition );
	//m_pAMesh->SetRotation( m_vRotation );
	//m_pAMesh->SetScale( 1.0f );
	//m_pAMesh->Render();

	//m_pGirlMesh->SetPosition( GetGrabPosition() );
	//m_pGirlMesh->SetRotation( m_vRotation );
	//m_pGirlMesh->SetScale( 1.0f );
	//m_pGirlMesh->Render();
}

// 掴んでいる座標の取得.
D3DXVECTOR3 CArm::GetGrabPosition()
{
	return m_GrabPosition;
}

// 出現する用意.
void CArm::SetAppearance()
{
	if( m_NowArmState == EArmState::Appearance ) return;
	if( m_NowArmState == EArmState::Grab ) return;
	m_OldArmState = m_NowArmState;
	m_NowArmState = EArmState::Appearance;	// 出現する状態へ.
	if( m_OldArmState == EArmState::CleanUp ) return;
	// 初期化.
	m_AppearanceCount	= 0.0f;
	m_ScalingValue		= SCALING_VALUE;
	m_pAC->ResetTime();
	m_pSkinMesh->ChangeAnimSet_StartPos( 0, 0.0, m_pAC );
	CSoundManager::PlaySE("Arm");
}

// 片づける用意.
void CArm::SetCleanUp()
{
	if( m_NowArmState == EArmState::CleanUp ) return;
	if( m_NowArmState == EArmState::Appearance ) return;
	if( m_NowArmState == EArmState::Grab ) return;
	m_OldArmState = m_NowArmState;
	m_NowArmState = EArmState::CleanUp;	// 片づける状態へ.
	if( m_OldArmState == EArmState::Appearance ) return;
	// 初期化.
	m_AppearanceCount	= APPEARANCE_COUNT_MAX;
	m_pAC->ResetTime();
	m_pSkinMesh->ChangeAnimSet_StartPos( 0, 0.0, m_pAC );
}

// 出現.
void CArm::Appearance()
{
	m_AppearanceCount += APPEARANCE_ADD_VALUE;	// 出現カウントの加算.
	if( m_AppearanceCount >= SCALING_VALUE ) m_ScalingValue += APPEARANCE_ADD_VALUE;	// 拡縮値を加算.
	
	// 出現カウントが最大値を超えたら.
	if( m_AppearanceCount >= APPEARANCE_COUNT_MAX ){
		// 各値が一定より大きくならないようにする.
		m_AppearanceCount	= APPEARANCE_COUNT_MAX;
		m_ScalingValue		= SCALING_VALUE_MAX;
		m_AnimSpeed			= 0.01;				// アニメーション速度変更.
		CSoundManager::PlaySE("ArmGrab");		// つかんだ時のSE再生.
		m_NowArmState		= EArmState::Grab;	// 次の状態へ移動.
	}
	if( m_ScalingValue >= SCALING_VALUE_MAX ) m_ScalingValue = SCALING_VALUE_MAX;
	// スケール値を計算.
	const float scale = fabsf(sinf( static_cast<float>(D3DX_PI)*0.5f * m_AppearanceCount ));
	m_vSclae = { scale * m_ScalingValue, scale*m_ScalingValue, scale };
}

// 掴む.
void CArm::Grab()
{
	if( m_pAC->GetTime() <= m_AnimEndFrame ) return;
	m_AnimSpeed = 0.0;
	m_NowArmState  = EArmState::End;	// 終了.
}

// 片づける.
void CArm::CleanUp()
{
	m_AppearanceCount -= CLEAN_UP_SUB_VALUE;	// 出現カウントの加算.

	// 出現カウントが最小値を超えたら.
	if( m_AppearanceCount <= CLEAN_UP_COUNT_MIN ){
		// 各値が一定より大きくならないようにする.
		m_AppearanceCount	= CLEAN_UP_COUNT_MIN;
		m_AnimSpeed			= 0.0;				// アニメーション速度変更.
		m_NowArmState		= EArmState::Start;	// 次の状態へ移動.
	}
	// スケール値を計算.
	const float scale = fabsf(sinf( static_cast<float>(D3DX_PI)*0.5f * m_AppearanceCount ));
	m_vSclae = { scale, scale, scale };
}