// ogg_int64_t �� DWORD�^�ɓ���鎞�̌x��������.
#pragma warning( disable: 4244 )


#include "OggLoad.h"
#include <memory.h>
#include <crtdbg.h>
#include <tchar.h>
COggLoad::COggLoad()
{
	m_WaveFormat			= nullptr;
	m_Size					= 0;
	m_isReady				= false;
	seWaveBuffer			= nullptr;
	seFileSize				= 0;
}

COggLoad::~COggLoad()
{
	Close();
}
//-------------------------------------------�
// ogg�f�[�^�쐬.
//----
HRESULT COggLoad::CreateOggData(std::string sFileName, const bool& isSE, bool isBuffered)
{
	HRESULT ret;
	if (FAILED(ret = Open(sFileName,isBuffered)))
	{
		_ASSERT_EXPR("error ogg file open ret=%d\n", ret);
		return E_FAIL;
	}

	if (isSE == true) {
		// SE�t�@�C���̃T�C�Y
		seFileSize = GetSize();
		// SE�t�@�C���̓ǂݍ��ݗp�o�b�t�@�[
		seWaveBuffer = new BYTE[seFileSize];

		// SE�t�@�C���̃f�[�^���o�b�t�@�[�ɓǂݍ���
		if (FAILED(ret = Read(seWaveBuffer, seFileSize, &seFileSize))) {
			_ASSERT_EXPR("error read SE File %#X\n", ret);
			return E_FAIL;
		}
	}

	ret = S_OK;

	return ret;
}

//-------------------------------------------�
// �t�@�C�����J��.
//----
HRESULT COggLoad::Open(std::string sFileName, bool isBuffered)
{
	HRESULT hr;
	errno_t error;
	

	// �t�@�C�����`�F�b�N
	if (sFileName == "") return E_INVALIDARG;

	// BGM�f�[�^������f�B���N�g���p�X������.
	std::string BGMPass = "Data\\Sound\\BGM\\" + sFileName;
	BGMPass += ".ogg";
	LPSTR CastLPSTR = const_cast<char*>(BGMPass.c_str());

	// CastLPSTR���G���[�l�Ȃ�SE�̃t�@�C���̖��O�̉\��������̂ŁASE�̃f�B���N�g���p�X�Ō���.
	if (_tfopen_s(&m_pFile, CastLPSTR, _T("rb")) != 0) {
		std::string SEPass = "Data\\Sound\\SE\\" + sFileName;
		SEPass += ".ogg";
		LPSTR CastLPSTR = const_cast<char*>(SEPass.c_str());
		if (_tfopen_s(&m_pFile, CastLPSTR, _T("rb")) != 0) {
			// �f�[�^�����݂��Ȃ�.
			return E_FAIL;
		}
	}

	error = ov_open(m_pFile, &m_OggVF, nullptr, 0);
	if (error != 0)
	{
		return E_FAIL;
	}

	// ogg�t�@�C�������擾���ăZ�b�g
	vorbis_info *info = ov_info(&m_OggVF, -1);
	SetFileName(sFileName);
	SetChannelNumber(info->channels);
	// bitrate��16�ɂ��Ă���
	SetBitRate(16);
	SetSamplingRate(info->rate);
	// �Z�b�g�������g���ăt�H�[�}�b�g�t�@�C���쐬
	if (!CreateWaveFormatEx())
		return E_FAIL;
	// �T�C�Y���v�Z���ē���Ă���
	m_Size = info->channels * 2 * ov_pcm_total(&m_OggVF, -1);

	// �t�@�C���̏������o����
	m_isReady = true;
	hr = S_OK;
	return hr;
}

//-------------------------------------------�
// wave�t�@�C���̃T�C�Y��Ԃ�.
//----
DWORD COggLoad::GetSize()
{
	return m_Size;
}

//-------------------------------------------�
// �t�@�C���̓ǂݎ��|�C���^�[�����Z�b�g����.
// = ������Ă񂾂珉�߂���Đ�.
//----
HRESULT COggLoad::ResetFile()
{
	if (!GetReady())
		return E_FAIL;

	ov_time_seek(&m_OggVF, 0.0);

	return S_OK;
}
HRESULT COggLoad::SeekFile(double & Second)
{
	if (!GetReady())
		return E_FAIL;
	//ov_clear(&m_OggVF);

	// �t�@�C���̍ŏI�Đ����Ԃ��傫����΁A�ŏ��ɖ߂�.
	if (Second >= ov_time_total(&m_OggVF, 0)) {
		ov_time_seek(&m_OggVF, 0.00000000);
	}
	else {
		ov_time_seek(&m_OggVF, Second);
	}
	return S_OK;
}
double COggLoad::GetFileFrame()
{
	return ov_time_tell(&m_OggVF);
}
//-------------------------------------------�
// �t�@�C�����e�����ׂăo�b�t�@�ɓǂݍ���.
//----
HRESULT COggLoad::Read(BYTE* buffer, DWORD size, DWORD* readSize)
{
	// �ǂݍ��ރo�b�t�@�[���������Ɋ��蓖��
	memset(buffer, 0, this->m_Size);
	char *tmpBuffer = new char[this->m_Size];
	int bitStream = 0;
	int tmpReadSize = 0;
	int comSize = 0;

	*readSize = 0;
	while (1)
	{
		tmpReadSize = ov_read(&m_OggVF, (char*)tmpBuffer, 4096, 0, 2, 1, &bitStream);
		// �ǂݍ��݃T�C�Y�����Ă���A�t�@�C�������܂œǂݍ��݂��Ă���break
		if (comSize + tmpReadSize >= static_cast<int>(this->m_Size) || tmpReadSize == 0 || tmpReadSize == EOF)
			break;
		// �������[�̃o�b�t�@�[�Ƀ|�C���^�[�ʒu���炵�Ȃ��珑������
		memcpy(buffer + comSize, tmpBuffer, tmpReadSize);
		comSize += tmpReadSize;
	}


	// �ǂݍ��񂾃T�C�Y���Z�b�g
	*readSize = comSize;
	// ��n��
	delete[] tmpBuffer;

	return S_OK;

}

