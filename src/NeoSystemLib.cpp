
#include <Windows.h>
#include <Tlhelp32.h>
#include <Windows.h>
//#include <atlbase.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <process.h>
#include <Tlhelp32.h>
#include <tchar.h>
#include <algorithm> 
//#include <Wingdi.h>
#include "neoCoLib.h"
#include "neoDebug.h"
//#include <string.h>
//#include <wchar.h>

#pragma comment (lib, "Gdi32.lib ")
#pragma comment (lib, "psapi.lib")
#pragma comment (lib, "User32.lib")

#include "utf8.h"
#include "psapi.h"
//#include"sharemem.h"
#include "NeoSystemLib.h"


std::wstring mbs_to_wcs(std::string const& str, std::locale const& loc = std::locale());
std::string wcs_to_mbs(std::wstring const& str, std::locale const& loc = std::locale());
std::wstring utf8_to_ucs(const std::string & str);
std::string ucs_to_utf8(std::wstring const& wstr);
std::string utf8_to_mbs(std::string & line, std::locale const& loc = std::locale());
std::string mbs_to_utf8(std::string const& str, std::locale const& loc = std::locale());


// template<class Ty_>
// ARRAYLIST<Ty_> * ConvertFromVector(vector<Ty_> & vecfindlist);
//LP_STATIC_TSTR NCL::GetStaticStr(STRING str);
tstring ConvTString(const wstring &str){
	tstring sz ;
#ifdef _USE_UTF8_
	return ucs_to_utf8(str);

#else
	return str;
#endif
	
}
wstring ConvWString(const tstring &str){
	tstring sz ;
#ifdef _USE_UTF8_
	return utf8_to_ucs(str);

#else
	return str;
#endif

}


bool NeoSystemLib::neoCreateProcess(const wstring& runPath) 
{
	// TODO: Add your control notification handler code here
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	int rc;
	si.cb = sizeof(STARTUPINFO);
	si.lpReserved = NULL;
	si.lpDesktop = NULL;
	si.lpTitle = NULL;
	si.dwFlags = 0;
	si.cbReserved2 = 0;
	si.lpReserved2 = NULL;

	SetLastError(0);

	rc = CreateProcess(runPath.c_str(),NULL, NULL, NULL, FALSE,
		NORMAL_PRIORITY_CLASS,
		NULL, NULL, &si, &pi);
	neocolib::uint ret = GetLastError();
	if (rc){
		CloseHandle (pi.hThread);
		CloseHandle(pi.hProcess);
	}
	return (rc == TRUE);
	//ShellExcuteEx
}


void NeoSystemLib::KillProcess(uint uID)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, uID);
	if(hProcess)
	{
		if(TerminateProcess(hProcess, 0))
		{
			unsigned long nCode; //프로세스 종료 상태
			GetExitCodeProcess(hProcess, &nCode);
		}
		CloseHandle(hProcess);
	}
}


