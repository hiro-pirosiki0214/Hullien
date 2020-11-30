#ifndef TITLE_H
#define TITLE_H

#include "..\..\SceneBase\SceneBase.h"

#include "..\..\..\Common\Sprite\CSprite.h"

#include "..\..\..\Collider\Colliders\Capsule\CapsuleModel.h"
#include "..\..\..\Collider\Colliders\Capsule\CapsuleCollision.h"

class CRotLookAtCenter;
class CTitleWidget;
class CConfigWidget;

/**********************************
*	�^�C�g���V�[���N���X.
*/
class CTitle : public CSceneBase
{
private:
	const float ALPHA_SPEED = 0.01f;

public:
	CTitle(CSceneManager* pSceneManager);
	virtual ~CTitle();

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
	std::unique_ptr<CTitleWidget>	m_pWidget;			// UI�N���X.
	bool							m_IsChangeScene;	// �V�[���̐؂�ւ����\��.
	bool							m_IsDecision;		// ���肵����.
};

#endif	// #ifndef TITLE_H.