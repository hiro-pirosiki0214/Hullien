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
	// コンストラクタ.
	CXAudio2PlayBGM();

	// デストラクタ.
	virtual ~CXAudio2PlayBGM();

	// 再生してるかどうか取得関数.
	const bool IsPlaying();

	// サウンド再生、前に一時停止されていた場合は記憶位置から再開.
	bool Play(std::shared_ptr< COggLoad> pWavData, const char* filename, bool& isEnd);

	// BGMサウンドを完全停止.(位置を保持しない).
	bool Stop();
	// サウンド一時停止.
	bool Pause();

	// ピッチ設定関数.
	bool SetPitch(float value);

	// ピッチを取得関数.
	const float GetPitch();

	// 最大ピッチを設定します.
	void  SetMaxPitch(float value);
	// ピッチを設定.
	void SetPitchValue(const float value) { m_fPitch = value; }

	HRESULT CreateOggSound(std::shared_ptr<COggLoad> pOggData, const char* filename);
	//=======================================================================
	// 基本音量操作系.
	//====
	// BGMの音量を取得します.
	const float GetVolume();
	// BGMの音量を設定します.
	bool SetBGMVolume(float value);
	// TrueでBGMの音量を直接触れるようにする.
	void SetOperationBGMVolumeFlag(const bool& bFlag) { m_bCanChangeVolume = bFlag; }

	// 現在の音量から加算または減算.
	bool  AdjustVolume(float value);
	// BGMをフェードアウトする関数.
	bool FadeOutBGM(float value, bool& isEmergencyCall);
	// BGMをフェードインする関数.
	bool FadeInBGM(float value, bool& isEmergencyCall);
	// SoundSourceを破壊する関数.
	void DestoroySource();
	// フェードアウトフラグのセッター.
	void SetFadeOutFlag(bool BGMFadeOutFlag) { m_bFadeOutStart = BGMFadeOutFlag; }
	// フェードアウトフラグのゲッター.
	bool GetFadeOutFlag() { return m_bFadeOutStart; }
	// フェードインフラグのセッター.
	void SetFadeInFlag(bool BGMFadeInFlag) { m_bFadeInStart = BGMFadeInFlag; }
	// フェードインフラグのゲッター.
	bool GetFadeInFlag() { return m_bFadeInStart; }
protected:
	//! 音源データをストリームに流し込みます.
	//  この関数は外部から呼び出せません.
	HRESULT OggSubmit(std::shared_ptr<COggLoad> pOggData, const char* filename);
protected:
	std::mutex				m_mtx;
	COggLoad*				m_pOggData; // Oggデータロードクラス.
	VoiceCallback			m_Callback;	// コールバック.

	XAUDIO2_BUFFER		m_Buffer;		// オーディオバッファー
	int					m_Buff_len;		// バッファレンジ.
	BYTE**				m_Buff;			// バッファサイズ.
	int					m_Len;			// レンジ.
	int					m_Buff_cnt;		// バッファカウント.
	int					m_Size;

	IXAudio2SourceVoice*   m_pSourceVoice; //  サウンドを再生するストリーム.
	bool  m_bCanChangeVolume;// BGM音量を外部からフェード以外で触れるかどうか.

	float m_fMaxPitch;		// 最大ピッチ.
	float m_fPitch;			// ピッチ.
	bool m_bFadeOutStart;	// フェードアウト開始フラグ.
	bool m_bFadeInStart;	// フェードイン開始フラグ.

	bool					m_bFirstPlay;	// 初回再生かどうか.
};

#endif // !C_XAUDIO2_PLAYSOUND_H