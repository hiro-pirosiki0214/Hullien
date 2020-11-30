#include "ItemManager.h"
#include "..\Items\LifeRecovery\LifeRecovery.h"
#include "..\Items\SPEffectTime\SPEffectTime.h"
#include "..\Items\AttackUp\AttackUp.h"
#include "..\Items\MoveSpeedUp\MoveSpeedUp.h"

#include "..\..\..\..\Utility\FileManager\FileManager.h"

CItemManager::CItemManager()
	: m_ItemList		()
	, m_ItemParam		()
	, m_EachItemEffect	()
{
}

CItemManager::~CItemManager()
{
}

// ������.
bool CItemManager::Init()
{
	// �e�p�����[�^�̓ǂݍ���.
	if( CFileManager::BinaryReading( ITEM_PARAM_FILE_PATH, m_ItemParam ) == false ) return false;
	if( CFileManager::BinaryReading( EACH_ITEM_EFFECT_FILE_PATH, m_EachItemEffect ) == false ) return false;
	return true;
}

// �h���b�v.
void CItemManager::Drop( const DropItemList& dropItemList )
{
	for( const auto& d : dropItemList ){
		switch( d.first )
		{
		default:
			continue;
		case EItemList::LifeRecovery:
			m_ItemList.emplace_back( std::make_shared<CLifeRecoveryItem>(
				// �񕜗͂ƃp�����[�^�[�̐ݒ�.
				&m_EachItemEffect.LifeRecoveryValue,
				&m_ItemParam ) );
			break;
		case EItemList::SPEffectTime:
			m_ItemList.emplace_back( std::make_shared<CSPEffectTimeItem>(
				// ����\�͉񕜗͂Ɠ���\�͉񕜎��Ԃƃp�����[�^�[�̐ݒ�.
				&m_EachItemEffect.SPRecoveryValue,
				&m_EachItemEffect.SPRecoveryTime,
				&m_ItemParam ) );
			break;
		case EItemList::AttackUp:
			m_ItemList.emplace_back( std::make_shared<CAttackUpItem>(
				// �U���͂ƍU����UP���Ԃƃp�����[�^�[�̐ݒ�.
				&m_EachItemEffect.AttackPower,
				&m_EachItemEffect.AttackUpTime,
				&m_ItemParam ) );
			break;
		case EItemList::MoveSpeedUp:
			m_ItemList.emplace_back( std::make_shared<CMoveSpeedUpItem>(
				// �ړ����x�ƈړ����xUP���Ԃƃp�����[�^�[�̐ݒ�.
				&m_EachItemEffect.MovePower,
				&m_EachItemEffect.MoveUpTime,
				&m_ItemParam ) );
			break;
		}
		// �A�C�e���̍��W��ݒ肵�āA���Ƃ�.
		m_ItemList.back()->Drop( d.second );
	}
}

// �X�V�֐�.
void CItemManager::Update( std::function<void(CActor*)> updateProc )
{
	// �e�A�C�e���̍X�V�A�����蔻��.
	for( auto& i : m_ItemList ) updateProc( i.get() );
	// �A�C�e�������X�g�������.
	for(size_t i = 0; i < m_ItemList.size(); i++)
	{
		if(m_ItemList[i]->IsHitDisappear() == true
			|| m_ItemList[i]->IsDelete() == true)
		{
			m_ItemList[i] = m_ItemList.back();
			m_ItemList.pop_back();
			i--;
		}
	}
}

// �`��֐�.
void CItemManager::Render()
{
	// �e�A�C�e���̕`��.
	for( auto& i : m_ItemList ) i->Render();
}

// �G�t�F�N�g�̕`��.
void CItemManager::EffectRender()
{
	// �e�A�C�e���̕`��.
	for( auto& i : m_ItemList ) i->EffectRender();
}