#include "SoundManager.h"
#include <crtdbg.h>
#include <filesystem>	// C++17 必須.
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
	// Mapコンテナのクリア.
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
// インスタンスの作成.
CSoundManager* CSoundManager::GetInstance()
{
	static std::unique_ptr<CSoundManager> pInstance =
		std::make_unique<CSoundManager>();	// インスタンスの作成.
	return pInstance.get();;
}

//========================================================================================
//	サウンドデータ作成.
//====
void CSoundManager::CreateSoundData()
{
	if (GetInstance()->m_bEndCreate == true) return;
	auto eachLoad = [&](const fs::directory_entry& entry)
	{
		const std::string extension = entry.path().extension().string();	// 拡張子.
		const std::string filePath = entry.path().string();					// ファイルパス.
		const std::string fileName = entry.path().stem().string();			// ファイル名.

		if (extension != ".ogg" && extension != ".OGG") return;	// 拡張子が "Oggファイル" じゃなければ終了.

		//ファイルパス内にBGMがなければSE.
		if (filePath.find("BGM") == std::string::npos) {
			// WavLoadクラス作成.
			GetInstance()->m_pOggWavData[fileName] = std::make_shared<COggLoad>();
			// Wavフォーマット取得.
			GetInstance()->m_pOggWavData[fileName]->CreateOggData(fileName.c_str(), true);
			// PlaySoundクラス作成.
			GetInstance()->pSeSource[fileName] = std::make_shared<CXAudio2PlaySE>();
			// SEのSoundSource作成.
			GetInstance()->pSeSource[fileName]->CreateOggSound(GetInstance()->m_pOggWavData[fileName], GetInstance()->m_pOggWavData[fileName]->GetFileName());
			// フォルダの名前をSE名前リストに入れる.
			GetInstance()->m_vsSENameList.emplace_back(fileName);
		}
		else {
			// WavLoadクラス作成.
			GetInstance()->m_pOggWavData[fileName] = std::make_shared<COggLoad>();
			// Wavフォーマット取得.
			GetInstance()->m_pOggWavData[fileName]->CreateOggData(fileName.c_str());
			// PlaySoundクラス作成.
			GetInstance()->pBgmSource[fileName] = std::make_shared<CXAudio2PlayBGM>();
			// SEのSoundSource作成.
			GetInstance()->pBgmSource[fileName]->CreateOggSound(GetInstance()->m_pOggWavData[fileName], GetInstance()->m_pOggWavData[fileName]->GetFileName());
			// フォルダの名前をSE名前リストに入れる.
			GetInstance()->m_vsBGMNameList.emplace_back(fileName);
		}
	};

	try {
		//指定パス(FILE_PATH)ディレクトリ内を再帰的に走査
		fs::recursive_directory_iterator dir_itr(GetInstance()->FILE_PATH), end_itr;
		// eachLoad関数を行う.
		std::for_each(dir_itr, end_itr, eachLoad);

	}
	// エラーキャッチ.
	catch (const fs::filesystem_error& e) {
		const char* errorMessage = e.path1().string().c_str();
		_ASSERT_EXPR(false, L"サウンドデータ作成失敗");
		MessageBox(nullptr, errorMessage, "サウンドデータ作成失敗", MB_OK);
	}

	LoadVolume();

	GetInstance()->m_bEndCreate = true;
}

// 音量を読み込む.
bool CSoundManager::LoadVolume()
{
	if( CFileManager::BinaryReading( GetInstance()->SETING_FAILE_PATH, GetInstance()->m_stSound ) == false ) return false;

	if( GetInstance()->m_stSound.MasterVolume < 0.0f ) GetInstance()->m_stSound.MasterVolume = 0.0f;
	if( GetInstance()->m_stSound.BGMVolume < 0.0f ) GetInstance()->m_stSound.BGMVolume = 0.0f;
	if( GetInstance()->m_stSound.SEVolume < 0.0f ) GetInstance()->m_stSound.SEVolume = 0.0f;

	return true;
}

// 音量を保存する.
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

// BGMソースの現在フレーム(秒)をdouble値で返す関数.
bool CSoundManager::GetMoveUpThread(const std::string & Name)
{
	DWORD ThreadExitCode = -1;

	GetExitCodeThread(GetInstance()->pBGMThread[Name].native_handle(), &ThreadExitCode);
	// 4294967295が帰ってきたらスレッドが動いていないのでfalse.
	if (ThreadExitCode == 0xFFFFFFFF) {
		return false;
	}
	return true;
}

