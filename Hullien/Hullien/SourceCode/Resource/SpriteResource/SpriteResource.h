#ifndef SPRITE_RESOURCE_H
#define SPRITE_RESOURCE_H

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>	// C++17 必須.

#include "..\..\Common\Sprite\CSprite.h"

namespace fs = std::filesystem;

class CSpriteResource
{
	const char* FILE_PATH = "Data\\Texture";	// テクスチャフォルダパス.
	typedef std::unordered_map<std::string, std::shared_ptr<CSprite>> sprite_map;
public:
	CSpriteResource();
	~CSpriteResource();

	// インスタンスの取得.
	static CSpriteResource* GetInstance();

	// スプライトの読み込み(ラッパー).
	static void Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );

	// スプライト取得関数.
	static std::shared_ptr<CSprite> GetSprite( const std::string& spriteName );

	// スプライト情報の読み込み.
	static CSprite::SPRITE_STATE SpriteStateRead( const std::string& filePath );
private:
	// スプライト読み込み.
	void SpriteLoad( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );


private:
	sprite_map m_SpriteList;	// スプライトリスト.
	bool m_HasFinishedLoading;	// 読込が終わったか.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CSpriteResource( const CSpriteResource & )				= delete;
	CSpriteResource& operator = ( const CSpriteResource & )	= delete;
	CSpriteResource( CSpriteResource && )					= delete;
	CSpriteResource& operator = ( CSpriteResource && )		= delete;
};

#endif	// #ifndef SPRITE_RESOURCE_H.