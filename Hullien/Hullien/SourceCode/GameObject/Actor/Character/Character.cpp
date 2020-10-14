#include "Character.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\XAudio2\SoundManager.h"

namespace
{
	const double DEFAULT_ANIM_SPEED = 0.01;	// デフォルトアニメーション速度.
}

CCharacter::CCharacter()
	: m_pSkinMesh				( nullptr )
	, m_NowAnimNo				( 0 )
	, m_OldAnimNo				( 0 )
	, m_AnimSpeed				( DEFAULT_ANIM_SPEED )
	, m_MoveVector				( 0.0f, 0.0f, 0.0f )
	, m_InvincibleCount			( 0 )
	, m_HasFinishedParamSetting	( false )
	, m_pFootCollision			()
	, m_pGroundCollision		( nullptr )
	, m_vGroundPosition			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, m_vRightPosition			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, m_vLeftPosition			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
{
}

CCharacter::~CCharacter()
{
}

// メッシュの表示.
void CCharacter::MeshRender()
{
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->SetAnimSpeed( m_AnimSpeed );
	m_pSkinMesh->Render();
}

// 無敵時間かどうか.
bool CCharacter::IsInvincibleTime( const int& invincibleTime )
{
	return m_InvincibleCount > invincibleTime * FPS;
}

// モデルの取得.
bool CCharacter::GetModel( const char* modelName )
{
	// 既に読み込めていたら終了.
	if( m_pSkinMesh != nullptr ) return true;
	// モデルの取得.
	CMeshResorce::GetSkin( m_pSkinMesh, modelName );
	// モデルが読み込めてなければ false.
	if( m_pSkinMesh == nullptr ) return false;
	return true;
}

// 目的の座標へ回転.
bool CCharacter::TargetRotation( const D3DXVECTOR3& targetVec, const float& rotSpeed, const float& ToleranceRadian )
{
	// 自身のベクトルを用意.
	const D3DXVECTOR3 myVector = { sinf(m_vRotation.y), 0.0f, cosf(m_vRotation.y) };

	// ベクトルの長さを求める.
	const float myLenght = sqrtf(myVector.x*myVector.x + myVector.z*myVector.z);
	const float targetLenght = sqrtf(targetVec.x*targetVec.x + targetVec.z*targetVec.z);

	// 内積を求める.
	float dot = myVector.x*targetVec.x + myVector.z*targetVec.z;
	dot = acosf( ( myLenght * targetLenght ) * dot );

	if( ( -ToleranceRadian < dot && dot < ToleranceRadian ) ||	// 内積が許容範囲なら.
		( std::isfinite( dot ) ) == false ){					// 内積の値が計算できない値なら.
		return true;	// 回転終了.
	} else {
		// 目的のベクトルと、自分のベクトルの外積を求める.
		const float cross = myVector.x*targetVec.z - myVector.z*targetVec.x;
		// 外積が0.0より少なければ 時計回り : 反時計回り に回転する.
		m_vRotation.y += cross < 0.0f ? rotSpeed : -rotSpeed;

		return false;	// 回転中.
	}
}

// アニメーション設定.
void CCharacter::SetAnimation( const int& animNo )
{
	if( m_pSkinMesh == nullptr ) return;
	if( m_NowAnimNo == m_OldAnimNo ) return;
	m_OldAnimNo = m_NowAnimNo;
	m_NowAnimNo = animNo;
	m_pSkinMesh->ChangeAnimSet( m_NowAnimNo );
}

// アニメーションをブレンドして設定.
void CCharacter::SetAnimationBlend( const int& animNo )
{
	if( m_pSkinMesh == nullptr ) return;
	if( m_NowAnimNo == m_OldAnimNo ) return;
	m_OldAnimNo = m_NowAnimNo;
	m_NowAnimNo = animNo;
	m_pSkinMesh->ChangeAnimBlend( m_NowAnimNo, m_OldAnimNo );
}

// 足音.
void CCharacter::FootStep(const char * rightfoot, const char * leftfoot)
{
	m_vGroundPosition = m_vPosition;
	m_pSkinMesh->GetPosFromBone(leftfoot, &m_vLeftPosition);
	m_pSkinMesh->GetPosFromBone(rightfoot, &m_vRightPosition);

	if (m_pFootCollision[0]->IsShereToShere(m_pGroundCollision.get()) == true
		|| m_pFootCollision[1]->IsShereToShere(m_pGroundCollision.get()) == true)
	{
		CSoundManager::NoMultipleSEPlay("Walk");
	}

	m_pGroundCollision->DebugRender();
	m_pFootCollision[0]->DebugRender();
	m_pFootCollision[1]->DebugRender();
}

// 足音用当たり判定の設定.
bool CCharacter::FootStepCollisionSetting()
{
	// 地面の当たり判定.
	if (m_pGroundCollision != nullptr) return true;
	if (m_pGroundCollision == nullptr)
	{
		m_pGroundCollision = std::make_shared<CCollisionManager>();
	}
	if (FAILED(m_pGroundCollision->InitSphere(
		&m_vGroundPosition,
		&m_vRotation,
		&m_vSclae.x,
		{0.0f,0.0f,0.0f},
		1.0f))) return false;

	// 足の当たり判定.
	if (m_pFootCollision.size() != 0) return true;
	m_pFootCollision.emplace_back(std::make_shared<CCollisionManager>());
	m_pFootCollision.emplace_back(std::make_shared<CCollisionManager>());
	if (FAILED(m_pFootCollision[0]->InitSphere(
		&m_vRightPosition,
		&m_vRotation,
		&m_vSclae.x,
		{ 0.0f,0.0f,0.0f },
		0.5f))) return false;
	if (FAILED(m_pFootCollision[1]->InitSphere(
		&m_vLeftPosition,
		&m_vRotation,
		&m_vSclae.x,
		{ 0.0f,0.0f,0.0f },
		0.5f))) return false;
	return true;
}
