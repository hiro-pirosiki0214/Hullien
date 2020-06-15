#include "Player.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include "..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"

#include "..\..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\Common\DebugText\DebugText.h"

CPlayer::CPlayer()
	: m_pCamera						( nullptr )
	, m_OldPosition					( 0.0f, 0.0f, 0.0f )
	, m_NowAnimNo					( CPlayer::enAnimNo::Walk )
	, m_OldAnimNo					( CPlayer::enAnimNo::None )
	, m_AttackComboCount			( 0 )
	, m_AttackEnabledFrameList		()
	, m_AttackDataQueue				()
	, m_IsDuringAvoid				( false )
	, m_AvoidMoveSpeed				( 0.0f )
	, m_Parameter					()
{
	m_ObjectTag = EObjectTag::Player;
	m_pCamera = std::make_shared<CRotLookAtCenter>();
	m_vSclae = { 0.03f, 0.03f, 0.03f };
}

CPlayer::~CPlayer()
{
}

// 初期化関数.
bool CPlayer::Init()
{
	if( ParameterSetting( PARAMETER_FILE_PATH, m_Parameter ) == false ) return false;
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( ColliderSetting() == false ) return false;

	SetAttackFrameList();
	return true;
}

// 更新関数.
void CPlayer::Update()
{
	Controller();			// 操作.
	AttackController();	// 攻撃操作.
	AvoidController();		// 回避操作.
	AttackAnimation();	// 攻撃アニメーション.
	Move();					// 移動.
	AvoidMove();			// 回避動作.

	m_pCamera->SetLength( m_Parameter.CameraDistance );	// 中心との距離を設定.
	m_pCamera->SetHeight( m_Parameter.CameraHeight );	// 高さの設定.
	// プレイヤーを注視して回転.
	m_pCamera->RotationLookAtObject( m_vPosition );

	// カメラをマネージャーに設定.
	CCameraManager::SetCamera( m_pCamera );
}

// 描画関数.
void CPlayer::Render()
{
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->SetAnimSpeed( 0.01 );
	m_pSkinMesh->Render();

#if _DEBUG
	m_pCollManager->DebugRender();
	// エディット用の描画関数をエディットレンダラーに追加.
	CEditRenderer::PushRenderProc( [&](){ EditRender(); } );
	// デバッグ描画.
	DebugRender();
#endif	// #if _DEBUG.
}

// 当たり判定関数.
void CPlayer::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	// 球体の当たり判定.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;
	
	// 攻撃関数.
	auto attackProc = [&]( float& life ){ life -= m_Parameter.AttackPower; };
	if( GetAsyncKeyState('C') & 0x8000 )
		pActor->LifeCalculation( attackProc );
}

// 相手座標の設定関数.
void CPlayer::SetTargetPos( CActor& actor )
{
//	m_vPosition = actor.GetPosition();
}

// 操作関数.
void CPlayer::Controller()
{
	// コントローラーのLスティックの傾きを取得.
	m_MoveVector.x = static_cast<float>(CXInput::LThumbX_Axis());
	m_MoveVector.z = static_cast<float>(CXInput::LThumbY_Axis());

	// カメラの回転移動.
	// 横方向.
	if( CXInput::RThumbX_Axis() >= IDLE_THUMB_MAX ) 
		m_pCamera->DegreeHorizontalMove(  m_Parameter.CameraMoveSpeed );	// 右方向.
	if( CXInput::RThumbX_Axis() <= IDLE_THUMB_MIN ) 
		m_pCamera->DegreeHorizontalMove( -m_Parameter.CameraMoveSpeed );	// 左方向.
}

// 攻撃操作関数.
void CPlayer::AttackController()
{
	// 回避中なら終了.
	if( m_IsDuringAvoid == true ) return;
	// Xボタンを押した瞬間じゃなければ終了.
	if( CXInput::X_Button() != CXInput::enPRESSED_MOMENT ) return;
	// 攻撃カウントが最大以上なら終了.
	if( m_AttackComboCount >= m_Parameter.AttackComboMax ) return;
	m_AttackComboCount++;	// 攻撃カウントを加算.
	// 攻撃データがキューに追加されたら終了.
	if( IsPushAttack() == true ) return;
	m_AttackComboCount--;	// 攻撃カウントを減算.
}