bool NeoSystemLib::neoCreateProcessSync(const wstring& runPath) 
{
	// TODO: Add your control notification handler code here
	bool retVal = FALSE;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	int rc;
	si.cb = sizeof(STARTUPINFO);
	si.lpReserved = NULL;
	si.lpDesktop = NULL;
	si.lpTitle = NULL;
	si.dwFlags = 0;
	si.cbReserved2 = 0;
	si.lpReserved2 = NULL;

	SetLastError(0);
	try{
		rc = CreateProcess(runPath.c_str(),NULL, NULL, NULL, FALSE,
			NORMAL_PRIORITY_CLASS,
			NULL, NULL, &si, &pi);

	}
	catch(...){
		int ad = 0;
	}
	neocolib::uint ret = GetLastError();
	if (rc){
		neocolib::uint ret = WaitForInputIdle (pi.hProcess,1000);
		if(ret == WAIT_OBJECT_0){
			retVal = TRUE;
		}

		CloseHandle (pi.hThread);
		CloseHandle(pi.hProcess);
	}
	return retVal;
}
LPPROCESSLIST NeoSystemLib::getProcessList()
{
	VECPROCESSINFO retProcessList;

	HANDLE hProcessSnap;

	PROCESSENTRY32 pe32;



	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
		//		printError( TEXT("CreateToolhelp32Snapshot (of processes)") );
		return NULL;
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof( PROCESSENTRY32 );



	// Retrieve information about the first process,
	// and exit if unsuccessful
	if( !Process32First( hProcessSnap, &pe32 ) )
	{
		//		printError( TEXT("Process32First") ); // show cause of failure
		CloseHandle( hProcessSnap );          // clean the snapshot object
		return NULL;
	}


	// Now walk the snapshot of processes, and
	// display information about each process in turn

	do
	{
		LPPROCESSINFO lpProcessInfo;
		PROCESSINFO prcocessInfo = {0,};
		lpProcessInfo = &prcocessInfo;
		lpProcessInfo->dwProcessID = pe32.th32ProcessID;
		wstring strfile = pe32.szExeFile;

		NCL::CopyPSTRFromSTRING<wchar>(lpProcessInfo->szExePath,pe32.szExeFile,MAX_NCL_PATH);

		//lstrcpy(lpProcessInfo->szExePath,pe32.szExeFile);
		
		retProcessList.push_back(prcocessInfo);
	} while( Process32Next( hProcessSnap, &pe32 ) );



	CloseHandle( hProcessSnap );
	return NCL::ConvertFromVector(retProcessList);
	//return retProcessList;
	//return retProcessList;
}

LPPROCESSLIST NeoSystemLib::neoGetProcessIDsByName(const wstring&  szPorcName)
{
	//CMANMEM<PROCESSLIST> cMM = getProcessList();
	//LPPROCESSLIST retPList = cMM;
	VECPROCESSINFO retPList;

	LPPROCESSLIST totalList = getProcessList();
	//CMANMEM<PROCESSLIST> cMM =retPList;
	FORNLOOP(totalList->length){
	//FORVECTER(VECPROCESSINFO,totalList){
	//	*piter;
		PROCESSINFO &processlist = totalList->array[nCount];
// 		TCHAR szTmpCmp[MAX_NCL_PATH] = {};
// 		SPRINTF(szTmpCmp,_t("%s.exe"),szPorcName);
		wstring str = szPorcName + L".exe";
		wstring str2 = processlist.szExePath;
		
		transform( str.begin(), str.end(), str.begin(), tolower );  
		transform( str2.begin(), str2.end(), str2.begin(), tolower );  

		if(!str.compare(str2)){
			retPList.push_back(totalList->array[nCount]);
			// 			memcpy(lpProcessInfo,&retPList->array[nCount],sizeof(PROCESSINFO));
			// 			lpProcessInfo++;
		}

	}

	return NCL::ConvertFromVector(retPList);

	//return retPList;

	//return (LPPROCESSLIST)MakeNSFormDwordBase(nListCount,sizeof(PROCESSINFO),retProcessList);
	//return (LPPROCESSLIST)realloc(retProcessList,sizeof(int)+sizeof(PROCESSLIST)*(nListCount+1));
}
uint NeoSystemLib::FindProcessByNameOnCurSession(const wstring& szProcessName)
{

	//LPPROCESSLIST lpProcList = CCA::neoGetProcessIDsByName(szProcessName);
	LPPROCESSLIST cMMProcList = NeoSystemLib::neoGetProcessIDsByName(szProcessName);

	if(cMMProcList == 0) return FALSE;
	uint curSessionID = NeoSystemLib::GetSessionID(GetCurrentProcessId());

	FORNLOOP(cMMProcList->length){
		PROCESSINFO &processInfo = cMMProcList->array[nCount];
		if (NeoSystemLib::GetSessionID(processInfo.dwProcessID) == curSessionID)
		{
			return processInfo.dwProcessID;
		}

	}


	return -1;
}
p_circular_tstr NeoSystemLib::GetCurComputerName( bool isReturnRuntimeMemory /*= FALSE*/ )
{
	//MUTEXLOCK(GetCurComputerName);
	wchar szComputerName[256];
	szComputerName[0] = NULL;
	neocolib::uint dwLength=256;
	if(!GetComputerName(szComputerName,(LPDWORD) &dwLength))	{
	}
	//if(isReturnRuntimeMemory) return CopyNewStr(szComputerName);

	return NCL::GetStaticStr(ConvTString(szComputerName));
	//return szComputerName;

}
p_circular_tstr NeoSystemLib::GetModuleName(neocolib::uint nPID)
{
	MUTEXLOCK(GetModuleName);
	HANDLE hProcess = NULL;
	wchar szProcessName[1024+1] = {0,};

#if 1
	if ( (hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, nPID)) != NULL )
	{
		HMODULE hMod;
		neocolib::uint cbNeeded;


		if ( EnumProcessModules(hProcess, &hMod, sizeof(hMod),  (LPDWORD) &cbNeeded) )
		{
			szProcessName[0] = 0;
			int n = GetModuleBaseName(hProcess, hMod, szProcessName, MAX_NCL_PATH);
			szProcessName[n] =0;

		}

		CloseHandle(hProcess);
	}
