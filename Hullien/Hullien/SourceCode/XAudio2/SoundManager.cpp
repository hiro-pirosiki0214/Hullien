#include "SoundManager.h"
#include <crtdbg.h>
#include <filesystem>	// C++17 �K�{.
#include "..\Utility\FileManager\FileManager.h"
namespace fs = std::filesystem;

namespace {
	const CSoundManager::SoundVolume INIT_VOLUME = { 0.5f,0.5f,0.5f };
}

CSoundManager::CSoundManager()
	: m_bEndGame(false)
	, m_bEndCreate(false)
	, m_bMoveBGMThread(true)
	, m_bMoveSEThread(true)
	, m_isCreateThread(false)
	, m_bResumeBGMThread(true)
	, m_bResumeSEThread(true)
{
	//---------------------------
	// Map�R���e�i�̃N���A.
	pWaveData.clear();
	pBgmSource.clear();
	pBGMThread.clear();
	m_bisEndThread.clear();
	m_bisThreadRelease.clear();
	InThreadID.clear();
	m_vsBGMNameList.clear();
	m_vsSENameList.clear();
	pSeSource.clear();
}

CSoundManager::~CSoundManager()
{
}
// �C���X�^���X�̍쐬.
CSoundManager* CSoundManager::GetInstance()
{
	static std::unique_ptr<CSoundManager> pInstance =
		std::make_unique<CSoundManager>();	// �C���X�^���X�̍쐬.
	return pInstance.get();;
}

//========================================================================================
//	�T�E���h�f�[�^�쐬.
//====
void CSoundManager::CreateSoundData()
{
	if (GetInstance()->m_bEndCreate == true) return;
	auto eachLoad = [&](const fs::directory_entry& entry)
	{
		const std::string extension = entry.path().extension().string();	// �g���q.
		const std::string filePath = entry.path().string();					// �t�@�C���p�X.
		const std::string fileName = entry.path().stem().string();			// �t�@�C����.

		if (extension != ".ogg" && extension != ".OGG") return;	// �g���q�� "Ogg�t�@�C��" ����Ȃ���ΏI��.

		//�t�@�C���p�X����BGM���Ȃ����SE.
		if (filePath.find("BGM") == std::string::npos) {
			// WavLoad�N���X�쐬.
			GetInstance()->m_pOggWavData[fileName] = std::make_shared<COggLoad>();
			// Wav�t�H�[�}�b�g�擾.
			GetInstance()->m_pOggWavData[fileName]->CreateOggData(fileName.c_str(), true);
			// PlaySound�N���X�쐬.
			GetInstance()->pSeSource[fileName] = std::make_shared<CXAudio2PlaySE>();
			// SE��SoundSource�쐬.
			GetInstance()->pSeSource[fileName]->CreateOggSound(GetInstance()->m_pOggWavData[fileName], GetInstance()->m_pOggWavData[fileName]->GetFileName());
			// �t�H���_�̖��O��SE���O���X�g�ɓ����.
			GetInstance()->m_vsSENameList.emplace_back(fileName);
		}
		else {
			// WavLoad�N���X�쐬.
			GetInstance()->m_pOggWavData[fileName] = std::make_shared<COggLoad>();
			// Wav�t�H�[�}�b�g�擾.
			GetInstance()->m_pOggWavData[fileName]->CreateOggData(fileName.c_str());
			// PlaySound�N���X�쐬.
			GetInstance()->pBgmSource[fileName] = std::make_shared<CXAudio2PlayBGM>();
			// SE��SoundSource�쐬.
			GetInstance()->pBgmSource[fileName]->CreateOggSound(GetInstance()->m_pOggWavData[fileName], GetInstance()->m_pOggWavData[fileName]->GetFileName());
			// �t�H���_�̖��O��SE���O���X�g�ɓ����.
			GetInstance()->m_vsBGMNameList.emplace_back(fileName);
		}
	};

	try {
		//�w��p�X(FILE_PATH)�f�B���N�g�������ċA�I�ɑ���
		fs::recursive_directory_iterator dir_itr(GetInstance()->FILE_PATH), end_itr;
		// eachLoad�֐����s��.
		std::for_each(dir_itr, end_itr, eachLoad);

	}
	// �G���[�L���b�`.
	catch (const fs::filesystem_error& e) {
		const char* errorMessage = e.path1().string().c_str();
		_ASSERT_EXPR(false, L"�T�E���h�f�[�^�쐬���s");
		MessageBox(nullptr, errorMessage, "�T�E���h�f�[�^�쐬���s", MB_OK);
	}

	LoadVolume();

	GetInstance()->m_bEndCreate = true;
}

