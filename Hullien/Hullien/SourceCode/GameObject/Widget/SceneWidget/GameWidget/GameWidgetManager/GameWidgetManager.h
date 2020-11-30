#ifndef GAMEWIDGETMANAGER_H
#define GAMEWIDGETMANAGER_H

#include "..\..\..\..\..\Global.h"

class CGameActorManager;	// �Q�[���I�u�W�F�N�g�Ǘ��N���X.
class CLimitTime;				//�������ԃN���X.
class CMiniMap;					//�~�j�}�b�v�N���X.
class CUltemateSing;			//�A���e�B���b�g�o���T�C���N���X.

/******************************************
*	�Q�[��UI�Ǘ��N���X.
**/
class CGameWidgetManager
{
public:
	CGameWidgetManager();
	virtual ~CGameWidgetManager();

	// �������֐�,
	bool Init();
	// �X�V�֐�.
	void Update(CGameActorManager* pActorManager);
	// �`��֐�.
	void Render();
	// �Q�[�����I��������.
	bool IsGameFinish();

private:
	std::unique_ptr<CLimitTime>	m_pLimitTime;
	std::unique_ptr<CMiniMap>	m_pMiniMap;
	// �X�|�[��UFO�N���X�Ɏ����čs����.
//	std::unique_ptr<CUltemateSing> m_pUltemateSing;
	
};

#endif	//#ifndef GAMEWIDGETMANAGER_H.
