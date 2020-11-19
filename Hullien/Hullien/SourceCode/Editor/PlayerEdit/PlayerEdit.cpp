#include "PlayerEdit.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\Utility\XInput\XInput.h"

CPlayerEdit::CPlayerEdit()	
	: m_pPlayer			( nullptr )
	, m_pPlayerParam	()
{
	m_pPlayer = std::make_unique<CPlayer>();
}

CPlayerEdit::~CPlayerEdit()
{
}

// 初期化関数.
bool CPlayerEdit::Init() 
{
	if( m_pPlayer->Init() == false ) return false;
	if( CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_pPlayerParam ) == false ) return false;
	return true;
}

// 更新関数.
void CPlayerEdit::Update()
{
	m_pPlayer->Update();
	if( CXInput::Back_Button() == CXInput::enSEPARATED ){
		m_IsSetCamera = false;
		// ゲームパッドの使用を許可する.
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	}
}

// 描画関数.
void CPlayerEdit::Render() 
{
	ImGui::SetNextWindowSize( WINDOW_SIZE );
	ImGui::SetNextWindowPos( RENDER_POSITION );
	ImGui::GetWindowSize();
	ImGui::Begin( u8"プレイヤーの設定" );

	ImGui::PushItemWidth(200.0f);

	// 各パラメータの設定.
	CImGuiManager::DragFloat( u8"移動速度",				&m_pPlayerParam.MoveSpeed );
	CImGuiManager::DragFloat( u8"体力",					&m_pPlayerParam.LifeMax );
	CImGuiManager::DragFloat( u8"攻撃力",				&m_pPlayerParam.AttackPower );
	CImGuiManager::DragInt(   u8"無敵時間",				&m_pPlayerParam.InvincibleTime );
	CImGuiManager::DragFloat( u8"特殊能力最大値",		&m_pPlayerParam.SpecialAbilityMax );
	CImGuiManager::DragFloat( u8"特殊能力回復値",		&m_pPlayerParam.SpecialAbilityValue );
	CImGuiManager::DragInt(   u8"攻撃コンボ最大数",		&m_pPlayerParam.AttackComboMax );
	CImGuiManager::DragInt(   u8"攻撃キュー追加最大数",	&m_pPlayerParam.AttackQueueMax );
	CImGuiManager::DragFloat( u8"回避の移動距離",		&m_pPlayerParam.AvoidMoveDistance );
	CImGuiManager::DragFloat( u8"回避用の移動速度",		&m_pPlayerParam.AvoidMoveSpeed );
	CImGuiManager::DragFloat( u8"カメラの移動速度",		&m_pPlayerParam.CameraMoveSpeed );
	CImGuiManager::DragFloat( u8"カメラの距離",			&m_pPlayerParam.CameraDistance );
	CImGuiManager::DragFloat( u8"カメラの高さ",			&m_pPlayerParam.CameraHeight );
	CImGuiManager::DragFloat( u8"スフィアの調整座標 X",	&m_pPlayerParam.SphereAdjPos.x );
	CImGuiManager::DragFloat( u8"スフィアの調整座標 Y",	&m_pPlayerParam.SphereAdjPos.y );
	CImGuiManager::DragFloat( u8"スフィアの調整座標 Z",	&m_pPlayerParam.SphereAdjPos.z );
	CImGuiManager::DragFloat( u8"スフィアの調整半径",	&m_pPlayerParam.SphereAdjRadius );
	


	static CImGuiManager::SSuccess s_Success;
	if( ImGui::Button(u8"読込") == true ){
		// データの読み込み.
		s_Success.IsSucceeded = CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_pPlayerParam );
		if( s_Success.IsSucceeded == true ){
//			ColliderSetting();
		}
	}
	ImGui::SameLine();
	if( ImGui::Button(u8"保存") == true ){
		// データの書き込み.
		s_Success.IsSucceeded = CFileManager::BinaryWriting( PARAMETER_FILE_PATH, m_pPlayerParam );
	}
	ImGui::SameLine();
	s_Success.Render();	// 成功かどうかを描画.
	ImGui::NewLine();
	if( ImGui::Button(u8"プレビュー") == true ){
		m_IsSetCamera = true;
		// ゲームパッドの使用をやめる.
		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;
	}

	ImGui::PopItemWidth();
	ImGui::End();
}

// モデルの描画.
void CPlayerEdit::ModelRender() 
{
	m_pPlayer->Render();
}

// エフェクトの描画.
void CPlayerEdit::EffectRender()
{
	m_pPlayer->EffectRender();
}