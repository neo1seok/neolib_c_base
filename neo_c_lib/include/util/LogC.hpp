/*****************************************************************************************
 �� �� �� : LogC.hpp
 �� �� �� : ������
 ��    �� : log���� Library Header file
 �ۼ����� : 2004��  4��  ��
 �������� :     ��   ��    ��
 ����ȭ�� : LogC.cpp
 �����Ϸ� :
 ��    Ÿ :
*****************************************************************************************/

#ifndef		_LOG_CLASS_HEADER_
#define		_LOG_CLASS_HEADER_

#include <stdio.h>
#include <stdarg.h>

//#if defined(_LSA_AIX_) && defined(_FORCE_PTHREAD_TYPE_DEFINE_)
//	#include "OsDep/pthread_xopen600_types.h"
//#endif

//#include <pthread.h>

#define	FILE_LOG	1					//log file����
#define SCREEN_LOG	2					//log ȭ�� ���
#define BOTH_LOG	3					//file & ȭ�� ���

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
	int m_logLevel;						//log Level����
	int m_device;							//��� ���� ���� 1:log file, 2:ȭ�� ���, 3:file & ȭ�� ���
	char m_ptrPath[256];					//log file path �� name
	//pthread_mutex_t		m_logLock;		//thread ����

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
