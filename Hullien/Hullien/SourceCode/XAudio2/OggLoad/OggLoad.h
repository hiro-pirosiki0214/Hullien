#ifndef OGGLOAD_H
#define OGGLOAD_H

#include "..\XAudio2.h"
#include <string>
#include <vorbis\vorbisfile.h>

namespace OggOperatorNS
{
	const unsigned int requestSize = 4096;	// 読み込み単位
	// 一度にメモリに読み込むファイルサイズ 1M
	const int pcmSize = 1024 * 1000;
}
class COggLoad
{
private:
	OggVorbis_File m_OggVF;			// OggVorbis構造体.


	FILE *m_pFile;					// ファイルを入れる変数.
	bool m_isReady;					// ファイルがオープン済みならtrue.
	std::string m_sFileName;		// ファイル名.
	unsigned int m_ChannelNumber;	// WAVEFORMATEXに渡すチャンネル.
	unsigned int m_SamplingRate;	// WAVEFORMATEXに渡すサンプリングレート.
	unsigned int m_BitRate;			// WAVEFORMATEXに渡すビットレート.
//----------------------------------
// SEオンリー用.
//----
	// SEファイルのサイズ
	DWORD seFileSize;
	// SEファイルの読み込み用バッファー
	BYTE* seWaveBuffer;

	WAVEFORMATEX* m_WaveFormat;		// WAVEFORMATEX構造体.		
	DWORD m_Size;					// Wavファイルのサイズを入れる変数.	

private:
	// セグメントデータ取得
	virtual bool GetSegment(char* buffer, unsigned int size, int* writeSize, bool* isEnd);
	bool CreateWaveFormatEx();					// Wavフォーマットデータ作成.
	void SetFileName(std::string sName);		// ファイルネームセット.

	void SetChannelNumber(int value) { m_ChannelNumber = value; }	// WAVEFORMATEXに渡すチャンネルセット関数.
	void SetSamplingRate(int value) { m_SamplingRate = value; }		// WAVEFORMATEXに渡すサンプリングレートセット関数.
	void SetBitRate(int value) { m_BitRate = value; }				// WAVEFORMATEXに渡すビットレートセット関数.
public:
	COggLoad();
	virtual ~COggLoad();
	// oggデータ作成.
	HRESULT CreateOggData(std::string sFileName,const bool& isSE = false, bool isBuffered = false);

	// ファイルがオープン済みかどうかを取得.
	bool GetReady() { return m_isReady; }
	// ファイルの名前を取得.
	const char* GetFileName() { return m_sFileName.c_str(); }
	 
	// ファイルを開く
	HRESULT Open(std::string sFileName,bool isBuffered = false);
	// ファイルを閉じる
	HRESULT Close();
	// ファイルを読み込む
	HRESULT Read(BYTE* buffer, DWORD size, DWORD* readSize);
	// ファイルに書き込む
	HRESULT Write(UINT writeSize, BYTE* data, UINT wroteSize);
	// チャンクデータを読み込み(細切れにでーたを読みこみ)
	HRESULT ReadChunk(BYTE** buffer, int bufferCount, const int len, int* readSize);
	// サイズを取得
	DWORD GetSize();
	// ファイルの位置をリセット
	HRESULT ResetFile();
	WAVEFORMATEX* GetFormat() { return m_WaveFormat; }
//------------------------------------
// SEオンリー.
//----
	BYTE* GetSEWaveBuffer() { return seWaveBuffer; }
	DWORD GetSEFileSize() { return seFileSize; }
};




#endif // #ifndef OGGLOAD_H.