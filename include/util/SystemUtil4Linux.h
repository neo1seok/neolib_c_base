#pragma once

#include "SystemUtil.h"

using namespace std;

class CSystemUtil4Linux: public CSystemUtil
{
public:
	CSystemUtil4Linux(void);
	~CSystemUtil4Linux(void);

	//void FindFileList(vector<FILESTATEINFO> & vecfilelist,rctstr lpwstrfilter,time_t startTime = 0,time_t endTime = 0);
	void  FindSubDirectorys(VECSTRING  & vecdirlist,rctstr startpath,int depth = -1);

	bool ReadBufferFile(rctstr fileName,pvoid buffer,neocolib::int64 offset,neocolib::uint & readSize);
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

	void  Test() ;





};