#endif
	
	return NCL::GetStaticStr(ConvTString(szProcessName));
	//return szProcessName;
}
p_circular_tstr NeoSystemLib::GetModuleName2(neocolib::uint nPID)
{
	//MUTEXLOCK(GetModuleName2);
	//static TCHAR szProcessName[MAX_NCL_PATH+1] = {0,};
	p_circular_tstr ret = NCL::GetStaticStr(_t(""));
	//szProcessName[0] = NULL;

	HANDLE hProcessSnap;

	PROCESSENTRY32 pe32;



	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
		return ret;
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if( !Process32First( hProcessSnap, &pe32 ) )
	{
		CloseHandle( hProcessSnap );          // clean the snapshot object
		return ret;
	}

	// Now walk the snapshot of processes, and
	// display information about each process in turn

	do
	{
		if(nPID == pe32.th32ProcessID){
			//lstrcpy(szProcessName,pe32.szExeFile);
			
			ret = NCL::GetStaticStr(ConvTString(pe32.szExeFile));
			//NCL::CopyLPTSTRFromSTRING(szProcessName,pe32.szExeFile,MAX_NCL_PATH);
		}

	} 
	while( Process32Next( hProcessSnap, &pe32 ) );

	CloseHandle( hProcessSnap );

	return ret;
	//return szProcessName;
}
p_circular_tstr NeoSystemLib::GetModuleFileName(neocolib::uint nPID){
	//MUTEXLOCK(GetModuleFileName);
	//static TCHAR strBProcessName[MAX_NCL_PATH];
	//strBProcessName[0]= NULL;

	p_circular_tstr ret = NCL::GetStaticStr(_t(""));

	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;

	// Take a snapshot of all modules in the specified process.
	hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, nPID );
	if( hModuleSnap == INVALID_HANDLE_VALUE )
	{
		//		printError( L"CreateToolhelp32Snapshot (of modules)" );
		return ret;

	}

	// Set the size of the structure before using it.
	me32.dwSize = sizeof( MODULEENTRY32 );

	// Retrieve information about the first module,
	// and exit if unsuccessful
	if( Module32First( hModuleSnap, &me32 ) )
	{
//		lstrcpy(strBProcessName,me32.szExePath);
		//NCL::CopyLPTSTRFromSTRING(strBProcessName,me32.szExePath,MAX_NCL_PATH);
		ret = NCL::GetStaticStr(ConvTString(me32.szExePath));
		

	}
#if 0
	HANDLE nHandle = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ, false, nPID);
	if (nHandle)
	{
		::GetModuleFileNameEx(nHandle,0,strBProcessName, MAX_NCL_PATH);
		CloseHandle(nHandle);
	}
#endif
	CloseHandle( hModuleSnap );    // Must clean up the
	return ret;
	//return strBProcessName;

}
neocolib::uint NeoSystemLib::ProcIDFromWnd(HWND hwnd) // 윈도우 핸들로 프로세스 아이디 얻기   
{   
	neocolib::uint idProc;   
	GetWindowThreadProcessId( hwnd,(LPDWORD) &idProc );   
	return idProc;   
}   
p_circular_tstr NeoSystemLib::GetCurStartDir()
{
	tstring lpName = NeoSystemLib::GetModuleFileName(GetCurrentProcessId());

	return NCL::GetDirNameFromPath(lpName);
};

