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
// スタートされていないソースボイスを探す.
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
// 再生されていないソースボイスを探す.
int CXAudio2PlaySE::CheckPlaySeBuffer()
{
	XAUDIO2_VOICE_STATE state;
	// 再生していないSEVoiceを探す.
	for (int Array = 0; Array < SE_VOICE_SIZE; Array++) {
		m_pSourceVoice[Array]->GetState(&state);
		if (state.BuffersQueued > 0)	// バッファが0より大きい.
		{
		}
		else {
			return Array;	// 再生されていない配列の値を返す.
		}
	}
	// すべてサウンド再生途中なので、リピート再生する.
	if (m_RepeatPlaySe < SE_VOICE_SIZE) {
		m_RepeatPlaySe++;
	}
	if (m_RepeatPlaySe == SE_VOICE_SIZE) {
		m_RepeatPlaySe = 0;
	}

	return m_RepeatPlaySe;
}

// 指定された配列番号のSEが再生されているか.
const bool CXAudio2PlaySE::IsPlayingSE(size_t ArrayNum)
{
	// SoundSourceがない場合リターン 0(再生していないときと同じ).
	if (m_pSourceVoice[ArrayNum] == nullptr) return 0;
	XAUDIO2_VOICE_STATE xState;
	// SoundSourceのステート取得.
	m_pSourceVoice[ArrayNum]->GetState(&xState);

	return xState.BuffersQueued != 0;
}