// BGM再開関数.
void CSoundManager::AgainPlayBGM(const std::string Name)
{
	// mapコンテナにキーが存在してなければリターン.
	if (GetInstance()->pBgmSource.find(Name) == GetInstance()->pBgmSource.end()) {
		return;
	}
	// 存在しなければリターン.
	if (GetInstance()->pBgmSource[Name] == nullptr) return;
	// ゲーム終了フラグがたっていればリターン.
	if (GetInstance()->m_bEndGame == true) return;
	// 指定したBGM停止.
	GetInstance()->pBgmSource[Name]->PlayStart();
}

// スレッドを作ってBGMを再生する関数(外部呼出).
void CSoundManager::ThreadPlayBGM(const std::string& Name, const bool& LoopFlag)
{
	// ゲーム終了フラグがたっていればリターン.
	if (GetInstance()->m_bEndGame == true) return;
	GetInstance()->m_bisEndThread[Name] = false;
	// mapコンテナにキーが存在してなければリターン.
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
	// 4294967295が帰ってきたらスレッドが動いていないので動かす.
	if (ThreadExitCode == 0xFFFFFFFF) {
		GetInstance()->pBGMThread[Name] = std::thread(BGM);
	}
	GetInstance()->InThreadID[Name] = GetInstance()->pBGMThread[Name].get_id();
	GetInstance()->m_bisThreadRelease[Name] = false;

}

