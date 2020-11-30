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
	: m_AnimFrameList		(player::EAnimNo_Max)
	, m_NowAnimNo			(player::EAnimNo::EAnimNo_Walk)
	, m_OldAnimNo			(player::EAnimNo::EAnimNo_None)
	, m_pEffects			()
	, m_SpecialAbility		(0.0f)
	, m_IsYButtonPressed	(false)
	, m_HasUsableSP			(false)
	, m_IsAlienHit			(false)
	, m_IsAttackSE			(false)
{
	m_ObjectTag = EObjectTag::Player;
}

CEventPlayer::~CEventPlayer()
{
}

// 初期化関数
bool CEventPlayer::Init()
{
#if 1
	// 既に読み込めていたら終了.
	if (m_pSkinMesh != nullptr) return true;
	// モデルの取得.
	CMeshResorce::GetSkin(m_pSkinMesh, MODEL_NAME);
	// モデルが読み込めてなければ false.
	if (m_pSkinMesh == nullptr) return false;
//	if (GetModel(MODEL_NAME) == false) return false;
	SetAnimation(m_NowAnimNo);		// アニメーションの設定.
	if( FootStepCollisionSetting() == false ) return false;	// 足音用の当たり判定の設定.
	if( SetAnimFrameList() == false ) return false;			// アニメーションフレームの設定.

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
	if (m_pSkinMesh == nullptr) return;
	if (m_Parameter.IsDisp == false) return;
	FootStep(RIGHT_FOOT, LEFT_FOOT);

	m_pSkinMesh->SetPosition(m_vPosition);
	m_pSkinMesh->SetRotation(m_vRotation);
	m_pSkinMesh->SetScale(m_vScale);
	m_pSkinMesh->SetAnimSpeed(m_AnimSpeed);
	m_pSkinMesh->Render();

//	MeshRender();	// メッシュの描画.
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

// 特殊能力.
bool CEventPlayer::IsSpecialAbility()
{
	if(m_IsYButtonPressed == false) return false;
	SetAnimationBlend(player::EAnimNo_SP);

	m_AnimFrameList[player::EAnimNo_SP].NowFrame += 0.01;

	if (m_AnimFrameList[player::EAnimNo_SP].NowFrame >= m_AnimFrameList[player::EAnimNo_SP].EndFrame - 0.5) {
		m_HasUsableSP = true;
		m_AnimSpeed = 0.0;
	}

	if (m_HasUsableSP == false) return false;
	// 特殊能力が使えるなら.
	m_IsYButtonPressed = false;
	m_HasUsableSP = false;	
	return true;
}

// ダメージアニメーションの更新.
void CEventPlayer::DamageAnimUpdate()
{
	if( m_IsAlienHit == false ) return;
	m_AnimFrameList[player::EAnimNo_Damage].UpdateFrame( m_AnimSpeed );

	if( m_AnimFrameList[player::EAnimNo_Damage].IsNowFrameOver() == false ) return;
	SetAnimationBlend( player::EAnimNo_Wait );
}

// 特殊能力操作関数.
void CEventPlayer::SPController()
{
	// Yボタンが押された瞬間じゃなければ終了.
//	if (CXInput::Y_Button() != CXInput::enPRESSED_MOMENT) return;
	if (CXInput::Y_Button() == CXInput::enPRESSED_MOMENT
		|| GetAsyncKeyState('Y') & 0x8000) {
		CSoundManager::PlaySE("PlayerVoiceSpecial");
		m_IsYButtonPressed = true;
	}
}

// ノックバック.
void CEventPlayer::KnockBack()
{
	if (m_AnimFrameList[player::EAnimNo_Damage].IsNowFrameOver() == true) return;

	// ダメージアニメーションを設定.
	SetAnimation(player::EAnimNo_Damage);

	m_AnimFrameList[player::EAnimNo_Damage].NowFrame += m_AnimSpeed;

	if (m_AnimFrameList[player::EAnimNo_Damage].IsNowFrameOver() == true) {
		m_AnimFrameList[player::EAnimNo_Damage].NowFrame = 0.0;
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

// 当たり判定の設定.
bool CEventPlayer::ColliderSetting()
{
#if 1
	if (m_pSkinMesh == nullptr) return false;
	if (m_pCollManager == nullptr) {
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if (FAILED(m_pCollManager->InitSphere(
		m_pSkinMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
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
		&m_vScale.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius))) return false;
	return true;
#endif	// #ifndef IS_MODEL_RENDER.
}

// エフェクトの設定.
bool CEventPlayer::EffectSetting()
{
	return false;
}

// アニメーションフレームの設定.
bool CEventPlayer::SetAnimFrameList()
{
	// 調整用アニメーションフレームのリストを用意.
	const double animAdjFrames[] =
	{
		ANIM_ADJ_FRAME_Wait,
		ANIM_ADJ_FRAME_Walk,
		ANIM_ADJ_FRAME_Attack1,
		ANIM_ADJ_FRAME_Attack2,
		ANIM_ADJ_FRAME_Attack3,
		ANIM_ADJ_FRAME_Avoid,
		ANIM_ADJ_FRAME_SP,
		ANIM_ADJ_FRAME_Damage,
		ANIM_ADJ_FRAME_Dead,
	};
	if (m_pSkinMesh == nullptr) return false;
	for (int i = player::EAnimNo_Begin; i < player::EAnimNo_End; i++) {
		m_AnimFrameList.at(i) = { 0.0, m_pSkinMesh->GetAnimPeriod(i) - animAdjFrames[i] };
	}
	return true;
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