p_circular_tstr NeoSystemLib::GetCurIPAdrress(){



	//static TCHAR strIpAddress[MAX_NCL_PATH];
#if 0
	strIpAddress[0] = 0;

	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD( 2, 0 );

	if( WSAStartup( wVersionRequested, &wsaData ) == 0 )
	{
		char name[256];

		if( gethostname( name, sizeof( name ) ) == 0 )
		{
			PHOSTENT hostinfo = gethostbyname( name );

			if( hostinfo != NULL)
			{
				char * ipAddr = inet_ntoa( *( struct in_addr* ) *hostinfo->h_addr_list );
				CMANMEM<TCHAR> cmnIp =  CCA::ConvMB2WC(949,ipAddr);
				lstrcpy(strIpAddress,cmnIp);
			}
		}

		WSACleanup();
	}
#endif
	return NCL::GetStaticStr( ConvTString(L""));

}
HWND NeoSystemLib::GetWinHandle(neocolib::uint pid) // 프로세스 아이디로 윈도우 핸들 얻기   
{   
	HWND tempHwnd = FindWindow(NULL,NULL); // 최상위 윈도우 핸들 찾기   

	while( tempHwnd != NULL )   
	{   
		if( GetParent(tempHwnd) == NULL ) // 최상위 핸들인지 체크, 버튼 등도 핸들을 가질 수 있으므로 무시하기 위해   
			if( pid == ProcIDFromWnd(tempHwnd) )   
				return tempHwnd;   
		tempHwnd = GetWindow(tempHwnd, GW_HWNDNEXT); // 다음 윈도우 핸들 찾기   
	}   
	return NULL;   
}

uint NeoSystemLib::GetSessionID(uint processID)
{
	neocolib::uint retSessionID = -1;
	if(TRUE == ProcessIdToSessionId(processID,(LPDWORD)&retSessionID )){
		return (uint)retSessionID;
	}
	return -1;
}
#if 0
HWND CCA::GetHwndFromPNameOnCurSession(LPCTSTR szProcessName)
{
	LPPROCESSLIST lpProcList = CCA::neoGetProcessIDsByName(szProcessName);
	if(!lpProcList) return FALSE;
	UINT curSessionID = CCA::GetSessionID(GetCurrentProcessId());



	for (DWORD i = 0; i < lpProcList->nProcess; i++)
	{
		LPPROCESSINFO lpProcInfo = lpProcList->processInfo + i;
		if (CCA::GetSessionID(lpProcInfo->dwProcessID) == curSessionID)
		{
			GetWindowThreadProcessId()

				return TRUE;
		}
	}

	return false;
}
#endif // 0

bool NeoSystemLib::IsVista()
{
	OSVERSIONINFO osver;

	osver.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );

////	if ( GetVersionEx (&osver) )
//	{
//		if ( osver.dwPlatformId == VER_PLATFORM_WIN32_NT && (osver.dwMajorVersion >= 6 ) )
//			return TRUE;
//	}
//
	return FALSE;
}
RECT NeoSystemLib::GetRectFromPtArray(LPPOINT lpPoint,neocolib::uint nPoint){
	int minX = 0x7fffffff,minY = 0x7fffffff,maxX = 0,maxY = 0;
	FORNLOOP(nPoint){
		int x = lpPoint[nCount].x;
		int y = lpPoint[nCount].y;
		minX = min(minX,x);
		maxX = max(maxX,x);

		minY = min(minY,y);
		maxY = max(maxY,y);
	}
	RECT rect = {minX,minY,maxX,maxY};
	return rect;
}

bool NeoSystemLib::DrawRect(HDC hDC,LPRECT lpRect,COLORREF col ){
	if(!lpRect) return FALSE;
	POINT point[5] = {
		{lpRect->left,lpRect->top},
		{lpRect->right,lpRect->top},
		{lpRect->right,lpRect->bottom},
		{lpRect->left,lpRect->bottom},
		{lpRect->left,lpRect->top}
	};
	HPEN hOld = (HPEN)SelectObject(hDC,CreatePen(PS_SOLID ,3 ,col));
	bool ret = (Polyline(hDC,point,5) == TRUE);
	//Rectangle(m_lpHDCInfo->hMemDC,minX,minY,maxX,maxY);
	HPEN hOrgBursh = (HPEN)SelectObject(hDC,hOld);
	DeleteObject(hOrgBursh);
	return ret;
}

