#include "ExplosionEdit.h"
#include "..\..\Utility\FileManager\FileManager.h"

CExplosionEdit::CExplosionEdit()
	: m_pExplosion		( nullptr )
	, m_ExplosionParam	()
{
	m_pExplosion = std::make_unique<CExplosion>();
}

CExplosionEdit::~CExplosionEdit()
{
}

// 初期化関数.
bool CExplosionEdit::Init()
{
	if( FileReading() == false ) return false;
	if( m_pExplosion->Init() == false ) return false;
	m_pExplosion->SetExplosionParam( m_ExplosionParam );
	return true;
}

// 更新関数.
void CExplosionEdit::Update()
{
	m_pExplosion->Update();
}

// 描画関数.
void CExplosionEdit::Render()
{
	ImGui::SetNextWindowSize( WINDOW_SIZE );
	ImGui::SetNextWindowPos( RENDER_POSITION );
	ImGui::Begin( u8"爆発パラメーター設定" );

	ImGui::PushItemWidth(200.0f);

	if( ImGui::Button( u8"再生" ) == true ){
		if( m_pExplosion->IsStop() == true ){
			m_pExplosion->Init();
			m_pExplosion->SetPosition( D3DXVECTOR3( 0.0f, 5.0f, 0.0f ) );
		}
	}

	ImGui::InputFloat( u8"爆発力", &m_ExplosionParam.AttackPower );
	if( ImGui::InputFloat( u8"爆発速度", &m_ExplosionParam.ExplosionSpeed ) ){
		m_pExplosion->SetExplosionParam( m_ExplosionParam );
	}
	if( ImGui::InputFloat( u8"スフィアの最大半径", &m_ExplosionParam.SphereMaxRadius ) ){
		m_pExplosion->SetExplosionParam( m_ExplosionParam );
	}
	ImGui::InputFloat( u8"スフィアの調整用座標 : X", &m_ExplosionParam.SphereAdjPos.x );
	ImGui::InputFloat( u8"スフィアの調整用座標 : Y", &m_ExplosionParam.SphereAdjPos.y );
	ImGui::InputFloat( u8"スフィアの調整用座標 : Z", &m_ExplosionParam.SphereAdjPos.z );

	static CImGuiManager::SSuccess s_success = {};
	if( ImGui::Button(u8"読込") ) s_success.IsSucceeded = FileReading();
	ImGui::SameLine();
	if( ImGui::Button(u8"保存") ) s_success.IsSucceeded = FileWriting();
	ImGui::SameLine();
	s_success.Render();

	ImGui::PopItemWidth();

	ImGui::End();
}

// モデルの描画.
void CExplosionEdit::ModelRender()
{
	m_pExplosion->Render();
}

// ファイルの読み込み.
bool CExplosionEdit::FileReading()
{
	return CFileManager::BinaryReading( FILE_PATH, m_ExplosionParam );
}

// ファイルの書き込み.
bool CExplosionEdit::FileWriting()
{
	return CFileManager::BinaryWriting( FILE_PATH, m_ExplosionParam );
}