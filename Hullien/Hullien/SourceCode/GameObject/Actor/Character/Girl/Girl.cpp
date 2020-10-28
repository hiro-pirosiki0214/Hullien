#include "Girl.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\XAudio2\SoundManager.h"
#include "..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\Common\DebugText\DebugText.h"
#include "..\..\..\..\Utility\FileManager\FileManager.h"

CGirl::CGirl()
	: m_Parameter			()
	, m_pSearchCollManager	( nullptr )
	, m_pWarning			( nullptr )
	, m_OldPosition			( 0.0f, 0.0f, 0.0f )
	, m_NowState			( ENowState::None )
	, m_NowMoveState		( EMoveState::None )
	, m_CameraRadianX		( 0.0f )
	, m_IsDanger			( false )
	, m_IsOnlyFirst			( false )
{
	m_ObjectTag		= EObjectTag::Girl;
	m_NowState		= ENowState::Protected;
	m_NowMoveState	= EMoveState::Wait;
	m_NowAnimNo		= EAnimNo_Wait;	// 現在のアニメーションを待機に設定.
	m_OldAnimNo		= EAnimNo_None;	// 過去のアニメーションは無し.
	m_pSearchCollManager = std::make_shared<CCollisionManager>();
	m_pWarning		= std::make_unique<CWarning>();
	m_AnimFrameList.resize( EAnimNo_Max );
	m_vPosition.z = 4.0f;
}

CGirl::~CGirl()
{
}

// 初期化関数.
bool CGirl::Init()
{
	if( GetModel( MODEL_NAME )	== false ) return false;
	if( SetAnimFrameList()		== false ) return false;
	if( CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_Parameter ) == false ) return false;
	if( ColliderSetting()		== false ) return false;
	if( m_pWarning->Init()		== false ) return false;
	
	// 待機アニメーションに変更.
	m_pSkinMesh->ChangeAnimSet_StartPos( EAnimNo_Wait, 0.0 );

	return true;
}

// 更新関数.
void CGirl::Update()
{
	switch( m_NowState )
	{
	case ENowState::None:
		break;
	case ENowState::Protected:
		break;
	case ENowState::Abduct:
		Abduct();
		break;
	case ENowState::Move:
		Move();
		break;
	default:
		break;
	}

	if (m_NowState == ENowState::Abduct || m_IsDanger == true)
	{
		m_pWarning->Update();
		m_pWarning->SetPosition(m_vPosition);
	}

	// サウンド.
	Sound();

	//警告.
	WarningRotation();

	m_IsDanger = false;
}

// 描画関数.
void CGirl::Render()
{
	// 画面の外なら終了.
	if( IsDisplayOut() == true ) return;

	MeshRender();	// メッシュの描画.

#if _DEBUG
	if( m_pCollManager == nullptr ) return;
	m_pCollManager->DebugRender();
	if( m_pSearchCollManager == nullptr ) return;
	m_pSearchCollManager->DebugRender();

//	DebugRender();
#endif	// #if _DEBUG.
}

// 当たり判定関数.
void CGirl::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;
	SearchCollision( pActor );
}

// 相手座標の設定関数.
void CGirl::SetPosition( const D3DXVECTOR3& pos )
{
	m_vPosition = pos;
	SetAnimationBlend( EAnimNo_Abduct );
	m_NowState = ENowState::Abduct;
}

// スプライトの描画.
void CGirl::SpriteRender()
{
	// 女の子が連れ去られている状態または危険な状態ならば警告を描画.
	if (m_NowState == ENowState::Abduct || m_IsDanger == true)
	{
		m_pWarning->SetPosition({ m_vPosition.x, m_vPosition.y+5.0f, m_vPosition.z});
		m_pWarning->Update();
		m_pWarning->Render();
	}
}

// 移動関数.
void CGirl::Move()
{
	switch( m_NowMoveState )
	{
	case EMoveState::None:
		break;
	case EMoveState::Rotation:
		CGirl::TargetRotation();
		break;
	case EMoveState::Move:
		TargetMove();
		break;
	case EMoveState::Wait:
		break;
	default:
		break;
	}
}

// 連れ去れている.
void CGirl::Abduct()
{
	// 前回の座標が今の座標なら(連れ去れている状態で移動していない).
	if( m_OldPosition == m_vPosition ){
		m_NowState		= ENowState::Move;		// 移動状態へ遷移.
		m_NowMoveState	= EMoveState::Rotation;	// 移動の回転状態へ遷移.
	}
	m_OldPosition = m_vPosition;	// 座標を記憶.
}

// 目的の場所に向けて回転.
void CGirl::TargetRotation()
{
	if( m_NowMoveState != EMoveState::Rotation ) return;
	// 目的の回転軸を取得.
	const float targetRotation = atan2f(
		m_vPosition.x - m_Parameter.InitPosition.x,
		m_vPosition.z - m_Parameter.InitPosition.z );
	// 回転軸からベクトルを取得.
	m_MoveVector.x = sinf( targetRotation );
	m_MoveVector.z = cosf( targetRotation );
	// 目的の座標に向けて回転.
	if( CCharacter::TargetRotation( m_MoveVector, m_Parameter.RotatlonalSpeed, TOLERANCE_RADIAN ) == false ) return;
	m_vRotation.y = targetRotation;		// ターゲットへの回転取得.
	m_NowMoveState = EMoveState::Move;	// 移動状態へ遷移.
	SetAnimationBlend( EAnimNo_Move );
}

