#ifndef SKY_DOME_H
#define SKY_DOME_H

#include "..\GameObject.h"

class CSkyDomeModel;

class CSkyDome : public CGameObject
{
	const char* MODEL_NAME = "Data\\Mesh\\space\\space.x";	// ���f����.

public:
	CSkyDome();
	virtual ~CSkyDome();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

private:
	std::unique_ptr<CSkyDomeModel>	m_pDomeModel;	// ���b�V��.
};

#endif	// #ifndef SKY_DOME_H.