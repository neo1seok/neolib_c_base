
#include "neoCoLib.h"
#include "NeoSystemLib.h"
//#include <Windows.h>
#include "util/SystemUtil4windows.h"

#include <Shlobj.h >

#include "neoDebug.h"

#define SECS_BETWEEN_EPOCHS 11644473600
#define SECS_TO_100NS 10000000 /* 10^7 */

CSystemUtil4windows::CSystemUtil4windows(void)
:CSystemUtil()
{
}

CSystemUtil4windows::~CSystemUtil4windows(void)
{
}
int64 getUniqFileID(HANDLE hFile)
{
	BY_HANDLE_FILE_INFORMATION byhandle = {0,};
	GetFileInformationByHandle(hFile,&byhandle);
	return MAKELONGLONG(byhandle.nFileIndexLow,byhandle.nFileIndexHigh);
}

time_t CSystemUtil4windows::filetime_to_timet(const FILETIME& ft)
{
	ULARGE_INTEGER ull;
	ull.LowPart = ft.dwLowDateTime;
	ull.HighPart = ft.dwHighDateTime;

	return ull.QuadPart / 10000000ULL - 11644473600ULL;
}

tstring CSystemUtil4windows::ConvTString(const wstring &str)
{

	tstring sz ;
	//CMANMEM<char> cmn;
#ifdef _USE_UTF8_
	sz =  NCL::ConvWC2UTF8(str);

	//sz =  NCL::GetStaticStr(_t("afdafdsfasdffdsfsadfaf"));
	//sz ="fdasdfsdafsdafsdfsdafsdafsdafsadffsdafasdfasdfasfdsaddffsadfsafsfsadfsafsafsa";

#else
	sz =  NCL::CopyNewStr(str);
#endif
	//sz = cmn;
	return sz;
}

wstring CSystemUtil4windows::ConvWString(const tstring &str){
	wstring sz ;
	//CMANMEM<wchar> cmn;
#ifdef _USE_UTF8_
	sz =  NCL::ConvUTF82WC(str);

#else
	sz =  NCL::CopyNewStr(str);
#endif
	//sz = cmn;
	return sz;
}



// 
// time_t CSystemUtil4windows::FILEFileTimeToUnixTime(FILETIME FileTime, long* nsec)
// {
// 	__int64 UnixTime;
// 
// 	/* get the full win32 value, in 100ns */
// 	UnixTime = ((__int64)FileTime.dwHighDateTime << 32) + FileTime.dwLowDateTime;
// 
// 	/* convert to the Unix epoch */
// 	UnixTime = UnixTime - (SECS_BETWEEN_EPOCHS * SECS_TO_100NS);
// 
// 	if ( nsec )
// 	{
// 		/* get the number of 100ns, convert to ns */
// 		*nsec = (UnixTime % SECS_TO_100NS) * 100;
// 	}
// 
// 	UnixTime /= SECS_TO_100NS; /* now convert to seconds */
// 	return (time_t)UnixTime;
// }



void CSystemUtil4windows::FindFileList_INTER(vector<FILESTATEINFO> & vecfilelist,rctstr lpwstrfilter,time_t startTime,time_t endTime)
{
	//vector<WIN32_FIND_DATA> * pret = new vector<WIN32_FIND_DATA>;

	WIN32_FIND_DATAW		FindFileData;
	int count = 0;

	wstring strfilter = ConvWString(lpwstrfilter);



	HANDLE hFind = FindFirstFileW(strfilter.c_str(), &FindFileData);

	while (hFind != INVALID_HANDLE_VALUE)
	{
		//caList.Add(NCL::GetNewStrForm(L"%s\\%s",cmnroot,FindFileData.cFileName));
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) goto FIANLLOOP;

		{
			WIN32_FIND_DATAW * piter1 = &FindFileData;
	
			
	
			FILESTATEINFO filestateinfo = {0,};

			time_t writeTIme  = filetime_to_timet(piter1->ftLastWriteTime);

			if(startTime != 0 && writeTIme < startTime ) goto FIANLLOOP;
			
			if(endTime != 0 && writeTIme > endTime ) goto FIANLLOOP;


			filestateinfo.lastAccess =  writeTIme ;



			filestateinfo.length = (piter1->nFileSizeHigh * ((__int64)MAXDWORD+1)) + piter1->nFileSizeLow;
	
			string name = ConvTString(piter1->cFileName);

		//	int64 lnegth = getUniqFileID(hFind);
	
	
			NCL::CopyPSTRFromSTRING(filestateinfo.szName , name,MAX_NCL_PATH);
			;
			//wsprintf(filestateinfo.szName,L"%s%s",middir.c_str(),piter1->cFileName);
	
	
			//vecfileList.push_back(filestateinfo);
			vecfilelist.push_back(filestateinfo);
	
	
			count++;
		}



		//caList.Add(lpfinddata);
FIANLLOOP:

		if (!FindNextFileW(hFind, &FindFileData))
		{
			FindClose(hFind);
			hFind = INVALID_HANDLE_VALUE;
		}

	}

	//return *pret;

}

