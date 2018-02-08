
/****************************************************************************
 * File Name			: neoDebug.h
 * Usage				: 디버깅시 편의를 위한 매크로 및 함수 정의
 * Author				: [DIOTEK] 신원석(neo1seok)
 * Creation				: 겁나 오래 됐음 한 4년전?
 * Last Modification	: 2007.05.10
 * Contents				:

						DIOTEK Co., Ltd.
701 ACE Techno Tower VIII, 191-7, Guro 3-dong, Guro-gu Seoul 152-780, Korea
****************************************************************************/

/***************************************************************************
 * Includes
 ***************************************************************************/
//#include "stdafx.h"
#include <string>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <memory.h>
//#include "tchar.h" 
#include "util/SystemUtil.h"
#include "neoCoLib.h"  
#include "neoDebug.h"  
#ifndef NEO_FX_ARG
#include <stdarg.h>       /* due to va_list,va_start */
#endif
 
using namespace std;

#ifdef 	NEOUSEMBCS 
	#define VSPRINTF  vsprintf
	#define SPRINTF	sprintf
	#define FPRINTF	fprintf
	#define SNCPY lstrcpynA
	#define VSCPRINTF	vsprintf
	//#define VSCPRINTF	_vscprintf
	#define  STRLEN		strlen
	
#else
	#define VSPRINTF  vswprintf
	#define SPRINTF	swprintf
	#define FPRINTF	fwprintf
	#define SNCPY lstrcpynW
	#define VSCPRINTF	_vscwprintf
	#define  STRLEN		wcslen 
	
	
	
#endif



#define LENPRJNAME 16
#define LENSTR (1024 + LENPRJNAME+2)
/***************************************************************************
 * Functions Descriptions
 ***************************************************************************/

//#ifdef _DEBUG
//#if defined(_DEBUG)||defined(NEODEBUG)
//#ifdef NEODEBUG



MCHAR szTemp[LENSTR+1] = {0,};
MCHAR prjName[LENPRJNAME+1] = {0,};

 
#define DEBUGFILE		"/DebugInfo.txt"
#define EXTFILE			"/extInfo.txt"

string _logname = "neolog.txt";
FILE * _logfpneodebug = NULL;

void neoPrintout(const MCHAR * str)
{
	_logfpneodebug = fopen(_logname.c_str(), "ab");
	if (!_logfpneodebug) return;
	fprintf(_logfpneodebug, "\n%s", str);
	fclose(_logfpneodebug);
	::printf("\n%s", str);
	fflush(stdout);

}
void neoSetLogName(const MCHAR * logname){
	_logname = logname;
}

#if 0
void dioSTRFILEOUT(MCHAR * str)
{

	

	FILE * fp;
	fp = fopen(DEBUGFILE, "rb");

#ifndef NEOUSEMBCS
	if(!fp){
		fp = fopen(DEBUGFILE, "wb");
		if(fp) fwprintf(fp,L"%c",0xfeff);
	}
#endif
	if(fp) fclose(fp);
	
	fp = fopen(DEBUGFILE, "ab");

	
	if(!fp) {
		fp = fopen(EXTFILE, "ab");
	}
	FPRINTF( fp, _N("\r\n"));
	FPRINTF( fp, _N("%s"),str);
	fclose(fp);


}
#endif
void dioOUTRETAIN(MCHAR * str)
{
	//RETAILMSG(1,(L"%s\r\n",str));
	//NKDbgPrintfW(L"%s\r\n",str);

}

void dioStrTrace(MCHAR * str)
{

}

void(*_fnStrTrace)(const MCHAR*) = NULL;


EXTERN_API void dioSetTraceFn( void(*fnDioTrace)(const MCHAR*))
{
	CSystemUtil *putil = CSystemUtil::GetSystemUtilClass();
	
	string time = NCL::GetStaticTimeForm(NULL,"\5YYYYMMDD.HHmmss\5");

	string newlog = _logname + "." + time+".tmp";


	putil->FileMove(_logname, newlog);

	_fnStrTrace = fnDioTrace;

}

void dioSetTD( MCHAR * strID)
{
//	SNCPY(prjName,strID,LENPRJNAME);
	prjName[LENPRJNAME] = L'\0';
}




#ifndef NEO_FX_ARG
int tr_len = 2048 * 10;
MCHAR * pChar = (MCHAR *)malloc(tr_len*sizeof(MCHAR));

EXTERN_API void neoDebug(const MCHAR * fmt, ...)
{
//static count = 0;
int tr_len=0;
va_list args;


	//tr_len = SPRINTF(szTemp,_N("%s:"),prjName);
	
	va_start(args, fmt);
	//tr_len = VSCPRINTF( fmt, args ) + 2; 
	//pChar[tr_len-3] = NULL;
	memset(pChar,0x00,tr_len*sizeof(MCHAR));

	VSPRINTF( pChar, fmt,args );
	va_end(args);

	if(_fnStrTrace) _fnStrTrace(pChar);
	//free(pChar);

}


EXTERN_API void neoDebugTitle(MCHAR *pTitle, const MCHAR * fmt, ...)
{
	size_t tr_len=0;
	va_list args;

	va_start(args, fmt);

	//tr_len = VSCPRINTF( fmt, args ) + 1 + STRLEN(pTitle) +2; 
	tr_len = 2048; 
	


	//tr_len = SPRINTF(szTemp,_N("%s:"),prjName);
	MCHAR * pChar = (MCHAR *)malloc(tr_len*sizeof(MCHAR));
	int len  = SPRINTF(pChar ,_N("%s|"),pTitle);

	
	VSPRINTF( pChar+len, fmt,args );
	va_end(args);

	
	free(pChar);
}



#else
EXTERN_API void neoDebug(const MCHAR * fmt, int pr0 ,int pr1, int pr2)
{
	int tr_len=0;
	tr_len = SPRINTF(szTemp,_N("%s:"),prjName);

	SPRINTF( &szTemp[tr_len], fmt, pr0 ,pr1,pr2);
	if(_fnStrTrace)  _fnStrTrace(szTemp);
}

#endif


 

#ifdef NEOTIMECHECK
unsigned long _dwTime;
void ViewTitle(unsigned short  * str)
{
	NKDbgPrintfW(L"\r\n@@@@ %s @@@",str);

}
unsigned long GetTakeTime(int isStart)
{
	unsigned long prevTime;;
	prevTime = _dwTime;
	_dwTime = GetTickCount();
	//NKDbgPrintfW(L"\r\n@@@@ %d %d @@@",prevTime,_dwTime);
	if(isStart){
		return 0;
	}


	return _dwTime - prevTime;


}

void ViewTakeTime()
{
	NKDbgPrintfW(L"\r\n@@@@ Take Time :%d@@@",GetTakeTime(0));

}

#endif
