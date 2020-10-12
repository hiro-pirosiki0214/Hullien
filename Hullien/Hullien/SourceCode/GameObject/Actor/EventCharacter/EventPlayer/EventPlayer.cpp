#include "EventPlayer.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\Actor\Actor.h"
#include "..\...\..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\XAudio2\SoundManager.h"

/********************************
*	イベント用プレイヤークラス.
**/
CEventPlayer::CEventPlayer()
	: m_NowAnimNo(EAnimNo::Walk)
	, m_OldAnimNo(EAnimNo::None)
	, m_AttackPosition()
	, m_pEffects( )
	, m_SpecialAbility(0.0f)
	, m_HasUsableSP(false)
	, m_IsAttackSE(false)
{
	m_ObjectTag = EObjectTag::Player;
	m_NowMoveState = EMoveState::Rotation;
}

CEventPlayer::~CEventPlayer()
{
}

// 初期化関数
bool CEventPlayer::Init()
{
#ifndef IS_TEMP_MODEL_RENDER
	if (GetModel(MODEL_NAME) == false) return false;
	// アニメーションの設定.
	SetAnimation(m_NowAnimNo);
	if (FootStepCollisionSetting() == false) return false;
#else
	if (GetModel(MODEL_TEMP_NAME) == false) return false;
#endif
	if( ColliderSetting() == false ) return false;
	if( SoundSetting() == false ) return false;

	return true;
}

// 更新関数.
void CEventPlayer::Update()
{
	if (m_Parameter.IsDisp == false) return;
	Move();
	SPController();			// 特殊能力操作.
}

// 描画関数.
void CEventPlayer::Render()
{
	if (m_Parameter.IsDisp == false) return;
	FootStep(RIGHT_FOOT, LEFT_FOOT);
	MeshRender();	// メッシュの描画.
	EffectRender();
}

// 当たり判定関数.
void CEventPlayer::Collision(CActor * pActor)
{
}

// 相手座標の設定関数.
void CEventPlayer::SetTargetPos(CActor & actor)
{
}

// 特殊能力を使っているか.
bool CEventPlayer::IsSpecialAbility()
{
	if (m_HasUsableSP == false) return false;
	// 特殊能力が使えるなら.
	m_HasUsableSP = false;	// 初期化して.
	return true;			// trueを返す.
}

// 特殊能力操作関数.
void CEventPlayer::SPController()
{
	// Yボタンが押された瞬間じゃなければ終了.
	//if (CXInput::Y_Button() != CXInput::enPRESSED_MOMENT) return;
	if (GetAsyncKeyState('Y') & 0x8000
		|| CXInput::Y_Button() == CXInput::enPRESSED_MOMENT)
	{
		CSoundManager::PlaySE("PlayerVoiceSpecial");
		m_SpecialAbility = 0.0f;
		m_HasUsableSP = true;
	}
}

// 移動関数.
void CEventPlayer::Move()
{
}

// エフェクト描画関数.
void CEventPlayer::EffectRender()
{
}

// アニメーション設定.
void CEventPlayer::SetAnimation(const EAnimNo & animNo)
{
	if (m_pSkinMesh == nullptr) return;
	if (animNo == m_OldAnimNo) return;
	m_OldAnimNo = m_NowAnimNo;
	m_NowAnimNo = animNo;
	m_pSkinMesh->ChangeAnimSet(static_cast<int>(m_NowAnimNo));
}

// 当たり判定の設定.
bool CEventPlayer::ColliderSetting()
{
#ifndef IS_TEMP_MODEL_RENDER
	if (m_pSkinMesh == nullptr) return false;
	if (m_pCollManager == nullptr) {
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if (FAILED(m_pCollManager->InitSphere(
		m_pSkinMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius))) return false;
	return true;
#else
	if (m_pTempStaticMesh == nullptr) return false;
	// メッシュを使用しての当たり判定初期化.
	if (m_pCollManager == nullptr) {
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if (FAILED(m_pCollManager->InitSphere(
		m_pTempStaticMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius))) return false;

	// 攻撃用の当たり判定初期化.
	if (m_pAttackCollManager == nullptr) {
		m_pAttackCollManager = std::make_shared<CCollisionManager>();
	}
	if (FAILED(m_pAttackCollManager->InitSphere(
		&m_AttackPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		1.0f))) return false;
	return true;
#endif	// #ifndef IS_MODEL_RENDER.
}

// エフェクトの設定.
bool CEventPlayer::EffectSetting()
{
	return false;
}

// サウンドの設定.
bool CEventPlayer::SoundSetting()
{
	VolumeSetting("PlayerVoiceSpecial", VOICE_VOLUME);
	return true;
}

// 音量の設定.
void CEventPlayer::VolumeSetting(const char * soung, float volume)
{
	CSoundManager::SetAnotherSEVolume(soung, volume);
	CSoundManager::SetUseAnotherSEVolumeFlag(soung, true);
}