// ���ʂ�ǂݍ���.
bool CSoundManager::LoadVolume()
{
	if( CFileManager::BinaryReading( GetInstance()->SETING_FAILE_PATH, GetInstance()->m_stSound ) == false ) return false;

	if( GetInstance()->m_stSound.MasterVolume < 0.0f ) GetInstance()->m_stSound.MasterVolume = 0.0f;
	if( GetInstance()->m_stSound.BGMVolume < 0.0f ) GetInstance()->m_stSound.BGMVolume = 0.0f;
	if( GetInstance()->m_stSound.SEVolume < 0.0f ) GetInstance()->m_stSound.SEVolume = 0.0f;

	return true;
}

// ���ʂ�ۑ�����.
bool CSoundManager::SaveVolume()
{
	if( GetInstance()->m_stSound.MasterVolume < 0.0f ) GetInstance()->m_stSound.MasterVolume = 0.0f;
	if( GetInstance()->m_stSound.BGMVolume < 0.0f ) GetInstance()->m_stSound.BGMVolume = 0.0f;
	if( GetInstance()->m_stSound.SEVolume < 0.0f ) GetInstance()->m_stSound.SEVolume = 0.0f;

	if( CFileManager::BinaryWriting( GetInstance()->SETING_FAILE_PATH, GetInstance()->m_stSound ) == false ) return false;
	return true;
}

//========================================================================================
//	BGM.
//====

void CSoundManager::BGMPointSeek(const std::string& Name, int Hour, int Minutes, double Second)
{
	GetInstance()->pBgmSource[Name]->BGMPointSeek(Hour,Minutes,Second);
}

double CSoundManager::GetBGMFrame(const std::string & Name)
{
	return GetInstance()->pBgmSource[Name]->GetNowPlayFrame();
}

// BGM�\�[�X�̌��݃t���[��(�b)��double�l�ŕԂ��֐�.
bool CSoundManager::GetMoveUpThread(const std::string & Name)
{
	DWORD ThreadExitCode = -1;

	GetExitCodeThread(GetInstance()->pBGMThread[Name].native_handle(), &ThreadExitCode);
	// 4294967295���A���Ă�����X���b�h�������Ă��Ȃ��̂�false.
	if (ThreadExitCode == 0xFFFFFFFF) {
		return false;
	}
	return true;
}

// BGM�ĊJ�֐�.
void CSoundManager::AgainPlayBGM(const std::string Name)
{
	// map�R���e�i�ɃL�[�����݂��ĂȂ���΃��^�[��.
	if (GetInstance()->pBgmSource.find(Name) == GetInstance()->pBgmSource.end()) {
		return;
	}
	// ���݂��Ȃ���΃��^�[��.
	if (GetInstance()->pBgmSource[Name] == nullptr) return;
	// �Q�[���I���t���O�������Ă���΃��^�[��.
	if (GetInstance()->m_bEndGame == true) return;
	// �w�肵��BGM��~.
	GetInstance()->pBgmSource[Name]->PlayStart();
}

// �X���b�h�������BGM���Đ�����֐�(�O���ďo).
void CSoundManager::ThreadPlayBGM(const std::string& Name, const bool& LoopFlag)
{
	// �Q�[���I���t���O�������Ă���΃��^�[��.
	if (GetInstance()->m_bEndGame == true) return;
	GetInstance()->m_bisEndThread[Name] = false;
	// map�R���e�i�ɃL�[�����݂��ĂȂ���΃��^�[��.
	if (GetInstance()->m_bisEndThread.find(Name) == GetInstance()->m_bisEndThread.end()) {
		return;
	}
	auto BGM = [&]()
	{
		if (Name.size() == 0) return;
		CSoundManager::PlayBGM(Name, GetInstance()->m_bisEndThread[Name]);
	};

	DWORD ThreadExitCode = -1;

	GetExitCodeThread(GetInstance()->pBGMThread[Name].native_handle(), &ThreadExitCode);
	// 4294967295���A���Ă�����X���b�h�������Ă��Ȃ��̂œ�����.
	if (ThreadExitCode == 0xFFFFFFFF) {
		GetInstance()->pBGMThread[Name] = std::thread(BGM);
	}
	GetInstance()->InThreadID[Name] = GetInstance()->pBGMThread[Name].get_id();
	GetInstance()->m_bisThreadRelease[Name] = false;

}

