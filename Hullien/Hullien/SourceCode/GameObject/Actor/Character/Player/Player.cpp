#include "Player.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include "..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\Widget\SceneWidget\GameWidget\CharacterWidget\ChatacterWidget.h"
#include "..\..\..\Widget\SceneWidget\GameWidget\CharacterWidget\LifeGauge\LifeGauge.h"
#include "..\..\..\Widget\SceneWidget\GameWidget\CharacterWidget\SpecialAbilityGauge\SpecialAbilityGauge.h"
#include "..\..\..\..\Common\Effect\EffectManager.h"
#include "..\..\Item\EffectTimer\EffectTimer.h"

#include "..\..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\Common\DebugText\DebugText.h"
#include "..\...\..\..\..\..\XAudio2\SoundManager.h"

CPlayer::CPlayer()
	: m_pCamera						( nullptr )
	, m_pSPCamera					( nullptr )
	, m_pWidget						()
	, m_pAttackCollManager			( nullptr )
	, m_OldPosition					( 0.0f, 0.0f, 0.0f )
	, m_GirlPosition				( 0.0f, 0.0f, 0.0f )
	, m_NowAnimNo					( player::EAnimNo_Wait )
	, m_OldAnimNo					( player::EAnimNo_None )
	, m_AttackComboCount			( player::EAttackNo_None )
	, m_AttackEnabledFrameList		()
	, m_AttackDataQueue				()
	, m_AttackPosition				( 0.0f, 0.0f, 0.0f )
	, m_pEffects					()
	, m_IsDuringAvoid				( false )
	, m_AvoidVector					( 0.0f, 0.0f, 0.0f )
	, m_Parameter					()
	, m_LifePoint					( 0.0f )
	, m_SpecialAbility				( 0.0f )
	, m_IsYButtonPressed			( false )
	, m_IsUsableSP					( false )
	, m_SpecialAbilityValue			( 0.0f )
	, m_ItemSpecialAbilityValue		( 0.0f )
	, m_AttackPower					( 0.0f )
	, m_MoveSpeed					( 0.0f )
	, m_MoveSpeedMulValue			( 0.0f )
	, m_CameraDefaultHeight			( 0.0f )
	, m_CameraHeight				( 0.0f )
	, m_CameraPosition				( 0.0f, 0.0f, 0.0f )
	, m_CameraLookPosition			( 0.0f, 0.0f, 0.0f )
	, m_CameraCount					( CAMERA_COUNT_MAX )
	, m_CameraLerp					( 0.0f )
	, m_NowSPCameraStete			( player::ESPCameraState_Start )
	, m_IsAttackHitCamera			( false )
	, m_CameraShakeCount			( 0.0f )
	, m_CameraShakeTime				( 10.0f )
	, m_CameraShakeCountAdd			( 1.0f )
	, m_pEffectTimers				( player::EEffectTimerNo_Max )
	, m_IsAttackSE					( false )
{
	m_ObjectTag = EObjectTag::Player;
	m_pCamera	= std::make_shared<CRotLookAtCenter>();
	m_pSPCamera = std::make_shared<CCamera>();
	for( auto& e : m_pEffectTimers ) e = std::make_shared<CEffectTimer>();
}

CPlayer::~CPlayer()
{
}

// 初期化関数.
bool CPlayer::Init()
{
	if( ParameterSetting( PARAMETER_FILE_PATH, m_Parameter ) == false ) return false;
#ifndef IS_TEMP_MODEL_RENDER
	if( GetModel( MODEL_NAME ) == false ) return false;
#if 1
	// アニメーション再生.
	SetAttackFrameList();
#endif	//#if 0
#else
	if( GetModel( MODEL_TEMP_NAME ) == false ) return false;
#endif	// #ifndef IS_TEMP_MODEL_RENDER.
	if( ColliderSetting() == false ) return false;
	if( WidgetSetting() == false ) return false;
	if( EffectSetting() == false ) return false;
	m_MoveSpeed		= m_Parameter.MoveSpeed;	// 移動速度の設定.
	m_AttackPower	= m_Parameter.AttackPower;	// 攻撃力の設定.
	m_LifePoint		= m_Parameter.LifeMax;		// 体力の設定.
	m_SpecialAbilityValue = m_Parameter.SpecialAbilityValue;	// 特殊能力回復値の設定.
	m_CameraHeight = m_CameraDefaultHeight = m_Parameter.CameraHeight;
	m_CameraLookPosition = { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z };
	m_CameraLerp = m_Parameter.CameraLerpValue;
	return true;
}

