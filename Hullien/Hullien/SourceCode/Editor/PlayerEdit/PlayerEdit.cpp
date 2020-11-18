#include "PlayerEdit.h"
#include "..\..\Utility\FileManager\FileManager.h"

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
	ImGui::InputFloat( u8"移動速度",		&m_pPlayerParam.MoveSpeed );
	ImGui::InputFloat( u8"体力",			&m_pPlayerParam.LifeMax );
	ImGui::InputFloat( u8"攻撃力",			&m_pPlayerParam.AttackPower );
	ImGui::InputInt( u8"無敵時間",			&m_pPlayerParam.InvincibleTime );
	ImGui::InputFloat( u8"特殊能力最大値",	&m_pPlayerParam.SpecialAbilityMax );
	ImGui::InputFloat( u8"特殊能力回復値",	&m_pPlayerParam.SpecialAbilityValue );
	ImGui::InputInt( u8"攻撃コンボ最大数",	&m_pPlayerParam.AttackComboMax );
	ImGui::InputInt( u8"攻撃キュー追加最大数",	&m_pPlayerParam.AttackQueueMax );
	ImGui::InputFloat( u8"回避の移動距離",		&m_pPlayerParam.AvoidMoveDistance );
	ImGui::InputFloat( u8"回避用の移動速度",	&m_pPlayerParam.AvoidMoveSpeed );
	ImGui::InputFloat( u8"カメラの移動速度",	&m_pPlayerParam.CameraMoveSpeed );
	ImGui::InputFloat( u8"カメラの距離",		&m_pPlayerParam.CameraDistance );
	ImGui::InputFloat( u8"カメラの高さ",		&m_pPlayerParam.CameraHeight );
	ImGui::InputFloat( u8"スフィアの調整座標 X",	&m_pPlayerParam.SphereAdjPos.x );
	ImGui::InputFloat( u8"スフィアの調整座標 Y",	&m_pPlayerParam.SphereAdjPos.y );
	ImGui::InputFloat( u8"スフィアの調整座標 Z",	&m_pPlayerParam.SphereAdjPos.z );
	ImGui::InputFloat( u8"スフィアの調整半径",		&m_pPlayerParam.SphereAdjRadius );

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