bool NeoSystemLib::IsAlreadyRunningProcessOnCurSession()
{
	tstring szProcessName = GetModuleName2(GetCurrentProcessId());
	// 	STRING szPer = StrChr(szProcessName,L'.');
	// 	if(szPer) *szPer = NULL;
	tstring newszProcessName =	NCL::GetSubPart(szProcessName,_t("."),FALSE,TRUE);

	LPPROCESSLIST cMMProcList = NeoSystemLib::neoGetProcessIDsByName(ConvWString(newszProcessName));

	//LPPROCESSLIST cMMProcList = NeoSystemLib::neoGetProcessIDsByName(szProcessName);

	if(cMMProcList == 0) return FALSE;
	uint curSessionID = NeoSystemLib::GetSessionID(GetCurrentProcessId());

	FORNLOOP(cMMProcList->length){
		PROCESSINFO &processInfo = cMMProcList->array[nCount];
		

		if (NeoSystemLib::GetSessionID(processInfo.dwProcessID) == curSessionID && processInfo.dwProcessID != GetCurrentProcessId())
		{
			return TRUE;
		}

	}

	return FALSE;
	

	

}

time_t NeoSystemLib::filetime_to_timet(const FILETIME& ft)
{
	ULARGE_INTEGER ull;
	ull.LowPart = ft.dwLowDateTime;
	ull.HighPart = ft.dwHighDateTime;

	return ull.QuadPart / 10000000ULL - 11644473600ULL;
}

LPBYTEARRAY  NeoSystemLib::GetLpDBFromFP(HANDLE hFile ,uint stOffset,uint edOffset)
{
	VECBYTE vecbyte;
	
	if(!hFile) return NULL;

	
	uint fileSize = GetFileSize(hFile,NULL);
	uint buffSize = 0;

	if(edOffset == -1 || edOffset >fileSize){
		edOffset = fileSize;
	}
	SetFilePointer(hFile,stOffset,NULL,FILE_BEGIN);
	buffSize = edOffset - stOffset;

	//lpRetArray = (LPBYTEARRAY)malloc( sizeof(uint) + buffSize+1);
	////lpRetArray = (LPBYTEARRAY)MakeNSFormDwordBase(buffSize,sizeof(BYTE));
	//vector<BYTE> vecbyte(buffSize);
	vecbyte.resize(buffSize);

	//vecbyte.get_allocator()


	buffSize = ReadFile(hFile,&vecbyte[0],buffSize);
	
	//*(lpRetArray->array + buffSize) = 0;
	
	return NCL::ConvertFromVector(vecbyte);
}
//CreateFileSimple(const wstring &lpfname,MODECREATE mode);
HANDLE NeoSystemLib::CreateFileSimple(const wstring &lpfname,MODECREATE dwMode)
{
	MUTEXLOCK(CreateFileSimple);

	uint dwFileAccess ;//= (isOpen == MODEREAD)? GENERIC_READ:FILE_GENERIC_WRITE;
	uint dwFileMode ;//= (isOpen ==MODEREAD)? OPEN_EXISTING:OPEN_ALWAYS;

	switch(dwMode){
		case MODEREAD:
			dwFileAccess = GENERIC_READ;
			dwFileMode = OPEN_EXISTING;
			break;
		case MODEAPPEND:
			dwFileAccess = FILE_GENERIC_WRITE;
			dwFileMode = OPEN_ALWAYS;
			break;
		case MODEWRITE:
			dwFileAccess = FILE_GENERIC_WRITE;
			dwFileMode = CREATE_ALWAYS;
			break;;

	}


	HANDLE 	hFile = CreateFileW(lpfname.c_str(),               // file to open
		dwFileAccess,          // open for reading
		FILE_SHARE_READ|FILE_SHARE_WRITE,       // share for reading
		NULL,                  // default security
		dwFileMode,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);
	if(MODEAPPEND == dwMode) SetFilePointer(hFile,0,NULL,FILE_END);
	return hFile;

}

