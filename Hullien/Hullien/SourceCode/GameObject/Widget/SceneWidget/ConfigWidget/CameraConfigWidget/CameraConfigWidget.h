#ifndef CAMERA_CONFIG_WIDGET_H
#define CAMERA_CONFIG_WIDGET_H

#include "..\..\..\Widget.h"
#include "..\..\..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include <vector>

class CCursor;
class CSlinder;

class CCameraConfigWidget : public CWidget
{
	const char* SPRITE_CONTROL_NAME		= "CmaeraControl";
	const char* SPRITE_SPEED_NAME		= "CmaeraSpeed";
	const char* SPRITE_CHOICE_NAME		= "Choice";
	const char* SPRITE_NORMAL_NAME		= "CameraNormal";
	const char* SPRITE_INVERSION_NAME	= "CameraInversion";
	const char* SPRITE_ICON_NAME		= "SelectIcon";
	const float INPUT_WAIT_TIME_MAX		= 30.0;
	// �I�����.
	enum enSelectState
	{
		ESelectState_None,

		ESelectState_Select,			// �I�����.
		ESelectState_CameraControl,		// ������.
		ESelectState_CameraSpeed,		// ���x���.

		ESelectState_Max,
	} typedef ESelectState;

	enum enCameraControlState
	{
		ECameraControlState_None,

		ECameraControlState_Normal,		// �m�[�}��.
		ECameraControlState_Inversion,	// ���].

		ECameraControlState_Max,
	} typedef ECameraControlState;

	enum enSpriteNo
	{
		ESpriteNo_None = -1,

		ESpriteNo_Control,		// ����.
		ESpriteNo_Speed,		// ���x.
		ESpriteNo_Choise,		// �I��.
		ESpriteNo_Normal,		// �ʏ�.
		ESpriteNo_Inversoin,	// ���].
		ESpriteNo_Icon,			// �A�C�R��.

		ESpriteNo_Max,
	} typedef ESpriteNo;

public:
	CCameraConfigWidget();
	virtual ~CCameraConfigWidget();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

	// �I��������.
	bool IsEnd();

private:
	// ��ނ̑I��.
	void SelectType();
	// ����.
	void Determination();
	// �J��������̍X�V.
	void CameraControlUpdate();
	// �J�������x�̍X�V.
	void CameraSpeedUpdate();
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;	// �摜.
	std::unique_ptr<CCursor>				m_pCursor;
	std::unique_ptr<CSlinder>				m_pSpeedSlinder;
	CRotLookAtCenter::SConfigState			m_ConfigState;
	D3DXVECTOR3								m_SlinderPosition;
	bool	m_IsOlsReverse;
	float	m_OldMoveSpeed;
	int		m_NowSelectState;
	int		m_OldNowSelectState;
	int		m_NowConfigState;
	int		m_NowControlState;
	float	m_InputWaitTime;
};

#endif	// #ifndef CAMERA_CONFIG_WIDGET_H.