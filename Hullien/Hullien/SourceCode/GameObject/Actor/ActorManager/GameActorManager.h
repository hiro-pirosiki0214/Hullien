#ifndef CHARACTER_MANAGER_H
#define CHARACTER_MANAGER_H

#include "..\..\..\Global.h"
#include "..\..\ObjectList.h"

#include <vector>

class CActor;
class CGroundStage;		// �n�ʃN���X.
class CPlayer;					// �v���C���[�N���X.
class CGirl;						// ���̎q�N���X.
class CAlienManager;		// �F���l�Ǘ��N���X.
class CItemManager;		// �A�C�e���Ǘ��N���X.
class CBarrier;				// �o���A�N���X.

class CGameActorManager
{
public:
	CGameActorManager();
	~CGameActorManager();

	// �������֐�.
	bool Init();
	// �X�V�֐�.
	void Update();
	// �`��֐�.
	void Render();
	// �I�u�W�F�N�g���W���X�g�̎擾.
	std::vector<std::pair<EObjectTag, D3DXVECTOR3>>	GetObjPositionList() const { return m_ObjPositionList; }

private:
	// ���W���X�g�̐ݒ�.
	void SetPositionList( CActor* pActor );

private:
	std::shared_ptr<CGroundStage>	m_pGroundStage;
	std::shared_ptr<CPlayer>		m_pPlayer;
	std::shared_ptr<CGirl>			m_pGirl;
	std::shared_ptr<CAlienManager>	m_pAlienManager;
	std::shared_ptr<CItemManager>	m_pItemManager;
	std::shared_ptr<CBarrier>		m_pBarrier;
	std::vector<std::pair<EObjectTag, D3DXVECTOR3>>	m_ObjPositionList;
	int m_ObjPosListCount;
};

#endif	// #ifndef CHARACTER_MANAGER_H.