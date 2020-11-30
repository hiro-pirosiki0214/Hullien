#ifndef INVISIBLE_WALL_H
#define INVISIBLE_WALL_H

#include "..\GameObject.h"
#include "BoxStruct.h"

class CCollisionManager;	// �����蔻��N���X.

/**************************************
*	�����Ȃ���.
*/
class CInvisibleWall : public CGameObject
{
	const float HALF = 0.5f;	// ����.
	const char* INVISIBLE_WALL_PARAM_FILE_NAME = "Data\\GameParam\\InvisibleWall.bin";
public:
	CInvisibleWall();
	virtual ~CInvisibleWall();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

	// �{�b�N�X�̐ݒ�.
	void SetBoxWall( const SBoxWall& boxWall );
	// �{�b�N�X�̎擾.
	inline SBoxWall* GetBoxWall() { return &m_BoxWall; }

private:
	// �p�����[�^�̏�����.
	bool InitParam();
	// �����蔻��̏�����.
	bool InitCollision();

private:
	SBoxWall	m_BoxWall;
#ifdef _DEBUG
	std::unique_ptr<CCollisionManager>	m_pCollision;
#endif	// #ifdef _DEBUG
};

#endif	// #ifndef INVISIBLE_WALL_H.