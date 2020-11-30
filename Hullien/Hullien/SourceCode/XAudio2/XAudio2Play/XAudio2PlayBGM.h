#ifndef C_XAUDIO2_PLAYBGM_H
#define C_XAUDIO2_PLAYBGM_H


#include <vector>
#include <mutex>
#include "..\XAudio2.h"
#include "..\XAudio2WaveLoad\XAudio2WaveLoad.h"
#include "..\XAudio2Master\XAudio2MasterVoice.h"
#include "..\OggLoad\OggLoad.h"
#include "..\XAudio2CallBack\VoiceCallBack.h"

struct IXAudio2SourceVoice;

class CXAudio2PlayBGM
{
public:
	// �R���X�g���N�^.
	CXAudio2PlayBGM();

	// �f�X�g���N�^.
	virtual ~CXAudio2PlayBGM();

	// �Đ����Ă邩�ǂ����擾�֐�.
	const bool IsPlaying();

	// �T�E���h�Đ��A�O�Ɉꎞ��~����Ă����ꍇ�͋L���ʒu����ĊJ.
	bool Play(bool& isEnd);
	// BGM�T�E���h�Đ����ĊJ.
	bool PlayStart();

	// BGM�T�E���h�����S��~.(�ʒu��ێ����Ȃ�).
	bool Stop();
	// �T�E���h�ꎞ��~.
	bool Pause();

	// �s�b�`�ݒ�֐�.
	bool SetPitch(float value);

	// �s�b�`���擾�֐�.
	const float GetPitch();

	// �ő�s�b�`��ݒ肵�܂�.
	void  SetMaxPitch(float value);
	// �s�b�`��ݒ�.
	void SetPitchValue(const float value) { m_fPitch = value; }

	HRESULT CreateOggSound(std::shared_ptr<COggLoad> pOggData, const char* filename);
	//=======================================================================
	// ��{���ʑ���n.
	//====
	// BGM�̉��ʂ��擾���܂�.
	const float GetVolume();
	// BGM�̉��ʂ�ݒ肵�܂�.
	bool SetBGMVolume(float value);
	// True��BGM�̉��ʂ𒼐ڐG���悤�ɂ���.
	void SetOperationBGMVolumeFlag(const bool& bFlag) { m_bCanChangeVolume = bFlag; }

	// ���݂̉��ʂ�����Z�܂��͌��Z.
	bool  AdjustVolume(float value);
	// BGM���t�F�[�h�A�E�g����֐�.
	bool FadeOutBGM(float value, bool& isEmergencyCall);
	// BGM���t�F�[�h�C������֐�.
	bool FadeInBGM(float value, bool& isEmergencyCall);
	// SoundSource��j�󂷂�֐�.
	void DestoroySource();
	// �t�F�[�h�A�E�g�t���O�̃Z�b�^�[.
	void SetFadeOutFlag(bool BGMFadeOutFlag) { m_bFadeOutStart = BGMFadeOutFlag; }
	// �t�F�[�h�A�E�g�t���O�̃Q�b�^�[.
	bool GetFadeOutFlag() { return m_bFadeOutStart; }
	// �t�F�[�h�C���t���O�̃Z�b�^�[.
	void SetFadeInFlag(bool BGMFadeInFlag) { m_bFadeInStart = BGMFadeInFlag; }
	// �t�F�[�h�C���t���O�̃Q�b�^�[.
	bool GetFadeInFlag() { return m_bFadeInStart; }
	// ��������Ŏ��c����Ȃ��悤��.
	void HundleIsSignal( const bool& isEnd = false )
	{
		if( isEnd == false ) return;
		SetEvent( m_Callback.event );
	}
//-----------------------------------------------------
// �Đ��ʒu�n.

	// ���ԁF�� : �b�ōĐ��ʒu�𓮂���.
	// ���� 60�𒴂�����J��グ�A�b��60�𒴂����番��.
	void BGMPointSeek(int& Hour, int& Minutes,double& Second);
	// ���ݍĐ��ʒu(����)��Ԃ�.
	double GetNowPlayFrame();

protected:
	//! �����f�[�^���X�g���[���ɗ������݂܂�.
	//  ���̊֐��͊O������Ăяo���܂���.
	HRESULT OggSubmit();
protected:
	std::mutex				m_mtx;
	COggLoad*				m_pOggData; // Ogg�f�[�^���[�h�N���X.
	VoiceCallback			m_Callback;	// �R�[���o�b�N.

	XAUDIO2_BUFFER		m_Buffer;		// �I�[�f�B�I�o�b�t�@�[
	int					m_Buff_len;		// �o�b�t�@�����W.
	BYTE**				m_Buff;			// �o�b�t�@�T�C�Y.
	int					m_Len;			// �����W.
	int					m_Buff_cnt;		// �o�b�t�@�J�E���g.
	int					m_Size;

	IXAudio2SourceVoice*   m_pSourceVoice; //  �T�E���h���Đ�����X�g���[��.
	bool  m_bCanChangeVolume;// BGM���ʂ��O������t�F�[�h�ȊO�ŐG��邩�ǂ���.

	float m_fMaxPitch;		// �ő�s�b�`.
	float m_fPitch;			// �s�b�`.
	bool m_bFadeOutStart;	// �t�F�[�h�A�E�g�J�n�t���O.
	bool m_bFadeInStart;	// �t�F�[�h�C���J�n�t���O.
	float m_fFadeInValue;
	float m_fFadeOutValue;

	bool					m_bFirstPlay;	// ����Đ����ǂ���.
};

#endif // !C_XAUDIO2_PLAYSOUND_H