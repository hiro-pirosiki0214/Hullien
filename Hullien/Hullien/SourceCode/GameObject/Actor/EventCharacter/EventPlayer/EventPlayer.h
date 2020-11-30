#ifndef EVENT_PLAYER_H
#define EVENT_PLAYER_H

#include "..\EventCharacter.h"
#include "..\..\Character\Player\PlayerParam.h"
#include <vector>

class CEffectManager;

/********************************
*	�C�x���g�p�v���C���[�N���X.
**/
class CEventPlayer : public CEventCharacter
{
	// �p�����[�^�̃t�@�C���p�X.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Player\\Player.bin";
	const char*	MODEL_NAME = "kaito_s";	// ���f����.
	const char* MODEL_TEMP_NAME = "kaito";	// �����f����.

	// ���̃{�[����.
	const char* RIGHT_FOOT = "kaito_rifa_2_R_asi_4";
	const char* LEFT_FOOT = "kaito_rifa_2_L_asi_4";

	// ���̉���.
	const float VOICE_VOLUME = 1.5f;

	// �A�j���[�V�����̒����t���[��.
	const double ANIM_ADJ_FRAME_Wait = 0.0;	// �ҋ@.
	const double ANIM_ADJ_FRAME_Walk = 0.0;	// ����.
	const double ANIM_ADJ_FRAME_Attack1 = 0.5;	// �U��1.
	const double ANIM_ADJ_FRAME_Attack2 = 0.5;	// �U��2.
	const double ANIM_ADJ_FRAME_Attack3 = 0.5;	// �U��3.
	const double ANIM_ADJ_FRAME_Avoid = 0.4;	// ���.
	const double ANIM_ADJ_FRAME_SP = 0.02;		// ����\��.
	const double ANIM_ADJ_FRAME_Damage = 0.4;	// �q�b�g��.
	const double ANIM_ADJ_FRAME_Dead = 0.01;	// ���S.

public:
	enum class enPlayerState
	{
		None = 0,

		Move,			//�ړ�.
		Wait,			//�ҋ@.
		SpecialAbility,	//����\��.
	} typedef EPlayerState;

public:
	CEventPlayer();
	virtual ~CEventPlayer();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��֐�.
	virtual void Collision(CActor* pActor) override;
	// ������W�̐ݒ�֐�.
	virtual void SetTargetPos(CActor& actor) override;
	// �A�j���[�V�������x�̐ݒ�.
	inline void SetAnimSpeed() { m_AnimSpeed = 0.01f; }

	// Y�{�^���������ꂽ��.
	inline bool IsYButtonPressed() { return m_IsYButtonPressed; }
	// ����\��.
	bool IsSpecialAbility();

	// �Փ˂�����.
	inline bool IsAlienHit(){ return m_IsAlienHit; }
	// �Փ�.
	inline void SetAlienHit(){ m_IsAlienHit = true; }
	// �_���[�W�A�j���[�V�������I��������.
	inline bool IsEndDamageAnim(){ return m_AnimFrameList[player::EAnimNo_Damage].IsNowFrameOver(); }
	// �_���[�W�A�j���[�V�����̍X�V.
	void DamageAnimUpdate();
	// �{�[���ʒu�̎擾.
	D3DXVECTOR3 GetBonePosition(const char* name);
private:
	// ����\�͑���֐�.
	void SPController();
	// �m�b�N�o�b�N����֐�.
	void KnockBack();

	// �ړ��֐�.
	virtual void Move() override;
	// �G�t�F�N�g�`��֐�.
	void EffectRender();

	// �����蔻��̐ݒ�.
	bool ColliderSetting();
	// �G�t�F�N�g�̐ݒ�.
	bool EffectSetting();
	// �A�j���[�V�����t���[���̐ݒ�.
	bool SetAnimFrameList();

	// �T�E���h�̐ݒ�.
	bool SoundSetting();
	// ���ʂ̐ݒ�.
	void VolumeSetting(const char* soung, float volume);

private:
	anim::AAnimFrameList							m_AnimFrameList;	// �A�j���[�V�����t���[���̃��X�g.
	player::EAnimNo									m_NowAnimNo;		// ���̃A�j���[�V�����ԍ�.
	player::EAnimNo									m_OldAnimNo;		// �O�̃A�j���[�V�����ԍ�.
	EPlayerState									m_State;			// ���.
	std::vector<std::shared_ptr<CEffectManager>>	m_pEffects;			// �G�t�F�N�g.
	float											m_SpecialAbility;	// ����\��.
	bool											m_IsYButtonPressed;	// Y�{�^���������ꂽ��.
	bool											m_HasUsableSP;		// ����\�͂��g���邩.
	bool											m_IsAlienHit;		// �F���l�Ɠ���������.
	bool											m_IsAttackSE;		// �U��SE��炷��.
};



#endif // #ifndef EVENT_PLAYER_H.
