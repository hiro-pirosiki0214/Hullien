#include "XAudio2PlaySE.h"
#include "XAudio2.h"
#include "..\XAudio2Master\XAudio2MasterVoice.h"
#include <vector>
#include <process.h>
#include <thread>

#include "..\SoundManager.h"

CXAudio2PlaySE::CXAudio2PlaySE()
	: m_pOggData			( nullptr )
	, m_CheckNonePlaySeVoice( 0 )
	, m_RepeatPlaySe		( 0 )
	, m_bCanChangeSE		( false )
	, m_bisAnotherVolume	( false )
	, m_fSEVolume			( 1.0f )
	, m_fAnotherVolume		( 0.2f )
{
	m_pSourceVoice[0] = nullptr;
	InitSEFlag();
}

CXAudio2PlaySE::~CXAudio2PlaySE()
{
}

void CXAudio2PlaySE::InitSEFlag()
{
	for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {
		m_isSePlayVoice[Array] = false;
	}
}
//-------------------------------------
// �X�^�[�g����Ă��Ȃ��\�[�X�{�C�X��T��.
int CXAudio2PlaySE::CheckPlaySeState()
{
	int Size = SE_VOICE_SIZE;
	for (int Array = 1; Array < SE_VOICE_SIZE; Array++) {
		if (m_isSePlayVoice[Array] == false) {
			if (m_pSourceVoice[Array] == nullptr) {
				_ASSERT_EXPR("Se Sorce is Null", false);
				return -1;
			}
			m_CheckNonePlaySeVoice = Array;
			return m_CheckNonePlaySeVoice;
		}
	}
	m_CheckNonePlaySeVoice = -1;
	return m_CheckNonePlaySeVoice;
}
//-------------------------------------
// �Đ�����Ă��Ȃ��\�[�X�{�C�X��T��.
int CXAudio2PlaySE::CheckPlaySeBuffer()
{
	XAUDIO2_VOICE_STATE state;
	// �Đ����Ă��Ȃ�SEVoice��T��.
	for (int Array = 0; Array < SE_VOICE_SIZE; Array++) {
		m_pSourceVoice[Array]->GetState(&state);
		if (state.BuffersQueued > 0)	// �o�b�t�@��0���傫��.
		{
		}
		else {
			return Array;	// �Đ�����Ă��Ȃ��z��̒l��Ԃ�.
		}
	}
	// ���ׂăT�E���h�Đ��r���Ȃ̂ŁA���s�[�g�Đ�����.
	if (m_RepeatPlaySe < SE_VOICE_SIZE) {
		m_RepeatPlaySe++;
	}
	if (m_RepeatPlaySe == SE_VOICE_SIZE) {
		m_RepeatPlaySe = 0;
	}

	return m_RepeatPlaySe;
}

// �w�肳�ꂽ�z��ԍ���SE���Đ�����Ă��邩.
const bool CXAudio2PlaySE::IsPlayingSE(size_t ArrayNum)
{
	// SoundSource���Ȃ��ꍇ���^�[�� 0(�Đ����Ă��Ȃ��Ƃ��Ɠ���).
	if (m_pSourceVoice[ArrayNum] == nullptr) return 0;
	XAUDIO2_VOICE_STATE xState;
	// SoundSource�̃X�e�[�g�擾.
	m_pSourceVoice[ArrayNum]->GetState(&xState);

	return xState.BuffersQueued != 0;
}

