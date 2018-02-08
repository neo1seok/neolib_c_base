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

#ifndef _NEOTEST_
#define _NEOTEST_

#include "comdef.h"
//#include "com.h"

#define NEO_FILENAME NEO_TITLE
#define NEO_DIRECT NEO_MARK
//디파인 이름 변경후 호환을 위한 정의 

//define 설명
//_DEBUG : 이 정의에 의해 동작이 된다.이게 없다면 소스상에서 아무동작도 안한다.
//NEO_FX_ARG: void neoDebug(MCHAR * fmt, ...) 이런 함수 사용이 어려운 단말기에서 사용을 위해 이 것을 정의 한다.

#ifdef NEOUSEMBCS
#define MCHAR	char
#define __NTEXT(quote) quote         // r_winnt
#else
#define MCHAR	wchar_t
#define __NTEXT(quote) L##quote      // r_winnt
#endif

#define _N(x)       __NTEXT(x)


// #ifdef NEO_STATIC
// #define EXTERN_API
// #else
// #ifdef NEO_EXPORTS
// #define EXTERN_API __declspec(dllexport)
// #else
// #define EXTERN_API __declspec(dllimport)
// #endif
// #endif

#ifdef __cplusplus
#define NEO_EXTERN_API extern "C"
#else
#define NEO_EXTERN_API
#endif


#ifndef NEO_FX_ARG
NEO_EXTERN_API void neoDebug(const MCHAR * fmt, ...);
NEO_EXTERN_API void neoDebugTitle(MCHAR *pTitle, const MCHAR * fmt, ...);
#else
NEO_EXTERN_API void neoDebug(const MCHAR * fmt, int pr0, int pr1, int pr2);
#endif
NEO_EXTERN_API void dioSetTD(MCHAR * strID);

NEO_EXTERN_API void dioSetTraceFn(void(*fnDioTrace)(const MCHAR*));
NEO_EXTERN_API void neoPrintout(const MCHAR * str);
NEO_EXTERN_API void dioSTRFILEOUT(MCHAR * str);
NEO_EXTERN_API void dioOUTRETAIN(MCHAR * str);
NEO_EXTERN_API void neoSetLogName(const MCHAR * logname);


#ifdef NEODEBUG //이 정의를 해야만 동작이 된다.

/* CODE DEL sodazim
#ifndef NEO_FX_ARG
	EXTERN_API	void neoDebug(const MCHAR * fmt, ...);
	EXTERN_API void neoDebugTitle(MCHAR *pTitle, const MCHAR * fmt, ...);
#else
	EXTERN_API	void neoDebug(const MCHAR * fmt, int pr0 ,int pr1, int pr2);
#endif
	EXTERN_API void dioSetTD( MCHAR * strID);

EXTERN_API	void dioSetTraceFn( void(*fnDioTrace)(MCHAR*));
EXTERN_API	void neoPrintout(MCHAR * str);
EXTERN_API	void dioSTRFILEOUT(MCHAR * str);
EXTERN_API	void dioOUTRETAIN(MCHAR * str);
*/



#define NEO_SETFN(a_)	dioSetTraceFn(a_) 
#define NEO_START		dioSetTraceFn(neoPrintout)
#define NEO_RETAILSTART		dioSetTraceFn(dioOUTRETAIN)


#define NEO_FORM(a)			neoDebug a
#define NEO_SETID(a)	dioSetTD(_N(#a));

#define NEO_DEBUG neoDebug
#define NEO_PRT printf
#ifdef NEOLOGTYPE


#define NEO_DEBUGT neoDebugTitle

