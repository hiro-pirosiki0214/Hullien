#ifndef VOLUME_CONFIG_WIDGET_H
#define VOLUME_CONFIG_WIDGET_H

#include "..\..\..\Widget.h"
#include <vector>

class CSlinder;	// �X���C�_�[.
class CCursor;	// �J�[�\��.

/********************************************
*	���ʐݒ�UI�N���X.
**/
class CVolumeConfigWidget : public CWidget
{
	const char* SPRITE_MASTER_NAME	= "MasterVolume";
	const char* SPRITE_BGM_NAME		= "BGMVolume";
	const char* SPRITE_SE_NAME		= "SEVolume";
	const char* SPRITE_ICON_NAME	= "SelectIcon";
	const char* SPRITE_RESET		= "ConfigReset";
	const char* SPRITE_SAVE			= "ConfigSave";

	const float BAR_POSITION_X = 200.0f;
	const float DEFALUT_VOLUME = 1.0f;

	inline static const float INPUT_WAIT_TIME_MAX = 30.0f;	// ���͎��̑ҋ@�t���[��.

	// �ݒ�̏��.
	enum enConfigState
	{
		EConfigState_None,

		EConfigState_Select,	// �I�����.
		EConfigState_Seting,	// �ݒ���.

		EConfigState_Max,
	} typedef EConfigState;

	// �I���̎��.
	enum enSelectType
	{
		ESelectType_None = -1,

		ESelectType_Master,	// �}�X�^�[.
		ESelectType_BGM,	// BGM.
		ESelectType_SE,		// SE.

		ESelectType_Reset,	// ���Z�b�g.
		ESelectType_Save,	// �ۑ�.

		EVolumeType_End,

		ESelectType_Max,

		ESelectType_Begin	= ESelectType_Master,	// �n�܂�.
		ESelectType_End		= ESelectType_Max,		// �I��.
		ESelectType_VolumeMax = ESelectType_SE+1
	} typedef EVolumeType;

	// �X�v���C�g�̎��.
	enum enSpriteNo
	{
		ESpriteNo_None = -1,

		ESpriteNo_Master,		// �}�X�^�[.
		ESpriteNo_BGM,			// BGM.
		ESpriteNo_SE,			// SE.
		ESpriteNo_Reset,		// ���Z�b�g.
		ESpriteNo_Save,			// �ۑ�.
		ESpriteNo_SelectIcon,	// �I�𒆂̃A�C�R��.

		ESpriteNo_Max,
	} typedef ESpriteNo;

public:
	CVolumeConfigWidget();
	virtual ~CVolumeConfigWidget();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

	// ���ʂ̐ݒ���ł���悤�ɂ���.
	void OnVolumeSeting();
	// ���ʂ̐ݒ���ł��Ȃ��悤�ɂ���.
	void OffVolumeSeting();
	// �ۑ��I��.
	bool IsSaveEnd();

private:
	// ��ނ̑I��.
	void SelectType();
	// ����.
	void Determination();
	// ���ʂ̐ݒ�.
	void VolumeSeting();

	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	std::unique_ptr<CCursor>				m_pCursor;			// �J�[�\���N���X.
	std::vector<std::shared_ptr<CSlinder>>	m_pVolumeSlinders;	// ���ʃX���C�_�[.
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;			// �摜.
	std::vector<D3DXVECTOR3>				m_SlinderPositions;	// �X���C�_�[�̍��W.
	D3DXVECTOR3	m_CursorPosition;	// �J�[�\���̍��W.
	int			m_NowConfigState;	// ���݂̐ݒ���.
	int			m_NowSelectVolume;	// ���݂̑I�����Ă��鉹�ʎ��.
	int			m_OldSelectVolume;
	float		m_InputWaitTime;	// ���͂����ۂ̑ҋ@����.
	bool		m_IsOneStep;		// ��񂾂�����.
};

#endif	// #ifndef VOLUME_CONFIG_WIDGET_H.