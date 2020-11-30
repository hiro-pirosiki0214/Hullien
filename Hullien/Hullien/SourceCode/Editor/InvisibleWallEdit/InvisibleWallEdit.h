#ifndef INVISIBLE_WALL_EDIT_H
#define INVISIBLE_WALL_EDIT_H

#include "..\EditBase.h"
#include "..\..\GameObject\InvisibleWall\BoxStruct.h"

class CInvisibleWall;	// �����Ȃ���.

class CInvisibleWallEdit : public CEditBase
{
	const char* INVISIBLE_WALL_PARAM_FILE_NAME = "Data\\GameParam\\InvisibleWall.bin";
public:
	CInvisibleWallEdit();
	virtual ~CInvisibleWallEdit();

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
	std::unique_ptr<CInvisibleWall>	m_pInvisibleWall;	// �����Ȃ���.
	SBoxWall	m_BoxWall;
};

#endif	// #ifndef INVISIBLE_WALL_EDIT_H.