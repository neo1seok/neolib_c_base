#pragma once


 
#include "comdef.h"
#include <Windows.h>

//using namespace neocolib;

#define NSL NeoSystemLib


typedef struct _tagPROCESSINFO{
	DWORD	dwProcessID;
	wchar	szExePath[MAX_PATH];
}PROCESSINFO,*LPPROCESSINFO;

// #ifndef HANDLE
// typedef neocolib::pvoid	HANDLE;
// #endif
// 
// #ifndef HWND
// typedef neocolib::pvoid	HWND;
// #endif
// 
// #ifndef HDC
// typedef neocolib::pvoid	 HDC;
// #endif
// 
// 
//  #ifndef tagPOINT
//  typedef struct tagPOINT
//  {
//  	int  x;
//  	int  y;
//  } POINT, *PPOINT,  *NPPOINT,  *LPPOINT;
//  #endif
// 
// #ifndef tagRECT
//  typedef struct tagRECT
//  {
// 	 int    left;
// 	 int    top;
// 	 int    right;
// 	 int    bottom;
//  } RECT, *PRECT,  *NPRECT,  *LPRECT;
// #endif
// 
// #ifndef COLORREF
//  typedef neocolib::uint COLORREF;
// #endif
// 
// #ifndef COLORREF
//  typedef struct _FILETIME {
// 	 neocolib::uint dwLowDateTime;
// 	 neocolib::uint dwHighDateTime;
//  } FILETIME, *PFILETIME, *LPFILETIME;
// #endif

typedef struct _tagREGVALUEINFO
{
	neocolib::tchar szValueName[MAX_VALUE_NAME];
	neocolib::tchar szValue[MAX_PATH];
	neocolib::uint dwEtc;
}REGVALUEINFO,*LPREGVALUEINFO;

typedef vector<PROCESSINFO> VECPROCESSINFO;

//typedef ARRAYLIST<POINT> POINTARRAY,* LPPOINTARRAY;
typedef ARRAYLIST<REGVALUEINFO> REGVALUELIST,*LPREGVALUELIST;
typedef ARRAYLIST<PROCESSINFO> PROCESSLIST,*LPPROCESSLIST;

class EXTERN_API CMUTEX{
private:
	HANDLE m_hMutex;
	bool m_isDirLock;
public:
	CMUTEX(const wchar* muname ,bool curRock = true);
	bool Lock(uint dwTimeout = 250);
	void UnLock();
	~CMUTEX();
};
class EXTERN_API CFILELOCK{
private:
	HANDLE m_hFile;
	OVERLAPPED overlapped;
	bool bRet;

public:
	CFILELOCK::CFILELOCK(HANDLE hFile):m_hFile(hFile) {
		overlapped.Offset=0;
		overlapped.OffsetHigh=0;
		bRet = (LockFileEx(m_hFile, LOCKFILE_EXCLUSIVE_LOCK, 0, 0xFFFFFFFF, 0, &overlapped) == TRUE);
	};

	CFILELOCK::~CFILELOCK(){
		bRet=(UnlockFileEx(m_hFile, 0, 0xFFFFFFFF, 0, &overlapped) == TRUE);

	};

};




class EXTERN_API NeoSystemLib
{

public:
	enum MODECREATE {
		MODEREAD,
		MODEAPPEND,
		MODEWRITE,
	};

	static bool neoCreateProcess(const wstring& runPath) ;
	static bool neoCreateProcessSync(const wstring& runPath) ;
	static neocolib::p_circular_tstr GetModuleName(neocolib::uint nPID);
	static neocolib::p_circular_tstr GetModuleName2(neocolib::uint nPID);
	static neocolib::p_circular_tstr GetModuleFileName(neocolib::uint nPID);
	static void KillProcess(neocolib::uint uID);
	static neocolib::uint FindProcessByNameOnCurSession(const wstring& szProcessName);

	static neocolib::uint ProcIDFromWnd(HWND hwnd); //
	static HWND GetWinHandle(neocolib::uint pid); // 
	static bool IsAlreadyRunningProcessOnCurSession();


	static neocolib::p_circular_tstr GetCurIPAdrress();
	static neocolib::p_circular_tstr GetCurComputerName(bool isReturnRuntimeMemory = false);
	static bool IsVista();



	static LPPROCESSLIST getProcessList();
	static LPPROCESSLIST neoGetProcessIDsByName(const wstring& szPorcName);
	static RECT GetRectFromPtArray(LPPOINT lpPoint,neocolib::uint nPoint);

	static bool DrawRect(HDC hDC,LPRECT lpRect,COLORREF col );
	static neocolib::uint NeoSystemLib::GetSessionID(neocolib::uint processID);
	static time_t NeoSystemLib::filetime_to_timet(const FILETIME& ft);

	static LPBYTEARRAY  GetLpDBFromFP(HANDLE hFile ,uint stOffset = 0,uint edOffset = -1);
	static HANDLE CreateFileSimple(const wstring &lpfname,MODECREATE mode);
	static uint GetLastOffset(HANDLE hFile);
	static uint WriteStrFile(HANDLE hFile,pctstr fmt,...);
	static uint WriteFile(HANDLE hFile,pvoid lpbuff, uint nBuffSize);
	static uint ReadFile(HANDLE hFile,pvoid lpbuff, uint nBuffSize);

	static LPBYTEARRAY  GetLpDBFromWFile(const wstring& lpfname);
	static bool CopyWChar2CBoard(const wstring& pOrg);
	static wchar* CopyCBoard2WChar();

	static bool IsExistFile(const wstring& lpfname);
	static uint GetLastOffset(const wstring &lpfname);
	static LPWSTRARRAY FindFilesInFolder2Array(const wstring& lpcFile);
	static LPBYTEARRAY  GetLpDBFromWFileEx(const wstring& lpfname ,uint stOffset,uint & edOffset);
	static LPTCHARSTR  GetStrFromWFile(const wstring&  lpfname);

	static p_circular_tstr GetErrorMessageStr( );
	static bool IsExistDir(const wstring& dirName);
	static void MakeDirPathFromFile(const wstring& lpCFile);

	static p_circular_tstr GetCurStartDir();
	static void CreateDir(const wstring& Path);



};