// 更新関数.
void CPlayer::Update()
{
	// 麻痺タイマーが動作してなければ.
	if( m_pEffectTimers[player::EEffectTimerNo_Paralysis]->IsUpdate() == false ){
		Controller();			// 操作.
		AttackController();		// 攻撃操作.
		SPController();			// 特殊能力操作.
		AvoidController();		// 回避操作.
		AttackAnimation();		// 攻撃アニメーション.
		Move();					// 移動.
		AvoidMove();			// 回避動作.
	} else {
		ParalysisUpdate();		// 麻痺時の更新.
	}
	CameraController();			// カメラ操作.
	AttackHitCameraUpdate();	// 攻撃ヒット時のカメラ動作.
	SPCameraUpdate();			// 特殊能力時のカメラ動作.
	SpecialAbilityUpdate();		// 特殊能力回復更新.
	AttackUpUpdate();			// 攻撃力UP更新.
	MoveSpeedUpUpdate();		// 移動速度UP更新.


	// プレイヤーを注視して回転.
	m_pCamera->RotationLookAtObject( { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z }, m_CameraLerp );
	m_pCamera->SetLength( m_Parameter.CameraDistance );	// 中心との距離を設定.
	m_pCamera->SetHeight( m_CameraHeight );	// 高さの設定.
	if( m_IsYButtonPressed == false ){
		m_CameraLerp = m_Parameter.CameraLerpValue;
		// カメラをマネージャーに設定.
		CCameraManager::SetCamera( m_pCamera );
	} else {
		CCameraManager::SetCamera( m_pSPCamera );
	}

	// 体力が1/3になったらSEを鳴らす.
	if (m_LifePoint <= m_Parameter.LifeMax / 3)
	{
		if (CSoundManager::GetIsPlaySE("HP", 0) == true) return;
		CSoundManager::NoMultipleSEPlay("HP");
	}
}

// 描画関数.
void CPlayer::Render()
{
	MeshRender();	// メッシュの描画.

#if _DEBUG
	if( m_pCollManager == nullptr ) return;
	m_pCollManager->DebugRender();
	if( m_pAttackCollManager == nullptr ) return;
	m_pAttackCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// 当たり判定関数.
void CPlayer::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	AttackCollision( pActor );	// 攻撃時の当たり判定.

}

// 相手座標の設定関数.
void CPlayer::SetTargetPos( CActor& actor )
{
	if( actor.GetObjectTag() != EObjectTag::Girl ) return;
	m_GirlPosition = actor.GetPosition();
}

// スプライトの描画.
void CPlayer::SpriteRender()
{
	// ゲージ.
	CCharacterWidget::SCharacterParam param;
	// ライフ.
	param.Life = m_LifePoint;
	param.LifeMax = m_Parameter.LifeMax;
	// 特殊能力.
	param.SpecialAbility = m_SpecialAbility;
	param.SpecialAbilityMax = m_Parameter.SpecialAbilityMax;
	for (const auto& s : m_pWidget)
	{
		s->SetParameter(param);
		s->Update();
		s->Render();
	}

#if _DEBUG
	// エディット用の描画関数をエディットレンダラーに追加.
	CEditRenderer::PushRenderProc( [&](){ EditRender(); } );
#endif	// #if _DEBUG.
}

// 特殊能力を使っているか.
bool CPlayer::IsSpecialAbility()
{
	if( m_IsUsableSP  == false ) return false;
	// 特殊能力が使えるなら.
	m_IsUsableSP = false;	// 初期化して.
	return true;			// trueを返す.
}

// カメラの方向.
float CPlayer::GetCameraRadianX()
{
	return m_pCamera->GetRadianX();
}

// 操作関数.
void CPlayer::Controller()
{
	// コントローラーのLスティックの傾きを取得.
	m_MoveVector.x = static_cast<float>(CXInput::LThumbX_Axis());
	m_MoveVector.z = static_cast<float>(CXInput::LThumbY_Axis());

	if( GetAsyncKeyState(VK_UP) & 0x8000 )		m_vPosition.z -= m_MoveSpeed;
	if( GetAsyncKeyState(VK_DOWN) & 0x8000 )	m_vPosition.z += m_MoveSpeed;
	if( GetAsyncKeyState(VK_RIGHT) & 0x8000 )	m_vPosition.x -= m_MoveSpeed;
	if( GetAsyncKeyState(VK_LEFT) & 0x8000 )	m_vPosition.x += m_MoveSpeed;
}

// カメラ操作.
void CPlayer::CameraController()
{
	if( m_IsYButtonPressed == true ) return;
	// カメラの回転移動.
	// 横方向.
	if( CXInput::RThumbX_Axis() >= IDLE_THUMB_MAX ) 
		m_pCamera->DegreeHorizontalMove(  m_Parameter.CameraMoveSpeed );	// 右方向.
	if( CXInput::RThumbX_Axis() <= IDLE_THUMB_MIN ) 
		m_pCamera->DegreeHorizontalMove( -m_Parameter.CameraMoveSpeed );	// 左方向.

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000 && GetAsyncKeyState(VK_RIGHT) & 0x8000) 
		m_pCamera->DegreeHorizontalMove(m_Parameter.CameraMoveSpeed);
	if (GetAsyncKeyState(VK_SHIFT)  & 0x8000 && GetAsyncKeyState(VK_LEFT) & 0x8000)	
		m_pCamera->DegreeHorizontalMove(-m_Parameter.CameraMoveSpeed);
}

