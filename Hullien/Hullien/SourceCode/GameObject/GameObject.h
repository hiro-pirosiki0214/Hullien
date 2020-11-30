#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "..\Global.h"
#include "..\Common\Shader\ShadowMap\ShadowMap.h"
#include "ObjectList.h"

/****************************************
*	�Q�[���I�u�W�F�N�g���N���X.
**/
class CGameObject
{
	// �E�B���h�E�O�̒����l.
	inline static const float WND_OUT_ADJ_SIZE = 300.0f;
public:
	CGameObject();
	virtual ~CGameObject();

	// �������֐�.
	virtual bool Init() = 0;
	// �X�V�֐�.
	virtual void Update() = 0;
	// �`��֐�.
	virtual void Render() = 0;

	// �I�u�W�F�N�g�^�O�擾�֐�.
	inline EObjectTag GetObjectTag() const { return m_ObjectTag; }
	// ���W�擾�֐�.
	inline D3DXVECTOR3 GetPosition() const { return m_vPosition; }
	// ���W�ݒ�֐�.
	virtual void SetPosition( const D3DXVECTOR3& vPos ){ m_vPosition = vPos; }
	// Y���̉�]�l�̎擾.
	inline float GetRotatinY() const { return m_vRotation.y; }
	// X���̉�]�l�̐ݒ�.
	inline void SetRotationX( const float& rotX ){ m_vRotation.x = rotX; }
	// Y���̉�]�l�̐ݒ�.
	inline void SetRotationY( const float& rotY ){ m_vRotation.y = rotY; }
	// Z���̉�]�l�̐ݒ�.
	inline void SetRotationZ( const float& rotZ ){ m_vRotation.z = rotZ; }
	// �T�C�Y�̐ݒ�.
	inline void SetScale( const float& size ){ m_vScale = { size, size, size }; }
	// �T�C�Y�̐ݒ�.
	inline void SetScale( const D3DXVECTOR3& vScale ){ m_vScale = vScale; }

	// ��ʂ̊O�ɏo�Ă��邩.
	bool IsDisplayOut( const float& adjSize = WND_OUT_ADJ_SIZE );

protected:
	EObjectTag	m_ObjectTag;	// �I�u�W�F�N�g�^�O.
	D3DXVECTOR3 m_vPosition;	// ���W.
	D3DXVECTOR3 m_vRotation;	// ��].
	D3DXVECTOR3 m_vScale;		// �傫��.
};

#endif	// #ifndef GAME_OBJECT_H.