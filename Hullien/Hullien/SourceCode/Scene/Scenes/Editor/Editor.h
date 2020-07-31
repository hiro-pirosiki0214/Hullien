#ifndef EDITOR_H
#define EDITOR_H

#include "..\..\SceneBase\SceneBase.h"

class CControllerEdit;
class CSpawnEdit;
class CAlienParamEdit;
class CExplosionEdit;
class CItemEdit;

class CEditor : public CSceneBase
{
public:
	CEditor( CSceneManager* pSceneManager );
	virtual ~CEditor();

	// ì«çûä÷êî.
	virtual bool Load() override;
	// çXêVä÷êî.
	virtual void Update() override;
	// ï`âÊä÷êî.
	virtual void Render() override;

private:
	std::unique_ptr<CControllerEdit>	m_pControllerEdit;
	std::unique_ptr<CSpawnEdit>			m_pSpawnEdit;
	std::unique_ptr<CAlienParamEdit>	m_pAlienParamEdit;
	std::unique_ptr<CExplosionEdit>		m_pExplosionEdit;
	std::unique_ptr<CItemEdit>			m_pItemEdit;
};

#endif	// #ifndef EDITOR_H.