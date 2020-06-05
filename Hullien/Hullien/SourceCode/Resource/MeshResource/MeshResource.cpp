#include "MeshResource.h"
#include "..\..\Global.h"
#include "..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"

CMeshResorce::CMeshResorce()
	: m_StaticMeshList		()
	, m_SkinMeshList		()
	, m_HasFinishedLoading	( false )
{
}

CMeshResorce::~CMeshResorce()
{
	m_SkinMeshList.clear();
	m_StaticMeshList.clear();
}

//-------------------------------------.
// インスタンスの取得.
//-------------------------------------.
CMeshResorce* CMeshResorce::GetInstance()
{
	static std::unique_ptr<CMeshResorce> pInstance =
		std::make_unique<CMeshResorce>();
	return pInstance.get();
}

//-------------------------------------.
// モデルの読み込み.
//-------------------------------------.
void CMeshResorce::Load( HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, LPDIRECT3DDEVICE9 pDevice9 )
{
	GetInstance()->ModelLoad( hWnd, pDevice11, pContext11, pDevice9 );
}

//-------------------------------------.
// スタティックメッシュの読み込み.
//-------------------------------------.
std::shared_ptr<CDX9StaticMesh> CMeshResorce::GetStatic( const std::string& name )
{
	// 指定したモデルを返す.
	for( auto& m : GetInstance()->m_StaticMeshList ){
		if( m.first == name ) return m.second;
	}

	// モデルの読込が終了してないので null を返す.
	if( GetInstance()->m_HasFinishedLoading == false ) return nullptr;

	ERROR_MESSAGE( name + " mesh not found" );
	return nullptr;
}

//-------------------------------------.
// スキンメッシュの読み込み.
//-------------------------------------.
std::shared_ptr<CDX9SkinMesh> CMeshResorce::GetSkin( const std::string& name )
{
	// 指定したモデルを返す.
	for( auto& m : GetInstance()->m_SkinMeshList ){
		if( m.first == name ) return m.second;
	}

	// モデルの読込が終了してないので null を返す.
	if( GetInstance()->m_HasFinishedLoading == false ) return nullptr;

	ERROR_MESSAGE( name + " mesh not found" );
	return nullptr;
}

//-------------------------------------.
// スタティックメッシュの読み込み.
//-------------------------------------.
bool CMeshResorce::GetStatic( std::shared_ptr<CDX9StaticMesh>& model, const std::string& name )
{
	// モデルを読み込む.
	model = GetStatic( name );
	// null なら false を返す.
	if( model == nullptr ) return false; 

	// 読み込めていたら true を返す.
	return true;
}

//-------------------------------------.
// スキンメッシュの読み込み.
//-------------------------------------.
bool CMeshResorce::GetSkin( std::shared_ptr<CDX9SkinMesh>& model, const std::string& name )
{
	// モデルを読み込む.
	model = GetSkin( name );
	// null なら false を返す.
	if( model == nullptr ) return false;

	// 読み込めていたら true を返す.
	return true;
}

//-------------------------------------.
// モデルの読み込み.
//-------------------------------------.
void CMeshResorce::ModelLoad( HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, LPDIRECT3DDEVICE9 pDevice9 )
{
	auto eachLoad = [&]( const fs::directory_entry& entry )
	{
		const std::string exe		= entry.path().extension().string();	// 拡張子.
		const std::string filePath	= entry.path().string();				// ファイルパス.
		const std::string fileName	= entry.path().stem().string();			// ファイル名.

		if( exe != ".x" && exe != ".X" ) return;	// 拡張子が "Xファイル" じゃなければ終了.

		// "_s" はスキンメッシュ.
		if( fileName.find("_s") == std::string::npos ){
			// "_s" が見つからないので スタティックメッシュ.
			m_StaticMeshList[fileName] = 
				std::make_shared<CDX9StaticMesh>( hWnd, pDevice11, pContext11, pDevice9, filePath.c_str() );
		} else {
			// スキンメッシュ.
			m_SkinMeshList[fileName] = 
				std::make_shared<CDX9SkinMesh>( hWnd, pDevice11, pContext11, pDevice9, filePath.c_str() );
		}
	};

	try {
		fs::recursive_directory_iterator dir_itr( FILE_PATH ), end_itr;
		std::for_each( dir_itr, end_itr, eachLoad );

	} catch( const fs::filesystem_error& e ){
		// エラーメッセージを表示.
		ERROR_MESSAGE( e.path1().string().c_str() );
	}

	// 読込が終わったので true にする.
	m_HasFinishedLoading = true;
}