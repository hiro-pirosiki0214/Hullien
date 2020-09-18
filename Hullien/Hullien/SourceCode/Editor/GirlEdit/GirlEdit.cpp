#include "GirlEdit.h"
#include "..\..\Utility\FileManager\FileManager.h"

CGirlEdit::CGirlEdit()	
	: m_pGirl		( nullptr )
	, m_GirlParam	()
{
	m_pGirl = std::make_unique<CGirl>();
}

CGirlEdit::~CGirlEdit()
{
}

// 初期化関数.
bool CGirlEdit::Init() 
{
	if( m_pGirl->Init() == false ) return false;
	if( CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_GirlParam ) == false ) return false;
	return true;
}

// 更新関数.
void CGirlEdit::Update()
{
	m_pGirl->Update();
}

// 描画関数.
void CGirlEdit::Render() 
{
	ImGui::SetNextWindowSize( WINDOW_SIZE );
	ImGui::SetNextWindowPos( RENDER_POSITION );
	ImGui::GetWindowSize();
	ImGui::Begin( u8"女の子設定" );

	ImGui::PushItemWidth(200.0f);

	// 各パラメータの設定.
	ImGui::InputFloat( u8"移動速度",		&m_GirlParam.MoveSpeed );
	ImGui::InputFloat( u8"回転速度",		&m_GirlParam.RotatlonalSpeed );
	ImGui::InputFloat( u8"初期座標 : X",	&m_GirlParam.InitPosition.x );
	ImGui::InputFloat( u8"初期座標 : Y",	&m_GirlParam.InitPosition.y );
	ImGui::InputFloat( u8"初期座標 : Z",	&m_GirlParam.InitPosition.z );
	ImGui::InputFloat( u8"初期座標の許容の長さ",	&m_GirlParam.InitPosLenght );
	ImGui::InputFloat( u8"索敵範囲の半径",			&m_GirlParam.SearchCollRadius );
	ImGui::InputFloat( u8"スフィア調整用座標 : X",	&m_GirlParam.SphereAdjPos.x );
	ImGui::InputFloat( u8"スフィア調整用座標 : Y",	&m_GirlParam.SphereAdjPos.y );
	ImGui::InputFloat( u8"スフィア調整用座標 : Z",	&m_GirlParam.SphereAdjPos.z );
	ImGui::InputFloat( u8"スフィア調整用半径",		&m_GirlParam.SphereAdjRadius );

	static CImGuiManager::SSuccess s_Success;
	if( ImGui::Button(u8"読込") == true ){
		// データの読み込み.
		s_Success.IsSucceeded = CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_GirlParam );
	}
	ImGui::SameLine();
	if( ImGui::Button(u8"保存") == true ){
		// データの書き込み.
		s_Success.IsSucceeded = CFileManager::BinaryWriting( PARAMETER_FILE_PATH, m_GirlParam );
	}
	ImGui::SameLine();
	s_Success.Render();	// 成功かどうかを描画.

	ImGui::PopItemWidth();
	ImGui::End();
}

// モデルの描画.
void CGirlEdit::ModelRender() 
{
	m_pGirl->Render();
}