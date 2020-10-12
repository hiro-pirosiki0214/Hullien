#include "EventCharacter.h"
#include "..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\XAudio2\SoundManager.h"

/***************************************
*	イベント用キャラクタクラス.
**/
CEventCharacter::CEventCharacter()
	: m_pSkinMesh( nullptr )
#ifdef IS_TEMP_MODEL_RENDER
	, m_pTempStaticMesh( nullptr )
#endif
	, m_Parameter				()
	, m_NowMoveState			( EMoveState::None )
	, m_pFootCollision			()
	, m_pGroundCollision		( nullptr )
	, m_vGroundPosition			(D3DXVECTOR3(0.0f,0.0f,0.0f))
	, m_vRightPosition			(D3DXVECTOR3(0.0f,0.0f,0.0f))
	, m_vLeftPosition			(D3DXVECTOR3(0.0f,0.0f,0.0f))
{
	m_vPosition.y = INIT_POSITION_ADJ_HEIGHT;
}

CEventCharacter::~CEventCharacter()
{
}

// 情報設定関数.
void CEventCharacter::SetOptionalState(const SOptionalState& state)
{
	m_vPosition = state.vPosition;
	m_vRotation = state.vRotation;
	m_vSclae = state.vScale;
	m_Parameter.ModelAlpha = state.ModelAlpha;
	m_Parameter.MoveSpeed = state.MoveSpeed;
	m_Parameter.RotationalSpeed = state.RotationalSpeed;
	m_Parameter.ScaleSpeed = state.ScaleSpeed;
	m_Parameter.AlphaSpeed = state.AlphaSpeed;
	m_Parameter.IsDisp = state.IsDisp;
}

// メッシュの表示関数.
void CEventCharacter::MeshRender()
{
#ifndef IS_TEMP_MODEL_RENDER
	if (m_pSkinMesh == nullptr) return;

	m_pSkinMesh->SetPosition(m_vPosition);
	m_pSkinMesh->SetRotation(m_vRotation);
	m_pSkinMesh->SetScale(m_vSclae);
	m_pSkinMesh->SetAnimSpeed(0.01);
	m_pSkinMesh->Render();
#else
	if (m_pTempStaticMesh == nullptr) return;

	m_pTempStaticMesh->SetPosition(m_vPosition);
	m_pTempStaticMesh->SetRotation(m_vRotation);
	m_pTempStaticMesh->SetScale(m_vSclae);
	m_pTempStaticMesh->Render();
#endif	// #ifdef IS_TEMP_MODEL_RENDER.
}

// モデルの取得関数
bool CEventCharacter::GetModel(const char * modelName)
{
#ifndef IS_TEMP_MODEL_RENDER
	// 既に読み込めていたら終了.
	if (m_pSkinMesh != nullptr) return true;
	// モデルの取得.
	CMeshResorce::GetSkin(m_pSkinMesh, modelName);
	// モデルが読み込めてなければ false.
	if (m_pSkinMesh == nullptr) return false;
	return true;
#else
	// 既に読み込めていたら終了.
	if (m_pTempStaticMesh != nullptr) return true;
	// モデルの取得.
	CMeshResorce::GetStatic(m_pTempStaticMesh, modelName);
	// モデルが読み込めてなければ false.
	if (m_pTempStaticMesh == nullptr) return false;
	return true;
#endif	// #ifndef IS_MODEL_RENDER.
}

// 足音.
void CEventCharacter::FootStep(const char* rightfoot, const char* leftfoot)
{
	if( m_pSkinMesh == nullptr ) return;
	m_vGroundPosition = m_vPosition;
	m_vGroundPosition.y = 0.0f;
	m_pSkinMesh->GetPosFromBone(leftfoot, &m_vLeftPosition);
	m_pSkinMesh->GetPosFromBone(rightfoot, &m_vRightPosition);

	if (m_pFootCollision[0]->IsShereToShere(m_pGroundCollision.get()) == true
		|| m_pFootCollision[1]->IsShereToShere(m_pGroundCollision.get()) == true )
	{
		CSoundManager::NoMultipleSEPlay("Walk");
	}

	m_pGroundCollision->DebugRender();
	m_pFootCollision[0]->DebugRender();
	m_pFootCollision[1]->DebugRender();
}

// 当たり判定の設定.
bool CEventCharacter::FootStepCollisionSetting()
{
#ifndef IS_TEMP_MODEL_RENDER
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
		m_Parameter.SphereAdjPos,
		0.5f))) return false;

	// 足の当たり判定.
	if(m_pFootCollision.size() != 0) return true;
	m_pFootCollision.emplace_back(std::make_shared<CCollisionManager>());
	m_pFootCollision.emplace_back(std::make_shared<CCollisionManager>());
	if (FAILED(m_pFootCollision[0]->InitSphere(
		&m_vRightPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		0.5f))) return false;
	if (FAILED(m_pFootCollision[1]->InitSphere(
		&m_vLeftPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		0.5f))) return false;
#endif
	return true;
}

/******************************************************
*	以下情報取得・設定関数.
*/

// X座標位置設定関数.
void CEventCharacter::SetPositionX(const float& vPos_x)
{
	m_vPosition.x = vPos_x;
}

// Y座標位置設定関数.
void CEventCharacter::SetPositionY(const float& vPos_y)
{
	m_vPosition.y = vPos_y;
}

// Z座標位置設定関数.
void CEventCharacter::SetPositionZ(const float& vPos_z)
{
	m_vPosition.z = vPos_z;
}

// 回転値取得関数.
D3DXVECTOR3 CEventCharacter::GetRotation() const
{
	return m_vRotation;
}

// 回転値設定関数.
void CEventCharacter::SetRotation(const D3DXVECTOR3& vRot)
{
	m_vRotation = vRot;
}

// X座標回転値設定関数.
void CEventCharacter::SetRotationX(const float& vRot_x)
{
	m_vRotation.x = vRot_x;
}

// Y座標回転値設定関数.
void CEventCharacter::SetRotationY(const float& vRot_y)
{
	m_vRotation.y = vRot_y;
}

// Z座標回転値設定関数.
void CEventCharacter::SetRotationZ(const float& vRot_z)
{
	m_vRotation.z = vRot_z;
}

// 大きさ取得関数.
D3DXVECTOR3 CEventCharacter::GetScale() const
{
	return m_vSclae;
}

// 大きさ設定関数.
void CEventCharacter::SetScale(const D3DXVECTOR3& vScale)
{
	m_vSclae = vScale;
}

// X座標大きさ設定関数.
void CEventCharacter::SetScaleX(const float & vScale_x)
{
	m_vSclae.x = vScale_x;
}

// Y座標大きさ設定関数.
void CEventCharacter::SetScaleY(const float & vScale_y)
{
	m_vSclae.y = vScale_y;
}

// Z座標大きさ設定関数.
void CEventCharacter::SetScaleZ(const float & vScale_z)
{
	m_vSclae.z = vScale_z;
}

float CEventCharacter::RotationMoveRight(const float& rotValue, const float& rotSpeed, bool IsRightRot)
{
	if (IsRightRot == true) 
	{
		if (m_vRotation.y >= rotValue) m_vRotation.y = rotValue;
	}
	else
	{
		if (m_vRotation.y <= rotValue) m_vRotation.y = rotValue;
	}

	if( m_vRotation.y == rotValue ) return rotValue;
	m_vRotation.y += static_cast<float>(D3DX_PI) * rotSpeed;

	return m_vRotation.y;
}
