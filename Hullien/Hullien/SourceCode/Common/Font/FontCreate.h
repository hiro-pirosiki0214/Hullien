#ifndef FONT_CREATE_H
#define FONT_CREATE_H

#include "..\Common.h"
#include <Shlwapi.h>

class CFontCreate
{
	const char*	FONT_PATH = "Data\\Font\\UDDigiKyokashoN-B.ttf";
	const char*	FONT_NAME = "UD �f�W�^�� ���ȏ��� N-B";
	const int	FONT_BMP_SIZE = 256;
public:
	CFontCreate( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );
	~CFontCreate();

	// �t�H���g�摜�̍쐬.
	HRESULT CreateFontTexture2D( const char* c, ID3D11ShaderResourceView** textur2D );

private:
	CFontCreate();
	// �t�H���g�𗘗p�\�ɂ���.
	int FontAvailable();
	// Textur2D_Desc���쐬.
	D3D11_TEXTURE2D_DESC CreateDesc( UINT width, UINT height );

private:
	ID3D11Device*			m_pDevice11;	// �f�o�C�X�I�u�W�F�N�g.
	ID3D11DeviceContext*	m_pContext11;	// �f�o�C�X�R���e�L�X�g.
};

#endif	// #ifndef FONT_CREATE_H.