uint NeoSystemLib::GetLastOffset(HANDLE hFile)
{
	MUTEXLOCK(GetLastOffset);
	uint ret = -1;
	return (uint)GetFileSize(hFile,(LPDWORD)&ret);
}


uint NeoSystemLib::WriteStrFile(HANDLE hFile,pctstr fmt,...)
{
	va_list args;
	uint dwNumber =0;
	tstring buffer;

	//tr_len = SPRINTF(szTemp,_N("%s:"),prjName);
	va_start(args, fmt);
	buffer= NCL::GetNewStrFormV(fmt,args);;
	//buffer = cMM.GetLpBuff();
	//GetNewStrFormV(buffer,fmt,args);

	//buffer =GetNewStrFormV(fmt,args);
	va_end(args);
	dwNumber = WriteFile(hFile,(pvoid)buffer.c_str(),(uint)buffer.size()*sizeof(tchar));
	//CCA::FreeMem(buffer);
	return dwNumber;

}
uint NeoSystemLib::WriteFile(HANDLE hFile,pvoid lpbuff, uint nBuffSize)
{
	uint dwNumber =0;
	::WriteFile(hFile,lpbuff,nBuffSize,(LPDWORD)&dwNumber,NULL);
	return dwNumber;
}
uint NeoSystemLib::ReadFile(HANDLE hFile,pvoid lpbuff, uint nBuffSize)
{
	uint dwNumber =0;
	::ReadFile(hFile,lpbuff,nBuffSize,(LPDWORD)&dwNumber,NULL);
	return dwNumber;
}
LPBYTEARRAY NeoSystemLib::GetLpDBFromWFile(const wstring&  lpfname )
{

	HANDLE 	hFile = CreateFile(lpfname.c_str(),               // file to open
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);
	uint err =GetLastError();
	if(hFile == INVALID_HANDLE_VALUE) return NULL;
	LPBYTEARRAY lpbytearray = GetLpDBFromFP(hFile);
	CloseHandle(hFile);
	return lpbytearray;
}
LPBYTEARRAY NeoSystemLib::GetLpDBFromWFileEx(const wstring&  lpfname ,uint stOffset,uint & edOffset)
{
	//VECBYTE ret;
	HANDLE 	hFile = CreateFile(lpfname.c_str(),               // file to open
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);
	uint err =GetLastError();
	if(hFile == INVALID_HANDLE_VALUE) return NULL;
	LPBYTEARRAY lpbytearray = GetLpDBFromFP(hFile,stOffset,edOffset);
	edOffset = GetFileSize(hFile,NULL);
	CloseHandle(hFile);
	return lpbytearray;
}
wchar_t* NeoSystemLib::CopyCBoard2WChar()
{
	wstring outstr;

	if(!OpenClipboard(NULL))	return FALSE;

	HANDLE clipbuffer = GetClipboardData(CF_UNICODETEXT);
	WCHAR* buffer = (WCHAR*)GlobalLock(clipbuffer);
	outstr = buffer;
	GlobalUnlock(clipbuffer);
	CloseClipboard();
	WCHAR * outbuffer = (WCHAR *)NCL::MallocMem(((uint)outstr.size()+1)*sizeof(WCHAR));
	wcscpy(outbuffer,outstr.c_str());
	return outbuffer;
}
bool NeoSystemLib::CopyWChar2CBoard(const wstring& pOrg)

{

	if(!OpenClipboard(NULL)) return FALSE;
	EmptyClipboard();

	HANDLE clipbuffer = GlobalAlloc(GMEM_DDESHARE, 2*(pOrg.size()+1));
	WCHAR* buffer = (WCHAR*)GlobalLock(clipbuffer);
	//		strTemp.MakeUpper();
	lstrcpyW(buffer,pOrg.c_str());
	GlobalUnlock(clipbuffer);
	SetClipboardData(CF_UNICODETEXT,clipbuffer);
	CloseClipboard();

	return TRUE;

}

bool NeoSystemLib::IsExistFile(const wstring& lpfname)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;


	// Find the first file in the directory.
	hFind = FindFirstFile(lpfname.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return FALSE;
	}
	FindClose(hFind);
	return TRUE;
}


