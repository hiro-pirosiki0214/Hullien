#ifndef EDITOR_H
#define EDITOR_H

#include "..\..\SceneBase\SceneBase.h"

#include <vector>

class CControllerEdit;
class CSpawnEdit;
class CAlienParamEdit;
class CExplosionEdit;
class CItemEdit;
class CPlayerEdit;
class CGirlEdit;
class CMotherShipUFOEdit;
class CInvisibleWallEdit;
class CEditCamera;
class CSkyDome;
class CGroundStage;
class CEditBase;

class CEditor : public CSceneBase
{
	enum enEditScenes
	{
		EEditScenes_None,

		EEditScenes_PlayerEdit,
		EEditScenes_GirlEdit,
		EEditScenes_SpawnEdit,
		EEditScenes_AlienParam,
		EEditScenes_Explosion,
		EEditScenes_ItemEdit,
		EEditScenes_MotherShipUFOEdit,
		EEditScenes_InvisibleWallEdit,
		EEditScenes_EditEnd,

		EEditScenes_Begin = EEditScenes_None,
		EEditScenes_End = EEditScenes_EditEnd,

		EEditScenes_Max,

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
	// エディットリストの作成.
	bool CreateEditList();
	// シーン切り替え関数.
	void ChangeScene();

private:
	std::shared_ptr<CEditCamera>		m_pEditCamera;
	std::unique_ptr<CSkyDome>			m_pSkyDome;
	std::unique_ptr<CGroundStage>		m_pGroundStage;
	std::vector<std::unique_ptr<CEditBase>>	m_pEdit;
	bool m_IsEditEnd;
	int m_NowEditScene;
	bool m_IsChangeScene;
};

#endif	// #ifndef EDITOR_H.