// 回避操作関数.
void CPlayer::AvoidController()
{
	// 回避中なら終了.
	if( m_IsDuringAvoid == true ) return;

	// 各値が有効範囲外なら終了.
	if( m_MoveVector.x < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.x &&
		m_MoveVector.z < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.z ) return;
	// Aボタンが押された瞬間じゃなければ終了.
	if( CXInput::A_Button() != CXInput::enPRESSED_MOMENT ) return;
	m_IsDuringAvoid = true;
	m_AvoidVector = m_MoveVector;	// 移動ベクトルを設定.
	m_OldPosition = m_vPosition;	// 現在の座標を設定.

	// 回避アニメーションを設定するなら ここ.
	//

}

// 移動関数.
void CPlayer::Move()
{
	// 回避中なら終了.
	if( m_IsDuringAvoid == true ) return;

	// 各値が有効範囲外なら終了.
	if( m_MoveVector.x < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.x &&
		m_MoveVector.z < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.z ) return;
	// スティックの傾いた方向に向く.
	m_vRotation.y = atan2f( m_MoveVector.x, m_MoveVector.z );
	// カメラの角度と足し合わせる.
	m_vRotation.y += m_pCamera->GetRadianX();

	// 回転軸で移動.
	m_vPosition.x -= sinf( m_vRotation.y ) * m_Parameter.MoveSpeed;
	m_vPosition.z -= cosf( m_vRotation.y ) * m_Parameter.MoveSpeed;
	m_OldPosition = m_vPosition;
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
	m_vRotation.z = 0.0f;	// 回転　アニメーション設定後消す.
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
			// アニメーションを待機に設定.
			SetAnimation( enAnimNo::Dead );
			return;
		}
		// 新しくアニメーションをセットする.
		SetAnimation( m_AttackDataQueue.front().AnimNo );
	}
	m_AttackDataQueue.front().Frame += 0.01;	// フレームの更新.
}

// アニメーション設定.
void CPlayer::SetAnimation( const enAnimNo& animNo )
{
	if( m_pSkinMesh == nullptr ) return;
	if( m_NowAnimNo == m_OldAnimNo ) return;
	m_OldAnimNo = m_NowAnimNo;
	m_NowAnimNo = animNo;
	m_pSkinMesh->ChangeAnimSet( static_cast<int>(animNo) );
}

// 攻撃アニメーションフレームの設定.
void CPlayer::SetAttackFrameList()
{
	if( m_pSkinMesh == nullptr ) return;
	m_AttackEnabledFrameList.emplace_back( m_pSkinMesh->GetAnimPeriod(3) );
	m_AttackEnabledFrameList.emplace_back( m_pSkinMesh->GetAnimPeriod(4) );
	m_AttackEnabledFrameList.emplace_back( m_pSkinMesh->GetAnimPeriod(1) );
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

	SAttackData tmpAttackData;	// 仮データを用意.
	switch( m_AttackComboCount )
	{
	case 1:	// 攻撃1.
		tmpAttackData.AnimNo = CPlayer::enAnimNo::Wait1;
		tmpAttackData.EnabledEndFrame = m_AttackEnabledFrameList[0];
		tmpAttackData.EndFrame = m_pSkinMesh->GetAnimPeriod(0);
		// 最初はアニメーションを設定する.
		SetAnimation( tmpAttackData.AnimNo );	
		break;
	case 2:	// 攻撃2.
		tmpAttackData.AnimNo = CPlayer::enAnimNo::Happy;
		tmpAttackData.EnabledEndFrame = m_AttackEnabledFrameList[1];
		tmpAttackData.EndFrame = m_pSkinMesh->GetAnimPeriod(4);
		break;
	case 3:	// 攻撃3.
		tmpAttackData.AnimNo = CPlayer::enAnimNo::Wait;
		tmpAttackData.EnabledEndFrame = m_AttackEnabledFrameList[2];
		tmpAttackData.EndFrame = m_pSkinMesh->GetAnimPeriod(1);
		break;
	default:
		break;
	}
	// キューにデータを挿入.
	m_AttackDataQueue.push( tmpAttackData );
	return true;
}

