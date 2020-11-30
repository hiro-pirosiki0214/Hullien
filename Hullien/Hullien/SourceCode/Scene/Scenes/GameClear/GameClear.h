#ifndef GAME_CLEAR_H
#define GAME_CLEAR_H

#include "..\..\SceneBase\SceneBase.h"

class CClearWidget;	// �N���AUI�N���X.
class CSTGManager;	// �V���[�e�B���O�Ǘ��N���X.

/**********************************
*	�Q�[���N���A�N���X.
*/
class CGameClear : public CSceneBase
{
	const int   SKIP_WAIT_COUNT = 100;
public:
	CGameClear( CSceneManager* pSceneManager );
	virtual ~CGameClear();

	// �Ǎ��֐�.
	virtual bool Load() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

private:
	// ���f���̕`��.
	void ModelRender();
	// �V�[����؂�ւ���.
	void OnChangeScene( const bool& isPlaySE );
	// �V�[���؂�ւ��֐�.
	void ChangeScene();
	// �X�L�b�v�̍X�V�֐�.
	void SkipUpdate();

private:
	std::unique_ptr<CClearWidget>	m_pClearWidget;	// �N���AUI�N���X.
	std::unique_ptr<CSTGManager>	m_pSTGManager;	// �V���[�e�B���O�Ǘ��N���X.
	bool m_IsChangeScene;							//�V�[���؂�ւ��\��.
	int	m_SkipWaitCount;
};

#endif	// #ifndef GAME_CLEAR_H.