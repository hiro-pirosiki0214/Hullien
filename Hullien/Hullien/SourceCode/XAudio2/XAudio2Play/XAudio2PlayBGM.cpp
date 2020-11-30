#include "XAudio2PlayBGM.h"
#include "XAudio2.h"
#include "..\XAudio2Master\XAudio2MasterVoice.h"
#include <vector>
#include <process.h>
#include <thread>
#include <chrono>

#include "..\SoundManager.h"
//-----------------------------
// �萔�錾.
namespace {
	const float MAX_PITCH = 5.0f;		// �ő�s�b�`(�ō�5.0f).
	const float INIT_FADE_VALUE = -100.0f;	// �t�F�[�h����l��1�񂾂��v�Z���邽�߂̏������l.
	const int DIVISION_FADE_VALUE = 3000;// �t�F�[�h����l���v�Z���邽�߂̊���Z�̒l.
}

CXAudio2PlayBGM::CXAudio2PlayBGM()
	:m_pSourceVoice		( nullptr )
	, m_pOggData		( nullptr )
	, m_fMaxPitch		( MAX_PITCH )
	, m_fPitch			( 1.0f )
	, m_fFadeInValue	( -100.0f )
	, m_fFadeOutValue	( -100.0f )
	, m_bFadeInStart	( false )
	, m_bFadeOutStart	( false )
	, m_bFirstPlay		( true )
	, m_bCanChangeVolume( false )
{
	m_Buffer	= { 0 };
	m_Buff_len	= 0;
	m_Buff		= nullptr;
	m_Len		= 0;
	m_Buff_cnt	= 0;
	m_Size		= 0;
}

CXAudio2PlayBGM::~CXAudio2PlayBGM()
{
}

HRESULT CXAudio2PlayBGM::OggSubmit()
{
	HRESULT ret;

	// �I�[�f�B�I�o�b�t�@�[��p��
	m_Buff_len = 2;
	m_Buff = new BYTE*[m_Buff_len];
	m_Len = m_pOggData->GetFormat()->nAvgBytesPerSec;

	for (int i = 0; i < m_Buff_len; i++) {
		m_Buff[i] = new BYTE[m_Len];
	}

	// �ŏ��̃o�b�t�@�[�փf�[�^��ǂݍ���
	ret = m_pOggData->ReadChunk(m_Buff, m_Buff_cnt, m_Len, &m_Size);
	if (FAILED(ret))
	{
		_ASSERT_EXPR("error SubmitSourceBuffer ret=%d\n", ret);
		return E_FAIL;
	}

	m_Buffer.AudioBytes = m_Size;
	m_Buffer.pAudioData = m_Buff[m_Buff_cnt];
	if (0 < m_Size)
	{
		ret = m_pSourceVoice->SubmitSourceBuffer(&m_Buffer);
		if (FAILED(ret)) {
			_ASSERT_EXPR("error SubmitSourceBuffer ret=%d\n", ret);
			return E_FAIL;
		}
	}

	if (m_Buff_len <= ++m_Buff_cnt) m_Buff_cnt = 0;

	return S_OK;
}

// �Đ������ǂ������擾����֐�.
const bool CXAudio2PlayBGM::IsPlaying()
{
	// SoundSource���Ȃ��ꍇ���^�[�� 0(�Đ����Ă��Ȃ��Ƃ��Ɠ���).
	if (m_pSourceVoice == nullptr) return 0;
	XAUDIO2_VOICE_STATE xState;
	// SoundSource�̃X�e�[�g�擾.
	m_pSourceVoice->GetState(&xState);

	return xState.BuffersQueued != 0;
}

