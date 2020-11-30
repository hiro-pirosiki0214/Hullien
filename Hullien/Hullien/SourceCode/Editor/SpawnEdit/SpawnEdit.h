#ifndef SPAWN_EDIT_H
#define SPAWN_EDIT_H

#include "..\EditBase.h"

class CDX9StaticMesh;
struct stSpawnUFOParam;

class CSpawnEdit : public CEditBase
{
	const char* FILE_PATH = "Data\\GameParam\\Alien\\SpaenParam\\SpaenParam.bin";
	const char* MODEL_NAME = "ufo_mini";
public:
	CSpawnEdit();
	~CSpawnEdit();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// ���f���̕`��.
	virtual void ModelRender() override;
	// �G�t�F�N�g�̕`��.
	virtual void EffectRender() override;

private:
	// �t�@�C���̓ǂݍ���.
	bool FileReading();
	// �t�@�C���̏�������.
	bool FileWriting();
	// �C���f�b�N�X�̕`��.
	void IndexRender();
	// �X�|�[�����̕`��.
	void SpawnParamRender( const int& index );

private:
	std::vector<stSpawnUFOParam> m_SpawnPramList;
	std::shared_ptr<CDX9StaticMesh> m_pStaticMesh;
	bool m_IsSucceeded;
	int m_SucceedRenderCount;
};

#endif	// #ifndef SPAWN_EDIT_H.