// BGM�Đ��֐�(�����g�p).
void CSoundManager::PlayBGM(const std::string Name, bool& isEnd)
{
	// map�R���e�i�ɃL�[�����݂��ĂȂ���΃��^�[��.
	if (GetInstance()->pBgmSource.find(Name) == GetInstance()->pBgmSource.end()) {
		return;
	}

	// ���݂��Ȃ���΃��^�[��.
	if (GetInstance()->pBgmSource[Name] == nullptr) return;
	// �Q�[���I���t���O�������Ă���΃��^�[��.
	if (GetInstance()->m_bEndGame == true) return;
	// �Đ�.
	GetInstance()->pBgmSource[Name]->Play(isEnd);
}
void CSoundManager::StateChangeVolumeThread(const bool& bFlag)
{
	GetInstance()->m_bResumeBGMThread = bFlag;
	GetInstance()->m_bResumeSEThread = bFlag;
	if (bFlag == true) {
		GetInstance()->m_BGMCv.notify_one();
		GetInstance()->m_SECv.notify_one();
	}
}
// BGM��~�֐�.
void CSoundManager::StopBGM(const std::string Name)
{
	// map�R���e�i�ɃL�[�����݂��ĂȂ���΃��^�[��.
	if (GetInstance()->pBgmSource.find(Name) == GetInstance()->pBgmSource.end()) {
		return;
	}

	// ���݂��Ȃ���΃��^�[��.
	if (GetInstance()->pBgmSource[Name] == nullptr) return;
	// �Q�[���I���t���O�������Ă���΃��^�[��.
	if (GetInstance()->m_bEndGame == true) return;
	// �w�肵��BGM��~.
	GetInstance()->pBgmSource[Name]->Stop();
}
// BGM�t�F�[�h�A�E�g�֐�.
void CSoundManager::FadeOutBGM(const std::string Name, const bool StopFadeIn)
{
	// ���݂��Ȃ���΃��^�[��.
	if (GetInstance()->pBgmSource[Name] == nullptr) return;

	// �Q�[���I���t���O�������Ă���΃��^�[��.
	if (GetInstance()->m_bEndGame == true) return;
	// �t���O�������Ă���΁A�t�F�[�h�C�������낤�ƁA������~�߃t�F�[�h�A�E�g���n�߂�.
	if (StopFadeIn == true) {
		GetInstance()->pBgmSource[Name]->SetFadeInFlag(false);
	}

	// �t�F�[�h�C���A�A�E�g�t���O�������ĂȂ���Β��ɓ����ăt�F�[�h�A�E�g�t���O�𗧂Ă�.
	if (GetInstance()->pBgmSource[Name]->GetFadeInFlag() == false && GetInstance()->pBgmSource[Name]->GetFadeOutFlag() == false) {
		GetInstance()->pBgmSource[Name]->SetFadeOutFlag(true);
	}

}
// BGM�t�F�[�h�C���֐�.
void CSoundManager::FadeInBGM(const std::string Name, const bool StopFadeOut)
{
	// ���݂��Ȃ���΃��^�[��.
	if (GetInstance()->pBgmSource[Name] == nullptr) return;

	// �Q�[���I���t���O�������Ă���΃��^�[��.
	if (GetInstance()->m_bEndGame == true) return;
	// �t���O�������Ă���΁A�t�F�[�h�A�E�g�����낤�ƁA������~�߃t�F�[�h�C�����n�߂�.
	if (StopFadeOut == true) {
		GetInstance()->pBgmSource[Name]->SetFadeOutFlag(false);
	}

	// �t�F�[�h�C���A�A�E�g�t���O�������ĂȂ���Β��ɓ����ăt�F�[�h�C���t���O�𗧂Ă�.
	if (GetInstance()->pBgmSource[Name]->GetFadeInFlag() == false && GetInstance()->pBgmSource[Name]->GetFadeOutFlag() == false) {
		GetInstance()->pBgmSource[Name]->SetFadeInFlag(true);
	}

}
// �w�肵��BGM�̉��ʂ��擾����֐�.
float CSoundManager::GetBGMVolume(const std::string Name)
{
	// �Q�[���I���t���O�������Ă���΃��^�[��.
	if (GetInstance()->m_bEndGame == true) return 0.0f;
	// ���݂��Ȃ���΃��^�[��.
	if (GetInstance()->pBgmSource[Name] == nullptr) return 0.0f;
	// �Đ����Ă��Ȃ�.
	if (GetInstance()->pBgmSource[Name]->IsPlaying() == false) return 0.0f;

	return GetInstance()->pBgmSource[Name]->GetVolume();
}
// �w�肵��BGM�̉��ʂ��Z�b�g����֐�.
void CSoundManager::SetBGMVolume(const std::string Name, float Volme)
{
	// �Q�[���I���t���O�������Ă���΃��^�[��.
	if (GetInstance()->m_bEndGame == true) return;

	GetInstance()->pBgmSource[Name]->SetBGMVolume(Volme);
}
// BGM���~���A����BGM���Đ����Ă����X���b�h���������.
bool CSoundManager::StopBGMThread(const std::string BGMName)
{
	if (GetInstance()->m_bisThreadRelease[BGMName] == true) {
		return true;
	}
	// �Đ����ɐݒ肵���t���O��true�ɂ��āABGM���~�Ɍ����킹��.
	GetInstance()->m_bisEndThread[BGMName] = true;

	GetInstance()->pBgmSource[BGMName]->HundleIsSignal( GetInstance()->m_bisEndThread[BGMName] );

	DWORD ThreadExitCode = -1;
	// �X���b�h����~�������ǂ�����ID�擾�Ŏ擾.
	GetExitCodeThread(GetInstance()->pBGMThread[BGMName].native_handle(), &ThreadExitCode);
	// �Ԃ��ė���l��4294967295�̏ꍇ�A���������X���b�h�������オ���Ă��Ȃ�(�Đ����Ă��Ȃ�)�̂�True��Ԃ�.
	if (ThreadExitCode == 0xFFFFFFFF) {
		return true;
	}
	// ��~���Ă�����0���Ԃ��ė���.
	if (ThreadExitCode == 0) {
		if (GetInstance()->InThreadID[BGMName] == GetInstance()->pBGMThread[BGMName].get_id()) {
			GetInstance()->pBGMThread[BGMName].join();
			GetInstance()->m_bisThreadRelease[BGMName] = true;
			while (1)
			{
				if (GetInstance()->pBGMThread[BGMName].joinable() != true) {
					return true;
				}
			}
		}
		return false;
	}
	return false;
}
// �w�肵�����O��BGM�X���b�h���Đ������ǂ���.
bool CSoundManager::GetIsPlayBGM(const std::string BGMName)
{
	DWORD ThreadExitCode = -1;
	// �X���b�h����~�������ǂ�����ID�擾�Ŏ擾.
	GetExitCodeThread(GetInstance()->pBGMThread[BGMName].native_handle(), &ThreadExitCode);
	// �Ԃ��ė���l��4294967295�̏ꍇ�A�Đ����Ă��Ȃ�.
	if (ThreadExitCode == 0xFFFFFFFF) {
		return false;
	}
	// 0�Ȃ�X���b�h��~�ς�.
	else if (ThreadExitCode == 0) {
		return false;
	}
	return true;
}
// BGM�̃s�b�`�ݒ�.
void CSoundManager::SetBGMPitch(const std::string Name, const float Value)
{
	// ���݂��Ȃ���΃��^�[��.
	if (GetInstance()->pBgmSource[Name] == nullptr) return;
	GetInstance()->pBgmSource[Name]->SetPitch(Value);
}