//--------------------------------------------
// �w��̃T�C�Y�܂Ŗ��߂��Z�O�����g�f�[�^���擾����.
//----
bool COggLoad::GetSegment(char* buffer, unsigned int size, int* writeSize, bool* isEnd)
{
	// �t�@�C���̏������I����ĂȂ���ΏI��
	if (GetReady() == false)
		return false;

	// �o�b�t�@�[�̎w��`�F�b�N
	if (buffer == 0)
	{
		if (isEnd) *isEnd = true;
		if (writeSize) *writeSize = 0;
		return false;
	}

	if (isEnd) *isEnd = false;

	// ��������Ƀo�b�t�@�[�̈���m��
	memset(buffer, 0, size);
	unsigned int requestSize = OggOperatorNS::requestSize;
	int bitStream = 0;
	int readSize = 0;
	unsigned int comSize = 0;
	bool isAdjust = false; // �[���̃f�[�^�̒������t���O

	// �w��T�C�Y���\��T�C�Y��菬�����ꍇ�͒������Ƃ݂Ȃ�
	if (size < requestSize)
	{
		requestSize = size;
		isAdjust = true;
	}

	// �o�b�t�@�[���w��T�C�Y�Ŗ��߂�܂ŌJ��Ԃ�
	while (1)
	{
		readSize = ov_read(&m_OggVF, (char*)(buffer + comSize), requestSize, 0, 2, 1, &bitStream);
		// �t�@�C���G���h�ɒB����
		if (readSize == 0)
		{
			// �I��
			if (isEnd) *isEnd = true;
			if (writeSize) *writeSize = comSize;
			return true;
		}

		// �ǂݎ��T�C�Y�����Z
		comSize += readSize;

		_ASSERT(comSize <= size);	// �o�b�t�@�I�[�o�[

		// �o�b�t�@�𖄂ߐs������
		if (comSize >= size)
		{
			if (writeSize) *writeSize = comSize;
			return true;
		}

		// �[���f�[�^�̒���
		if (size - comSize < OggOperatorNS::requestSize)
		{
			isAdjust = true;
			requestSize = size - comSize;
		}

	}

	// �G���[
	if (writeSize) *writeSize = 0;
	return false;
}

//--------------------------------------------
// �t�@�C������`�����N�f�[�^��ǂݍ���Ńo�b�t�@�[�����.
//----
HRESULT COggLoad::ReadChunk(BYTE** buffer, int bufferCount, const int len, int* readSize)
{
	if (readSize != NULL)
		*readSize = 0;
	if (buffer == NULL || buffer == nullptr) return E_FAIL;


	// �t�@�C������w�肳�ꂽ�T�C�Y�����f�[�^��ǂݎ��
	bool result = GetSegment((char *)buffer[bufferCount], len, readSize, 0);
	if (result == false)
		return E_FAIL;

	return S_OK;
}

//--------------------------------------------
// �f�[�^���.
//----
HRESULT COggLoad::Close()
{

	ov_clear(&m_OggVF);	// OggVorbis_File�\���̂��N���A.
	if (m_pFile)		// �t�@�C�����J����Ă���.
	{
		fclose(m_pFile);// �t�@�C�������.
		m_pFile = nullptr;	// ���g��nullptr.
	}
	// seWaveBuffer��nullptr�o�Ȃ��ꍇ�Z�[�t���.
	if (seWaveBuffer != nullptr) {
		SAFE_DELETE(seWaveBuffer);
	}
	SAFE_DELETE(m_WaveFormat);	// WAVEFORMATEX�̉��.

	return S_OK;
}

//--------------------------------------------
// Wave�t�@�C���Ƀf�[�^����������.
//----
HRESULT COggLoad::Write(UINT writeSize, BYTE* data, UINT wroteSize)
{
	return E_FAIL;
}

//--------------------------------------------
// ogg�t�@�C�������Z�b�g����..
//----
void COggLoad::SetFileName(std::string sName)
{
	m_sFileName = sName;
}

//--------------------------------------------
// Wav�t�H�[�}�b�g�f�[�^�쐬.
//----
bool COggLoad::CreateWaveFormatEx()
{
	m_WaveFormat = nullptr;
	m_WaveFormat = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
	if (nullptr == m_WaveFormat)
		return false;

	m_WaveFormat->wFormatTag = WAVE_FORMAT_PCM;
	m_WaveFormat->nChannels = m_ChannelNumber;
	m_WaveFormat->nSamplesPerSec = m_SamplingRate;
	m_WaveFormat->wBitsPerSample = m_BitRate;
	m_WaveFormat->nBlockAlign = m_ChannelNumber * m_BitRate / 8;
	m_WaveFormat->nAvgBytesPerSec = m_WaveFormat->nSamplesPerSec * m_WaveFormat->nBlockAlign;
	m_WaveFormat->cbSize = 0;

	return true;
}
