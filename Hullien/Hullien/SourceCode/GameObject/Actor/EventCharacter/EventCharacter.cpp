#include "EventCharacter.h"
#include "..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"

/***************************************
*	�C�x���g�p�L�����N�^�N���X.
**/
CEventCharacter::CEventCharacter()
	: m_pSkinMesh( nullptr )
#ifdef IS_TEMP_MODEL_RENDER
	, m_pTempStaticMesh( nullptr )
#endif
	, m_Parameter					()
	, m_NowMoveState				( EMoveState::None )
{
	m_vPosition.y = INIT_POSITION_ADJ_HEIGHT;
}

CEventCharacter::~CEventCharacter()
{
}

// ���ݒ�֐�.
void CEventCharacter::SetOptionalState(SOptionalState state)
{
	m_vPosition = state.vPosition;
	m_vRotation = state.vRotation;
	m_vSclae = state.vScale;
	m_Parameter.ModelAlpha = state.ModelAlpha;
	m_Parameter.MoveSpeed = state.MoveSpeed;
	m_Parameter.RotationalSpeed = state.RotationalSpeed;
	m_Parameter.ScaleSpeed = state.ScaleSpeed;
	m_Parameter.AlphaSpeed = state.AlphaSpeed;
	m_Parameter.IsDisp = state.IsDisp;
}

// ���b�V���̕\���֐�.
void CEventCharacter::MeshRender()
{
#ifndef IS_TEMP_MODEL_RENDER
	if (m_pSkinMesh == nullptr) return;

	m_pSkinMesh->SetPosition(m_vPosition);
	m_pSkinMesh->SetRotation(m_vRotation);
	m_pSkinMesh->SetScale(m_vSclae);
	m_pSkinMesh->SetAnimSpeed(0.01);
	m_pSkinMesh->Render();
#else
	if (m_pTempStaticMesh == nullptr) return;

	m_pTempStaticMesh->SetPosition(m_vPosition);
	m_pTempStaticMesh->SetRotation(m_vRotation);
	m_pTempStaticMesh->SetScale(m_vSclae);
	m_pTempStaticMesh->Render();
#endif	// #ifdef IS_TEMP_MODEL_RENDER.
}

// ���f���̎擾�֐�
bool CEventCharacter::GetModel(const char * modelName)
{
#ifndef IS_TEMP_MODEL_RENDER
	// ���ɓǂݍ��߂Ă�����I��.
	if (m_pSkinMesh != nullptr) return true;
	// ���f���̎擾.
	CMeshResorce::GetSkin(m_pSkinMesh, modelName);
	// ���f�����ǂݍ��߂ĂȂ���� false.
	if (m_pSkinMesh == nullptr) return false;
	return true;
#else
	// ���ɓǂݍ��߂Ă�����I��.
	if (m_pTempStaticMesh != nullptr) return true;
	// ���f���̎擾.
	CMeshResorce::GetStatic(m_pTempStaticMesh, modelName);
	// ���f�����ǂݍ��߂ĂȂ���� false.
	if (m_pTempStaticMesh == nullptr) return false;
	return true;
#endif	// #ifndef IS_MODEL_RENDER.
}

/******************************************************
*	�ȉ����擾�E�ݒ�֐�.
*/

// X���W�ʒu�ݒ�֐�.
void CEventCharacter::SetPositionX(const float& vPos_x)
{
	m_vPosition.x = vPos_x;
}

// Y���W�ʒu�ݒ�֐�.
void CEventCharacter::SetPositionY(const float& vPos_y)
{
	m_vPosition.y = vPos_y;
}

// Z���W�ʒu�ݒ�֐�.
void CEventCharacter::SetPositionZ(const float& vPos_z)
{
	m_vPosition.z = vPos_z;
}

// ��]�l�擾�֐�.
D3DXVECTOR3 CEventCharacter::GetRotation() const
{
	return m_vRotation;
}

// ��]�l�ݒ�֐�.
void CEventCharacter::SetRotation(const D3DXVECTOR3& vRot)
{
	m_vRotation = vRot;
}

// X���W��]�l�ݒ�֐�.
void CEventCharacter::SetRotationX(const float& vRot_x)
{
	m_vRotation.x = vRot_x;
}

// Y���W��]�l�ݒ�֐�.
void CEventCharacter::SetRotationY(const float& vRot_y)
{
	m_vRotation.y = vRot_y;
}

// Z���W��]�l�ݒ�֐�.
void CEventCharacter::SetRotationZ(const float& vRot_z)
{
	m_vRotation.z = vRot_z;
}

// �傫���擾�֐�.
D3DXVECTOR3 CEventCharacter::GetScale() const
{
	return m_vSclae;
}

// �傫���ݒ�֐�.
void CEventCharacter::SetScale(const D3DXVECTOR3& vScale)
{
	m_vSclae = vScale;
}

// X���W�傫���ݒ�֐�.
void CEventCharacter::SetScaleX(const float & vScale_x)
{
	m_vSclae.x = vScale_x;
}

// Y���W�傫���ݒ�֐�.
void CEventCharacter::SetScaleY(const float & vScale_y)
{
	m_vSclae.y = vScale_y;
}

// Z���W�傫���ݒ�֐�.
void CEventCharacter::SetScaleZ(const float & vScale_z)
{
	m_vSclae.z = vScale_z;
}

float CEventCharacter::RotationMoveRight(const float& rotValue, const float& rotSpeed, bool IsRightRot)
{
	if (IsRightRot == true) 
	{
		if (m_vRotation.y >= rotValue) m_vRotation.y = rotValue;
	}
	else
	{
		if (m_vRotation.y <= rotValue) m_vRotation.y = rotValue;
	}

	if( m_vRotation.y == rotValue ) return rotValue;
	m_vRotation.y += static_cast<float>(D3DX_PI) * rotSpeed;

	return m_vRotation.y;
}