bool CXAudio2PlaySE::Play(
	std::shared_ptr<COggLoad> pWavData)
{
	HRESULT hr;
	int Array = 0;

	if (m_pSourceVoice[0] == nullptr) return true;

	// �I�v�V������ʒ��A�{�C�X�\�[�X���ʂ��S�̂�SE���ʂɍ��킹�ďグ�����ɍ��킹����悤��.
	if (m_bCanChangeSE == false) {
		if (m_fSEVolume >= CSoundManager::GetInstance()->m_stSound.SEVolume) {
			m_fSEVolume = CSoundManager::GetInstance()->m_stSound.SEVolume;
		}
	}
	else {
		m_fSEVolume = CSoundManager::GetInstance()->m_stSound.SEVolume;
	}

	// ����SE���Đ�����Ă��Ȃ���Β��ɓ���Đ�����.
	if (m_isSePlayVoice[0] == false) {
		if (m_pSourceVoice[0] == nullptr) return true;
		m_isSePlayVoice[0] = true;
		m_pSourceVoice[0]->Start();	// �\�[�X�{�C�X�Đ�.
	}
	if (m_bisAnotherVolume == true) {
		SetSEVolume(m_fAnotherVolume, 0);	// SE�ɑS�̂Ƃ͕ʂ̉��ʃZ�b�g.
	}
	else {
		SetSEVolume(m_fSEVolume, 0);		// SE�̉��ʃZ�b�g.
	}
	XAUDIO2_VOICE_STATE state;
	if (m_pSourceVoice[0] == nullptr) return true;
	m_pSourceVoice[0]->GetState(&state);
	if (state.BuffersQueued > 0)	// �o�b�t�@��0���傫��.
	{
		// -1�Ȃ�S�\�[�X�Đ��ς�.
		if (m_CheckNonePlaySeVoice != -1) CheckPlaySeState();

		if (m_CheckNonePlaySeVoice != -1) {
			m_isSePlayVoice[m_CheckNonePlaySeVoice] = true;
			m_pSourceVoice[m_CheckNonePlaySeVoice]->Start();	// �\�[�X�{�C�X�Đ�.
		}

		Array = CheckPlaySeBuffer();		// �Đ�����Ă��Ȃ��\�[�X�{�C�X�̔z��ԍ���Ԃ�.

		m_pSourceVoice[Array]->Stop();		// �\�[�X�{�C�X��~.
		m_pSourceVoice[Array]->FlushSourceBuffers();// �\�[�X�o�b�t�@���N���A.
		m_pSourceVoice[Array]->Start();		// �\�[�X�{�C�X�Đ�.
		if (m_bisAnotherVolume == true) {
			SetSEVolume(m_fAnotherVolume, Array);	// SE�ɑS�̂Ƃ͕ʂ̉��ʃZ�b�g.
		}
		else {
			SetSEVolume(m_fSEVolume, Array);		// SE�̉��ʃZ�b�g.
		}
	}
	else {
		m_RepeatPlaySe = 0;	// ���s�[�g���čĐ�����z���0����ɂ���.
	}
	// �o�b�t�@�[�𐶐�����
	XAUDIO2_BUFFER seBuffer = { 0 };
	seBuffer.pAudioData = pWavData->GetSEWaveBuffer();
	seBuffer.AudioBytes = pWavData->GetSEFileSize();

	// �L���[�Ƀo�b�t�@�[�𓊓�����
	if (FAILED(hr = m_pSourceVoice[Array]->SubmitSourceBuffer(&seBuffer)))
	{
		return true;
	}

	return true;
}
// SE�𑽏d�Đ����Ȃ��ōĐ�.
// �����Ƃ���Ȃ��Ă���SE���~�߂Ĉ�ɂ�����̂ł͂Ȃ�.
bool CXAudio2PlaySE::NoMultiplePlay(std::shared_ptr<COggLoad> pWavData)
{
	HRESULT hr;
	// �I�v�V������ʒ��A�{�C�X�\�[�X���ʂ��S�̂�SE���ʂɍ��킹�ďグ�����ɍ��킹����悤��.
	if (m_bCanChangeSE == false) {
		if (m_fSEVolume >= CSoundManager::GetInstance()->m_stSound.SEVolume) {
			m_fSEVolume = CSoundManager::GetInstance()->m_stSound.SEVolume;
		}
	}
	else {
		m_fSEVolume = CSoundManager::GetInstance()->m_stSound.SEVolume;
	}


	// ����SE���Đ�����Ă��Ȃ���Β��ɓ���Đ�����.
	if (m_isSePlayVoice[0] == false) {
		if (m_pSourceVoice[0] == nullptr) return true;
		m_isSePlayVoice[0] = true;
		m_pSourceVoice[0]->Start();	// �\�[�X�{�C�X�Đ�.
	}
	if (m_bisAnotherVolume == true) {
		SetSEVolume(m_fAnotherVolume, 0);	// SE�ɑS�̂Ƃ͕ʂ̉��ʃZ�b�g.
	}
	else {
		SetSEVolume(m_fSEVolume, 0);		// SE�̉��ʃZ�b�g.
	}

	XAUDIO2_VOICE_STATE state;
	m_pSourceVoice[0]->GetState(&state);
	if (state.BuffersQueued > 0)	// �o�b�t�@��0���傫��.
	{
		m_pSourceVoice[0]->Stop();		// �\�[�X�{�C�X��~.
		m_pSourceVoice[0]->FlushSourceBuffers();// �\�[�X�o�b�t�@���N���A.
		m_pSourceVoice[0]->Start();		// �\�[�X�{�C�X�Đ�.
	}

	// �o�b�t�@�[�𐶐�����
	XAUDIO2_BUFFER seBuffer = { 0 };
	seBuffer.pAudioData = pWavData->GetSEWaveBuffer();
	seBuffer.AudioBytes = pWavData->GetSEFileSize();

	// �L���[�Ƀo�b�t�@�[�𓊓�����
	if (FAILED(hr = m_pSourceVoice[0]->SubmitSourceBuffer(&seBuffer)))
	{
		return true;
	}

	return true;

}