// 攻撃操作関数.
void CPlayer::AttackController()
{
	// 回避中なら終了.
	if( m_IsDuringAvoid == true ) return;
	// Yボタン：特殊能力を使っていたら.
	if( m_IsYButtonPressed == true ) return;
	// Xボタンを押した瞬間じゃなければ終了.
	if( CXInput::X_Button() != CXInput::enPRESSED_MOMENT ) return;
	// 攻撃カウントが最大以上なら終了.
	if( m_AttackComboCount >= m_Parameter.AttackComboMax ) return;
	CSoundManager::NoMultipleSEPlay("PlayerAttackSE");

	m_AttackComboCount++;	// 攻撃カウントを加算.
	// 攻撃データがキューに追加されたら終了.
	if( IsPushAttack() == true ) return;
	m_AttackComboCount--;	// 攻撃カウントを減算.

}

// 特殊能力操作関数.
void CPlayer::SPController()
{
	// 回避中なら終了.
	if( m_IsDuringAvoid == true ) return;
	// 攻撃中は移動しない.
	if( m_AttackComboCount > 0 ) return;
	if( m_SpecialAbility < m_Parameter.SpecialAbilityMax ) return;
	// Yボタンが押された瞬間じゃなければ終了.
	if( CXInput::Y_Button() != CXInput::enPRESSED_MOMENT ) return;

	m_CameraPosition = m_pCamera->GetPosition();
	m_SpecialAbility = 0.0f;
	m_IsYButtonPressed = true;

	if( m_NowAnimNo == player::EAnimNo_Wait )		return;	// 既に待機モーションなら終了.
	SetAnimationBlend( player::EAnimNo_Wait );	// 待機アニメーションを設定.
}

// 回避操作関数.
void CPlayer::AvoidController()
{
	// 回避中なら終了.
	if( m_IsDuringAvoid == true ) return;
	// Yボタン：特殊能力を使っていたら.
	if( m_IsYButtonPressed == true ) return;

	// 各値が有効範囲外なら終了.
	if( m_MoveVector.x < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.x &&
		m_MoveVector.z < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.z ) return;
	// Aボタンが押された瞬間じゃなければ終了.
	if( CXInput::A_Button() != CXInput::enPRESSED_MOMENT ) return;
	m_IsDuringAvoid = true;
	m_AvoidVector = m_MoveVector;	// 移動ベクトルを設定.
	m_OldPosition = m_vPosition;	// 現在の座標を設定.
	CSoundManager::NoMultipleSEPlay("AvoidMove");
	m_pEffects[player::enEffectNo_Avoidance]->Play( m_vPosition );
	// 回避アニメーションを設定するなら ここ.
	//

}

// 移動関数.
void CPlayer::Move()
{
	// 回避中なら終了.
	if( m_IsDuringAvoid == true ) return;
	// 攻撃中は移動しない.
	if( m_AttackComboCount > 0 ) return;
	// Yボタン：特殊能力を使っていたら.
	if( m_IsYButtonPressed == true ) return;

	// 各値が有効範囲外なら終了.
	if( m_MoveVector.x < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.x &&
		m_MoveVector.z < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.z ){
		m_MoveSpeedMulValue = 0.0f;
		if( m_NowAnimNo == player::EAnimNo_Attack1 )	return;	// アニメーションが攻撃1,2,3の時は.
		if( m_NowAnimNo == player::EAnimNo_Attack2 )	return;	//　待機モーションに設定できないようにする.
		if( m_NowAnimNo == player::EAnimNo_Attack3 )	return;	// 
		if( m_NowAnimNo == player::EAnimNo_Wait )		return;	// 既に待機モーションなら終了.
		SetAnimationBlend( player::EAnimNo_Wait );	// 待機アニメーションを設定.
		return;
	}

	// 掛け合わせる移動量の加算.
	m_MoveSpeedMulValue += MOVE_SPEED_MUL_VALUE_ADD;
	if( m_MoveSpeedMulValue >= MOVE_SPEED_MUL_VALUE_MAX ) m_MoveSpeedMulValue = MOVE_SPEED_MUL_VALUE_MAX;

	// ターゲットのベクトルを用意.
	float targetRot = atan2f( m_MoveVector.x, m_MoveVector.z ) + m_pCamera->GetRadianX();
	D3DXVECTOR3 targetVec = { 0.0f, 0.0f, 0.0f };
	targetVec.x = sinf( targetRot );
	targetVec.z = cosf( targetRot );

	if( TargetRotation( targetVec, ROTATIONAL_SPEED, TOLERANCE_RADIAN ) == true ){
		// ターゲットの回転を取得.
		m_vRotation.y = targetRot;

		// 回転軸で移動.
		m_vPosition.x -= targetVec.x * m_MoveSpeed * m_MoveSpeedMulValue;
		m_vPosition.z -= targetVec.z * m_MoveSpeed * m_MoveSpeedMulValue;

		m_OldPosition = m_vPosition;
		if( m_NowAnimNo == player::EAnimNo_Attack1 )	return;	// アニメーションが攻撃1,2,3の場合は、
		if( m_NowAnimNo == player::EAnimNo_Attack2 )	return;	//　移動しないので、
		if( m_NowAnimNo == player::EAnimNo_Attack3 )	return;	//	アニメーションを設定せずに終了.
		if( m_NowAnimNo == player::EAnimNo_Walk )		return;	// 既に移動モーションなら終了.
		SetAnimationBlend( player::EAnimNo_Walk );
	}
}