//========================================================================================
//	SE.
//====

// SE�Đ��֐�.
void CSoundManager::PlaySE(
	const std::string& Name)
{
	// map�R���e�i�ɃL�[�����݂��ĂȂ���΃��^�[��.
	if (GetInstance()->pSeSource.find(Name) == GetInstance()->pSeSource.end()) {
		return;
	}
	if (GetInstance()->pSeSource[Name] == nullptr) return;

	// �Đ�.
	GetInstance()->pSeSource[Name]->Play(GetInstance()->m_pOggWavData[Name]);
}
// ���d�Đ����Ȃ�SE�Đ��֐�.
void CSoundManager::NoMultipleSEPlay(const std::string & Name)
{
	// map�R���e�i�ɃL�[�����݂��ĂȂ���΃��^�[��.
	if (GetInstance()->pSeSource.find(Name) == GetInstance()->pSeSource.end()) {
		return;
	}
	if (GetInstance()->pSeSource[Name] == nullptr)  return;

	// �Đ�.
	GetInstance()->pSeSource[Name]->NoMultiplePlay(GetInstance()->m_pOggWavData[Name]);
}
// SE��~�֐�.
void CSoundManager::StopSE(const std::string Name, const size_t ArrayNum)
{
	// ���݂��Ȃ���΃��^�[��.
	if (GetInstance()->pSeSource[Name] == nullptr) return;
	GetInstance()->pSeSource[Name]->SeStop(ArrayNum);
}
// ���������̑S�Ă�SE�\�[�X��~�֐�.
void CSoundManager::StopAllSE(const std::string Name)
{
	// ���݂��Ȃ���΃��^�[��.
	if (GetInstance()->pSeSource[Name] == nullptr) return;
	GetInstance()->pSeSource[Name]->AllSeStop();
}
// SE��Voice�\�[�X���ő剹�ʂ�ݒ�.
void CSoundManager::SetSEVolume(const std::string Name, const float Volume)
{
	// ���݂��Ȃ���΃��^�[��.
	if (GetInstance()->pSeSource[Name] == nullptr) return;
	GetInstance()->pSeSource[Name]->SetMaxSEVolume(Volume);
}
// �w�肵�����O��SE���Đ������ǂ�����Ԃ� : �Đ����Ȃ�true.
bool CSoundManager::GetIsPlaySE(const std::string Name, const size_t ArrayNum)
{
	// ���݂��Ȃ���΃��^�[��false.
	if (GetInstance()->pSeSource[Name] == nullptr) return false;
	return GetInstance()->pSeSource[Name]->IsPlayingSE(ArrayNum);
}
// �S��SE���ʂƂ͕ʂ̃\�[�X���̉��ʂ��Z�b�g.
void CSoundManager::SetAnotherSEVolume(const std::string Name, const float & Volume)
{
	// ���݂��Ȃ���΃��^�[��.
	if (GetInstance()->pSeSource[Name] == nullptr) return;
	GetInstance()->pSeSource[Name]->SetAnotherSEVolume(Volume);
}
// �\�[�X���Őݒ肳��Ă��鉹�ʂ�K�p���邩�ǂ���.
void CSoundManager::SetUseAnotherSEVolumeFlag(const std::string Name, const bool & bFlag)
{
	// ���݂��Ȃ���΃��^�[��.
	if (GetInstance()->pSeSource[Name] == nullptr) return;
	GetInstance()->pSeSource[Name]->SetUseAnotherSEVolume(bFlag);
}
// ����֐�.
void CSoundManager::Release()
{

	while (ReleaseChangeSoundVolumeThread() == false);

	// SE�̖��O���X�g�������[�v����.
	for (size_t i = 0; i < GetInstance()->m_vsSENameList.size(); i++) {
		StopAllSE(GetInstance()->m_vsSENameList[i]);
		GetInstance()->m_pOggWavData[GetInstance()->m_vsSENameList[i]]->Close();			//WavData�����.
		GetInstance()->pSeSource[GetInstance()->m_vsSENameList[i]]->DestoroySource();	// SoundSource�����.
	}
	// BGM�̖��O���X�g�������[�v����.
	for (size_t i = 0; i < GetInstance()->m_vsBGMNameList.size(); i++) {
		while (StopBGMThread(GetInstance()->m_vsBGMNameList[i]) == false);
		GetInstance()->m_pOggWavData[GetInstance()->m_vsBGMNameList[i]]->Close();		//WavData�����.
		GetInstance()->pBgmSource[GetInstance()->m_vsBGMNameList[i]]->DestoroySource();	// SoundSource�����.
	}



	// �Q�[���I���t���O�𗧂Ă�.
	GetInstance()->m_bEndGame = true;
}

