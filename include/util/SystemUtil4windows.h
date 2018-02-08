#pragma once
#include "windows.h"
#include "SystemUtil.h"

using namespace std;

class CSystemUtil4windows: public CSystemUtil
{

public:
	CSystemUtil4windows(void);
	~CSystemUtil4windows(void);

	
	void  FindSubDirectorys(VECSTRING  & vecdirlist,rctstr startpath,int depth = -1);

	bool ReadBufferFile(rctstr fileName,pvoid buffer,neocolib::int64 offset,uint & readSize);
	neocolib::int64 GetFileSize(rctstr fileName);
	neocolib::pvoid CreateThread(neocolib::pvoid pfunction,neocolib::pvoid pparam);
	void CloseThreand(neocolib::pvoid threadhandle);
	neocolib::uint GetTickCount() ;

	void  WaitTime(neocolib::uint msec);
	

	void FindFileList(vector<FILESTATEINFO> & vecfilelist, 
		rctstr dir_only, 
		rctstr mid_dir,
		rctstr filter_only,
		time_t startTime = 0,
		time_t endTime = 0);

	// NEW
	//void FindSubDirectorys_Opt(VECSTRING & vecdirlist, rctstr startpath, bool skip_subtree);
	void FindSubDirectorys_Opt(VECSTRING & vecdirlist, 
		rctstr startpath,
		rctstr mid_dir, 
		bool skip_subtree);

	
	neocolib::int64 ConvStringUint64(rctstr str);

	bool createDirectoryRecursively( rctstr path );
	void CorrectFullPath(string & path);
	void SetCurPosDir(string& olddir);

	bool FileMove(rctstr src,rctstr dst);

	void  Test();



	

private:
	CRITICAL_SECTION cs;
	void FindFileList_INTER(vector<FILESTATEINFO> & vecfilelist,rctstr lpwstrfilter,time_t startTime = 0,time_t endTime = 0);
	//time_t FILEFileTimeToUnixTime(FILETIME FileTime, long* nsec);
	time_t filetime_to_timet(const FILETIME& ft);
	//void FindFileList(vector<WIN32_FIND_DATAW> & vecWIN32_FIND_DATA,rctstr lpwstrfilter);
	
	tstring ConvTString(const wstring &str);
	wstring ConvWString(const tstring &str);
	
	void FindSebDirectorysRecurse(vector<wstring> &ret , wstring baseDir,int depth = -1);



};
