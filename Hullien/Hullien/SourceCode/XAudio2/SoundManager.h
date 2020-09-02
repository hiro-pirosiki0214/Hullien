#ifndef C_SOUNDMANAGER_H
#define C_SOUNDMANAGER_H

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <thread>
#include "XAudio2Play\XAudio2PlayBGM.h"
#include "XAudio2Play\XAudio2PlaySE.h"

#include "XAudio2WaveLoad/XAudio2WaveLoad.h"
#include "OggLoad/OggLoad.h"


#include <mutex>
#include <condition_variable>
class CSoundManager
{
private:
	// Soundデータが入っているディレクトリパス.
	const char* FILE_PATH = "Data\\Sound";
public:
	CSoundManager();
	virtual ~CSoundManager();

	// インスタンスの取得.
	static CSoundManager* GetInstance();
	
	// サウンドデータ作成.
	static void CreateSoundData();

	// ゲーム全体のBGMの最大音量を取得.
	static float GetGameBGMVolume() { return GetInstance()->m_fMaxBGMVolume * GetInstance()->m_fMasterVolume; }
	// 最大マスター音量取得
	static float GetMasterVolume() { return GetInstance()->m_fMasterVolume; }
	// 最大BGM音量取得
	static float GetBGMVolume() { return GetInstance()->m_fMaxBGMVolume; }
	// ゲーム全体のSEの最大音量を取得.
	static float GetGameSEVolume() { return GetInstance()->m_fMaxSEVolume * GetInstance()->m_fMasterVolume; }
//========================================================================================
//	BGM
//====
	// スレッドを作ってBGMを再生する関数(外部呼出).
	static void ThreadPlayBGM(const std::string& Name);
	// BGM停止関数.
	static void StopBGM(const std::string Name);
	// BGMフェードアウト関数.
	static void FadeOutBGM(const std::string Name, const bool StopFadeIn = true);
	// BGMフェードイン関数.
	static void FadeInBGM(const std::string Name, const bool StopFadeOut = true);
	// BGMの音量取得関数.
	static float GetBGMVolume(const std::string Name);
	// BGMの音量セット関数.
	static void SetBGMVolume(const std::string Name, const float Volume);
	// BGMを止めて、スレッドを放棄する関数.
	static bool StopBGMThread(const std::string BGMName);
	// 指定した名前のBGMスレッドが再生中かどうかを返す : 再生中ならtrue.
	static bool GetIsPlayBGM(const std::string BGMName);
	// BGMのピッチ設定.
	static void SetBGMPitch(const std::string Name, const float Value);
//========================================================================================
//	SE
//====
	// SE再生関数.
	static void PlaySE(const std::string& Name);
	// 多重再生しないSE再生関数.
	static void NoMultipleSEPlay(const std::string& Name);
	// SE停止関数.
	static void StopSE(const std::string Name, const size_t ArrayNum);
	// 同じ音源の全てのSEソース停止関数.
	static void StopAllSE(const std::string Name);
	// SEのVoiceソース内最大音量を設定.
	static void SetSEVolume(const std::string Name,const float Volume);
	// 指定した名前のSEが再生中かどうかを返す : 再生中ならtrue.
	static bool GetIsPlaySE(const std::string Name,const size_t ArrayNum);
	// 全体SE音量とは別のソース毎の音量をセット.
	static void SetAnotherSEVolume(const std::string Name, const float& Volume);
	// ソース内で設定されている音量を適用するかどうか.
	static void SetUseAnotherSEVolumeFlag(const std::string Name, const bool& bFlag);

	// 解放処理関数.
	static void Release();
//========================================================================================
// オプション画面用全ソースのサウンド調整.
// ループで呼ぶべきではない.
//====
	// マスター音量セット.
	static void SetMasterVolume(float& MasterVolume);
	// BGMの音量を変更できるようにする.
	static void SetCanChangeBGMVolumeFlag();
	// SEの音量を変更できるようにする.
	static void SetCanChangeSEVolumeFlag();
	// BGM,SEの音量を変更できないように.
	static bool InitChangeSoundVolumeFlag();

