#ifndef CHARACTER_MANAGER_H
#define CHARACTER_MANAGER_H

#include "..\..\..\Global.h"
#include "..\..\ObjectList.h"

#include <vector>

class CGameObject;
class CSkyDome;			// �w�i�N���X.
class CGroundStage;		// �n�ʃN���X.
class CPlayer;			// �v���C���[�N���X.
class CGirl;			// ���̎q�N���X.
class CMotherShipUFO;	// �}�U�[�V�b�vUFO.
class CAlienManager;	// �F���l�Ǘ��N���X.
class CItemManager;		// �A�C�e���Ǘ��N���X.
class CBarrier;			// �o���A�N���X.
class CWarning;			// �x���N���X.
class CInvisibleWall;	// �����Ȃ���.

class CGameActorManager
{
	const float GIRL_LIMIT_POS_Y = 5.0f;	//���̎q��UFO�ɋz�����܂ꂽ���̌��E�l.
public:
	CGameActorManager();
	~CGameActorManager();

	// �������֐�.
	bool Init();
	// �X�V�֐�.
	void Update();
	// �`��֐�.
	void Render();
	// �X�v���C�g�̕`��.
	void SpriteRender();
	// �I�u�W�F�N�g���W���X�g�̎擾.
	inline std::vector<std::pair<EObjectTag, std::pair<D3DXVECTOR3, float>>>	GetObjPositionList() const { return m_ObjPositionList; }
	// �Q�[���I�[�o�[���ǂ���.
	bool IsGameOver();
	// ���̎q��A�ꋎ���Ă��邩.
	bool IsGirlAbduct();
	// ���̎q���댯�ȏ�Ԃ�.
	bool IsDanger();
	// �F���l���A�҂�����.
	bool IsReturnAlien();

private:
	// ���W���X�g�̐ݒ�.
	void SetPositionList( CGameObject* pObj );
	// �A�j���[�V�������~�߂�.
	void AnimationStop();

private:
	std::unique_ptr<CSkyDome>		m_pSkyDome;			// �w�i.
	std::shared_ptr<CGroundStage>	m_pGroundStage;		// �n��.
	std::shared_ptr<CPlayer>		m_pPlayer;			// �v���C���[.
	std::shared_ptr<CGirl>			m_pGirl;			// ���̎q.
	std::unique_ptr<CMotherShipUFO>	m_pMotherShipUFO;	// �}�U�[�V�b�vUFO.
	std::shared_ptr<CAlienManager>	m_pAlienManager;	// �F���l�}�l�[�W���[.
	std::shared_ptr<CItemManager>	m_pItemManager;		// �A�C�e���}�l�[�W���[.
	std::shared_ptr<CBarrier>		m_pBarrier;			// �o���A.
	std::unique_ptr<CInvisibleWall>	m_pInvisibleWall;	// �����Ȃ���.

	std::vector<std::pair<EObjectTag, std::pair<D3DXVECTOR3, float>>>	m_ObjPositionList;
	int		m_ObjPosListCount;		// �I�u�W�F�N�g�̍��W���X�g�̐�.
	bool	m_IsOllAnimationStop;	// �S���̃A�j���[�V��������~������.
};

#endif	// #ifndef CHARACTER_MANAGER_H.