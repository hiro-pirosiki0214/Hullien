#ifndef OGGLOAD_H
#define OGGLOAD_H

#include "..\XAudio2.h"
#include <string>
#include <vorbis\vorbisfile.h>

namespace OggOperatorNS
{
	const unsigned int requestSize = 4096;	// �ǂݍ��ݒP��
	// ��x�Ƀ������ɓǂݍ��ރt�@�C���T�C�Y 1M
	const int pcmSize = 1024 * 1000;
}
class COggLoad
{
private:
	OggVorbis_File m_OggVF;			// OggVorbis�\����.


	FILE *m_pFile;					// �t�@�C��������ϐ�.
	bool m_isReady;					// �t�@�C�����I�[�v���ς݂Ȃ�true.
	std::string m_sFileName;		// �t�@�C����.
	unsigned int m_ChannelNumber;	// WAVEFORMATEX�ɓn���`�����l��.
	unsigned int m_SamplingRate;	// WAVEFORMATEX�ɓn���T���v�����O���[�g.
	unsigned int m_BitRate;			// WAVEFORMATEX�ɓn���r�b�g���[�g.
//----------------------------------
// SE�I�����[�p.
//----
	// SE�t�@�C���̃T�C�Y
	DWORD seFileSize;
	// SE�t�@�C���̓ǂݍ��ݗp�o�b�t�@�[
	BYTE* seWaveBuffer;

	WAVEFORMATEX* m_WaveFormat;		// WAVEFORMATEX�\����.		
	DWORD m_Size;					// Wav�t�@�C���̃T�C�Y������ϐ�.	

private:
	// �Z�O�����g�f�[�^�擾
	virtual bool GetSegment(char* buffer, unsigned int size, int* writeSize, bool* isEnd);
	bool CreateWaveFormatEx();					// Wav�t�H�[�}�b�g�f�[�^�쐬.
	void SetFileName(std::string sName);		// �t�@�C���l�[���Z�b�g.

	void SetChannelNumber(int value) { m_ChannelNumber = value; }	// WAVEFORMATEX�ɓn���`�����l���Z�b�g�֐�.
	void SetSamplingRate(int value) { m_SamplingRate = value; }		// WAVEFORMATEX�ɓn���T���v�����O���[�g�Z�b�g�֐�.
	void SetBitRate(int value) { m_BitRate = value; }				// WAVEFORMATEX�ɓn���r�b�g���[�g�Z�b�g�֐�.
public:
	COggLoad();
	virtual ~COggLoad();
	// ogg�f�[�^�쐬.
	HRESULT CreateOggData(std::string sFileName,const bool& isSE = false, bool isBuffered = false);

	// �t�@�C�����I�[�v���ς݂��ǂ������擾.
	bool GetReady() { return m_isReady; }
	// �t�@�C���̖��O���擾.
	const char* GetFileName() { return m_sFileName.c_str(); }
	 
	// �t�@�C�����J��
	HRESULT Open(std::string sFileName,bool isBuffered = false);
	// �t�@�C�������
	HRESULT Close();
	// �t�@�C����ǂݍ���
	HRESULT Read(BYTE* buffer, DWORD size, DWORD* readSize);
	// �t�@�C���ɏ�������
	HRESULT Write(UINT writeSize, BYTE* data, UINT wroteSize);
	// �`�����N�f�[�^��ǂݍ���(�א؂�ɂŁ[����ǂ݂���)
	HRESULT ReadChunk(BYTE** buffer, int bufferCount, const int len, int* readSize);
	// �T�C�Y���擾
	DWORD GetSize();
	// �t�@�C���̈ʒu�����Z�b�g
	HRESULT ResetFile();
	// �t�@�C���̈ʒu���w�肵�Đi�߂�.
	HRESULT SeekFile(double& Second);
	// �t�@�C���̌��݈ʒu(�b)��Ԃ�.
	double GetFileFrame();

	WAVEFORMATEX* GetFormat() { return m_WaveFormat; }
//------------------------------------
// SE�I�����[.
//----
	BYTE* GetSEWaveBuffer() { return seWaveBuffer; }
	DWORD GetSEFileSize() { return seFileSize; }
};




#endif // #ifndef OGGLOAD_H.