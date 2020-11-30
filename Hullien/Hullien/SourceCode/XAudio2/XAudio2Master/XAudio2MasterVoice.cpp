#include "XAudio2.h"
#include <iostream>
#include "XAudio2MasterVoice.h"
clsXAudio2MasterVoice& clsXAudio2MasterVoice::GetGlobalSystem()
{
	static clsXAudio2MasterVoice xAudioMaster;
	return xAudioMaster;
}

clsXAudio2MasterVoice::clsXAudio2MasterVoice()
{
	Initialize();
}

clsXAudio2MasterVoice::~clsXAudio2MasterVoice()
{
}
// �C���X�^���X�̍쐬.
clsXAudio2MasterVoice* clsXAudio2MasterVoice::GetInstance()
{
	static std::unique_ptr<clsXAudio2MasterVoice> pInstance =
		std::make_unique<clsXAudio2MasterVoice>();	// �C���X�^���X�̍쐬.
	return pInstance.get();
}


// XAudio�擾.
IXAudio2* clsXAudio2MasterVoice::GetInterface()const
{
	return m_pXAudioInterface;
}

// �}�X�^�[�{�C�X�擾.
IXAudio2MasteringVoice* clsXAudio2MasterVoice::GetMasteringVoice()const
{
	return m_pMasterVoice;
}

//������ XAudio�쐬 �}�X�^�[�{�C�X�쐬.
bool clsXAudio2MasterVoice::Initialize()
{
	// COM �̏�����
	if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)) &&
		FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
	{
		return false;
	}

	// XAudio2 �C���^�[�t�F�[�X�̍쐬
	if (FAILED(XAudio2Create(&m_pXAudioInterface, 0)))
	{
		return false;
	}

	// �}�X�^�[�{�C�X�̍쐬
	if (FAILED(m_pXAudioInterface->CreateMasteringVoice(&m_pMasterVoice)))
	{
		return FALSE;
	}

	return true;
}

void clsXAudio2MasterVoice::Release()
{
	if (GetInstance()->m_pMasterVoice == nullptr) return;
	GetInstance()->m_pMasterVoice->DestroyVoice();
	GetInstance()->m_pMasterVoice = nullptr;
	COM_SAFE_RELEASE(GetInstance()->m_pXAudioInterface);
	GetInstance()->m_pXAudioInterface = nullptr;
	CoUninitialize();
}