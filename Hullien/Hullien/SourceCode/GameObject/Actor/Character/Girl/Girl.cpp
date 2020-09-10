#include "Girl.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\XAudio2\SoundManager.h"
#include "..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\Common\DebugText\DebugText.h"

CGirl::CGirl()
	: m_Parameter			()
	, m_pSearchCollManager	( nullptr )
	, m_pWarning					( nullptr )
	, m_OldPosition				( 0.0f, 0.0f, 0.0f )
	, m_NowState					( ENowState::None )
	, m_NowMoveState			( EMoveState::None )
	, m_IsDanger					( false )
	, m_IsOnlyFirst					( false )
{
	m_ObjectTag	= EObjectTag::Girl;
	m_NowState	= ENowState::Protected;
	m_NowMoveState = EMoveState::Wait;
	m_pSearchCollManager = std::make_shared<CCollisionManager>();
	m_pWarning = std::make_unique<CWarning>();
}

CGirl::~CGirl()
{
}

// 初期化関数.
bool CGirl::Init()
{
#ifndef IS_TEMP_MODEL_RENDER
	if( GetModel( MODEL_NAME ) == false ) return false;
#else
	if( GetModel( MODEL_TEMP_NAME ) == false ) return false;
#endif	// #ifndef IS_TEMP_MODEL_RENDER.
	if( ColliderSetting() == false ) return false;
	if ( m_pWarning->Init() == false ) return false;

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
		if( m_OldPosition == m_vPosition ){
			m_NowState = ENowState::Move;
			m_NowMoveState = EMoveState::Rotation;
		}
		m_OldPosition = m_vPosition;
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

	//=============Warning=====================//.
	WarningRotation();
}

// 描画関数.
void CGirl::Render()
{
	float rotY = m_vRotation.y;
	m_vRotation.y += static_cast<float>(D3DX_PI);
	MeshRender();	// メッシュの描画.
	m_vRotation.y = rotY;

#if _DEBUG
	if( m_pCollManager == nullptr ) return;
	m_pCollManager->DebugRender();
	if( m_pSearchCollManager == nullptr ) return;
	m_pSearchCollManager->DebugRender();

	DebugRender();
#endif	// #if _DEBUG.
}

// 当たり判定関数.
void CGirl::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	SearchCollision( pActor );
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;
}

// 相手座標の設定関数.
void CGirl::SetTargetPos( CActor& actor )
{
	m_vPosition = actor.GetPosition();
	m_NowState = ENowState::Abduct;
}

