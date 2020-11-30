#include "FontResource.h"
#include "..\..\Common\Font\FontCreate.h"
#include "..\..\Utility\FileManager\FileManager.h"

CFontResource::CFontResource()
	: m_HasFinishedLoading	( false )
{
}

CFontResource::~CFontResource()
{
	for( auto m : m_FontTexturList ){
		SAFE_RELEASE(m.second);
	}
}

//-------------------------------------.
// �C���X�^���X�̎擾.
//-------------------------------------.
CFontResource* CFontResource::GetInstance()
{
	static std::unique_ptr<CFontResource> pInstance =
		std::make_unique<CFontResource>();
	return pInstance.get();
}

//-------------------------------------.
// �t�H���g�̓ǂݍ���.
//-------------------------------------.
void CFontResource::Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	GetInstance()->FontLoad( pDevice11, pContext11 );
}

//-------------------------------------.
// �t�H���g�e�N�X�`���̓ǂݍ���.
//-------------------------------------.
ID3D11ShaderResourceView* CFontResource::GetTexture2D( const std::string key )
{
	// �w�肵�����f����Ԃ�.
	for( auto& m : GetInstance()->m_FontTexturList ){
		if( m.first == key ) return m.second;
	}

	// ���f���̓Ǎ����I�����ĂȂ��̂� null ��Ԃ�.
	if( GetInstance()->m_HasFinishedLoading == false ) return nullptr;

	ERROR_MESSAGE( key + " key not found" );
	return nullptr;
}

//-------------------------------------.
// �t�H���g�̓ǂݍ���.
//-------------------------------------.
void CFontResource::FontLoad( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	std::unique_ptr<CFontCreate> pFontCreate = std::make_unique<CFontCreate>( pDevice11, pContext11 );
	std::vector<std::string> textList = CFileManager::TextLoading(TEXT_LIST_PATH);

	for( const auto& v : textList ){
		for( int i = 0; i < static_cast<int>(v.size()); i++ ){
			std::string f = v.substr( i, 1 );
			if( IsDBCSLeadByte( v[i] ) == TRUE ){
				f = v.substr( i++, 2 );	// �S�p����.
			} else {
				f = v.substr( i, 1 );	// ���p����.
			}
			// map�ɂ��łɓ���key������΃R���e�j���[.
			bool keyFind = (m_FontTexturList.find(f) != m_FontTexturList.end());
			if( keyFind == true ) continue;

			// �e�N�X�`���̍쐬.
			pFontCreate->CreateFontTexture2D( f.c_str(), &m_FontTexturList[f] );
		}
	}

	// �Ǎ����I������̂� true �ɂ���.
	m_HasFinishedLoading = true;
}