// 回避動作関数.
void CPlayer::AvoidMove()
{
	// 回避中じゃなければ終了.
	if( m_IsDuringAvoid == false ) return;

	// スティックの傾いた方向に向く.
	m_vRotation.y = atan2f( m_AvoidVector.x, m_AvoidVector.z );
	// カメラの角度と足し合わせる.
	m_vRotation.y += m_pCamera->GetRadianX();

	// 回転軸で移動.
	m_vPosition.x -= sinf( m_vRotation.y ) * m_Parameter.AvoidMoveSpeed;
	m_vPosition.z -= cosf( m_vRotation.y ) * m_Parameter.AvoidMoveSpeed;

	// 回転　アニメーション設定後消す.
	m_vRotation.z -= 0.1f;

	D3DXVECTOR3 length = m_OldPosition - m_vPosition;
	// 移動距離を計算.
	float moveDistance = D3DXVec3Length( &length );
	// 移動距離が一定以下なら終了.
	if( moveDistance <= m_Parameter.AvoidMoveDistance ) return;
	m_IsDuringAvoid = false;	// 回避中じゃなくする.
	m_vRotation.z = 0.0f;		// 回転　アニメーション設定後消す.
}

// 目的の座標へ回転.
bool CPlayer::TargetRotation( const D3DXVECTOR3& targetVec, const float& rotSpeed, const float& ToleranceRadian )
{
	// 自身のベクトルを用意.
	D3DXVECTOR3 myVector = { 0.0f, 0.0f ,0.0f };
	myVector.x = sinf( m_vRotation.y );
	myVector.z = cosf( m_vRotation.y );

	// ベクトルの長さを求める.
	float myLenght = sqrtf(myVector.x*myVector.x + myVector.z*myVector.z);
	float targetLenght = sqrtf(targetVec.x*targetVec.x + targetVec.z*targetVec.z);

	// 内積を求める.
	float dot = myVector.x*targetVec.x + myVector.z*targetVec.z;
	dot = acosf( ( myLenght * targetLenght ) * dot );

	if( ( -ToleranceRadian < dot && dot < ToleranceRadian ) ||	// 内積が許容範囲なら.
		( std::isfinite( dot ) ) == false ){					// 内積の値が計算できない値なら.
		return true;	// 回転終了.
	} else {
		// 目的のベクトルと、自分のベクトルの外積を求める.
		float cross = myVector.x*targetVec.z - myVector.z*targetVec.x;
		// 外積が0.0より少なければ 時計回り : 反時計回り に回転する.
		m_vRotation.y += cross < 0.0f ? rotSpeed : -rotSpeed;

		return false;	// 回転中.
	}
}

// エフェクト描画関数.
void CPlayer::EffectRender()
{
	if( m_AttackComboCount > player::EAttackNo_None ){
		m_pEffects[m_AttackComboCount-1]->SetLocation( m_vPosition );
	}
	for( auto& e : m_pEffects ){
		e->Render();
	}
}

