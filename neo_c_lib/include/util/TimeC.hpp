/*****************************************************************************************
 파 일 명 : timeC.hpp
 작 성 자 : 김지훈
 기    능 : Date & Time관련 Library Header file
 작성일자 : 2004년  4월  일
 변경일자 :     년   월    일
 관련화일 : timeC.cpp
 컴파일러 :
 기    타 :
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
