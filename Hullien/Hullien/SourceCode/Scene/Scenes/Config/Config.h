#ifndef CONFIG_H
#define CONFIG_H

#include "..\..\SceneBase\SceneBase.h"

class CConfigWidget;	// �ݒ�UI�N���X.

/**********************************
*	�ݒ�V�[���N���X.
*/
class CConfig : public CSceneBase
{
public:
	CConfig( CSceneManager* pSceneManager );
	virtual ~CConfig();

	// �Ǎ��֐�.
	virtual bool Load() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

private:
	// �V�[���؂�ւ��֐�.
	void ChangeScene();

private:
	std::unique_ptr<CConfigWidget>	m_pConfigWidget;	// �ݒ�UI�N���X.
	bool	m_IsChangeScene;
};

#endif	// #ifndef CONFIG_H.