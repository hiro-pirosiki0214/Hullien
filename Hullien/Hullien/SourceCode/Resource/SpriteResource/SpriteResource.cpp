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
// �C���X�^���X�̎擾.
//-------------------------------.
CSpriteResource* CSpriteResource::GetInstance()
{
	static std::unique_ptr<CSpriteResource> pInstance = 
		std::make_unique<CSpriteResource>();
	return pInstance.get();
}

//-------------------------------.
// �X�v���C�g�̓ǂݍ���(���b�p�[).
//-------------------------------.
HRESULT CSpriteResource::Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	if( FAILED( GetInstance()->SpriteLoad( pDevice11, pContext11 ) )) return E_FAIL;
	return S_OK;
}

//-------------------------------.
// �X�v���C�g�擾�֐�.
//-------------------------------.
std::shared_ptr<CSprite> CSpriteResource::GetSprite( const std::string& spriteName )
{
	// �w�肵���X�v���C�g�����擾����.
	for( auto& s : GetInstance()->m_SpriteList ){
		if( s.first == spriteName ) return s.second;
	}

	// �X�v���C�g�̓Ǎ����I�����ĂȂ��̂� null ��Ԃ�.
	if( GetInstance()->m_HasFinishedLoading == false ) return nullptr;

	ERROR_MESSAGE( spriteName + "sprite not found" );
	return nullptr;
}

//-------------------------------.
// �X�v���C�g�ǂݍ���.
//-------------------------------.
HRESULT CSpriteResource::SpriteLoad( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	auto eachLoad = [&]( const fs::directory_entry& entry )
	{
		const std::string exe		= entry.path().extension().string();	// �g���q.
		const std::string filePath	= entry.path().string();				// �t�@�C���p�X.
		const std::string fileName	= entry.path().stem().string();			// �t�@�C����.
		const std::string errorMsg	= fileName+" : �X�v���C�g�ǂݍ��ݎ��s";	// ���炩���߃G���[���b�Z�[�W��ݒ肷��.

		// "_"�����������͖̂��������.
		if( filePath.find("_") != std::string::npos ) return;
		// �w�肵���g���q�ȊO�͓ǂݍ���.
		if( exe != ".png" && exe != ".PNG" && 
			exe != ".bmp" && exe != ".BMP" &&
			exe != ".jpg" && exe != ".JPG" ) return;

		// �X�v���C�g���̎擾.
		SSpriteState ss;
		if( FAILED( SpriteStateRead( filePath, &ss ))) throw errorMsg;

		// �X�v���C�g�̍쐬.
		m_SpriteList[fileName] = std::make_shared<CSprite>();
		if( FAILED( m_SpriteList[fileName]->Init( pDevice11, pContext11, filePath.c_str(), ss ) ))
			throw errorMsg;
	};

	try {
		fs::recursive_directory_iterator dir_itr(FILE_PATH), end_itr;
		std::for_each( dir_itr, end_itr, eachLoad );
	} catch ( const fs::filesystem_error& e ){
		// �G���[���b�Z�[�W��\��.
		ERROR_MESSAGE( e.path1().string().c_str() );
		return E_FAIL;
	} catch( const std::string& e ){
		// �G���[���b�Z�[�W��\��.
		ERROR_MESSAGE( e );
		return E_FAIL;
	}

	// �Ǎ����I������̂� true �ɂ���.
	m_HasFinishedLoading = true;
	return S_OK;
}

//-------------------------------.
// �X�v���C�g���̓ǂݍ���.
//-------------------------------.
SSpriteState CSpriteResource::SpriteStateRead( const std::string& path )
{
	SSpriteState ss;

	std::string filePath = path;
	// �g���q��"."�̈ʒu���擾.
	size_t i = filePath.find(".");
	// �g���q��菜����".txt"�ɕύX.
	filePath.erase( i, filePath.size() ) += ".txt";

	std::vector<std::string> pramList = CFileManager::TextLoading( filePath );

	if( pramList.empty() == true ){
		ERROR_MESSAGE( "The list of " + filePath + " was empty" );
		return ss;
	}

	ss.LocalPosNum	= std::stoi(pramList[SSpriteState::enLocalPosNum]);
	ss.Disp.w		= std::stof(pramList[SSpriteState::enDisp_w]);
	ss.Disp.h		= std::stof(pramList[SSpriteState::enDisp_h]);
	ss.Base.w		= std::stof(pramList[SSpriteState::enBase_w]);
	ss.Base.h		= std::stof(pramList[SSpriteState::enBase_h]);
	ss.Stride.w		= std::stof(pramList[SSpriteState::enStride_w]);
	ss.Stride.h		= std::stof(pramList[SSpriteState::enStride_h]);
	ss.vPos.x		= std::stof(pramList[SSpriteState::envPos_x]);
	ss.vPos.y		= std::stof(pramList[SSpriteState::envPos_y]);
	ss.vPos.z		= std::stof(pramList[SSpriteState::envPos_z]);
	ss.AnimNum		= std::stoi(pramList[SSpriteState::enAnimNum]);

	return ss;
}

// �X�v���C�g���̓ǂݍ���.
HRESULT CSpriteResource::SpriteStateRead( const std::string& path, SSpriteState* ss )
{
	std::string filePath = path;
	// �g���q��"."�̈ʒu���擾.
	size_t i = filePath.find(".");
	// �g���q��菜����".txt"�ɕύX.
	filePath.erase( i, filePath.size() ) += ".txt";

	std::vector<std::string> pramList = CFileManager::TextLoading( filePath );

	if( pramList.empty() == true ){
		ERROR_MESSAGE( "The list of " + filePath + " was empty" );
		return E_FAIL;
	}

	ss->LocalPosNum	= std::stoi(pramList[SSpriteState::enLocalPosNum]);
	ss->Disp.w		= std::stof(pramList[SSpriteState::enDisp_w]);
	ss->Disp.h		= std::stof(pramList[SSpriteState::enDisp_h]);
	ss->Base.w		= std::stof(pramList[SSpriteState::enBase_w]);
	ss->Base.h		= std::stof(pramList[SSpriteState::enBase_h]);
	ss->Stride.w	= std::stof(pramList[SSpriteState::enStride_w]);
	ss->Stride.h	= std::stof(pramList[SSpriteState::enStride_h]);
	ss->vPos.x		= std::stof(pramList[SSpriteState::envPos_x]);
	ss->vPos.y		= std::stof(pramList[SSpriteState::envPos_y]);
	ss->vPos.z		= std::stof(pramList[SSpriteState::envPos_z]);
	ss->AnimNum		= std::stoi(pramList[SSpriteState::enAnimNum]);

	return S_OK;
}