// �Đ�.
bool CXAudio2PlayBGM::Play(bool& isEnd)
{
	m_mtx.lock();
	HRESULT ret;
	if (m_pSourceVoice == nullptr) return false;
	SetPitch(m_fPitch);
	if (CSoundManager::GetInstance()->m_stSound.BGMVolume != GetVolume()) {
	}
	else {
		SetBGMVolume(CSoundManager::GetInstance()->m_stSound.BGMVolume);
	}
	// SoundSource���Đ�.
	m_pSourceVoice->Start();
	// Submit�͏���Đ����̂ݎ��s����.
	if (m_bFirstPlay == true) {
		OggSubmit();
		m_bFirstPlay = false;
	}

	// �Đ����[�v
	do {
		if (isEnd == true) break;
		if (m_bCanChangeVolume == true) {
			SetBGMVolume(CSoundManager::GetInstance()->m_stSound.BGMVolume);
		}
		// �t�@�C������f�[�^��ǂݎ��
		m_pOggData->ReadChunk(m_Buff, m_Buff_cnt, m_Len, &m_Size);

		// �Ō�܂œǂݐ؂���.
		if (m_Size <= 0) {
			// �I��(��~)�t���O�������Ă��Ȃ�.
			if (isEnd == false) {
				Sleep(20);
				//�f�[�^�̓ǂݎ��|�C���^�����Z�b�g.
				m_pOggData->ResetFile();
				m_pOggData->ReadChunk(m_Buff, m_Buff_cnt, m_Len, &m_Size);// �`�����N�ǂݍ���.
			}
			else {
				// �I���̂���while�𔲂���.
				break;
			}
		}
		// �o�b�t�@�[����
		m_Buffer.AudioBytes = m_Size;
		m_Buffer.pAudioData = m_Buff[m_Buff_cnt];
		// �o�b�t�@�[���L���[�ɒǉ�
		ret = m_pSourceVoice->SubmitSourceBuffer(&m_Buffer);
		if (FAILED(ret)) {
			_ASSERT_EXPR("error SubmitSourceBuffer ret=%d\n", false);
			return true;
		}
		// 2�m�ۂ��Ă���o�b�t�@�[�����݂Ɏg���悤�ɃJ�E���^�`�F�b�N�ƃ��Z�b�g
		if (m_Buff_len <= ++m_Buff_cnt) m_Buff_cnt = 0;

		if (m_bCanChangeVolume == true) {
			SetBGMVolume(CSoundManager::GetInstance()->m_stSound.BGMVolume);
		}

		// �t�F�[�h�C���t���O�������Ă���΁A�t�F�[�h�C���֐����ĂԃX���b�h�𗧂Ă�.
		if (m_bFadeInStart == true) {
			auto BGM = [&]()	// �����_�֐�.
			{
				// 1�x�����t�F�[�h����l���v�Z.
				if (m_fFadeInValue <= INIT_FADE_VALUE) {
					m_fFadeInValue = CSoundManager::GetInstance()->m_stSound.BGMVolume * CSoundManager::GetInstance()->m_stSound.MasterVolume / DIVISION_FADE_VALUE;
				}
				FadeInBGM(m_fFadeInValue, isEnd); // �t�F�[�h�C���֐�.
				m_bFadeInStart = false;					// �����ɗ�����t�F�[�h�C�����I����Ă�̂Ńt���O������.
				m_fFadeInValue = INIT_FADE_VALUE;
			};
			std::thread Th = std::thread(BGM);
			Th.detach();
		}
		// �t�F�[�h�A�E�g�t���O�������Ă���΁A�t�F�[�h�A�E�g�֐����ĂԃX���b�h�𗧂Ă�.
		if (m_bFadeOutStart == true) {
			auto BGM = [&]()
			{
				// 1�x�����t�F�[�h����l���v�Z.
				if (m_fFadeOutValue <= INIT_FADE_VALUE) {
					m_fFadeOutValue = GetVolume() / DIVISION_FADE_VALUE;
				}
				FadeOutBGM(-m_fFadeOutValue, isEnd);
				m_bFadeOutStart = false;
				m_fFadeOutValue = INIT_FADE_VALUE;
			};
			std::thread Th = std::thread(BGM);
			Th.detach();
		}
	} while (WaitForSingleObject(m_Callback.event, INFINITE) == WAIT_OBJECT_0);	// �o�b�t�@�I���܂�while���[�v

	if (m_pSourceVoice == nullptr) return true;

	XAUDIO2_VOICE_STATE xState;
	m_pSourceVoice->GetState(&xState);
	while (IsPlaying() == true) {
		Stop();
		xState.BuffersQueued = 0;
	}
	m_pOggData->ResetFile(); //�f�[�^�̓ǂݎ��|�C���^�����Z�b�g.

	m_mtx.unlock();

	return true;
}