void CSoundManager::SetMasterVolume(float& MasterVolume)
{
	if (MasterVolume >= 1.0f) MasterVolume = 1.0f;
	if (MasterVolume <= 0.0f) MasterVolume = 0.0f;
	GetInstance()->m_stSound.MasterVolume = MasterVolume;
}

void CSoundManager::CreateChangeSoundVolumeThread()
{
	if (GetInstance()->m_isCreateThread == true) return;
	SetChangeBGMVolumeThread();
	SetChangeSEVolumeThread();
	GetInstance()->m_isCreateThread = true;
}

void CSoundManager::SetChangeBGMVolumeThread()
{
	DWORD ThreadExitCode = -1;
	// �X���b�h��Ԃ��擾.
	GetExitCodeThread(GetInstance()->m_BGMVolume.native_handle(), &ThreadExitCode);
	// �X���b�h�������Ă���΃��^�[��.
	if (ThreadExitCode != 0xFFFFFFFF && ThreadExitCode != 0) {
		return;
	}
	GetInstance()->m_bMoveBGMThread = true;
	auto SetBGMVolme = [&]()
	{
		while (GetInstance()->m_bMoveBGMThread)
		{
			std::unique_lock<std::mutex> lk(GetInstance()->m_BGMmtx);			// mutex.
			GetInstance()->m_BGMCv.wait(lk, [&] { return GetInstance()->m_bResumeBGMThread; });	// �X���b�h��m_bResumeBGMThread��false�̊Ԃ����ŃT�X�y���h(��ؓ������Ȃ�)�Atrue�ōĊJ.
			for (size_t i = 0; i < GetInstance()->m_vsBGMNameList.size(); i++) {
				GetInstance()->pBgmSource[GetInstance()->m_vsBGMNameList[i]]->SetBGMVolume(CSoundManager::GetInstance()->m_stSound.BGMVolume);	// ���ʂ��Z�b�g.
				if (GetInstance()->m_bMoveBGMThread == false) break;
			}
		}
	};
	GetInstance()->m_BGMVolume = std::thread(SetBGMVolme);
}

