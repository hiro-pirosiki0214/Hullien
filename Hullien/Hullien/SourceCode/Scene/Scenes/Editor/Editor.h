#ifndef EDITOR_H
#define EDITOR_H

#include "..\..\SceneBase\SceneBase.h"

class CControllerEdit;
class CSpawnEdit;

class CEditor : public CSceneBase
{
public:
	CEditor( CSceneManager* pSceneManager );
	virtual ~CEditor();

	// “ÇŠÖ”.
	virtual bool Load() override;
	// XVŠÖ”.
	virtual void Update() override;
	// •`‰æŠÖ”.
	virtual void Render() override;

private:
	std::unique_ptr<CControllerEdit> m_pControllerEdit;
	std::unique_ptr<CSpawnEdit>	m_pSpawnEdit;
};

#endif	// #ifndef EDITOR_H.