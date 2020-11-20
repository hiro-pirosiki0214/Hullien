#include "SpriteResource.h"
#include "..\..\Utility\FileManager\FileManager.h"

CSpriteResource::CSpriteResource()
	: m_SpriteList			()
	, m_HasFinishedLoading	( false )
{
}

CSpriteResource::~CSpriteResource()
{
	m_SpriteList.clear();
}

//-------------------------------.
// インスタンスの取得.
//-------------------------------.
CSpriteResource* CSpriteResource::GetInstance()
{
	static std::unique_ptr<CSpriteResource> pInstance = 
		std::make_unique<CSpriteResource>();
	return pInstance.get();
}

//-------------------------------.
// スプライトの読み込み(ラッパー).
//-------------------------------.
void CSpriteResource::Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	GetInstance()->SpriteLoad( pDevice11, pContext11 );
}

//-------------------------------.
// スプライト取得関数.
//-------------------------------.
std::shared_ptr<CSprite> CSpriteResource::GetSprite( const std::string& spriteName )
{
	// 指定したスプライト名を取得する.
	for( auto& s : GetInstance()->m_SpriteList ){
		if( s.first == spriteName ) return s.second;
	}

	// スプライトの読込が終了してないので null を返す.
	if( GetInstance()->m_HasFinishedLoading == false ) return nullptr;

	ERROR_MESSAGE( spriteName + "sprite not found" );
	return nullptr;
}

//-------------------------------.
// スプライト読み込み.
//-------------------------------.
void CSpriteResource::SpriteLoad( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	auto eachLoad = [&]( const fs::directory_entry& entry )
	{
		const std::string exe		= entry.path().extension().string();	// 拡張子.
		const std::string filePath	= entry.path().string();				// ファイルパス.
		const std::string fileName	= entry.path().stem().string();			// ファイル名.

		// "_"が入ったものは無視される.
		if( filePath.find("_") != std::string::npos ) return;
		// 指定した拡張子以外は読み込い.
		if( exe != ".png" && exe != ".PNG" && 
			exe != ".bmp" && exe != ".BMP" &&
			exe != ".jpg" && exe != ".JPG" ) return;

		CSprite::SPRITE_STATE ss = SpriteStateRead( filePath );
		m_SpriteList[fileName] =
			std::make_shared<CSprite>( pDevice11, pContext11, filePath.c_str(), ss );
	};

	try {
		fs::recursive_directory_iterator dir_itr(FILE_PATH), end_itr;
		std::for_each( dir_itr, end_itr, eachLoad );
	} catch ( const fs::filesystem_error& ){

	}

	// 読込が終わったので true にする.
	m_HasFinishedLoading = true;
}

//-------------------------------.
// スプライト情報の読み込み.
//-------------------------------.
CSprite::SPRITE_STATE CSpriteResource::SpriteStateRead( const std::string& path )
{
	CSprite::SPRITE_STATE ss;

	std::string filePath = path;
	// 拡張子の"."の位置を取得.
	size_t i = filePath.find(".");
	// 拡張子取り除いて".txt"に変更.
	filePath.erase( i, filePath.size() ) += ".txt";

	std::vector<std::string> pramList = CFileManager::TextLoading( filePath );

	if( pramList.empty() == true ){
		ERROR_MESSAGE( "The list of " + filePath + " was empty" );
		return ss;
	}

	ss.LocalPosNum	= std::stoi(pramList[ss.enLocalPosNum]);
	ss.Disp.w		= std::stof(pramList[ss.enDisp_w]);
	ss.Disp.h		= std::stof(pramList[ss.enDisp_h]);
	ss.Base.w		= std::stof(pramList[ss.enBase_w]);
	ss.Base.h		= std::stof(pramList[ss.enBase_h]);
	ss.Stride.w		= std::stof(pramList[ss.enStride_w]);
	ss.Stride.h		= std::stof(pramList[ss.enStride_h]);
	ss.vPos.x		= std::stof(pramList[ss.envPos_x]);
	ss.vPos.y		= std::stof(pramList[ss.envPos_y]);
	ss.vPos.z		= std::stof(pramList[ss.envPos_z]);
	ss.AnimNum		= std::stoi(pramList[ss.enAnimNum]);

	return ss;
}