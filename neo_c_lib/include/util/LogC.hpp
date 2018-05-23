/*****************************************************************************************
 파 일 명 : LogC.hpp
 작 성 자 : 김지훈
 기    능 : log관련 Library Header file
 작성일자 : 2004년  4월  일
 변경일자 :     년   월    일
 관련화일 : LogC.cpp
 컴파일러 :
 기    타 :
*****************************************************************************************/

#ifndef		_LOG_CLASS_HEADER_
#define		_LOG_CLASS_HEADER_

#include <stdio.h>
#include <stdarg.h>

//#if defined(_LSA_AIX_) && defined(_FORCE_PTHREAD_TYPE_DEFINE_)
//	#include "OsDep/pthread_xopen600_types.h"
//#endif

//#include <pthread.h>

#define	FILE_LOG	1					//log file생성
#define SCREEN_LOG	2					//log 화면 출력
#define BOTH_LOG	3					//file & 화면 출력

#define LEVEL_DEBUG	0
#define LEVEL_INFO	1
#define LEVEL_WARN	2
#define LEVEL_ERROR	3
#define LEVEL_TEST 	99
#define LEVEL_TRACE	100


class LogC
{
private :
	FILE *m_fpLogFile;
	int m_logLevel;						//log Level정의
	int m_device;							//출력 방향 지정 1:log file, 2:화면 출력, 3:file & 화면 출력
	char m_ptrPath[256];					//log file path 및 name
	//pthread_mutex_t		m_logLock;		//thread 변수

	char STR_DEBUG[12];
	char STR_WARN[12];
	char STR_ERROR[12];
	char STR_INFO[12];
	char STR_NOT_USE[12];
	char STR_UNKNOWN[12];
	char STR_TEST[12];
	char STR_TRACE[12];

public :
	LogC (
		const char *strPath,
		const char *strFileName,
		const int stdLevel = LEVEL_INFO,
		const int outDevice = FILE_LOG
	);

	LogC (
		const char *strPath,
		const char *strFileName,
		const int stdLevel,
		const int outDevice,
		const char *name,
		const int pID,
		const char *date,
		const char *time,
		const char *host
	);

	virtual
	~LogC (
		void
	);

	void
	SetLevel (
		const int stdLevel
	);

	char*
	GetLevelString (
		const int stdLevel
	);

	void
	Logf (
		const int level,
		const char *strDateFormat,
		const char *strLogContents,
		...
	);

	void
	Log (
		const int level,
		const char *strLogContents,
		...
	);

	void
	Log2 (
		const int level,
		const char *strLogContents,
		...
	);

	FILE*
	GetFd (
		void
	)
	{
		return this->m_fpLogFile;
	}

public:
	void
	Debug (
		const char *strLogContents,
		...
	);

	void
	Info (
		const char *strLogContents,
		...
	);

	void
	Warning (
		const char *strLogContents,
		...
	);

	void
	Error (
		const char *strLogContents,
		...
	);

	void
	Test (
		const char *strLogContents,
		...
	);

	void
	Trace (
		const char *strLogContents,
		...
	);

private:
	void
	InternalWrite (
		const int level,
		const char *strLogContents,
		va_list ap
	);
};

#endif