// 目的の場所に向けて移動.
void CGirl::TargetMove()
{
	if( m_NowMoveState != EMoveState::Move ) return;

	// 移動ベクトルを使用して移動.
	m_vPosition.x -= m_MoveVector.x * m_Parameter.MoveSpeed;
	m_vPosition.z -= m_MoveVector.z * m_Parameter.MoveSpeed;
	const float lenght = D3DXVec3Length( &D3DXVECTOR3(m_Parameter.InitPosition-m_vPosition) );
	// 目的の座標との距離が一定値より少ないか比較.
	if( lenght >= m_Parameter.InitPosLenght ) return;
	SetAnimationBlend( EAnimNo_Wait );
	m_NowMoveState = EMoveState::Wait;	// 待機状態へ遷移.
}

// 索敵の当たり判定.
void CGirl::SearchCollision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pSearchCollManager == nullptr ) return;
	if( m_pSearchCollManager->GetSphere() == nullptr ) return;

	// 既に連れ去られていたら終了.
	if( m_NowState == ENowState::Abduct ){
		m_IsDanger = true;
		return;
	}

	// 対象オブジェクトじゃなければ終了.
	if( ( pActor->GetObjectTag() != EObjectTag::Alien_A ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_B ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_C ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_D )) return;

	// 球体の当たり判定.
	if (m_pSearchCollManager->IsShereToShere(pActor->GetCollManager()) == false) return;
	m_IsDanger = true;

	// 敵が複数いた場合、衝突していても、
	// 衝突していない敵がいた場合、
	// false が上書きされてしまうので、
	// 当たり判定を行う前に Updateで、
	// 初期化してあげる.
//	{
//		m_IsDanger = false;
//	}
//	else {
//		m_IsDanger = true;
//	}
}

// 当たり判定の作成.
bool  CGirl::ColliderSetting()
{
	if( m_pSkinMesh == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere( 
		m_pSkinMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius ) )) return false;
	// 索敵の当たり判定.
	if( FAILED( m_pSearchCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SearchCollRadius ) )) return false;
	return true;
}

// アニメーションフレームの設定.
bool CGirl::SetAnimFrameList()
{
	// 調整用アニメーションフレームのリストを用意.
	const double animAdjFrames[] =
	{
		0.0f,
		0.0f,
		0.0f,
	};
	if( m_pSkinMesh == nullptr ) return false;
	for( int i = EAnimNo_Begin; i < EAnimNo_End; i++ ){
		m_AnimFrameList.at(i) = { 0.0, m_pSkinMesh->GetAnimPeriod(i)-animAdjFrames[i] };
	}
	return true; 
}

// サウンド.
void CGirl::Sound()
{
	// 女の子が危険な状態ならばSEを鳴らす.
	if (m_IsDanger == true && m_IsOnlyFirst == false)
	{
		CSoundManager::NoMultipleSEPlay("Warning");
		m_IsOnlyFirst = true;
	}
	// 危険な状態を脱したらSEを再度鳴らせる状態にする.
	if (m_IsDanger == false)
	{
		m_IsOnlyFirst = false;
	}
}

// 危険矢印の回転.
void CGirl::WarningRotation()
{
	// 女の子が危険な状態でなければ処理しない.
	if (m_IsDanger == false) return;

	// カメラから女の子への回転軸を取得.
	const float targetRotation = atan2f(
		m_vPosition.x - CCameraManager::GetPosition().x,
		m_vPosition.z - CCameraManager::GetPosition().z);
	D3DXVECTOR3 MoveVector = { 0.0f, 0.0f, 0.0f };
	MoveVector.x = sinf( targetRotation );
	MoveVector.z = cosf( targetRotation );

	// カメラの前ベクトルを用意.
	D3DXVECTOR3 myVector = { 0.0f, 0.0f ,0.0f };
	myVector.x = sinf(m_CameraRadianX);
	myVector.z = cosf(m_CameraRadianX);

	// ベクトルの長さを求める.
	float myLenght = sqrtf(myVector.x*myVector.x + myVector.z*myVector.z);
	float targetLenght = sqrtf(MoveVector.x*MoveVector.x + MoveVector.z*MoveVector.z);

	// カメラから女の子へのベクトルと、カメラの前ベクトルの外積を求める.
	float cross = myVector.x * MoveVector.z - myVector.z * MoveVector.x;
	float dot = myVector.x * MoveVector.x + myVector.z * MoveVector.z;
	dot = acosf(dot / (myLenght * targetLenght));

	// 内積がしきい値を超えたら女の子が画面外に出た.
	bool IsOffScreen = dot < THRESHOLD_VALUE_RADIAN ? true : false;
	m_pWarning->SetIsGirlOffScreen( IsOffScreen );

	// 外積が0.0fより大きければ 右：左 に表示.
	bool IsRight = cross > 0.0f ? true : false;
	m_pWarning->SetIsDispRight( IsRight );
}

// デバッグ描画関数.
void CGirl::DebugRender()
{
	const float pos_y = 40.0f;
	const float pos_x = 880.0f;
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale()*10, 0.0f });
	CDebugText::Render("- warning -");
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 11, 0.0f });
	CDebugText::Render("-- CameraRadianX --");
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 12, 0.0f });
	CDebugText::Render("CameraRadianX : ", m_CameraRadianX);
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 14, 0.0f });
	CDebugText::Render("-- CameraPosition --");
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 15, 0.0f });
	CDebugText::Render("Pos_X : ", CCameraManager::GetPosition().x);
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 16, 0.0f });
	CDebugText::Render("Pos_Y : ", CCameraManager::GetPosition().y);
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 17, 0.0f });
	CDebugText::Render("Pos_Z : ", CCameraManager::GetPosition().z);
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 19, 0.0f });
	CDebugText::Render("-- WarningPosition --");
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 20, 0.0f });

}
