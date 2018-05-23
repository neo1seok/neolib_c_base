#pragma once

#include <string>
#include <vector>
#include "comdef.h"
//#include "lsadefine.h"
using namespace neocolib;

typedef struct _tagFILESTATEINFO{
	char szName[MAX_NCL_PATH + 2];
	//string szName;
	time_t lastAccess;
	int64 length;
} FILESTATEINFO, *LPFILESTATEINFO;

class CSystemUtil
{
public:
	CSystemUtil(void);
	~CSystemUtil(void);

	//virtual void FindFileList(vector<FILESTATEINFO> & vecfilelist, rctstr lpwstrfilter,time_t startTime = 0,time_t endTime = 0) = 0;

	virtual void FindSubDirectorys(VECSTRING & vecdirlist, rctstr startpath, int depth = -1) = 0;

	virtual bool ReadBufferFile(rctstr fileName,pvoid buffer,neocolib::int64 offset,neocolib::uint & readSize) =0;
	virtual neocolib::int64 GetFileSize(rctstr fileName) = 0;
	virtual neocolib::int64 ConvStringUint64(rctstr str)= 0;
	virtual neocolib::pvoid CreateThread(neocolib::pvoid pfunction,neocolib::pvoid pparam) = 0;
	virtual void CloseThreand(neocolib::pvoid threadhandle)= 0;
	virtual neocolib::uint GetTickCount() = 0;
	virtual void  WaitTime(neocolib::uint msec) = 0;

	virtual void FindFileList(vector<FILESTATEINFO> & vecfilelist,
									rctstr dir_only,
									rctstr mid_dir,
									rctstr filter_only,time_t startTime = 0,time_t endTime = 0) = 0;

	virtual void FindSubDirectorys_Opt(VECSTRING & vecdirlist,
									rctstr startpath,
									rctstr mid_dir,
									bool skip_subtree) = 0;

	virtual bool createDirectoryRecursively( rctstr path ) = 0;
	virtual void CorrectFullPath(string & path) = 0;
	virtual void SetCurPosDir(string& olddir) = 0;

	virtual bool FileMove(rctstr src,rctstr dst) = 0;
	virtual void  Test() = 0;

	static CSystemUtil * GetSystemUtilClass();

	


};