// BGM�T�E���h�Đ����ĊJ.
bool CXAudio2PlayBGM::PlayStart()
{
	if (m_pSourceVoice == nullptr) return true;
	if (IsPlaying() == false)  return true;

	m_pSourceVoice->Start();
	return true;
}

// �T�E���h���S��~.
bool CXAudio2PlayBGM::Stop()
{
	if (m_pSourceVoice == nullptr) return true;

	m_pSourceVoice->Stop(0);
	m_pSourceVoice->FlushSourceBuffers();

	return true;
}

// �T�E���h�ꎞ��~.
bool CXAudio2PlayBGM::Pause()
{
	if (m_pSourceVoice == nullptr) return true;

	m_pSourceVoice->Stop(0);

	return true;
}

HRESULT CXAudio2PlayBGM::CreateOggSound(std::shared_ptr<COggLoad> pOggData, const char * filename)
{
	//���f�[�^������Ă����烊�^�[��.
	if (m_pOggData != nullptr) return true;
	m_pOggData = pOggData.get();
	HRESULT hr;
	// XAudio2�}�X�^�[�{�C�X�̃C���X�^���X�擾.
	clsXAudio2MasterVoice& xAudioMaster = clsXAudio2MasterVoice::GetGlobalSystem();

	IXAudio2*   pHandle = xAudioMaster.GetInterface();

	// �\�[�X�{�C�X���쐬����.
	if (FAILED(hr = pHandle->CreateSourceVoice(&m_pSourceVoice,
		m_pOggData->GetFormat(), 0, m_fMaxPitch, &m_Callback)))
	{
		// ���s����.
		_ASSERT_EXPR("error %#X creating bgm source voice\n", hr);
		return E_FAIL;
	}

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

// ���ʎ擾.
const float CXAudio2PlayBGM::GetVolume()
{
	if (m_pSourceVoice == nullptr) return 0.0f;
	float Volume = 0.0f;

	m_pSourceVoice->GetVolume(&Volume);

	return Volume;
}

// ���ʐݒ�.
bool  CXAudio2PlayBGM::SetBGMVolume(float value)
{
	if (m_pSourceVoice == nullptr) return false;

	if (value >= CSoundManager::GetBGMVolume()) {
		value = CSoundManager::GetBGMVolume();
	}

	m_pSourceVoice->SetVolume(value * CSoundManager::GetInstance()->m_stSound.MasterVolume, 0);
	return true;
}

// ���݂̉��ʂ�����Z�܂��͌��Z.
bool CXAudio2PlayBGM::AdjustVolume(float value)
{
	if (m_pSourceVoice == nullptr) return false;

	float Calc = GetVolume() + value;

	m_pSourceVoice->SetVolume(Calc, 0);

	return true;
}

// �s�b�`��ݒ�.
bool CXAudio2PlayBGM::SetPitch(float value)
{
	if (m_pSourceVoice == nullptr) return false;

	m_pSourceVoice->SetFrequencyRatio(max(min(value, m_fMaxPitch), XAUDIO2_MIN_FREQ_RATIO));

	return true;
}

//�s�b�`���擾.
const float CXAudio2PlayBGM::GetPitch()
{
	float pitch = XAUDIO2_MIN_FREQ_RATIO;
	if (m_pSourceVoice == nullptr) return pitch;

	m_pSourceVoice->GetFrequencyRatio(&pitch);

	return pitch;
}

// �ő�s�b�`�ݒ�.
void  CXAudio2PlayBGM::SetMaxPitch(float value)
{
	m_fMaxPitch = max(min(value, 2.0f), XAUDIO2_MIN_FREQ_RATIO);
}

bool CXAudio2PlayBGM::FadeOutBGM(float value, bool& isEmergencyCall)
{
	while (GetVolume() > 0.0f)
	{
		if (isEmergencyCall == true) return true;
		if (m_bFadeOutStart == false) return true;
		AdjustVolume(value);
		Sleep(1);
	}
	SetBGMVolume(0.0f);
	return true;
}

bool CXAudio2PlayBGM::FadeInBGM(float value, bool& isEmergencyCall)
{
	while (GetVolume() < CSoundManager::GetInstance()->m_stSound.BGMVolume * CSoundManager::GetInstance()->m_stSound.MasterVolume)
	{
		if (isEmergencyCall == true) return true;
		if (m_bFadeInStart == false) return true;// �ő�{�����[���ɍs���O�ɁA�t�F�[�h�C���X�^�[�g�t���O�������ꂽ�̂Ń��^�[��.
		AdjustVolume(value);
		Sleep(1);
	}
	SetBGMVolume(CSoundManager::GetInstance()->m_stSound.BGMVolume);

	return true;
}

// �Đ��ʒu��ύX����.
void CXAudio2PlayBGM::BGMPointSeek(int& Hour, int& Minutes, double& Second)
{
	int GetHour = Hour;
	int GetMin = Minutes;
	double GetSec = Second;

	m_Buff_cnt = 0;

	// �b����60�b�𒴂��Ă����ꍇ�A���ɕϊ�.
	if (GetSec >= 60.0)
	{
		int Second = static_cast<int>(GetSec);
		int Division = Second / 60;
		int	Remainder = Second % 60;
		GetMin += Division;
		GetSec = static_cast<double>(Remainder);
	}
	// ����60���𒴂��Ă����ꍇ�A���Ԃɕϊ�.
	if (GetMin >= 60)
	{
		int Division = GetMin / 60;
		int	Remainder = GetMin % 60;
		GetHour += Division;
		GetMin = Remainder;
	}

	// ����b�ɕϊ�.
	double HourCalc = static_cast<double>(GetHour * 3600);
	// ����b�ɕϊ�.
	double MinCalc = static_cast<double>(GetMin * 60);
	// ���ԕ��b�𑫂����킹��.
	double CalcSeekFrame = HourCalc + MinCalc + GetSec;
	
	//m_pOggData->ResetFile();
	// �w��ʒu�܂Ńf�[�^���V�[�N.
	m_pOggData->SeekFile(CalcSeekFrame);
	// �o�b�t�@�[����
	m_Buffer.AudioBytes = m_Size;
	m_Buffer.pAudioData = m_Buff[m_Buff_cnt];
	// �\�[�X�{�C�X���~.
	m_pSourceVoice->Stop(0);
	// �\�[�X�{�C�X�̃o�b�t�@������.
	m_pSourceVoice->FlushSourceBuffers();
	// �\�[�X�{�C�X���ēx�Đ�����.
	m_pSourceVoice->Start();

	//m_Buff_cnt++;
}

double CXAudio2PlayBGM::GetNowPlayFrame()
{
	return m_pOggData->GetFileFrame();
}

void CXAudio2PlayBGM::DestoroySource()
{
	bool NoSource = true;
	if (m_pSourceVoice != nullptr) {
		m_pSourceVoice->DestroyVoice();	// �\�[�X�{�C�X�̉��.
		m_pSourceVoice = nullptr;
		NoSource = false;
	}

	// �����J�����Ȃ������ꍇ���݂��Ă��Ȃ��̂ł����Ń��^�[��.
	if (NoSource == true) return;

	// �o�b�t�@�����[�X.
	for (int i = 0; i < m_Buff_len; i++) {
		SAFE_DELETE(m_Buff[i]);
	}
	SAFE_DELETE(m_Buff);
}
