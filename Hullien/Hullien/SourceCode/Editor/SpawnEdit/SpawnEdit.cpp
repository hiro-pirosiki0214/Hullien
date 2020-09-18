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
	, m_Index				( 0 )
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
		m_pStaticMesh = CMeshResorce::GetStatic("SpawnPoint");
	}
	if( m_pStaticMesh == nullptr ) return false;
	return true;
}

// 描画関数.
void CSpawnEdit::Render()
{
	if( m_pStaticMesh == nullptr ) return;
	if( m_SpawnPramList.empty() == false ){
		m_pStaticMesh->SetPosition( m_SpawnPramList[m_Index].Position );
		m_pStaticMesh->SetScale( 0.5f );
		m_pStaticMesh->Render();
	}

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

	ImGui::Separator();

	if( ImGui::Button(u8"読込") ) m_IsSucceeded = FileReading();
	ImGui::SameLine();
	if( ImGui::Button(u8"保存") ) m_IsSucceeded = FileWriting();
	ImGui::SameLine();
	if( m_IsSucceeded == true ){
		ImGui::Text( u8"成功" );
		m_SucceedRenderCount++;
		if( m_SucceedRenderCount == 60 ){
			m_SucceedRenderCount = 0;
			m_IsSucceeded = false;
		}
	}
}

// スポーン情報の描画.
void CSpawnEdit::SpawnParamRender( const int& index )
{
	if( m_SpawnPramList.empty() == true ) return;

	auto& s = m_SpawnPramList[index];

	ImGui::InputFloat( u8"座標 : X", &s.Position.x );
	ImGui::InputFloat( u8"座標 : Y", &s.Position.y );
	ImGui::InputFloat( u8"座標 : Z", &s.Position.z );
	ImGui::InputInt( u8"スポーン間隔時間", &s.SpawnTime );
	ImGui::InputInt( u8"宇宙人の最大数", &s.MaxAlienCount );

	ImGui::InputInt( u8"宇宙人Cのウェーブ時間", &s.AlienCWaveTime );
	ImGui::InputInt( u8"宇宙人Cのウェーブの間隔時間", &s.AlienCWaveIntervalTime );
	ImGui::InputInt( u8"宇宙人Dのウェーブ時間", &s.AlienDWaveTime );
	ImGui::InputInt( u8"宇宙人Dのウェーブの間隔時間", &s.AlienDWaveIntervalTime );

	ImGui::PushItemWidth(100); // これから先のUIパーツの幅を70で固定します.

	ImGui::PopItemWidth();
}