/*****************************************************************************************
 �� �� �� : timeC.hpp
 �� �� �� : ������
 ��    �� : Date & Time���� Library Header file
 �ۼ����� : 2004��  4��  ��
 �������� :     ��   ��    ��
 ����ȭ�� : timeC.cpp
 �����Ϸ� :
 ��    Ÿ :
*****************************************************************************************/

#ifndef		_TIME_CLASS_HEADER_
#define		_TIME_CLASS_HEADER_

#include <time.h>
#include <sys/timeb.h>
#include <stdlib.h>

#include <string.h>
#include <string>

#if defined(_TARGET_OS_LINUX_) || defined(__CYGWIN__)
	#include <sys/time.h>
	#include <unistd.h>
#endif

#define TIMEC_YEAR		1
#define TIMEC_MONTH		2
#define TIMEC_DAY		3

#if !defined(_LSA_HPUX_) || !defined(RWSTD_NO_NAMESPACE)
using namespace std;
#endif

class TimeC
{
private :
	int yearVal;
	int monthVal;
	int dayVal;
	int hourVal;
	int minuteVal;
	int secondVal;
	int microSecond;

public :
	TimeC (
		void
	);

	virtual
	~TimeC (
		void
	);

	void
	SetCurrent (
		void
	);

	void
	SetYesterDay (
		void
	);

	void
	SetTime (
		time_t time
	);

	void
	RollDate (
		int		how
	);

	void
	RollDate (
		int		kind,
		int		how
	);

	void
	RetDateTime (
		char *strRetVal,
		const char *strFormat
	);

	string
	GetTimeString (
			time_t time
	);
};

#endif