uint NeoSystemLib::GetLastOffset(const wstring& lpfname)
{
	HANDLE hFile = CreateFileSimple(lpfname,MODEREAD);

	if(hFile == INVALID_HANDLE_VALUE) return -1;
	uint ret = GetLastOffset(hFile);
	CloseHandle(hFile);
	return ret;
}
LPWSTRARRAY NeoSystemLib::FindFilesInFolder2Array(const wstring& lpcFile)
//bool NeoCoLib::FindFilesInFolder2Array(VECSTRING &caList,const STRING & lpcFile)
{

	VECSTRING caList;
	WIN32_FIND_DATA		FindFileData;
	// Establish string and get the first token:

	{
		// While there are tokens in "string"

		// Get next token: 

		p_circular_tstr cmnroot = NeoCoLib::GetDirNameFromPath(ConvTString(lpcFile));
		HANDLE hFind = FindFirstFile(lpcFile.c_str(), &FindFileData);
		while (hFind != INVALID_HANDLE_VALUE)
		{
			caList.push_back(NeoCoLib::GetStaticStrForm(_t("%s\\%s"),cmnroot,FindFileData.cFileName));


			if (!FindNextFile(hFind, &FindFileData))
			{
				FindClose(hFind);
				hFind = INVALID_HANDLE_VALUE;
			}
		}


	}
	return NCL::ConvertStrArrayFromVECSTRING(caList);
}
p_circular_tstr NeoSystemLib::GetErrorMessageStr( )
{
	uint eNum;
	eNum = GetLastError( );
	//MUTEXLOCK(GetErrorMessageStr);


	wchar sysMsg[256];


	FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, eNum,
		MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), // Default language
		sysMsg, 256, NULL );
	return NCL::GetStaticStr(ConvTString(sysMsg));

	//return sysMsg;
}

bool NeoSystemLib::IsExistDir(const wstring& dirName)
{
	uint attribs = ::GetFileAttributes(dirName.c_str());
	if (attribs == INVALID_FILE_ATTRIBUTES) {
		return false;
	}
	return ((attribs & FILE_ATTRIBUTE_DIRECTORY) != 0);
}
LPTCHARSTR  NeoSystemLib::GetStrFromWFile(const wstring&  lpfname)
{
	//VECSTRING ret;
	//VECBYTE cmnbyte ;
	LPBYTEARRAY lpbytearray = NSL::GetLpDBFromWFile(lpfname);
	if(!lpbytearray) return NULL;


	vector<tchar> cmnret(lpbytearray->length/2 +1);

	memcpy(&cmnret[0],lpbytearray->array,lpbytearray->length);


	return NCL::ConvertFromVector(cmnret);
}
void NeoSystemLib::MakeDirPathFromFile(const wstring& lpCFile){
	
	tstring pcDirPath = NeoCoLib::GetDirNameFromPath(ConvTString(lpCFile));
	wstring wdir = ConvWString(pcDirPath);


	if(NeoSystemLib::IsExistDir(wdir) == false){
		NeoSystemLib::CreateDir(wdir);
	}

}
void NeoSystemLib::CreateDir(const wstring& Path)
{

#if 0
	VECSTRING vecstr ;

	LPWSTRARRAY lpwstrarray = NCL::GetSplit2Array(vecstr,Path,_t("\\/"));

	FORVECTER(VECSTRING ,vecstr){
		CreateDirectory(piter->c_str(), NULL);
	}
#endif

}



CMUTEX::CMUTEX(const wchar* muname,bool curRock):m_isDirLock(FALSE) {
	m_hMutex = CreateMutex(NULL,FALSE,muname);
	if(curRock) 	 Lock();
};


void CMUTEX::UnLock(){
	ReleaseMutex(m_hMutex);
	m_isDirLock = FALSE;
}
CMUTEX::~CMUTEX(){
	if(m_isDirLock) UnLock();
	if(m_hMutex) CloseHandle(m_hMutex);
};

bool CMUTEX::Lock(uint dwTimeout )
{
	if (m_isDirLock)
		return TRUE; // Prevents multiple locking

	m_isDirLock = ::WaitForSingleObject(m_hMutex, dwTimeout) == WAIT_OBJECT_0;
	return m_isDirLock;
}
