#ifndef MY_LOG_H
#define MY_LOG_H

#include <Windows.h>
#include <memory>
#include <string>
#include <time.h>

class CLog
{
	typedef tm TIME_DATA;
	const char* LOG_TEXT_FILE_PATH = "Data\\Log.txt";	// ���O�e�L�X�g�̃t�@�C���p�X.

public:
	CLog();
	~CLog();

	// �C���X�^���X�̎擾.
	static CLog* GetInstance();

	// ���O�e�L�X�g�t�@�C�����쐬.
	// �A�v���N�����ɍ쐬����.
	static HRESULT OpenLogTextFile();

	// ���O�e�L�X�g�t�@�C�������.
	static HRESULT CloseLogTextFile();

	// ���O�̓���.
	static HRESULT Print( const std::string& message );

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CLog( const CLog & )				= delete;
	CLog& operator = ( const CLog & )	= delete;
	CLog( CLog && )						= delete;
	CLog& operator = ( CLog && )		= delete;
};

#endif	// #ifndef MY_LOG_H.