// 攻撃の当たり判定.
void CPlayer::AttackCollision( CActor* pActor )
{
	const float attackLength = 10.0f;
	if( m_AttackComboCount <= player::EAttackNo_None ){
		m_AttackPosition = { 0.0f, -1.0f, 0.0f };
		return;
	}

	//
	//	アニメーションが来るまでプレイヤーの目の前だけ攻撃.
	//

	// 回転軸で移動.
//	m_AttackPosition.x = m_vPosition.x - sinf( m_vRotation.y ) * attackLength;
//	m_AttackPosition.y = 5.0f;
//	m_AttackPosition.z = m_vPosition.z - cosf( m_vRotation.y ) * attackLength;

	m_pSkinMesh->GetPosFromBone( "kaito_rifa_2_L_ude_1", &m_AttackPosition );

	// 球体の当たり判定.
	if( m_pAttackCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;

	// 攻撃関数.
	auto attackProc = [&]( float& life ){ life -= 10.0f; };
	pActor->LifeCalculation( attackProc );
	m_IsAttackHitCamera = true;
}

// 攻撃ヒット時のカメラ動作.
void CPlayer::AttackHitCameraUpdate()
{
	if( m_IsAttackHitCamera == false ) return;
	m_CameraShakeCount += m_CameraShakeCountAdd;
	m_CameraHeight = m_CameraDefaultHeight + sinf( m_CameraShakeCount ) * (m_AttackComboCount*0.1f);
	if( m_CameraShakeCount <= m_CameraShakeTime ) return;
	m_CameraShakeCount = 0.0f;
	m_CameraHeight = m_CameraDefaultHeight;
	m_IsAttackHitCamera = false;
}

// 特殊能力時のカメラ動作.
void CPlayer::SPCameraUpdate()
{
	// 特殊能力が使えないなら終了.
	if( m_IsYButtonPressed == false ){
		m_CameraLookPosition = { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z };
		return;
	}

	m_CameraLookPosition = m_GirlPosition;		// 女の子を視点にする.
	D3DXVECTOR3 vec = m_vPosition - m_CameraLookPosition;	// 現在の座標と女の子の座標とのベクトルを取得する.
	float targetRot = atan2f( vec.x, vec.z );	// 回転値を設定する.

	switch( m_NowSPCameraStete )
	{
	case player::ESPCameraState_TargetRotation:
	{
		//-------------------------------------.
		// プレイヤーを女の子のほうへ向ける.
		//-------------------------------------.
		// 目的のベクトルを用意.
		D3DXVECTOR3 targetVec = { 0.0f, 0.0f, 0.0f };
		targetVec.x = sinf( targetRot );
		targetVec.z = cosf( targetRot );
		
		if( TargetRotation( targetVec, ROTATIONAL_SPEED, TOLERANCE_RADIAN ) == true ){
			m_vRotation.y = atan2f( targetVec.x, targetVec.z );
			m_NowSPCameraStete = player::ESPCameraState_PlayerBack;
		} else {
			m_CameraLookPosition = { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z };
		}
		break;
	}
	case player::ESPCameraState_PlayerBack:
	{
		//-------------------------------------.
		// カメラをプレイヤーの後ろに移動させる.
		//-------------------------------------.
		// カメラの座標を設定する.
		m_CameraNextPosition = m_vPosition;
		m_CameraNextPosition.x += sinf( targetRot ) * CAMERA_BACK_DIRECTION_X;
		m_CameraNextPosition.z += cosf( targetRot ) * CAMERA_BACK_DIRECTION_Y;
		m_CameraNextPosition.y = CAMERA_BACK_HEIGHT;
		// プレイヤーの後ろに移動.
		D3DXVec3Lerp( &m_CameraPosition, &m_CameraPosition, &m_CameraNextPosition, CAMERA_BACK_LERP_VALUE );
		if( fabsf(D3DXVec3Length(&m_CameraPosition) - D3DXVec3Length(&m_CameraNextPosition)) < 0.01f ){
			m_IsUsableSP = true;	// 特殊能力を使う.
			m_NowSPCameraStete = player::ESPCameraState_CameraShake;
		}
	}
		break;
	case player::ESPCameraState_CameraShake:
	{
		//-------------------------------------.
		// カメラを揺らす.
		//-------------------------------------.
		m_CameraCount--;	// カウントの減算.
		// カメラの揺れ.
		const float SHAKE_VALUE = sinf(static_cast<float>(D3DX_PI) * TWO / CAMERA_FREQUENCY_LOOKPOS * m_CameraCount) * (m_CameraCount * CAMERA_AMPLITUDE_LOOKPOS);
		m_CameraLookPosition.x += SHAKE_VALUE;
		m_CameraLookPosition.y += SHAKE_VALUE;
		
		if( m_CameraCount <= 0 ){
			m_CameraCount			= CAMERA_COUNT_MAX;
			m_CameraNextPosition	= m_pCamera->GetPosition();
			m_CameraReturnCount		= 0.0f;
			m_NowSPCameraStete		= player::ESPCameraState_CameraReturn;
		}
	}
		break;
	case player::ESPCameraState_CameraReturn:
	{
		//-------------------------------------.
		// カメラをもとの位置に戻す.
		//-------------------------------------.
		m_CameraReturnCount += CAMERA_RETURN_COUNT_ADD;
		if( m_CameraReturnCount >= CAMERA_RETURN_COUNT_MAX ) m_CameraReturnCount = 1.0f;

		m_CameraLookPosition = { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z };
		// プレイヤーの後ろに移動.
		D3DXVec3Lerp( &m_CameraPosition, &m_CameraPosition, &m_CameraNextPosition, m_CameraReturnCount );
		if( m_CameraReturnCount >= CAMERA_RETURN_COUNT_MAX ){
			m_pCamera->SetLookPosition( m_CameraLookPosition );
			m_pCamera->SetPosition( m_CameraPosition );
			m_IsYButtonPressed	= false;
			m_CameraReturnCount = 0.0f;
			m_CameraLerp		= 0.0f;
			m_NowSPCameraStete	= player::ESPCameraState_Start;
		}
	}
		break;
	default:
		break;
	}
	// 特殊能力カメラ用の座標と視点座標を設定.
	m_pSPCamera->SetLookPosition( m_CameraLookPosition );
	m_pSPCamera->SetPosition( m_CameraPosition );
}

// 特殊能力回復更新関数.
void CPlayer::SpecialAbilityUpdate()
{							
	// アイテムでの回復状態なら.
	if( m_pEffectTimers[player::EEffectTimerNo_SPRecovery]->Update() == true ){
		m_SpecialAbilityValue = m_Parameter.SpecialAbilityValue;	// 回復値をもとに戻す.
	}

	// 特殊能力値が最大以上なら終了.
	if( m_SpecialAbility >= m_Parameter.SpecialAbilityMax ) return;
	m_SpecialAbility += m_SpecialAbilityValue;	// 特殊能力値を加算.

	if( m_SpecialAbility < m_Parameter.SpecialAbilityMax ) return;
	m_SpecialAbility = m_Parameter.SpecialAbilityMax;	// 最大値を超えないようにする.
}

// 攻撃力UP更新関数.
void CPlayer::AttackUpUpdate()
{
	if( m_pEffectTimers[player::EEffectTimerNo_Attack]->Update() == false ) return;
	m_AttackPower		= m_Parameter.AttackPower;
}

// 移動速度UP更新関数.
void CPlayer::MoveSpeedUpUpdate()
{
	if( m_pEffectTimers[player::EEffectTimerNo_MoveSpeedUp]->Update() == false ) return;
	m_MoveSpeed = m_Parameter.MoveSpeed;
}

// 麻痺中の更新関数.
void CPlayer::ParalysisUpdate()
{
	if( m_pEffectTimers[player::EEffectTimerNo_Paralysis]->Update() == false ) return;
}

// 攻撃アニメーション.
void CPlayer::AttackAnimation()
{
	// キューが空なら終了.
	if( m_AttackDataQueue.empty() == true ) return;
	if( m_AttackDataQueue.front().Frame >= m_AttackDataQueue.front().EndFrame ){
		m_AttackDataQueue.pop();	// キューの先頭を取り出す.
		// キューが空なら終了.
		if( m_AttackDataQueue.empty() == true ){
			// これが最後の攻撃なので、攻撃カウントを0にする.
			m_AttackComboCount = 0;	
			// 各値が有効範囲外なら終了.
			if( m_MoveVector.x < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.x &&
				m_MoveVector.z < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.z ){
				// アニメーションを待機に設定.
				if( m_NowAnimNo == player::EAnimNo_Wait ) return;
				SetAnimationBlend( player::EAnimNo_Wait );
				return;
			} else {
				// アニメーションを移動に設定.
				if( m_NowAnimNo == player::EAnimNo_Walk ) return;
				SetAnimationBlend( player::EAnimNo_Walk );
			}
			return;
		}
		// 新しくアニメーションをセットする.
		SetAnimation( m_AttackDataQueue.front().AnimNo );
	}
	m_AttackDataQueue.front().Frame += 0.01;	// フレームの更新.
}

// アニメーション設定.
void CPlayer::SetAnimation( const player::EAnimNo& animNo )
{
	if( m_pSkinMesh == nullptr ) return;
	if( m_NowAnimNo == m_OldAnimNo ) return;
	m_OldAnimNo = m_NowAnimNo;
	m_NowAnimNo = animNo;
	m_pSkinMesh->ChangeAnimSet( m_NowAnimNo );
}

void CPlayer::SetAnimationBlend( const player::EAnimNo& animNo )
{
	if( m_pSkinMesh == nullptr ) return;
	if( m_NowAnimNo == m_OldAnimNo ) return;
	m_OldAnimNo = m_NowAnimNo;
	m_NowAnimNo = animNo;
	m_pSkinMesh->ChangeAnimBlend( m_NowAnimNo, m_OldAnimNo );
}

// 攻撃アニメーションフレームの設定.
void CPlayer::SetAttackFrameList()
{
	if( m_pSkinMesh == nullptr ) return;
	m_AttackEnabledFrameList.emplace_back( m_pSkinMesh->GetAnimPeriod(player::EAnimNo_Attack1) );
	m_AttackEnabledFrameList.emplace_back( m_pSkinMesh->GetAnimPeriod(player::EAnimNo_Attack2) );
	m_AttackEnabledFrameList.emplace_back( m_pSkinMesh->GetAnimPeriod(player::EAnimNo_Attack3) );
}

// 攻撃の追加ができたか.
bool CPlayer::IsPushAttack()
{
	if( static_cast<int>(m_AttackDataQueue.size()) >= m_Parameter.AttackComboMax ) return false;
	if( static_cast<int>(m_AttackDataQueue.size()) >= m_Parameter.AttackQueueMax ) return false;
	if( m_AttackDataQueue.empty() == false ){
		// キューにデータが入っていたら.
		// 経過フレームが攻撃有効フレームより大きければ終了.
		if( m_AttackDataQueue.front().Frame >= 
			m_AttackDataQueue.front().EnabledEndFrame ) return false;
	}

	player::SAttackData tmpAttackData;	// 仮データを用意.
	switch( m_AttackComboCount )
	{
	case player::EAttackNo_One:	// 攻撃1.
#ifndef INTERMEDIATE_ANNOUCEMENT_ATTACK
		tmpAttackData.AnimNo = player::EAnimNo_Attack1;
		tmpAttackData.EnabledEndFrame = m_AttackEnabledFrameList[player::EAttackNo_One-1]-0.5;
		tmpAttackData.EndFrame = m_pSkinMesh->GetAnimPeriod( player::EAnimNo_Attack1 )-0.5;
		// 最初はアニメーションを設定する.
		SetAnimation( tmpAttackData.AnimNo );
#endif	// #if INTERMEDIATE_ANNOUCEMENT_ATTACK.
		
		break;
	case player::EAttackNo_Two:	// 攻撃2.
#ifndef INTERMEDIATE_ANNOUCEMENT_ATTACK
		tmpAttackData.AnimNo = player::EAnimNo_Attack2;
		tmpAttackData.EnabledEndFrame = m_AttackEnabledFrameList[player::EAttackNo_Two-1]-0.5;
		tmpAttackData.EndFrame = m_pSkinMesh->GetAnimPeriod( player::EAnimNo_Attack2 )-0.5;
#endif	// #if INTERMEDIATE_ANNOUCEMENT_ATTACK.

		break;
	case player::EAttackNo_Three:// 攻撃3.
#ifndef INTERMEDIATE_ANNOUCEMENT_ATTACK
		tmpAttackData.AnimNo = player::EAnimNo_Attack3;
		tmpAttackData.EnabledEndFrame = m_AttackEnabledFrameList[player::EAttackNo_Three-1]-0.5;
		tmpAttackData.EndFrame = m_pSkinMesh->GetAnimPeriod( player::EAnimNo_Attack3 )-0.5;
#endif	// #if INTERMEDIATE_ANNOUCEMENT_ATTACK.

		break;
	default:
		break;
	}
	// キューにデータを挿入.
	m_AttackDataQueue.push( tmpAttackData );
	// エフェクトを再生.
	m_pEffects[m_AttackComboCount-1]->Play( m_vPosition );
	return true;
}

// ライフ計算関数.
void CPlayer::LifeCalculation( const std::function<void(float&)>& proc )
{	
	proc( m_LifePoint );
	if( m_LifePoint < m_Parameter.LifeMax ) return;
	m_LifePoint = m_Parameter.LifeMax;
}

// 特殊能力回復時間、効力時間設定関数.
void CPlayer::SetSPEffectTime( const std::function<void(float&,float&)>& proc )
{
	if( m_pEffectTimers[player::EEffectTimerNo_SPRecovery]->IsUpdate() == true ) return;

	float tmpTime = 0.0f;
	proc( m_ItemSpecialAbilityValue, tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_SPRecovery]->SetTime( tmpTime );

	if( m_SpecialAbility >= m_Parameter.SpecialAbilityMax ) return;
	m_pEffectTimers[player::EEffectTimerNo_SPRecovery]->Set();
}

// 攻撃力、効力時間設定関数.
void CPlayer::SetAttackEffectTime( const std::function<void(float&,float&)>& proc )
{
	if( m_pEffectTimers[player::EEffectTimerNo_Attack]->IsUpdate() == true ) return;

	float tmpTime = 0.0f;
	proc( m_AttackPower, tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_Attack]->SetTime( tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_Attack]->Set();
}

// 移動速度、効力時間設定関数.
void CPlayer::SetMoveSpeedEffectTime( const std::function<void(float&,float&)>& proc )
{
	if( m_pEffectTimers[player::EEffectTimerNo_MoveSpeedUp]->IsUpdate() == true ) return;

	float tmpTime = 0.0f;
	proc( m_MoveSpeed, tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_MoveSpeedUp]->SetTime( tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_MoveSpeedUp]->Set();
}

// 麻痺の設定.
void CPlayer::SetParalysisTime( const std::function<void(float&)>& proc )
{
	if( m_IsDuringAvoid == true ) return;
	float tmpTime = 0.0f;
	proc( tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_Paralysis]->SetTime( tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_Paralysis]->Set();
	CSoundManager::NoMultipleSEPlay("PlayerHitSE");

}

// 当たり判定の設定.
bool CPlayer::ColliderSetting()
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
#else
	if( m_pTempStaticMesh == nullptr ) return false;
	// メッシュを使用しての当たり判定初期化.
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere( 
		m_pTempStaticMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius ) )) return false;
	if( FAILED( m_pCollManager->InitCapsule( 
		m_pTempStaticMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		-5.0f,
		0.0f ) )) return false;
