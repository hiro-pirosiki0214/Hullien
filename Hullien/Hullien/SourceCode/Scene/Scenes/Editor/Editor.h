#ifndef EDITOR_H
#define EDITOR_H

#include "..\..\SceneBase\SceneBase.h"

class CControllerEdit;
class CSpawnEdit;
class CAlienParamEdit;
class CExplosionEdit;
class CItemEdit;
class CPeraRenderer;
class CSkyDome;
class CGroundStage;

class CEditor : public CSceneBase
{
	enum enEditScenes
	{
		None,

		SpawnEdit,
		AlienParam,
		Explosion,
		ItemEdit,

		Max

	} typedef EEditScenes;
public:
	CEditor( CSceneManager* pSceneManager );
	virtual ~CEditor();

	// 読込関数.
	virtual bool Load() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

private:
	// モデルの描画.
	void ModelRender();
	// UIの表示.
	void ImGuiRender();

private:
	std::unique_ptr<CControllerEdit>	m_pControllerEdit;
	std::unique_ptr<CSpawnEdit>			m_pSpawnEdit;
	std::unique_ptr<CAlienParamEdit>	m_pAlienParamEdit;
	std::unique_ptr<CExplosionEdit>		m_pExplosionEdit;
	std::unique_ptr<CItemEdit>			m_pItemEdit;

	std::unique_ptr<CPeraRenderer>		m_pPeraRenderer;
	std::unique_ptr<CSkyDome>			m_pSkyDome;
	std::unique_ptr<CGroundStage>		m_pGroundStage;

	int m_NowEditScene;
};

#endif	// #ifndef EDITOR_H.