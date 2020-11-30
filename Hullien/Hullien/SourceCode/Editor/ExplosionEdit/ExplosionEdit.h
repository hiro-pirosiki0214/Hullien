#ifndef EXPLOSION_EDIT_H
#define EXPLOSION_EDIT_H

#include "..\EditBase.h"
#include "..\..\GameObject\Actor\Explosion\Explosion.h"
#include <vector>

class CExplosionEdit : public CEditBase
{
	const ImVec2 WINDOW_SIZE = { 510.0f, WND_H-70.0f };	// �E�B���h�E�T�C�Y.
	const ImVec2 RENDER_POSITION = { 0.0f, 70.0f };		// �`����W.
	const char* FILE_PATH = "Data\\GameParam\\Alien\\Explosion\\Explosion.bin";

public:
	CExplosionEdit();
	~CExplosionEdit();

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

private:
	std::unique_ptr<CExplosion>	m_pExplosion;		// ����.
	CExplosion::SExplosionParam m_ExplosionParam;	// �����p�����[�^�[.
};

#endif	// #ifndef EXPLOSION_EDIT_H.