void CSoundManager::SetChangeSEVolumeThread()
{
	DWORD ThreadExitCode = -1;
	// �X���b�h��Ԃ��擾.
	GetExitCodeThread(GetInstance()->m_SEVolume.native_handle(), &ThreadExitCode);
	// �X���b�h�������Ă���΃��^�[��.
	if (ThreadExitCode != 0xFFFFFFFF && ThreadExitCode != 0) {
		return;
	}
	GetInstance()->m_bMoveSEThread = true;

	auto SetSEVolme = [&]()
	{
		while (GetInstance()->m_bMoveSEThread)
		{
			std::unique_lock<std::mutex> lk(GetInstance()->m_SEmtx);			// mutex.
			GetInstance()->m_SECv.wait(lk, [&] { return GetInstance()->m_bResumeSEThread; });	// �X���b�h��m_bResumeSEThread��false�̊Ԃ����ŃT�X�y���h(��ؓ������Ȃ�)�Atrue�ōĊJ.
			for (size_t i = 0; i < GetInstance()->m_vsSENameList.size(); i++) {
				GetInstance()->pSeSource[GetInstance()->m_vsSENameList[i]]->SetOperationSEVolumeFlag(true);
				GetInstance()->pSeSource[GetInstance()->m_vsSENameList[i]]->SetMaxSEVolume(CSoundManager::GetInstance()->m_stSound.SEVolume);	// ���ʂ��Z�b�g.
			}
		}
		for (size_t i = 0; i < GetInstance()->m_vsSENameList.size(); i++) {
			GetInstance()->pSeSource[GetInstance()->m_vsSENameList[i]]->SetOperationSEVolumeFlag(false);
		}
	};
	GetInstance()->m_SEVolume = std::thread(SetSEVolme);
}

bool CSoundManager::ReleaseChangeSoundVolumeThread()
{
	GetInstance()->m_bResumeBGMThread = true;
	GetInstance()->m_bResumeSEThread = true;
	GetInstance()->m_BGMCv.notify_one();
	GetInstance()->m_SECv.notify_one();

	if (GetInstance()->m_BGMVolume.joinable() == true) {
		GetInstance()->m_bMoveBGMThread = false;
		GetInstance()->m_BGMVolume.join();
		while (1)
		{
			if (GetInstance()->m_BGMVolume.joinable() != true) {
				break;
			}
		}
	}
	if (GetInstance()->m_SEVolume.joinable() == true) {
		GetInstance()->m_bMoveSEThread = false;
		GetInstance()->m_SEVolume.join();
		while (1)
		{
			if (GetInstance()->m_SEVolume.joinable() != true) {
				break;
			}
		}
	}
	GetInstance()->m_isCreateThread = false;
	return true;
}

void CSoundManager::SetSelectChangeSEVolumeFlag(std::string & sName, const bool & bFlag)
{
	// ���݂��Ȃ���΃��^�[��.
	if (GetInstance()->pSeSource[sName] == nullptr) return;
	GetInstance()->pSeSource[sName]->SetOperationSEVolumeFlag(bFlag);
}