// SE��~.
bool CXAudio2PlaySE::SeStop(const size_t ArrayNum)
{
	if (m_pSourceVoice[ArrayNum] == nullptr) return true;

	m_pSourceVoice[ArrayNum]->Stop(0);
	m_pSourceVoice[ArrayNum]->FlushSourceBuffers();
	m_isSePlayVoice[ArrayNum] = false;
	return true;
}
// �쐬���Ă�\������SE����~.
bool CXAudio2PlaySE::AllSeStop()
{
	if (m_pSourceVoice[0] == nullptr) return true;

	for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {
		m_pSourceVoice[Array]->Stop(0);
		m_pSourceVoice[Array]->FlushSourceBuffers();
		m_isSePlayVoice[Array] = false;
	}

	return true;
}

HRESULT CXAudio2PlaySE::CreateOggSound(std::shared_ptr<COggLoad> pOggData, const char * filename)
{
	//���f�[�^������Ă����烊�^�[��.
	if (m_pOggData != nullptr) return true;
	m_pOggData = pOggData.get();
	HRESULT hr;
	// XAudio2�}�X�^�[�{�C�X�̃C���X�^���X�擾.
	clsXAudio2MasterVoice& xAudioMaster = clsXAudio2MasterVoice::GetGlobalSystem();

	IXAudio2*   pHandle = xAudioMaster.GetInterface();

	// �\�[�X�{�C�X���쐬����.
	for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {
		m_pSourceVoice[Array] = nullptr;
		if (FAILED(hr = pHandle->CreateSourceVoice(&m_pSourceVoice[Array], m_pOggData->GetFormat())))
		{
			// ���s����.
			_ASSERT_EXPR("error %#X creating se source voice\n", hr);
			return E_FAIL;
		}
	}

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

const float CXAudio2PlaySE::GetSEVolume(size_t Array)
{
	if (m_pSourceVoice[Array] == nullptr) return 0.0f;
	float Volume = 0.0f;

	m_pSourceVoice[Array]->GetVolume(&Volume);

	return Volume;
}
// SE���ʐݒ�.
bool CXAudio2PlaySE::SetSEVolume(float value, size_t Array)
{
	if (m_pSourceVoice[Array] == nullptr) return false;

	m_pSourceVoice[Array]->SetVolume(value * CSoundManager::GetInstance()->m_stSound.MasterVolume, 0);
	return true;
}

void CXAudio2PlaySE::DestoroySource()
{
	if (m_pSourceVoice[0] != nullptr) {
		for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {
			m_pSourceVoice[Array]->DestroyVoice();
			m_pSourceVoice[Array] = nullptr;
		}
	}
}
