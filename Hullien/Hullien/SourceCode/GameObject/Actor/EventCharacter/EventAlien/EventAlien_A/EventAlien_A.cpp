#include "EventAlien_A.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Arm\Arm.h"
#include "..\..\..\..\..\XAudio2\SoundManager.h"	

/*************************************
*	イベント用宇宙人A.
**/
CEventAlienA::CEventAlienA()
{
	m_ObjectTag = EObjectTag::Alien_A;
	m_NowState = EEventAlienState::None;
	m_pArm = std::make_shared<CArm>();
}

CEventAlienA::~CEventAlienA()
{
}

// 初期化関数.
bool CEventAlienA::Init()
{
	if (GetModel(MODEL_NAME) == false) return false;
	if( SetAnimFrameList() == false ) return false;
	if(ColliderSetting() == false) return false;
	if(m_pArm->Init() == false) return false;
	m_pSkinMesh->ChangeAnimSet_StartPos( EAnimNo_Move, 0.0f );
	return true;
}

// 更新関数.
void CEventAlienA::Update()
{
	if (m_Parameter.IsDisp == false) return;
	// アニメーションフレームの更新.
	m_AnimFrameList[m_NowAnimNo].UpdateFrame( m_AnimSpeed );

	CurrentStateUpdate();	// 現在の状態の更新.
	m_pArm->Update();
	m_pArm->SetPosition({m_vPosition.x, m_vPosition.y+5.5f, m_vPosition.z});
	m_pArm->SetRotationY(m_vRotation.y);
}

// 描画関数.
void CEventAlienA::Render()
{
	if (m_Parameter.IsDisp == false) return;

	if (m_pSkinMesh == nullptr) return;

	m_pSkinMesh->SetPosition(m_vPosition);
	m_pSkinMesh->SetRotation(m_vRotation);
	m_pSkinMesh->SetScale(m_vScale);
	m_pSkinMesh->SetColor({ 0.5f, 0.8f, 0.5f, m_Parameter.ModelAlpha });
	m_pSkinMesh->SetRasterizerState(CCommon::enRS_STATE::Back);
	m_pSkinMesh->SetAnimSpeed( m_AnimSpeed );
	m_pSkinMesh->Render();
	m_pSkinMesh->SetRasterizerState(CCommon::enRS_STATE::None);

	// アームの描画.
	m_pArm->Render();

#if _DEBUG
	if (m_pCollManager == nullptr) return;
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// 当たり判定関数.
void CEventAlienA::Collision(CActor * pActor)
{
	if (m_Parameter.IsDisp == false) return;
	if (pActor == nullptr) return;
	if (m_pCollManager == nullptr) return;
	if (m_pCollManager->GetSphere() == nullptr) return;

	GirlCollision(pActor);
	BarrierCollision(pActor);
}

// スポーン.
bool CEventAlienA::Spawn(const D3DXVECTOR3& spawnPos)
{
	// 既にスポーン済みなら終了.
	if (m_NowState != EEventAlienState::None) return true;
	// 初期化に失敗したら終了.
	if (Init() == false) return false;

	if (m_Parameter.IsDisp == false)
	{
		CSoundManager::PlaySE("AlienApp");
		m_Parameter.IsDisp = true;
		m_Parameter.vScale = { 0.0f,0.0f,0.0f };
	}
	m_AnimSpeed = 0.0;
	m_NowState = EEventAlienState::Spawn;
	//Spawning();
	return true;
}

// アームを完全にとりだす.
void CEventAlienA::SetArmAppearance()
{
	m_pArm->SetAppearance();
	m_pSkinMesh->ChangeAnimSet_StartPos( EAnimNo_Arm, m_AnimFrameList[EAnimNo_Arm].EndFrame );
	m_AnimSpeed = 0.0;
}

// スポーン.
void CEventAlienA::Spawning()
{
	CEventAlien::Spawning();
}

void CEventAlienA::Move()
{
	if( m_AnimFrameList[EAnimNo_Arm].IsNowFrameOver() == true ){
		m_AnimSpeed = 0.0;
	}
}

// 当たり判定の設定
bool CEventAlienA::ColliderSetting()
{
	if (m_pSkinMesh == nullptr) return false;
	if (m_pCollManager == nullptr) {
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	float Scale = 1.0f;
	if (FAILED(m_pCollManager->InitSphere(
		m_pSkinMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&Scale,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius))) return false;
	return true;
}

// アニメーションフレームの設定.
bool CEventAlienA::SetAnimFrameList()
{
	if( m_pSkinMesh == nullptr ) return false;
	// 調整用アニメーションフレームのリストを用意.
	const double animAdjFrames[] =
	{
		0.0f,
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