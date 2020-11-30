#include "AlienManager.h"
#include "..\..\..\..\MotherShipUFO\MotherShipUFO.h"
#include "..\..\..\..\SpawnUFO\SpawnUFO.h"
#include "..\AlienList.h"
#include "..\..\..\Explosion\Explosion.h"

#include "..\..\..\..\..\Common\DebugText\DebugText.h"
#include "..\..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\..\Common\SceneTexRenderer\SceneTexRenderer.h"

#include <algorithm>

CAlienManager::CAlienManager()
	: m_AilenList			()
	, m_SpawnUFOList		()
	, m_AlienParamList		()
	, m_DropItemList		()
	, m_MotherShipUFOPos	( 0.0f, 0.0f, 0.0f )
	, m_IsAlienAbduct		( false )
	, m_SortCount			( 0 )
	, m_IsRisingMotherShip	( false )
{
}

CAlienManager::~CAlienManager()
{
}

// �������֐�.
bool CAlienManager::Init()
{
	
	if( SpawnUFOInit() == false )			return false;
	if( ReadAlienParamList() == false )		return false;
	if( ReadExplosionParam() == false )		return false;
	
	return true;
}

// �X�V�֐�.
void CAlienManager::Update( std::function<void(CActor*)> updateProc )
{
	Spawn();	// �X�|�[��.
	bool isAbduct = false;
	m_DropItemList.clear();

	// �F���l�B�̍X�V.
	for( size_t i = 0; i < m_AilenList.size(); i++ ){
		m_AilenList[i]->SetOtherAbduct( &m_IsAlienAbduct );

		// �X�V�֐�.
		updateProc( m_AilenList[i].get() );

		// ���̎q��A�ꋎ���Ă��邩�m�F.
		if( isAbduct == false ){
			if( m_AilenList[i]->IsAbduct() == true ){
				isAbduct = m_IsAlienAbduct = true;
			}
		}

		// �����ł��邩�m�F.
		ExplosionConfirming( m_AilenList[i].get() );
		// �����ƉF���l�̓����蔻��.
		for( auto& e : m_ExplosionList ){
			if( e.IsStop() == true ) continue;
			e.Collision( m_AilenList[i].get() );
		}

		// �}�U�[�V�b�v�ɏ����Ă��邩�m�F.
		if( m_AilenList[i]->IsRisingMotherShip() == true ){
			m_IsRisingMotherShip = true;
		}

		// ���X�g����w��̉F���l��������.
		if( m_AilenList[i]->IsDelete() == false ) continue;

		// �A�C�e�����X�g�̐ݒ�.
		SetDropItemList( m_AilenList[i].get() );

		m_AilenList[i] = m_AilenList.back();
		m_AilenList.pop_back();
		i--;
	}
	if( isAbduct == false ) m_IsAlienAbduct = false;
}

// �`��֐�.
void CAlienManager::Render()
{
	// �F���l�B�̕`��.
	for( auto& a : m_AilenList ) a->Render();
	// �X�|�[��UFO�̕`��.
	for( auto& s : m_SpawnUFOList ) s->Render();
}

// �G�t�F�N�g�`��֐�.
void CAlienManager::EffectRender()
{
	for( auto& a : m_AilenList ) a->EffectRender();
	// �����̕`��.
	for( auto& e : m_ExplosionList ){
		e.Render();
		e.EffectRender();
	}
}

// �X�v���C�g�`��֐�.
void CAlienManager::SpriteRender()
{
	// �F���l�B�̕`��.
	for( auto& a : m_AilenList ) a->SpriteRender();
	// �X�|�[��UFO�̕`��.
	for( auto& s : m_SpawnUFOList ) s->SpriteRender();
}

// ���̎q��A�ꋎ���Ă��邩.
bool CAlienManager::IsGirlAbduct()
{
	return m_IsRisingMotherShip;
}

// �A�j���[�V�������~�߂�.
void CAlienManager::StopAnimation()
{
	for( auto& a : m_AilenList ) a->StopAnimation();
}

// �X�|�[��.
void CAlienManager::Spawn()
{
	for( auto& s : m_SpawnUFOList ){
		s->SpawnAlien( m_AilenList );
	}
}

// �����ł��邩�ǂ����m�F.
void CAlienManager::ExplosionConfirming( CAlien* ailen )
{
	// �F���lC����Ȃ���ΏI��.
	if( ailen->GetObjectTag() != EObjectTag::Alien_C ) return;
	// �������Ȃ���ΏI��.
	if( ailen->IsExplosion() == false ) return;
	// �����ƉF���l�̓����蔻��.
	for( auto& e : m_ExplosionList ){
		if( e.IsStop() == false ) continue;
		// �������I�����Ă���Ύg���܂킷.
		e.Init();
		e.SetTargetPos( *ailen );
		return;
	}
	// �I�������������Ȃ����.
	// ������ǉ�.
	m_ExplosionList.emplace_back();
	m_ExplosionList.back().Init();
	m_ExplosionList.back().SetExplosionParam( m_ExplosionParam );
	m_ExplosionList.back().SetTargetPos( *ailen );
}

// ���Ƃ��A�C�e���̐ݒ�.
void CAlienManager::SetDropItemList( CAlien* ailen )
{
	if( ailen->IsDead()		== false )				return;
	if( ailen->GetAnyItem()	== EItemList::None )	return;
	if( ailen->GetAnyItem()	== EItemList::Max )		return;

	// ���Ƃ��A�C�e���̐ݒ�.
	D3DXVECTOR3 dropPos = ailen->GetPosition();
	dropPos.y += 4.0f;	// ���߂ɒ���.
	m_DropItemList[ailen->GetAnyItem()] = dropPos;
}

// �X�|�[��UFO�̏�����.
bool CAlienManager::SpawnUFOInit()
{
	std::vector<SSpawnUFOParam> spawnPramList;
	if( CFileManager::BinaryVectorReading( SPAWN_PARAM_FILE_PATH, spawnPramList ) == false ) return false;
	for( const auto& s : spawnPramList ){
		m_SpawnUFOList.emplace_back( std::make_shared<CSpawnUFO>() );
		m_SpawnUFOList.back()->Init();
		m_SpawnUFOList.back()->SetAbductUFOPosition( &m_MotherShipUFOPos );
		m_SpawnUFOList.back()->SetSpawnParameter( s );
	}
	m_AilenList.reserve( spawnPramList[0].MaxAlienCount );
	return true;
}

// �F���l�p�����[�^���X�g�̓ǂݍ���.
bool CAlienManager::ReadAlienParamList()
{
	// �e�F���l�̃p�����[�^�p�X���X�g�̎擾.
	std::vector<std::string> readList = 
		CFileManager::TextLoading( ALIEN_PARAM_LIST_FILE_PATH );

	// �e�L�X�g���ǂݍ��߂ĂȂ�������I��.
	if( readList.empty() == true ) return false;

	for( const auto& l : readList ){
		// �e�F���l�̃p�����[�^�ǂݍ���.
		SAlienParam param = {};
		CFileManager::BinaryReading( l.c_str(), param );
		m_AlienParamList.emplace_back( param );
	}
	// �X�|�[��UFO�Ɋe�F���l�̃p�����[�^�ݒ�.
	for( auto& s : m_SpawnUFOList ){
		s->SetAlienParameterList( &m_AlienParamList );
		s->LightDischarge();
	}

	return true;
}

// �����p�����[�^�̓ǂݍ���.
bool CAlienManager::ReadExplosionParam()
{
	if( CFileManager::BinaryReading( EXPLOSION_PARAM_FILE_PATH, m_ExplosionParam ) == false ) return false;
	return true;
}