bool CXAudio2PlaySE::Play(
	std::shared_ptr<COggLoad> pWavData)
{
	HRESULT hr;
	int Array = 0;

	if (m_pSourceVoice[0] == nullptr) return true;

	// オプション画面中、ボイスソース音量も全体のSE音量に合わせて上げ下げに合わせられるように.
	if (m_bCanChangeSE == false) {
		if (m_fSEVolume >= CSoundManager::GetInstance()->m_stSound.SEVolume) {
			m_fSEVolume = CSoundManager::GetInstance()->m_stSound.SEVolume;
		}
	}
	else {
		m_fSEVolume = CSoundManager::GetInstance()->m_stSound.SEVolume;
	}

	// このSEが再生されていなければ中に入り再生する.
	if (m_isSePlayVoice[0] == false) {
		if (m_pSourceVoice[0] == nullptr) return true;
		m_isSePlayVoice[0] = true;
		m_pSourceVoice[0]->Start();	// ソースボイス再生.
	}
	if (m_bisAnotherVolume == true) {
		SetSEVolume(m_fAnotherVolume, 0);	// SEに全体とは別の音量セット.
	}
	else {
		SetSEVolume(m_fSEVolume, 0);		// SEの音量セット.
	}
	XAUDIO2_VOICE_STATE state;
	if (m_pSourceVoice[0] == nullptr) return true;
	m_pSourceVoice[0]->GetState(&state);
	if (state.BuffersQueued > 0)	// バッファが0より大きい.
	{
		// -1なら全ソース再生済み.
		if (m_CheckNonePlaySeVoice != -1) CheckPlaySeState();

		if (m_CheckNonePlaySeVoice != -1) {
			m_isSePlayVoice[m_CheckNonePlaySeVoice] = true;
			m_pSourceVoice[m_CheckNonePlaySeVoice]->Start();	// ソースボイス再生.
		}

		Array = CheckPlaySeBuffer();		// 再生されていないソースボイスの配列番号を返す.

		m_pSourceVoice[Array]->Stop();		// ソースボイス停止.
		m_pSourceVoice[Array]->FlushSourceBuffers();// ソースバッファをクリア.
		m_pSourceVoice[Array]->Start();		// ソースボイス再生.
		if (m_bisAnotherVolume == true) {
			SetSEVolume(m_fAnotherVolume, Array);	// SEに全体とは別の音量セット.
		}
		else {
			SetSEVolume(m_fSEVolume, Array);		// SEの音量セット.
		}
	}
	else {
		m_RepeatPlaySe = 0;	// リピートして再生する配列を0からにする.
	}
	// バッファーを生成する
	XAUDIO2_BUFFER seBuffer = { 0 };
	seBuffer.pAudioData = pWavData->GetSEWaveBuffer();
	seBuffer.AudioBytes = pWavData->GetSEFileSize();

	// キューにバッファーを投入する
	if (FAILED(hr = m_pSourceVoice[Array]->SubmitSourceBuffer(&seBuffer)))
	{
		return true;
	}

	return true;
}
// SEを多重再生しないで再生.
// ※もとからなっているSEを止めて一つにするものではない.
bool CXAudio2PlaySE::NoMultiplePlay(std::shared_ptr<COggLoad> pWavData)
{
	HRESULT hr;
	// オプション画面中、ボイスソース音量も全体のSE音量に合わせて上げ下げに合わせられるように.
	if (m_bCanChangeSE == false) {
		if (m_fSEVolume >= CSoundManager::GetInstance()->m_stSound.SEVolume) {
			m_fSEVolume = CSoundManager::GetInstance()->m_stSound.SEVolume;
		}
	}
	else {
		m_fSEVolume = CSoundManager::GetInstance()->m_stSound.SEVolume;
	}


	// このSEが再生されていなければ中に入り再生する.
	if (m_isSePlayVoice[0] == false) {
		if (m_pSourceVoice[0] == nullptr) return true;
		m_isSePlayVoice[0] = true;
		m_pSourceVoice[0]->Start();	// ソースボイス再生.
	}
	if (m_bisAnotherVolume == true) {
		SetSEVolume(m_fAnotherVolume, 0);	// SEに全体とは別の音量セット.
	}
	else {
		SetSEVolume(m_fSEVolume, 0);		// SEの音量セット.
	}

	XAUDIO2_VOICE_STATE state;
	m_pSourceVoice[0]->GetState(&state);
	if (state.BuffersQueued > 0)	// バッファが0より大きい.
	{
		m_pSourceVoice[0]->Stop();		// ソースボイス停止.
		m_pSourceVoice[0]->FlushSourceBuffers();// ソースバッファをクリア.
		m_pSourceVoice[0]->Start();		// ソースボイス再生.
	}

	// バッファーを生成する
	XAUDIO2_BUFFER seBuffer = { 0 };
	seBuffer.pAudioData = pWavData->GetSEWaveBuffer();
	seBuffer.AudioBytes = pWavData->GetSEFileSize();

	// キューにバッファーを投入する
	if (FAILED(hr = m_pSourceVoice[0]->SubmitSourceBuffer(&seBuffer)))
	{
		return true;
	}

	return true;

}

// SE停止.
bool CXAudio2PlaySE::SeStop(const size_t ArrayNum)
{
	if (m_pSourceVoice[ArrayNum] == nullptr) return true;

	m_pSourceVoice[ArrayNum]->Stop(0);
	m_pSourceVoice[ArrayNum]->FlushSourceBuffers();
	m_isSePlayVoice[ArrayNum] = false;
	return true;
}
// 作成してる予備分のSEも停止.
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
	//一回データが作られていたらリターン.
	if (m_pOggData != nullptr) return true;
	m_pOggData = pOggData.get();
	HRESULT hr;
	// XAudio2マスターボイスのインスタンス取得.
	clsXAudio2MasterVoice& xAudioMaster = clsXAudio2MasterVoice::GetGlobalSystem();

	IXAudio2*   pHandle = xAudioMaster.GetInterface();

	// ソースボイスを作成する.
	for (size_t Array = 0; Array < SE_VOICE_SIZE; Array++) {
		m_pSourceVoice[Array] = nullptr;
		if (FAILED(hr = pHandle->CreateSourceVoice(&m_pSourceVoice[Array], m_pOggData->GetFormat())))
		{
			// 失敗した.
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
// SE音量設定.
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
