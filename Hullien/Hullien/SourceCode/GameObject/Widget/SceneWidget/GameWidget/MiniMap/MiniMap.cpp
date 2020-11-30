#include "MiniMap.h"

#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Actor\ActorManager\GameActorManager.h"
#include "..\..\..\..\..\Common\DebugText\DebugText.h"

/**********************************
*	�~�j�}�b�v�N���X.
**/
CMiniMap::CMiniMap()
	: m_IconList		()
	, m_ObjPosListCount	( 0 )
{
}

CMiniMap::~CMiniMap()
{
}

// �������֐�.
bool CMiniMap::Init()
{
	// �����X�v���C�g�ǂݍ���.
	if (SpriteSetting() == false ) return false;

	return true;
}

// �X�V�֐�.
void CMiniMap::Update()
{
}

// �I�u�W�F�N�g�ʒu�ݒ�֐�.
void CMiniMap::SetObjPosition(CGameActorManager* pObj)
{
	// �X�v���C�g�̐ݒ�.
	auto List = pObj->GetObjPositionList();
	SpriteSetting( List );

	// �A�C�R���ʒu�̍X�V.
	int objCount = 1;	// �}�b�v���n�͊܂߂Ȃ��̂Ŕz��ԍ���1����.
	for (const auto& obj : pObj->GetObjPositionList())
	{
		// None�Ȃ�Ώ������Ȃ�.
		if (obj.first == EObjectTag::None) continue;
		// �^�O���̍X�V.
		if (obj.first != m_IconList[objCount].EObjTag) 
		{ 
			m_IconList[objCount].EObjTag = obj.first; 
			m_IconList[objCount].AnimNumber = SetAnimNumber(obj.first);
		}

		// �ʒu���̍X�V.
		if (m_IconList[objCount].Pos == obj.second.first) return;
		m_IconList[objCount].Pos.x = m_IconList[MAP_BACK].pSprite->GetRenderPos().x - (obj.second.first.x * MAP_ICON_POSITION_CORRECTION_VALUE);
		m_IconList[objCount].Pos.y = m_IconList[MAP_BACK].pSprite->GetRenderPos().y + (obj.second.first.z * MAP_ICON_POSITION_CORRECTION_VALUE);
		m_IconList[objCount].Rot.z = obj.second.second;
		objCount++;
	}
}

// �`��֐�.
void CMiniMap::Render()
{
	if (m_IconList.size() == 0) return;
	for (const auto& l : m_IconList)
	{
		// �^�O��None�Ȃ�Δ�\��.
		if (l.EObjTag == EObjectTag::None) continue;
		l.pSprite->SetPosition(l.Pos );
		l.pSprite->SetRotation(l.Rot );
		l.pSprite->SetAnimNumber(l.AnimNumber );
		l.pSprite->SetDeprh( false );
		l.pSprite->SetBlend( true );
		l.pSprite->RenderUI();
		l.pSprite->SetBlend( false );
		l.pSprite->SetDeprh( true );
	}
	// �}�b�v�̕`��.
	m_pSprite->SetDeprh( false );
	m_pSprite->SetBlend( true );
	m_pSprite->RenderUI();
	m_pSprite->SetBlend( false );
	m_pSprite->SetDeprh( true );
}

// �X�v���C�g�ݒ�֐�.
bool CMiniMap::SpriteSetting()
{
	if ( m_IconList.size() != 0 ) return true;

	// �}�b�v�w�i�̓ǂݍ���.
	m_IconList.emplace_back();
	m_IconList[MAP_BACK].pSprite = CSpriteResource::GetSprite(SPRITE_MAP_BACK);
	m_IconList[MAP_BACK].Pos = m_IconList[MAP_BACK].pSprite->GetRenderPos();
	if(m_IconList[MAP_BACK].pSprite == nullptr ) return false;

	m_pSprite = CSpriteResource::GetSprite(SPRITE_MAP_FRONT);
	if( m_pSprite == nullptr ) return false;

	return true;
}

// �X�v���C�g�ݒ�֐�.
void CMiniMap::SpriteSetting(OBJLIST objList)
{
	if(m_ObjPosListCount == objList.size()) return;
	// �J�E���g���I�u�W�F�N�g���𒴂�����.
	// �I�u�W�F�N�g�����܂Ō��炷.
	if (m_ObjPosListCount > objList.size())
	{
		m_ObjPosListCount = objList.size();
		m_IconList.resize(objList.size()+1);	// �}�b�v���n�̕�+1.
		m_IconList.shrink_to_fit();
		return;
	}

	// ���X�g�𑝂₷.
	for (auto obj = objList.begin() + m_ObjPosListCount; obj < objList.end(); obj++)
	{
		m_ObjPosListCount++;
		ObjSpriteSetting(SPRITE_DEFAULT_ICON, obj->first);
	}
}

// �I�u�W�F�N�g�̃X�v���C�g�ݒ�֐�.
void CMiniMap::ObjSpriteSetting(const char* spriteName, const EObjectTag& tag)
{
	SIconInfo info;
	// �ǂݍ��ރX�v���C�g�̐ݒ�.
	info.pSprite = CSpriteResource::GetSprite( spriteName );
	// �����ʒu�̐ݒ�.
	info.Pos = m_IconList[MAP_BACK].pSprite->GetRenderPos();
	// �^�O�̐ݒ�.
	info.EObjTag = tag;
	// �A�j���[�V�����ԍ��̐ݒ�.
	info.AnimNumber = SetAnimNumber( tag );
	// ���X�g�𑝂₷.
	m_IconList.emplace_back( info );
}

// �A�j���[�V�����ԍ��̐ݒ�֐�.
int CMiniMap::SetAnimNumber(const EObjectTag& tag)
{
	int animNumber = 0;
	switch (tag)
	{
	case EObjectTag::Player:
		animNumber = static_cast<int>(EIconType::Player);
		break;
	case EObjectTag::Girl:
		animNumber = static_cast<int>(EIconType::Girl);
		break;
	case EObjectTag::Alien_A:
		animNumber = static_cast<int>(EIconType::Alien_A);
		break;
	case EObjectTag::Alien_B:
		animNumber = static_cast<int>(EIconType::Alien_B);
		break;
	case EObjectTag::Alien_C:
		animNumber = static_cast<int>(EIconType::Alien_C);
		break;
	case EObjectTag::Alien_D:
		animNumber = static_cast<int>(EIconType::Alien_D);
		break;
	case EObjectTag::SPEffectTimeItem:
	case EObjectTag::AttackUpItem:
	case EObjectTag::LifeRecoveryItem:
	case EObjectTag::MoveSpeedUpItem:
		animNumber = static_cast<int>(EIconType::Item);
		break;
	case EObjectTag::MotherShipUFO:
		animNumber = static_cast<int>(EIconType::MotherShipUFO);
		break;
	default:
		break;
	}

	return animNumber;
}

