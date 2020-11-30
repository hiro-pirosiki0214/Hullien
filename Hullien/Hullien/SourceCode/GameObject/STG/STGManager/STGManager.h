#ifndef STG_MANAGER_H
#define STG_MANAGER_H

#include <memory>

#include "..\..\..\Global.h"

namespace STG
{
	class CPlayer;			// �v���C���[�N���X.
	class CEnemyManager;	// �G�Ǘ��N���X.
}
class CSkyDome;	// �w�i.
class CCamera;	// �J����.

class CSTGManager
{

	const float STG_END_FRAME	= 30*60;	// STG�̏I���t���[��.
	const float SKY_DOME_SCROLL_SPEED		= -0.001f;
	const D3DXVECTOR3 CAMERA_POSITION		= { 0.0f, 150.0f, 10.0f };
	const D3DXVECTOR3 CAMERA_LOOK_POSITION	= { 0.0f, 0.0f, 0.0f };
public:
	CSTGManager();
	~CSTGManager();

	// �������֐�.
	bool Init();
	// �X�V�֐�.
	void Update();
	// �`��֐�.
	void Render();
	// �V���[�e�B���O���I��������.
	bool IsSTGEnd();

private:
	std::shared_ptr<CCamera>			m_pCamera;
	std::unique_ptr<CSkyDome>			m_pSkyDome;			// �w�i.
	std::shared_ptr<STG::CPlayer>		m_pPlayer;			// �v���C���[�N���X.
	std::unique_ptr<STG::CEnemyManager>	m_pEnemyManager;	// �G�Ǘ��N���X.
	float	m_STGEndFrame;	// STG�̏I���t���[��.
	float	m_STGNowFrame;	// STG�̌��݃t���[��.
	float	m_SkyDomeRotX;	// �w�i�̃X�N���[����].
};

#endif	// #ifndef STG_MANAGER_H.