// BGM再生関数(内部使用).
void CSoundManager::PlayBGM(const std::string Name, bool& isEnd)
{
	// mapコンテナにキーが存在してなければリターン.
	if (GetInstance()->pBgmSource.find(Name) == GetInstance()->pBgmSource.end()) {
		return;
	}

	// 存在しなければリターン.
	if (GetInstance()->pBgmSource[Name] == nullptr) return;
	// ゲーム終了フラグがたっていればリターン.
	if (GetInstance()->m_bEndGame == true) return;
	// 再生.
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
// BGM停止関数.
void CSoundManager::StopBGM(const std::string Name)
{
	// mapコンテナにキーが存在してなければリターン.
	if (GetInstance()->pBgmSource.find(Name) == GetInstance()->pBgmSource.end()) {
		return;
	}

	// 存在しなければリターン.
	if (GetInstance()->pBgmSource[Name] == nullptr) return;
	// ゲーム終了フラグがたっていればリターン.
	if (GetInstance()->m_bEndGame == true) return;
	// 指定したBGM停止.
	GetInstance()->pBgmSource[Name]->Stop();
}
// BGMフェードアウト関数.
void CSoundManager::FadeOutBGM(const std::string Name, const bool StopFadeIn)
{
	// 存在しなければリターン.
	if (GetInstance()->pBgmSource[Name] == nullptr) return;

	// ゲーム終了フラグがたっていればリターン.
	if (GetInstance()->m_bEndGame == true) return;
	// フラグが立っていれば、フェードイン中だろうと、それを止めフェードアウトを始める.
	if (StopFadeIn == true) {
		GetInstance()->pBgmSource[Name]->SetFadeInFlag(false);
	}

	// フェードイン、アウトフラグが立ってなければ中に入ってフェードアウトフラグを立てる.
	if (GetInstance()->pBgmSource[Name]->GetFadeInFlag() == false && GetInstance()->pBgmSource[Name]->GetFadeOutFlag() == false) {
		GetInstance()->pBgmSource[Name]->SetFadeOutFlag(true);
	}

}
// BGMフェードイン関数.
void CSoundManager::FadeInBGM(const std::string Name, const bool StopFadeOut)
{
	// 存在しなければリターン.
	if (GetInstance()->pBgmSource[Name] == nullptr) return;

	// ゲーム終了フラグがたっていればリターン.
	if (GetInstance()->m_bEndGame == true) return;
	// フラグが立っていれば、フェードアウト中だろうと、それを止めフェードインを始める.
	if (StopFadeOut == true) {
		GetInstance()->pBgmSource[Name]->SetFadeOutFlag(false);
	}

	// フェードイン、アウトフラグが立ってなければ中に入ってフェードインフラグを立てる.
	if (GetInstance()->pBgmSource[Name]->GetFadeInFlag() == false && GetInstance()->pBgmSource[Name]->GetFadeOutFlag() == false) {
		GetInstance()->pBgmSource[Name]->SetFadeInFlag(true);
	}

}
// 指定したBGMの音量を取得する関数.
float CSoundManager::GetBGMVolume(const std::string Name)
{
	// ゲーム終了フラグがたっていればリターン.
	if (GetInstance()->m_bEndGame == true) return 0.0f;
	// 存在しなければリターン.
	if (GetInstance()->pBgmSource[Name] == nullptr) return 0.0f;
	// 再生していない.
	if (GetInstance()->pBgmSource[Name]->IsPlaying() == false) return 0.0f;

	return GetInstance()->pBgmSource[Name]->GetVolume();
}
// 指定したBGMの音量をセットする関数.
void CSoundManager::SetBGMVolume(const std::string Name, float Volme)
{
	// ゲーム終了フラグがたっていればリターン.
	if (GetInstance()->m_bEndGame == true) return;

	GetInstance()->pBgmSource[Name]->SetBGMVolume(Volme);
}
// BGMを停止し、そのBGMを再生していたスレッドを放棄する.
bool CSoundManager::StopBGMThread(const std::string BGMName)
{
	if (GetInstance()->m_bisThreadRelease[BGMName] == true) {
		return true;
	}
	// 再生時に設定したフラグをtrueにして、BGMを停止に向かわせる.
	GetInstance()->m_bisEndThread[BGMName] = true;

	GetInstance()->pBgmSource[BGMName]->HundleIsSignal( GetInstance()->m_bisEndThread[BGMName] );

	DWORD ThreadExitCode = -1;
	// スレッドが停止したかどうかをID取得で取得.
	GetExitCodeThread(GetInstance()->pBGMThread[BGMName].native_handle(), &ThreadExitCode);
	// 返って来る値が4294967295の場合、そもそもスレッドが立ち上がっていない(再生していない)のでTrueを返す.
	if (ThreadExitCode == 0xFFFFFFFF) {
		return true;
	}
	// 停止していたら0が返って来る.
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
// 指定した名前のBGMスレッドが再生中かどうか.
bool CSoundManager::GetIsPlayBGM(const std::string BGMName)
{
	DWORD ThreadExitCode = -1;
	// スレッドが停止したかどうかをID取得で取得.
	GetExitCodeThread(GetInstance()->pBGMThread[BGMName].native_handle(), &ThreadExitCode);
	// 返って来る値が4294967295の場合、再生していない.
	if (ThreadExitCode == 0xFFFFFFFF) {
		return false;
	}
	// 0ならスレッド停止済み.
	else if (ThreadExitCode == 0) {
		return false;
	}
	return true;
}
// BGMのピッチ設定.
void CSoundManager::SetBGMPitch(const std::string Name, const float Value)
{
	// 存在しなければリターン.
	if (GetInstance()->pBgmSource[Name] == nullptr) return;
	GetInstance()->pBgmSource[Name]->SetPitch(Value);
}

//========================================================================================
//	SE.
//====

// SE再生関数.
void CSoundManager::PlaySE(
	const std::string& Name)
{
	// mapコンテナにキーが存在してなければリターン.
	if (GetInstance()->pSeSource.find(Name) == GetInstance()->pSeSource.end()) {
		return;
	}
	if (GetInstance()->pSeSource[Name] == nullptr) return;

	// 再生.
	GetInstance()->pSeSource[Name]->Play(GetInstance()->m_pOggWavData[Name]);
}
// 多重再生しないSE再生関数.
void CSoundManager::NoMultipleSEPlay(const std::string & Name)
{
	// mapコンテナにキーが存在してなければリターン.
	if (GetInstance()->pSeSource.find(Name) == GetInstance()->pSeSource.end()) {
		return;
	}
	if (GetInstance()->pSeSource[Name] == nullptr)  return;

	// 再生.
	GetInstance()->pSeSource[Name]->NoMultiplePlay(GetInstance()->m_pOggWavData[Name]);
}
// SE停止関数.
void CSoundManager::StopSE(const std::string Name, const size_t ArrayNum)
{
	// 存在しなければリターン.
	if (GetInstance()->pSeSource[Name] == nullptr) return;
	GetInstance()->pSeSource[Name]->SeStop(ArrayNum);
}
// 同じ音源の全てのSEソース停止関数.
void CSoundManager::StopAllSE(const std::string Name)
{
	// 存在しなければリターン.
	if (GetInstance()->pSeSource[Name] == nullptr) return;
	GetInstance()->pSeSource[Name]->AllSeStop();
}
// SEのVoiceソース内最大音量を設定.
void CSoundManager::SetSEVolume(const std::string Name, const float Volume)
{
	// 存在しなければリターン.
	if (GetInstance()->pSeSource[Name] == nullptr) return;
	GetInstance()->pSeSource[Name]->SetMaxSEVolume(Volume);
}
// 指定した名前のSEが再生中かどうかを返す : 再生中ならtrue.
bool CSoundManager::GetIsPlaySE(const std::string Name, const size_t ArrayNum)
{
	// 存在しなければリターンfalse.
	if (GetInstance()->pSeSource[Name] == nullptr) return false;
	return GetInstance()->pSeSource[Name]->IsPlayingSE(ArrayNum);
}
// 全体SE音量とは別のソース毎の音量をセット.
void CSoundManager::SetAnotherSEVolume(const std::string Name, const float & Volume)
{
	// 存在しなければリターン.
	if (GetInstance()->pSeSource[Name] == nullptr) return;
	GetInstance()->pSeSource[Name]->SetAnotherSEVolume(Volume);
}
// ソース内で設定されている音量を適用するかどうか.
void CSoundManager::SetUseAnotherSEVolumeFlag(const std::string Name, const bool & bFlag)
{
	// 存在しなければリターン.
	if (GetInstance()->pSeSource[Name] == nullptr) return;
	GetInstance()->pSeSource[Name]->SetUseAnotherSEVolume(bFlag);
}
// 解放関数.
void CSoundManager::Release()
{

	while (ReleaseChangeSoundVolumeThread() == false);

	// SEの名前リスト数分ループを回す.
	for (size_t i = 0; i < GetInstance()->m_vsSENameList.size(); i++) {
		StopAllSE(GetInstance()->m_vsSENameList[i]);
		GetInstance()->m_pOggWavData[GetInstance()->m_vsSENameList[i]]->Close();			//WavDataを閉じる.
		GetInstance()->pSeSource[GetInstance()->m_vsSENameList[i]]->DestoroySource();	// SoundSourceを解放.
	}
	// BGMの名前リスト数分ループを回す.
	for (size_t i = 0; i < GetInstance()->m_vsBGMNameList.size(); i++) {
		while (StopBGMThread(GetInstance()->m_vsBGMNameList[i]) == false);
		GetInstance()->m_pOggWavData[GetInstance()->m_vsBGMNameList[i]]->Close();		//WavDataを閉じる.
		GetInstance()->pBgmSource[GetInstance()->m_vsBGMNameList[i]]->DestoroySource();	// SoundSourceを解放.
	}



	// ゲーム終了フラグを立てる.
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
	// スレッド状態を取得.
	GetExitCodeThread(GetInstance()->m_BGMVolume.native_handle(), &ThreadExitCode);
	// スレッドが動いていればリターン.
	if (ThreadExitCode != 0xFFFFFFFF && ThreadExitCode != 0) {
		return;
	}
	GetInstance()->m_bMoveBGMThread = true;
	auto SetBGMVolme = [&]()
	{
		while (GetInstance()->m_bMoveBGMThread)
		{
			std::unique_lock<std::mutex> lk(GetInstance()->m_BGMmtx);			// mutex.
			GetInstance()->m_BGMCv.wait(lk, [&] { return GetInstance()->m_bResumeBGMThread; });	// スレッドをm_bResumeBGMThreadがfalseの間ここでサスペンド(一切動かさない)、trueで再開.
			for (size_t i = 0; i < GetInstance()->m_vsBGMNameList.size(); i++) {
				GetInstance()->pBgmSource[GetInstance()->m_vsBGMNameList[i]]->SetBGMVolume(CSoundManager::GetInstance()->m_stSound.BGMVolume);	// 音量をセット.
				if (GetInstance()->m_bMoveBGMThread == false) break;
			}
		}
	};
	GetInstance()->m_BGMVolume = std::thread(SetBGMVolme);
}

void CSoundManager::SetChangeSEVolumeThread()
{
	DWORD ThreadExitCode = -1;
	// スレッド状態を取得.
	GetExitCodeThread(GetInstance()->m_SEVolume.native_handle(), &ThreadExitCode);
	// スレッドが動いていればリターン.
	if (ThreadExitCode != 0xFFFFFFFF && ThreadExitCode != 0) {
		return;
	}
	GetInstance()->m_bMoveSEThread = true;

	auto SetSEVolme = [&]()
	{
		while (GetInstance()->m_bMoveSEThread)
		{
			std::unique_lock<std::mutex> lk(GetInstance()->m_SEmtx);			// mutex.
			GetInstance()->m_SECv.wait(lk, [&] { return GetInstance()->m_bResumeSEThread; });	// スレッドをm_bResumeSEThreadがfalseの間ここでサスペンド(一切動かさない)、trueで再開.
			for (size_t i = 0; i < GetInstance()->m_vsSENameList.size(); i++) {
				GetInstance()->pSeSource[GetInstance()->m_vsSENameList[i]]->SetOperationSEVolumeFlag(true);
				GetInstance()->pSeSource[GetInstance()->m_vsSENameList[i]]->SetMaxSEVolume(CSoundManager::GetInstance()->m_stSound.SEVolume);	// 音量をセット.
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
	// 存在しなければリターン.
	if (GetInstance()->pSeSource[sName] == nullptr) return;
	GetInstance()->pSeSource[sName]->SetOperationSEVolumeFlag(bFlag);
}