#define NEO_TITLE(t_)		neoDebug((const MCHAR *)_N(#t_)_N("|") _N("TITLE|") _N("#########<< START >>#########\t")	_N("|(%s:%d)") ,_N(__FILE__),__LINE__ ,0,0)
#define NEO_MARK(t_,a)		neoDebug((const MCHAR *)_N(#t_)_N("|") _N("MARK|")	_N(#a)									_N("|(%s:%d)") ,_N( __FILE__ ),__LINE__  ,0,0)
#define NEO_LINE(t_)		neoDebug((const MCHAR *)_N(#t_)_N("|") _N("LINE|")											_N("|(%s:%d)") ,_N(__FILE__),__LINE__  ,0,0)
#define NEO_DWORD(t_,a)		neoDebug((const MCHAR *)_N(#t_)_N("|") _N("DWORD|")_N(#a) _N("=0x%x\t")					_N("|(%s:%d)"),(int)a,_N(__FILE__),__LINE__ ,0)
//#define NEOL_INT(t_,a)		neoDebug((const MCHAR *)_N(#t_)_N("|") _N("INT|")	_N(#a) _N("=%d\t")						_N("|(%s:%d)"),a,_N(__FILE__),__LINE__  ,0)
#define NEO_INT(t_,a)		neoDebug((const MCHAR *)_N(#t_)_N("|") _N("INT|")	_N(#a) _N("=%d\t")						_N("|(%s:%d)"),a,_N(__FILE__),__LINE__  ,0)
#define NEO_STR(t_,a)		neoDebug((const MCHAR *)_N(#t_)_N("|") _N("STR|")  _N(#a) _N("=\"%s\"\t")					_N("|(%s:%d)"),(int)a,_N(__FILE__),__LINE__ ,0)
//#define NEOL_DEBUG(t_,a)	neoDebug(_N(#t_),(const MCHAR *)_N("|") _N(#a) _N(":\"%s\"\t(%d)"),(int)a,__LINE__ ,0)
#define NEO_RECT(t_,a)		neoDebug((const MCHAR *)_N(#t_)_N("|") _N("RECT|")_N(#a) _N("- left:%d , top:%d , right:%d , bottom:%d\t(%d)"),(a).left,(a).top,(a).right,(a).bottom,__LINE__)

#else



#define NEO_TITLE(a)	neoDebug((const MCHAR *)_N("\r\n#########<< ") _N(#a) _N(">>#########\t(%s,%d)"),_N(__FILE__),__LINE__ ,0,0)
#define NEO_MARK(a)			neoDebug((const MCHAR *)_N("-->") _N(#a) _N("\t(%s:%d)"),_N( __FILE__ ),__LINE__  ,0,0)
#define NEO_LINE			neoDebug((const MCHAR *)_N("-->")_N("(%s:%d)"),_N(__FILE__),__LINE__  ,0,0)
#define NEO_N				neoDebug((const MCHAR *)_N("")  ,0,0,0)
#define NEO_DWORD(a)		neoDebug((const MCHAR *)_N("-->") _N(#a) _N(":0x%x\t(%d)"),(long )a,__LINE__ ,0)
#define NEO_INT(a)			neoDebug((const MCHAR *)_N("-->") _N(#a) _N(":%d\t(%d)"),a,__LINE__  ,0)

#define NEO_INT64(a)			neoDebug((const MCHAR *)_N("-->") _N(#a) _N(":%lldL \t(%d)"),(long long)a,__LINE__  ,0)
#define NEO_HEX64(a)		neoDebug((const MCHAR *)_N("-->") _N(#a) _N(":0x%llx\t(%d)"),(long long)a,__LINE__ ,0)
//// neo1seok 2017.04.06 ICTK: 우선 %I64X 대신 %llx 로 바꾸었다. 
//차후 문제가 생기면 디파인으로 구분해야 할듯 하다.
//확실한건 리눅스에서는 %I64X 가 안먹는다.