void CSystemUtil4windows::FindSebDirectorysRecurse(vector<wstring> &ret , wstring baseDir,int depth)
{
	// build a string with wildcards

	WIN32_FIND_DATAW		FindFileData;
	static int curdepth = -1;
	curdepth++;

	//wstring baseDir = pstr;
	wstring startparam = baseDir + L"/*.*";
	if(depth != -1 && depth <= curdepth) {
		curdepth--;
		return;
	}

	// start working for files
	HANDLE hFind = FindFirstFileW(startparam.c_str(), &FindFileData);

	while (hFind != INVALID_HANDLE_VALUE)
	{

		// skip . and .. files; otherwise, we'd
		// recur infinitely!

		// if it's a directory, recursively search it

		if(FindFileData.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY && FindFileData.cFileName[0] != L'.'){

			startparam = 	baseDir + L"/"+	FindFileData.cFileName;
			string path = NCL::ConvWC2UTF8(startparam);

			//NEO_DEBUG("%s %d/%d", path.c_str(), curdepth,depth);

			if( depth == -1 || depth == (curdepth +1))
				ret.push_back(startparam.c_str());

			FindSebDirectorysRecurse( ret,startparam.c_str(),depth);
			
		}

		if (!FindNextFileW(hFind, &FindFileData))
		{
			FindClose(hFind);
			hFind = INVALID_HANDLE_VALUE;
		}
	}
	curdepth--;
}
void  CSystemUtil4windows::FindSubDirectorys(VECSTRING  & vecdirlist,rctstr startpath,int depth )
{
	//NEO_TITLE(FindSubDirectorys);
	tstring baseDir = startpath;//NCL::GetDirNameFromPath(startpath);
	//wstring filtername = NCL::GetFileNameFromPath(startpath);

	

	if(depth == -1 || depth == 0)  vecdirlist.push_back(baseDir);

//	NEO_MARK(FindSubDirectorys 0);

	vector<wstring> vecwstr;
	FindSebDirectorysRecurse( vecwstr , ConvWString(baseDir),depth);

	FORVECTER(vector<wstring>,vecwstr){
		//			piter;
		vecdirlist.push_back(ConvTString(*piter));

	}
	//NEO_MARK(FindSubDirectorys END);
	//LPWSTRARRAY lp = (LPWSTRARRAY)NCL::MakeNSFormDwordBase(ret.size(),sizeof(LPWSTR));
	int count = 0;
	// 		for(std::vector<wstring>::iterator IterPos = ret.begin(); IterPos != ret.end();++IterPos,count++){
	// 			lp->array[count] = NCL::CopyNewStr(IterPos->c_str());
	// 		}

	//return *pret;

}

#if 1
neocolib::int64 CSystemUtil4windows::GetFileSize(rctstr fileName)
{
		neocolib::int64 fileSize = -1;
		LARGE_INTEGER larginterger  ={0,};
		HANDLE hFile = NSL::CreateFileSimple(ConvWString(fileName),NSL::MODEREAD);
		if(hFile == INVALID_HANDLE_VALUE) return -1;

		//fileSize = GetFileSize(hFile,NULL);
		GetFileSizeEx(hFile,&larginterger);
		fileSize = larginterger.QuadPart;


		

		CloseHandle(hFile);

		return fileSize;
}


#endif

bool CSystemUtil4windows::ReadBufferFile(rctstr fileName,pvoid buffer,neocolib::int64 offset,uint & readSize)
{
	LARGE_INTEGER larginterger  ={0,};

	HANDLE hFile = NSL::CreateFileSimple(ConvWString(fileName),NSL::MODEREAD);
	if(hFile == INVALID_HANDLE_VALUE) return false;
	larginterger.QuadPart = offset;
	::SetFilePointerEx(hFile,larginterger,NULL,FILE_BEGIN);

	uint readlegnth = NSL::ReadFile(hFile,buffer,(uint)readSize);
	readSize = readlegnth;

	CloseHandle(hFile);
	return true;


}
neocolib::int64 CSystemUtil4windows::ConvStringUint64(rctstr str){

	//return 0;
	return _atoi64(str.c_str());
}

