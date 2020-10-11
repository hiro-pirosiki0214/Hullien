#ifndef MOTHER_SHIP_UFO_H
#define MOTHER_SHIP_UFO_H

#include "..\GameObject.h"

class CActor;
class CCollisionManager;	// �����蔻��N���X.

class CMotherShipUFO : public CGameObject
{
public:
	// �}�U�[�V�b�v�̃p�����[�^.
	struct stMotherShipUFOParam
	{
		D3DXVECTOR3 Position;			// ���W.
		float		AddPosYPower;		// �����グ���.
		D3DXVECTOR3 SphereAdjPos;		// �X�t�B�A�̒������W.
		float		CollisionRadius;	// �����蔻��̔��a.

		stMotherShipUFOParam()
			: Position			( 0.0f, 0.0f, 0.0f )
			, AddPosYPower		( 0.0f )
			, SphereAdjPos		( 0.0f, 0.0f, 0.0f )
			, CollisionRadius	( 1.0f )
		{}

	} typedef SMotherShipUFOParam;

private:
	const char* MODEL_NAME = "ufo";	// ���f����.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Alien\\MotherShipUFOParam\\MotherShipUFOParam.bin";
//	const SMotherShipUFOParam*	pPRAMETER;	// �p�����[�^(�O������|�C���^�Ŏ󂯎��).

public:
	CMotherShipUFO();
	virtual ~CMotherShipUFO();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��.
	void Collision( CActor* pActor );
	// �p�����[�^�̐ݒ�.
	void SetParameter( const SMotherShipUFOParam& param );
	// �`��t���O�ݒ�֐�.
	void SetDisp(const bool& disp) { m_IsDisp = disp; }

private:
	// ���f���̎擾.
	bool GetModel();
	// �����蔻��̐ݒ�.
	bool CollisionSetting();

private:
	std::shared_ptr<CDX9StaticMesh>		m_pStaticMesh;	// �X�^�e�B�b�N���b�V��.
	std::shared_ptr<CCollisionManager>	m_pCollManager;	// �����蔻��N���X.
	SMotherShipUFOParam					m_Param;
	bool								m_IsDisp;		//�`�悵�Ă��邩.
};

#endif	// #ifndef MOTHER_SHIP_UFO_H.