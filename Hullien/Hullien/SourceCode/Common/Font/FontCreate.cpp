#include "FontCreate.h"

CFontCreate::CFontCreate()
	: m_pDevice11	( nullptr )
	, m_pContext11	( nullptr )
{
	FontAvailable();	// フォントを利用可能にする.
}

CFontCreate::CFontCreate( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
	: m_pDevice11	( pDevice11 )
	, m_pContext11	( pContext11 )
{
	FontAvailable();	// フォントを利用可能にする.
}

CFontCreate::~CFontCreate()
{
	DESIGNVECTOR design;
	RemoveFontResourceEx( 
		TEXT(FONT_PATH),
		FR_PRIVATE,
		&design
	);
}

//-----------------------------------.
// フォント画像の作成.
//-----------------------------------.
HRESULT CFontCreate::CreateFontTexture2D( const char* c, ID3D11Texture2D** textur2D )
{
	if( m_pDevice11 == nullptr ) return E_FAIL;
	if( m_pContext11 == nullptr ) return E_FAIL;

	HRESULT hr;
	
	// 文字コード取得
	UINT code = 0;
#if _UNICODE
	// unicodeの場合、文字コードは単純にワイド文字のUINT変換です
	code = (UINT)*c;
#else
	// マルチバイト文字の場合、
	// 1バイト文字のコードは1バイト目のUINT変換、
	// 2バイト文字のコードは[先導コード]*256 + [文字コード]です
	if(IsDBCSLeadByte(*c))
		code = (BYTE)c[0]<<8 | (BYTE)c[1];
	else
		code = c[0];
#endif

	HDC hdc = nullptr;
	HFONT hFont = nullptr;
	HFONT oldFont = nullptr;

	// フォントの生成
	LOGFONT lf = { 
		FONT_BMP_SIZE,
		0,
		0, 
		0, 
		0, 
		0,
		0,
		0, 
		SHIFTJIS_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS, 
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		TEXT("Data\\Font\\mplus-1p-medium.ttf")
	};

	hFont = CreateFontIndirect( &lf );

	// デバイスコンテキスト取得
	// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
	hdc = GetDC( NULL );
	oldFont = (HFONT)SelectObject( hdc, hFont );

	// フォントビットマップ取得
	TEXTMETRIC TM;
	GetTextMetrics( hdc, &TM );
	GLYPHMETRICS GM;
	CONST MAT2 Mat = { {0,1}, {0,0}, {0,0}, {0,1} };
	DWORD size = GetGlyphOutline( hdc, code, GGO_GRAY4_BITMAP, &GM, 0, NULL, &Mat );
	BYTE* ptr = new BYTE[size];
	GetGlyphOutline( hdc, code, GGO_GRAY4_BITMAP, &GM, size, ptr, &Mat );

	// デバイスコンテキストとフォントハンドルの開放
	SelectObject( hdc, oldFont );
	DeleteObject( hFont );
	ReleaseDC( NULL, hdc );


	//--------------------------------
	// 書き込み可能テクスチャ作成
	// CPUで書き込みができるテクスチャを作成
	// テクスチャ作成
	D3D11_TEXTURE2D_DESC desc = CreateDesc( GM.gmCellIncX, TM.tmHeight );

	hr = m_pDevice11->CreateTexture2D( &desc, 0, textur2D );

	D3D11_MAPPED_SUBRESOURCE hMappedResource;
	hr = m_pContext11->Map( 
		*textur2D,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&hMappedResource );

	// ここで書き込む
	BYTE* pBits = (BYTE*)hMappedResource.pData;
	
	// フォント情報の書き込み
	// iOfs_x, iOfs_y : 書き出し位置(左上)
	// iBmp_w, iBmp_h : フォントビットマップの幅高
	// Level : α値の段階 (GGO_GRAY4_BITMAPなので17段階)
	int iOfs_x = GM.gmptGlyphOrigin.x;
	int iOfs_y = TM.tmAscent - GM.gmptGlyphOrigin.y;
	int iBmp_w = GM.gmBlackBoxX + ( 4 - ( GM.gmBlackBoxX % 4 ) ) % 4 ;
	int iBmp_h = GM.gmBlackBoxY;
	int Level = 17;
	int x, y;
	DWORD Alpha, Color;
	memset( pBits, 0, hMappedResource.RowPitch * TM.tmHeight );
	for( y = iOfs_y; y < iOfs_y + iBmp_h; y++ ){
		for( x = iOfs_x; x < iOfs_x + iBmp_w; x++){
			Alpha = ( 255 * ptr[x - iOfs_x + iBmp_w * ( y - iOfs_y )] ) / ( Level - 1 );
			Color = 0x00ffffff | ( Alpha << 24 );

			memcpy( (BYTE*)pBits + hMappedResource.RowPitch * y + 4 * x, &Color, sizeof(DWORD) );
		}
	}
	m_pContext11->Unmap( *textur2D, 0 );

	delete[] ptr;

	return S_OK;

}

//-----------------------------------.
// フォントを利用可能にする.
//-----------------------------------.
int CFontCreate::FontAvailable()
{
	DESIGNVECTOR design;

	return AddFontResourceEx( 
		TEXT(FONT_PATH),	// フォントリソース名.
		FR_PRIVATE,			// プロセス終了時にインストールしたフォントを削除.
		&design );			// フォント構造体.
}

//-----------------------------------.
// Textur2D_Descを作成.
//-----------------------------------.
D3D11_TEXTURE2D_DESC CFontCreate::CreateDesc( UINT width, UINT height )
{
	D3D11_TEXTURE2D_DESC desc;
	memset( &desc, 0, sizeof( desc ) );
	desc.Width				= width;
	desc.Height				= height;
	desc.MipLevels			= 1;
	desc.ArraySize			= 1;
	desc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;	// RGBA(255,255,255,255)タイプ
	desc.SampleDesc.Count	= 1;
	desc.Usage				= D3D11_USAGE_DYNAMIC;			// 動的（書き込みするための必須条件）
	desc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;	// シェーダリソースとして使う
	desc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;		// CPUからアクセスして書き込みOK

	return desc;
}