neocolib::pvoid CSystemUtil4windows::CreateThread(neocolib::pvoid pfunction,neocolib::pvoid pparam) {
	DWORD dwd = 0;
	return ::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)pfunction, 	pparam, 0, (LPDWORD)&dwd );
}
void CSystemUtil4windows::CloseThreand(neocolib::pvoid threadhandle){
	if(threadhandle) CloseHandle(threadhandle);
}
neocolib::uint CSystemUtil4windows::GetTickCount() {
	return ::GetTickCount();
}
void CSystemUtil4windows::FindFileList(vector<FILESTATEINFO> & vecfilelist, 
							  rctstr dir_only, 
							  rctstr mid_dir,
							  rctstr filter_only,time_t startTime,time_t endTime
							  
							  ) 
{
	string middir = mid_dir;
// 		fulldirpath = dir_only;
	if (mid_dir.size() > 0) 
		middir += "/";

	string str = dir_only +"/"+filter_only;
	wstring wstr = ConvWString(str);



	FindFileList_INTER(vecfilelist,dir_only+"/"+filter_only,startTime,endTime);
	FORVECTEREXT(piter1,vector<FILESTATEINFO>,vecfilelist){
		string name = middir + piter1->szName;
		NCL::CopyPSTRFromSTRING(piter1->szName , name,MAX_NCL_PATH);
		;
	}


}
void  CSystemUtil4windows::WaitTime(neocolib::uint msec){
	Sleep(msec);

}

// NEW
//void FindSubDirectorys_Opt(VECSTRING & vecdirlist, rctstr startpath, bool skip_subtree);
void CSystemUtil4windows::FindSubDirectorys_Opt(VECSTRING & vecdirlist, 
								   rctstr startpath,
								   rctstr mid_dir, 
								   bool skip_subtree)
{
	string fulldirpath;


	if (mid_dir.size() > 0) 
		fulldirpath = startpath + "/" + mid_dir;
	else
		fulldirpath = startpath;


	if(skip_subtree){
		vecdirlist.push_back(fulldirpath);
		return;
	}


	FindSubDirectorys(vecdirlist,fulldirpath);

}

bool CSystemUtil4windows::createDirectoryRecursively( rctstr path )
{
	string strpath = path;
	NCL::ReplaceAll(strpath,"/","\\");

	return false;
	//return (SHCreateDirectoryExW( NULL, ConvWString(strpath).c_str(), NULL ) ==ERROR_SUCCESS) ;
}
void CSystemUtil4windows::CorrectFullPath(string & path)
{
	wchar_t szPath[MAX_PATH];

	//SetCurrentDirectoryA(	);
	GetFullPathNameW(ConvWString(path).c_str(),MAX_PATH,	szPath,	NULL);

	path = ConvTString(szPath);
}
void CSystemUtil4windows::SetCurPosDir(string& olddir){

	wchar_t szPath[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH,szPath);
	olddir = ConvTString(szPath);
	GetModuleFileNameW(NULL, szPath, ARRAYSIZE(szPath));
	SetCurrentDirectoryW(ConvWString(NCL::GetDirNameFromPath(ConvTString(szPath))).c_str());
}

bool CSystemUtil4windows::FileMove(rctstr src,rctstr dst){
	string strsrc =  src;
	string strdst =  dst;

	NCL::ReplaceAll(strsrc,"/","\\");
	NCL::ReplaceAll(strdst,"/","\\");

	wstring wsrc= ConvWString(strsrc);
	wstring wdst= ConvWString(strdst);

	return (::MoveFileW(wsrc.c_str(),wdst.c_str()) ==TRUE);

	//return true;
}

#if 0
void CSystemUtil4windows::StartLock(){
	InitializeCriticalSection(&cs);
	EnterCriticalSection(&cs);
	//return true;
}

void CSystemUtil4windows::EndLock(){
	
	LeaveCriticalSection(&cs);  // 임계 영역을 빠져나온다.
	DeleteCriticalSection(&cs);  // 임계 영역을 정리한다.
	//return true;
}

#endif // 0


void  CSystemUtil4windows::Test(){

}


void gentest()
{
	



}