	// 個別でSEの音量を変更できるようにする.
	static void SetSelectChangeSEVolumeFlag(std::string& sName, const bool& bFlag);
//===========================================================
// オプションで、音量を可視化する際に(今回はMAX10個)、オプションクラスが破壊されてもその値を維持する為の関数.
	// マスター音量数画像として描画する画像数のセット.
	static void SetDispMasterVolumeCount(const int Master) { GetInstance()->m_MasterCount = Master; }
	// BGM音量数画像として描画する画像数のセット.
	static void SetDispBGMVolumeCount(const int BGM) { GetInstance()->m_BGMCount = BGM; }
	// SE音量数画像として描画する画像数のセット.
	static void SetDispSEVolumeCount(const int SE) { GetInstance()->m_SECount = SE; }
	// マスター音量数画像として描画する画像数の取得関数.
	static const int GetDispMasterVolumeCount()	{ return GetInstance()->m_MasterCount; }
	// BGM音量数画像として描画する画像数の取得関数.
	static const int GetDispBGMVolumeCount()		{ return GetInstance()->m_BGMCount; }
	// SE音量数画像として描画する画像数の取得関数.
	static const int GetDispSEVolumeCount()		{ return GetInstance()->m_SECount; }
	static void StateChangeVolumeThread(const bool& bFlag);
public:
	float m_fMasterVolume;	// マスター音量.
	float m_fMaxBGMVolume;	// BGM音量.	
	float m_fMaxSEVolume;	// SE音量.
private:
	// BGM再生関数.
	static void PlayBGM(const std::string Name, bool& isEnd);
private:


	// Wavデータクラスの箱.
	std::unordered_map <std::string, std::shared_ptr<clsXAudio2WaveLoad>> pWaveData;

	// Oggデータクラスの箱
	std::unordered_map <std::string, std::shared_ptr<COggLoad>>	m_pOggWavData;

	// BGM用PlaySoundクラスの箱.
	std::unordered_map <std::string, std::shared_ptr<CXAudio2PlayBGM>> pBgmSource;
	// BGMストリーミング再生用スレッド.
	std::unordered_map <std::string, std::thread> pBGMThread;
	// スレッド内で再生しているBGMを停止するためのフラグ.
	std::unordered_map <std::string, bool> m_bisEndThread;
	// スレッドリリース用関数.
	std::unordered_map <std::string, bool> m_bisThreadRelease;
	// 各スレッドID保持.
	std::unordered_map <std::string, std::thread::id> InThreadID;


	// BGMの名前リスト.
	// 解放時に必要.
	std::vector<std::string>	m_vsBGMNameList;
	// SEの名前リスト.
	// 解放時に必要.
	std::vector<std::string>	m_vsSENameList;
	// SE用PlaySoundクラスの箱.
	std::unordered_map <std::string, std::shared_ptr<CXAudio2PlaySE>> pSeSource;
	// 終了用フラグ.
	bool	m_bEndGame;
	// BGM,SEの作成が終わった後かどうか.
	bool	m_bEndCreate;

//=================================================
// 以下オプション画面用.
// オプション画面がない場合はいらない.
//====
	int		m_MasterCount;		// オプション画面作成時、マスター音量設定を実装する場合の音量描画に使用.
	int		m_BGMCount;			// オプション画面作成時、BGM音量設定を実装する場合の音量描画に使用.
	int		m_SECount;			// オプション画面作成時、SE音量設定を実装する場合の音量描画に使用.
	// 以下二つはスレッドで回さないとストリーミングの入れ込みを待ってからの変更になるため
	// スレッドに回す.
	std::thread m_BGMVolume;	// オプション画面で、BGMの音量を同時に変更するためのスレッド.
	std::thread m_SEVolume;		// オプション画面で、SEの音量を同時に変更するためのスレッド.

	bool	m_bMoveBGMThread;	// オプション画面で、BGMを設定と同時に変更するためのスレッド用変数.
	bool	m_bMoveSEThread;	// オプション画面で、Seを設定と同時に変更するためのスレッド用変数.

	bool	m_bResumeBGMThread;	// 条件変数.
	bool	m_bResumeSEThread;	// 条件変数.
	std::mutex m_BGMmtx;
	std::condition_variable m_BGMCv;
	std::mutex m_SEmtx;
	std::condition_variable m_SECv;
private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CSoundManager(const CSoundManager &) = delete;
	CSoundManager& operator = (const CSoundManager &) = delete;
	CSoundManager(CSoundManager &&) = delete;
	CSoundManager& operator = (CSoundManager &&) = delete;
};



#endif // #ifndef C_SOUNDMANAGER_H.
