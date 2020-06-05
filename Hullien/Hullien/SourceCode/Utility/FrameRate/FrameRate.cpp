#include "FrameRate.h"
#include <thread>
#include <chrono>

double CFrameRate::FRAME_TIME = 0.0;
double CFrameRate::m_OutFPS = 0.0;

CFrameRate::CFrameRate( double fps )
	: m_FrameTime	( 0.0 )
	, m_StartTime	()
	, m_NowTime		()
	, m_FreqTime	()
{
	FRAME_TIME = 1.0 / fps;
	QueryPerformanceFrequency( &m_FreqTime );
	QueryPerformanceCounter( &m_StartTime );
}

CFrameRate::~CFrameRate()
{
}

void CFrameRate::Wait()
{
	// 現在の時間を取得.
	QueryPerformanceCounter( &m_NowTime );
	// (今の時間 - 前フレームの時間) / 周波数 = 経過時間(秒単位).
	m_FrameTime =
		(static_cast<double>(m_NowTime.QuadPart) - static_cast<double>(m_StartTime.QuadPart)) / 
		static_cast<double>(m_FreqTime.QuadPart);

	m_OutFPS = 0.0;
	// 処理時間に余裕がある場合はその分待機.
	if( m_FrameTime < FRAME_TIME ){
		// 待機時間を計算.
		DWORD sleepTime =
			static_cast<DWORD>((FRAME_TIME - m_FrameTime) * 1000.0);

		timeBeginPeriod(1);
		Sleep( sleepTime );	// 待機.
		timeEndPeriod(1); 

		return;
	}
	m_OutFPS = 1.0/m_FrameTime;

	m_StartTime = m_NowTime;
}