#endif	// #ifndef IS_MODEL_RENDER.

	// 攻撃用の当たり判定初期化.
	if( m_pAttackCollManager == nullptr ){
		m_pAttackCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pAttackCollManager->InitSphere(
		&m_AttackPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		1.0f ) )) return false;
	return true;
}

// エフェクトの設定.
bool CPlayer::EffectSetting()
{
	const char* effectNames[] =
	{
		ATTACK_ONE_EFFECT_NAME,
		ATTACK_TWO_EFFECT_NAME,
		ATTACK_THREE_EFFECT_NAME,
		SPECIAL_ABILITY_EFFECT_NAME,
		AVOIDANCE_EFFECT_NAME,
	};
	const int effectNum = sizeof(effectNames)/sizeof(effectNames[0]);
	// メモリの最大値設定.
	m_pEffects.reserve(effectNum);

	for( int i = 0; i < effectNum; i++ ){
		m_pEffects.emplace_back( std::make_shared<CEffectManager>() );
		if( m_pEffects[i]->SetEffect( effectNames[i] ) == false ) return false;
	}

	return true;
}

// エディット用の描画関数.
void CPlayer::EditRender()
{
#if _DEBUG
	ImGui::SetNextWindowSize( ImVec2(440.0f,470.0f), ImGuiCond_::ImGuiCond_Once );
	ImGui::SetNextWindowPos( ImVec2(WND_W-440,0.0f), ImGuiCond_::ImGuiCond_Once );
	ImGui::GetWindowSize();
	bool isOpen = true;
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_WindowBg] = { 0.3f, 0.3f, 0.3f, 0.9f };
	ImGui::Begin( u8"プレイヤーの設定", &isOpen );

	// 各パラメータの設定.
	ImGui::InputFloat( u8"移動速度", &m_MoveSpeed );
	ImGui::InputFloat( u8"体力", &m_LifePoint );
	ImGui::InputFloat( u8"攻撃力", &m_Parameter.AttackPower );
	ImGui::InputInt( u8"無敵時間", &m_Parameter.InvincibleTime );
	ImGui::InputFloat( u8"特殊能力最大値", &m_Parameter.SpecialAbilityMax );
	ImGui::InputFloat( u8"特殊能力回復値", &m_Parameter.SpecialAbilityValue );
	ImGui::InputInt( u8"攻撃コンボ最大数", &m_Parameter.AttackComboMax );
	ImGui::InputInt( u8"攻撃キュー追加最大数", &m_Parameter.AttackQueueMax );
	ImGui::InputFloat( u8"回避の移動距離", &m_Parameter.AvoidMoveDistance );
	ImGui::InputFloat( u8"回避用の移動速度", &m_Parameter.AvoidMoveSpeed );
	ImGui::InputFloat( u8"カメラの移動速度", &m_Parameter.CameraMoveSpeed );
	ImGui::InputFloat( u8"カメラの距離", &m_Parameter.CameraDistance );
	ImGui::InputFloat( u8"カメラの高さ", &m_Parameter.CameraHeight );
	ImGui::InputFloat( u8"カメラ注視点の高さ", &m_Parameter.CameraLookHeight );
	ImGui::InputFloat( u8"カメラ移動の補完値", &m_Parameter.CameraLerpValue );
	ImGui::InputFloat( u8"スフィアの調整座標 X", &m_Parameter.SphereAdjPos.x );
	ImGui::InputFloat( u8"スフィアの調整座標 Y", &m_Parameter.SphereAdjPos.y );
	ImGui::InputFloat( u8"スフィアの調整座標 Z", &m_Parameter.SphereAdjPos.z );
	ImGui::InputFloat( u8"スフィアの調整半径", &m_Parameter.SphereAdjRadius );

	static CImGuiManager::SSuccess s_Success;
	if( ImGui::Button(u8"読込") == true ){
		// データの読み込み.
		s_Success.IsSucceeded = CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_Parameter );
		if( s_Success.IsSucceeded == true ){
			ColliderSetting();
		}
	}
	ImGui::SameLine();
	if( ImGui::Button(u8"保存") == true ){
		// データの書き込み.
		s_Success.IsSucceeded = CFileManager::BinaryWriting( PARAMETER_FILE_PATH, m_Parameter );
	}
	ImGui::SameLine();
	s_Success.Render();	// 成功かどうかを描画.

	ImGui::End();
#endif	// #if _DEBUG.
}

// ウィジェット設定.
bool CPlayer::WidgetSetting()
{
	m_pWidget.emplace_back(std::make_shared<CLifeGauge>());
	m_pWidget.emplace_back(std::make_shared<CSpecialAbilityGauge>());

	for (const auto& s : m_pWidget)
	{
		if (s->Init() == false) return false;
	}

	return true;
}