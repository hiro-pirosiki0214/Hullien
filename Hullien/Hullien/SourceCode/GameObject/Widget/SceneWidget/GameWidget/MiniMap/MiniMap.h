#ifndef MINIMAP_H
#define MINIMAP_H

#include "..\..\SceneWidget.h"
#include <vector>

class CGameActorManager;	//�A�N�^�Ǘ��N���X.

/**********************************
*	�~�j�}�b�v�N���X.
**/
class CMiniMap : public CSceneWidget
{
private:
	typedef std::pair<D3DXVECTOR3, float> pos_rot_pair;	// ���W�Ɖ�]�̃y�A.
	typedef std::vector<std::pair<EObjectTag, pos_rot_pair>> OBJLIST;

	const char* SPRITE_MAP_BACK		= "minimapBase";		//�~�j�}�b�v�w�i�X�v���C�g��.
	const char* SPRITE_MAP_FRONT	= "minimapWaku";		//�~�j�}�b�v�w�i�X�v���C�g��.
	const char* SPRITE_DEFAULT_ICON = "mapiconsize";	//�W���A�C�R���X�v���C�g��.
	const int	MAP_BACK = 0;							//�~�j�}�b�v�w�i�z��ԍ�.
	const float MAP_ICON_POSITION_CORRECTION_VALUE	= 0.55f;
	// �A�C�R�����.
	struct stIconInfo
	{
		std::shared_ptr<CSprite> pSprite;	// �X�v���C�g.
		D3DXVECTOR3 Pos;					// �ʒu.
		D3DXVECTOR3 Rot;					// ��].
		EObjectTag EObjTag;					// �^�O.
		int AnimNumber;						// �A�j���[�V�����ԍ�.

		stIconInfo()
			: pSprite	( nullptr )
			, Pos		(D3DXVECTOR3(0.0f,0.0f,0.0f))
			, Rot		(D3DXVECTOR3(0.0f,0.0f,0.0f))
			, EObjTag	(EObjectTag::Max)
			, AnimNumber (0)
		{}
	}typedef SIconInfo;

	// �A�C�R���̎��.
	enum class IconType
	{
		Player = 0,
		Girl,
		Alien_A,
		Alien_B,
		Alien_C,
		Alien_D,
		Item,
		MotherShipUFO,
	}typedef EIconType;

public:
	CMiniMap();
	virtual ~CMiniMap();

	// �������֐�.
	virtual bool Init() override;

	// �X�V�֐�.
	virtual void Update() override;
	// �I�u�W�F�N�g�ʒu�ݒ�֐�.
	void SetObjPosition(CGameActorManager* pObj);

	// �`��֐�.
	virtual void Render() override;

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;
	//�X�v���C�g�ݒ�֐�.
	void SpriteSetting(OBJLIST objList);
	// �I�u�W�F�N�g�̃X�v���C�g�ݒ�֐�.
	void ObjSpriteSetting(const char* spriteName, const EObjectTag& tag);
	// �A�j���[�V�����ԍ��̐ݒ�֐�.
	int SetAnimNumber(const EObjectTag& tag);

private:
	std::vector<SIconInfo>	m_IconList;			//�A�C�R�����X�g,
	size_t					m_ObjPosListCount;	//�I�u�W�F�N�g�̃J�E���g.

};

#endif	//#ifndef MINIMAP_H.