// スプライトの描画.
void CGirl::SpriteRender()
{
	// 女の子が連れ去られている状態または危険な状態ならば警告を描画.
	if (m_NowState == ENowState::Abduct || m_IsDanger == true)
	{
		m_pWarning->SetPosition(m_vPosition);
		m_pWarning->Update();
		m_pWarning->Render();
	}
	// 女の子が連れ去られている状態出ないなら警告を非表示.
	if(m_NowState != ENowState::Abduct)
	{
		m_IsDanger = false;
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
		TargetRotation();
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

// 目的の場所に向けて回転.
void CGirl::TargetRotation()
{
	if( m_NowMoveState != EMoveState::Rotation ) return;

	const D3DXVECTOR3 targetPosition = { 0.0f, 0.0f, 0.0f };
	// 目的の回転軸を取得.
	D3DXVECTOR3 targetRotation = { 0.0f, 0.0f, 0.0f };
	targetRotation.y = atan2f( 
		targetPosition.x - m_vPosition.x,
		targetPosition.z - m_vPosition.z );
	m_MoveVector.x = sinf( targetRotation.y );
	m_MoveVector.z = cosf( targetRotation.y );

	// 自身のベクトルを用意.
	D3DXVECTOR3 myVector = { 0.0f, 0.0f ,0.0f };
	myVector.x = sinf( m_vRotation.y );
	myVector.z = cosf( m_vRotation.y );

	// ベクトルの長さを求める.
	float myLenght = sqrtf(myVector.x*myVector.x + myVector.z*myVector.z);
	float targetLenght = sqrtf(m_MoveVector.x*m_MoveVector.x + m_MoveVector.z*m_MoveVector.z);

	// 目的のベクトルと、自分のベクトルの外積を求める.
	float cross = myVector.x * m_MoveVector.z-myVector.z * m_MoveVector.x;
	float dot = myVector.x * m_MoveVector.x+myVector.z * m_MoveVector.z;
	dot = acosf( dot / ( myLenght * targetLenght ));

	const float ROTATIONAL_SPEED = 0.05f;	// 回転速度.
	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(10.0));	// 回転の許容範囲.

																			// 外積が0.0より少なければ 時計回り : 反時計回り に回転する.
	m_vRotation.y += cross < 0.0f ? ROTATIONAL_SPEED : -ROTATIONAL_SPEED;

	// 内積が許容範囲なら.
	if( -TOLERANCE_RADIAN < dot && dot < TOLERANCE_RADIAN ){
		m_vRotation.y = targetRotation.y;	// ターゲットへの回転取得.
											// 移動用ベクトルを取得.
		m_MoveVector.x = sinf( m_vRotation.y );
		m_MoveVector.z = cosf( m_vRotation.y );
		m_NowMoveState = EMoveState::Move;
	}
}

// 目的の場所に向けて移動.
void CGirl::TargetMove()
{
	if( m_NowMoveState != EMoveState::Move ) return;

	// 目的の場所.
	const D3DXVECTOR3 targetPosition = { 0.0f, 5.0f, 0.0f };
	const float moveSpeed = 0.05f;
	m_vPosition.x -= sinf( m_vRotation.y+static_cast<float>(D3DX_PI) ) * moveSpeed;
	m_vPosition.z -= cosf( m_vRotation.y+static_cast<float>(D3DX_PI) ) * moveSpeed;

	float lenght = D3DXVec3Length( &D3DXVECTOR3(targetPosition - m_vPosition) );

	if( lenght >= 1.0f ) return;

	m_NowMoveState = EMoveState::Wait;
}

// 索敵の当たり判定.
void CGirl::SearchCollision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pSearchCollManager == nullptr ) return;
	if( m_pSearchCollManager->GetSphere() == nullptr ) return;

	// 既に連れ去られていたら終了.
	if( m_NowState == ENowState::Abduct ) return;

	// 対象オブジェクトじゃなければ終了.
	if( ( pActor->GetObjectTag() != EObjectTag::Alien_A ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_B ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_C ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_D )) return;

	// 球体の当たり判定.
	if( m_pSearchCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;
	m_IsDanger = true;

}

// 当たり判定の作成.
bool  CGirl::ColliderSetting()
{
#ifndef IS_TEMP_MODEL_RENDER
	if( m_pSkinMesh == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere( 
		m_pSkinMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius ) )) return false;
	return true;
#else
	if( m_pTempStaticMesh == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	// 女の子の当たり判定.
	if( FAILED( m_pCollManager->InitSphere( 
		m_pTempStaticMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius ) )) return false;
	// 索敵の当たり判定.
	if( FAILED( m_pSearchCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SearchCollRadius ) )) return false;

	return true;
#endif	// #ifndef IS_MODEL_RENDER.
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
	const D3DXVECTOR3 targetPosition = { 0.0f, 0.0f, 0.0f };
	// カメラから女の子への回転軸を取得.
	D3DXVECTOR3 targetRotation = { 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 MoveVector = { 0.0f, 0.0f, 0.0f };
	targetRotation.y = atan2f(
		m_vPosition.x - CCameraManager::GetPosition().x,
		m_vPosition.z - CCameraManager::GetPosition().z);
	MoveVector.x = sinf(targetRotation.y);
	MoveVector.z = cosf(targetRotation.y);

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

	if (dot > 2.4f)
	{
		// カメラ内.
		m_pWarning->SetIsGirlOffScreen(false);
	}
	else
	{
		// カメラ外.
		m_pWarning->SetIsGirlOffScreen(true);
	}

	const float pos_y = 40.0f;
	const float pos_x = 880.0f;

	if (m_IsWarning == true)
	{
		CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 23, 0.0f });
		if (cross > 0.0f) {
			CDebugText::Render("right");
		}
		else
		{
			CDebugText::Render("left");
		}
	}

	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 21, 0.0f });
	CDebugText::Render("cross : ", cross);
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 22, 0.0f });
	CDebugText::Render("dot : ", dot);
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
	if (m_IsWarning == true)
	{
		CDebugText::Render("GirlPosition");
	}
	else
	{
		CDebugText::Render("!GirlPosition");
	}

}