// ライフ計算関数.
void CPlayer::LifeCalculation( const std::function<void(float&)>& proc )
{	
	proc( m_Parameter.Life );
}

// 当たり判定の設定.
bool CPlayer::ColliderSetting()
{
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
	ImGui::InputFloat( u8"移動速度", &m_Parameter.MoveSpeed );
	ImGui::InputFloat( u8"体力", &m_Parameter.Life );
	ImGui::InputFloat( u8"攻撃力", &m_Parameter.AttackPower );
	ImGui::InputInt( u8"無敵時間", &m_Parameter.InvincibleTime );
	ImGui::InputInt( u8"攻撃コンボ最大数", &m_Parameter.AttackComboMax );
	ImGui::InputInt( u8"攻撃キュー追加最大数", &m_Parameter.AttackQueueMax );
	ImGui::InputFloat( u8"回避の移動距離", &m_Parameter.AvoidMoveDistance );
	ImGui::InputFloat( u8"回避用の移動速度", &m_Parameter.AvoidMoveSpeed );
	ImGui::InputFloat( u8"カメラの移動速度", &m_Parameter.CameraMoveSpeed );
	ImGui::InputFloat( u8"カメラの距離", &m_Parameter.CameraDistance );
	ImGui::InputFloat( u8"カメラの高さ", &m_Parameter.CameraHeight );
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

// デバッグ用の描画.
void CPlayer::DebugRender()
{
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*-1, 0.0f } );
	CDebugText::Render( "- Player Parameter -" );

	// 座標の描画.
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*0 ,0.0f } );
	CDebugText::Render( "----- Position -----" );

	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*1, 0.0f } );
	CDebugText::Render( "Pos_x : ", m_vPosition.x );				 
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*2, 0.0f } );
	CDebugText::Render( "Pos_y : ", m_vPosition.y );				 
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*3, 0.0f } );
	CDebugText::Render( "Pos_z : ", m_vPosition.z );

	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*5, 0.0f } );
	CDebugText::Render( "----- Rotation -----" );

	// 回転の描画.
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*6, 0.0f } );
	CDebugText::Render( "Rot_x : ", m_vRotation.x );				 
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*7, 0.0f } );
	CDebugText::Render( "Rot_y : ", m_vRotation.y );				 
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*8, 0.0f } );
	CDebugText::Render( "Rot_z : ", m_vRotation.z );

	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*10, 0.0f } );
	CDebugText::Render( "----- Parameter ----" );

	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*11, 0.0f } );
	CDebugText::Render( "LifePoint : ", m_Parameter.Life );
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*12, 0.0f } );
	CDebugText::Render( "AttackPower : ", m_Parameter.AttackPower );
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*13, 0.0f } );
	CDebugText::Render( "InvincibleTime : ", m_Parameter.InvincibleTime );

	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*15, 0.0f } );
	CDebugText::Render( "----- Animation ----" );
	
	// アニメーション番号の描画.
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*16, 0.0f } );
	CDebugText::Render( "Now_AnimationNo : ", (int)m_NowAnimNo );
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*17, 0.0f } );
	CDebugText::Render( "Old_AnimationNo : ", (int)m_OldAnimNo );

	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*19, 0.0f } );
	CDebugText::Render( "------ Other -------" );

	// 攻撃カウントの描画.
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*20, 0.0f } );
	CDebugText::Render( "AttackComboCount : ", m_AttackComboCount );

	// 回避中か.
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*21, 0.0f } );
	CDebugText::Render( "IsDuringAvoid : ", m_IsDuringAvoid==true?"true":"false" );
}