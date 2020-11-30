#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "..\SceneBase\SceneBase.h"
#include "..\..\GameObject\Widget\Fade\Fade.h"

class CCommand;

// �V�[���̎��.
enum class enSceneNo
{
	None,

	Title,		// �^�C�g��.
	GameMain,	// �Q�[�����C��.
	GameClear,	// �Q�[���N���A.
	GameOver,	// �Q�[���I�[�o�[.
	Config,		// �ݒ�.

	Edit,		// �G�f�B�b�g.

	Max,

	ResultCheck,		// �Q�[���N���A���Q�[���I�[�o�[�̊m�F.
	Start = Title,		// ���߂̃V�[��.

} typedef EScene;

/**********************************
*	�V�[���Ǘ��N���X.
*/
class CSceneManager
{	
public:
	CSceneManager();
	~CSceneManager();

	// �X�V�֐�.
	void Update();
	// ���̃V�[���Ɉړ�.
	void NextSceneMove();
	// �Q�[���I��.
	void EndGameClose();
	// �ݒ�V�[���ֈړ�.
	void ConfigSceneMove();
	// �Q�[���I�[�o�[�̐ݒ�.
	inline void OnGameOver(){ m_IsGameOver = true; }
	// �Q�[�����g���C.
	void RetryGame();
	// HWND�ݒ�.
	inline void SethWnd(HWND hWnd) { m_hWnd = hWnd; }
	inline HWND GethWnd(){ return m_hWnd; }

	// ���g���C������.
	inline bool GetRetry() const { return m_IsRetry; }

	// ���ݍĐ�����Ă���BGM�̖��O�̐ݒ�.
	inline void SetNowBGMName( const char* name ){ m_NowBGMName = name; }

	// �G�f�B�b�g�V�[���ɐ؂�ւ���邩.
	inline void OnEditSceneChangeActive(){ m_IsEditSceneChangeActive = true; }
	inline void OffEditSceneChangeActive(){ m_IsEditSceneChangeActive = false; }

private:
	// �G�f�B�b�g�V�[���ɕύX.
	void ChangeEditScene();
	
private:
	HWND m_hWnd;
	std::shared_ptr<CSceneBase> m_pScene;
	std::unique_ptr<CCommand>	m_pCommand;
	std::string					m_NowBGMName;
	EScene	m_NowScene;					// ���݂̃V�[��.
	EScene	m_NextScene;				// ���̃V�[��.
	bool	m_IsLoadEnd;				// ���[�h���I��������.
	bool	m_IsGameOver;				// �Q�[���I�[�o�[��.
	bool	m_IsGameEnd;				// �Q�[�����I��������.
	bool	m_IsRetry;					// ���g���C������.
	bool	m_IsChangeEditScene;		// �G�f�B�b�g�V�[���ɐ؂�ւ��邩.
	bool	m_IsEditSceneChangeActive;	// �G�f�B�b�g�V�[���ɐ؂�ւ���邩.
};

#endif // #ifndef SCENE_MANAGER_H.