#define NEO_STR(a)			neoDebug((const MCHAR *)_N("-->") _N(#a) _N(":\"%s\"\t(%d)"),(const MCHAR *)a,__LINE__ ,0)
#define NEO_STLS(a)			neoDebug((const MCHAR *)_N("-->") _N(#a) _N(":\"%s\"\t(%d)"),a.c_str(),__LINE__ ,0)
#define NEO_STRD(a)		neoDebug((const MCHAR *)_N("%s, "),a,__LINE__ ,0)
#define NEO_RECT(a)		neoDebug((const MCHAR *)_N(#a) _N("- left:%d , top:%d , right:%d , bottom:%d\t(%d)"),(a).left,(a).top,(a).right,(a).bottom,__LINE__)
#define NEO_POS(a)			neoDebug((const MCHAR *)_N(#a) _N("- x:%d , y:%d\t(%d)"),(a).x,(a).y,__LINE__)
#define NEO_RET			neoDebug((const MCHAR *)_N("-->RET \t(%d)"),__LINE__  ,0,0)

#define NEO_DSTART(a)		neoSetId(#a);neoDebug((const MCHAR *)_N("-->%s\t(%d)"),neoGetId(),__LINE__ )
#define NEO_D			neoDebug((const MCHAR *)_N("-->%s\t(%d)"),neoGetId(),__LINE__ )
#define NEO_DEND		neoDebug((const MCHAR *)_N("-->%s End \t(%d)"),neoGetId(),__LINE__ );neoSetId(L"")


#endif

#define NEO_SETLOGNAME(a_)	neoSetLogName(_N(a_))

#else
//_DEBUG 모드가 아닐때는 소스 상에서 아무 동작을 하지 않게 된다.


#define NEO_DEBUG 
#define NEO_PRT

#define NEO_SETFN(a_)
#define NEO_START 
#define NEO_RETAILSTART

#define NEO_FORM(a)			

//#define NEO_INIT			

#define NEO_SETID(a)

#define NEO_SETHEAD(a)
#define NEO_TITLE(a)
#define NEO_MARK(a)
#define NEO_LINE
#define NEO_DWORD(a)	
#define NEO_INT(a)
#define NEO_INT64(a)
#define NEO_HEX64(a)
#define NEO_STLS(a)
#define NEO_STR(a)		
#define NEO_STRD(a)
#define NEO_RETURN(a)	
#define NEO_RETURN_N
#define NEO_RECT(a)	
#define NEO_POS(a)	
#define NEO_RET

#define NEO_DSTART(a)
#define NEO_D		
#define NEO_DEND	

#define NEO_ON
#define NEO_OFF
	
#define NEOL_TITLE(t_)		
#define NEOL_MARK(t_,a)		
#define NEOL_LINE(t_)		
#define NEOL_DWORD(t_,a)		
//#define NEO_INT(t_,a)		
#define NEOL_INT(t_,a)		
#define NEOL_STR(t_,a)		
//#define NEO_DEBUG(t_,a)	


#define NEO_SETLOGNAME(a_)
#endif


#ifdef NEOTIMECHECK

EXTERN_API void ViewTitle(unsigned short  * str);
EXTERN_API unsigned long GetTakeTime(int isStart);
EXTERN_API void ViewTakeTime();

#define	NEOTIMECHECKTITLE(str) ViewTitle(L#str)
#define NEOTIMECHECKSTART  GetTakeTime(TRUE)
#define NEOTIMECHECKVIEW  ViewTakeTime()

#else

#define	NEOTIMECHECKTITLE(str)
#define NEOTIMECHECKSTART
#define NEOTIMECHECKVIEW

#endif

//#define	FOUT_ON			NEO_START
#define	FOUT_ON
#define FOUT_FILENAME	NEO_TITLE
#define FOUT_DIRECT		NEO_MARK
#define FOUT_MARK		NEO_MARK
#define FOUT_LINE		NEO_LINE	
#define FOUT_N			NEO_N		
#define FOUT_DWORD		NEO_DWORD
#define FOUT_INT		NEO_INT
#define FOUT_STR		NEO_STR
#define FOUT_STRD		NEO_STRD
#define FOUT_RECT		NEO_RECT
#define FOUT_POS		NEO_POS





#endif //#ifndef _NEOTEST

/*

NEO_ON;
NEO_FILENAME(DimeEXE);
NEO_DIRECT(START);


NEO_ON;
NEO_STR( _T(__TIMESTAMP__) )
NEO_STR( _T(__FILE__) )

*/