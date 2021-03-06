#include "SpawnEdit.h"
#include "..\..\GameObject\SpawnUFO\SpawnUFOParam.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Utility\FileManager\FileManager.h"

CSpawnEdit::CSpawnEdit()
	: m_SpawnPramList		()
	, m_pStaticMesh			( nullptr )
	, m_IsSucceeded			( false )
	, m_SucceedRenderCount	( 0 )
{
}

CSpawnEdit::~CSpawnEdit()
{
}

// 初期化関数.
bool CSpawnEdit::Init()
{
	if( FileReading() == false ) return false;
	if( m_pStaticMesh != nullptr ) return true;
	if( m_pStaticMesh == nullptr ){
		m_pStaticMesh = CMeshResorce::GetStatic( MODEL_NAME );
	}
	if( m_pStaticMesh == nullptr ) return false;
	return true;
}

// 更新関数.
void CSpawnEdit::Update()
{
}

// 描画関数.
void CSpawnEdit::Render()
{

	ImGui::SetNextWindowSize( WINDOW_SIZE );
	ImGui::SetNextWindowPos( RENDER_POSITION );
	ImGui::GetWindowSize();
	ImGui::Begin( u8"スポーン情報設定" );

	ImGui::PushItemWidth(200.0f);

	std::vector<std::string> tagNameList;
	tagNameList.emplace_back("Index");
	for( size_t i = 0; i < m_SpawnPramList.size(); i++ ){
		tagNameList.emplace_back("SpawnNo." + std::to_string(i) );
	}
	if( ImGui::BeginTabBar( "TabBarID" ) == true ){
		for( size_t i = 0; i < tagNameList.size(); i++ ){
			if( ImGui::BeginTabItem( tagNameList[i].c_str() ) == false ) continue;
			if( i == 0 ){
				IndexRender();
			} else {
				SpawnParamRender( i-1 );
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::PopItemWidth();
	ImGui::End();
}

// モデルの描画.
void CSpawnEdit::ModelRender()
{
	// モデルの描画.
	if( m_pStaticMesh == nullptr ) return;
	if( m_SpawnPramList.empty() == false ){
		for( const auto& p : m_SpawnPramList ){
			m_pStaticMesh->SetPosition( p.Position );
			m_pStaticMesh->SetScale( 1.0f );
			m_pStaticMesh->Render();
		}
	}
}

// エフェクトの描画.
void CSpawnEdit::EffectRender()
{
}

// ファイルの読み込み.
bool CSpawnEdit::FileReading()
{
	return CFileManager::BinaryVectorReading( FILE_PATH, m_SpawnPramList );
}

// ファイルの書き込み.
bool CSpawnEdit::FileWriting()
{
	return CFileManager::BinaryVectorWriting( FILE_PATH, m_SpawnPramList );
}

// インデックスの描画.
void CSpawnEdit::IndexRender()
{
	ImGui::TextWrapped( u8"敵は各設定した最大・最小の値を、" );
	ImGui::TextWrapped( u8"ランダムで取得しスポーンする" );

	ImGui::Separator();

	ImGui::Text( u8"Position : スポーンポイントの座標" );
	ImGui::Text( u8"SpawnTime : スポーンする時間" );

	ImGui::Separator();

	ImGui::Text( u8"現在のスポーン数を" );
	ImGui::SameLine();
	if( ImGui::Button(u8"増やす") ) m_SpawnPramList.emplace_back();
	ImGui::SameLine();
	if( ImGui::Button(u8"減らす") ) m_SpawnPramList.pop_back();
	static CImGuiManager::SSuccess s_Success;
	if( ImGui::Button(u8"読込") == true ){
		// データの読み込み.
		s_Success.IsSucceeded =  FileReading();
	}
	ImGui::SameLine();
	if( ImGui::Button(u8"保存") == true ){
		// データの書き込み.
		s_Success.IsSucceeded = FileWriting();
	}
	ImGui::SameLine();
	s_Success.Render();	// 成功かどうかを描画.
}

// スポーン情報の描画.
void CSpawnEdit::SpawnParamRender( const int& index )
{
	if( m_SpawnPramList.empty() == true ) return;

	auto& s = m_SpawnPramList[index];

	CImGuiManager::DragFloat( u8"座標 : X", &s.Position.x );
	CImGuiManager::DragFloat( u8"座標 : Y", &s.Position.y );
	CImGuiManager::DragFloat( u8"座標 : Z", &s.Position.z );
	CImGuiManager::DragFloat( u8"スポーンポイントの高さ", &s.SpawnPointHight );
	CImGuiManager::DragInt( u8"スポーン間隔時間", &s.SpawnTime );
	CImGuiManager::DragInt( u8"宇宙人の最大数", &s.MaxAlienCount );
	CImGuiManager::DragInt( u8"宇宙人Cの出現率", &s.ProbabilityC );
	CImGuiManager::DragInt( u8"宇宙人Dの出現率", &s.ProbabilityD );
	CImGuiManager::DragInt( u8"アイテムを落とす確率", &s.ProbabilityItem );

	ImGui::PushItemWidth(100); // これから先のUIパーツの幅を70で固定します.

	ImGui::PopItemWidth();
}