#ifndef LIMITTIME_H
#define LIMITTIME_H

#include "..\..\SceneWidget.h"
#include <vector>

/*****************************************
*	�������ԃN���X.
**/
class CLimitTime : public CSceneWidget
{
private:
	const char* SPRITE_TIME				= "timeNumber";		//���Ԃ̃X�v���C�g��.
	const char* SPRITE_TIME_BACK		= "timeBack";		//���Ԃ̔w�i�X�v���C�g��.
	const char* SPRITE_TIME_BACK_MINI	= "timeBackMini";	//���Ԃ̃~�j�w�i�X�v���C�g��.
	const D3DXVECTOR3 ONETIME_INITPOS	= D3DXVECTOR3(655.0f, 80.0f, 0.0f);	//���Ԉ�̈ʏ����ʒu.
	const D3DXVECTOR3 TENTIME_INITPOS	= D3DXVECTOR3(625.0f, 80.0f, 0.0f);	//���ԏ\�̈ʏ����ʒu.
	const D3DXVECTOR3 MINIBACK_INITPOS	= D3DXVECTOR3(640, 40.0f, 0.0f);	//�~�~�w�i�̏����ʒu.
	const float MINIBACK_MAX_WIDTH		= 50.0f;
	const float MINIBACK_COUNT_ADD		= 0.005f;
	const int TIME_BACK_NUM				= 2;	// �w�i�̔z��ԍ�.
	const int LIMITTIME_MAX				= 60;	//�������ԍő�.
	const int FRAME_DIVISION			= 100;//�t���[�����鐔.
	const int ANIM_MAX					= 10;	//�A�j���[�V�����ő吔.
	const int TEN						= 10;	//�\.

public:
	CLimitTime();
	virtual ~CLimitTime();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

	// �I���̎擾�֐�.
	inline bool IsFinish() const { return m_IsFinish; }	

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;
	// �ʒu�ݒ�֐�.
	bool PositionSetting();
	// �A�j���[�V�����ԍ��ݒ�֐�.
	bool AnimSetting();
	// �J�E���g�_�E���֐�.
	void CountDown();
	// �t���[������b�ɕϊ��֐�.
	void FramesToSeconds();
	// �~�j�w�i�̍��W�X�V.
	void MiniBackPositionUpdate();

private:
	std::vector<std::shared_ptr<CSprite>>	m_pTimeSprites;		// �X�v���C�g�N���X.
	std::vector<std::shared_ptr<CSprite>>	m_pMiniBackSprites;		// �X�v���C�g�N���X.
	std::vector<D3DXVECTOR3>				m_vTimePosition;	// �ʒu.
	std::vector<D3DXVECTOR3>				m_vMiniBackPosition;	// �ʒu.
	std::vector<float>						m_vMiniBackScale;	// �ʒu.
	std::vector<int>						m_Anim;			// �A�j���[�V�����ԍ�.
	int										m_FrameCount;	// �t���[���J�E���g.
	int										m_Seconds;		// �b.
	bool									m_IsFinish;		// �I���������ǂ���.
	float	m_RotationCount;
};

#endif	//#ifndef LIMITTIME_H.