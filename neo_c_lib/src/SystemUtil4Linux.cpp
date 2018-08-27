#define XOPEN_SOURCE	500

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>		// fstat64()
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>
#include <ftw.h>			// ftw(), nftw()
#include <stdio.h>
#include <time.h>
//#include <sys/types.h>
#include <sys/time.h>

#ifdef __USEPTHREAD__
#include <pthread.h>
#endif



#include "neoDebug.h"
#include "util/SystemUtil4Linux.h"
#include "neoCoLib.h"

int g_depth = -1;

VECSTRING g_vecDirList;
//VECSTRING g_vecFileList;

CSystemUtil4Linux::CSystemUtil4Linux(void) :CSystemUtil()
{
}


CSystemUtil4Linux::~CSystemUtil4Linux(void)
{
	g_vecDirList.clear();
	//g_vecFileList.clear();
}


//static int CallBack_FileList(const char *fpath, const struct stat *sb, int tflag)
//{
//	if (tflag == FTW_F) {
//		printf("file : %s\n", fpath);
//		g_vecFileList.push_back(fpath);
//	}
//
//	return 0;
//}



#if 0
void CSystemUtil4Linux::FindFileList(vector<FILESTATEINFO> & vecfilelist,rctstr lpwstrfilter,time_t startTime ,time_t endTime )
{
	/* Not Use */
}
//-------------------------------------------------------------------
// 하위 디렉토리 검색은 없음
// 디렉토리에 와일드카드 (*, ?)는 지원하지 않음
// 파일명에 해당하는 필터의 와일드카드를 기준으로 filter를 한다.
// mid_dir 을 함께 받아서 파일명 세팅을 할 때 mid_dir/filename 으로
//-------------------------------------------------------------------
void CSystemUtil4Linux::FindFileList
(
		vector<FILESTATEINFO> & vecfilelist,
		rctstr dir_only,
		rctstr filter_only
)
{
	//NEO_TITLE(CSystemUtil4Linux::FindFileList_Opt);

	//printf("\n");
	//printf("dir_only    : %s\n", dir_only.c_str());
	//printf("filter_only : %s\n", filter_only.c_str());

	DIR *dp;
	struct dirent *entry;
	struct stat64 stat_buf;
	char filename[4096];
	int ret;
	FILESTATEINFO filestateinfo;

	bool isOneFileFilter = false;

	// 1개 파일 대상작업인지 여러파일 대상작업인지 체크
	if (filter_only.find('*') == std::string::npos &&
		filter_only.find('?') == std::string::npos) {
		isOneFileFilter = true;
	}

	//----------------------------------//
	// 1개의 파일만 찾는 필터일 경우    //
	//----------------------------------//
	if (isOneFileFilter == true) {

		memset(&stat_buf, 0x00, sizeof(struct stat));
		sprintf(filename, "%s/%s", dir_only.c_str(), filter_only.c_str());

		if ((ret = stat64(filename, &stat_buf)) < 0) {
			perror("stat() error");
			return;
		}

		if (S_ISREG(stat_buf.st_mode)) {
			//printf("[Regular File] : fullpath = %s filelength = %llu \n", filename, stat_buf.st_size);
			memset(&filestateinfo, 0x00, sizeof(filestateinfo));
			filestateinfo.lastAccess = stat_buf.st_mtime;
			filestateinfo.length     = stat_buf.st_size;
			strncpy(filestateinfo.szName, filename, strlen(filename));
			//printf("szName : %s\n", filestateinfo.szName);
			vecfilelist.push_back(filestateinfo);
		}
		return;
	}

	//----------------------------------//
	// 여러개의 파일이 존재할 경우      //
	//----------------------------------//
	if ((dp = opendir(dir_only.c_str())) == NULL) {
		printf("\nCannot open directory : %s\n", dir_only.c_str());
		return;
	}

	while ((entry = readdir(dp)) != NULL) {

		//memset(&stat_buf, 0x00, sizeof(struct stat));
		sprintf(filename, "%s/%s", dir_only.c_str(), entry->d_name);
		//printf("entry->d_name : %s\n", entry->d_name);
		//printf("filename : %s\n", filename);

		// filter_only

		if ((ret = stat64(filename, &stat_buf)) < 0) {
			perror("stat() error");
			return;
		}

		if (S_ISREG(stat_buf.st_mode)) {
			//printf("[Regular File] : fullpath = %s filelength = %llu \n", filename, stat_buf.st_size);
			memset(&filestateinfo, 0x00, sizeof(filestateinfo));
			filestateinfo.lastAccess = stat_buf.st_mtime;
			filestateinfo.length     = stat_buf.st_size;
			strncpy(filestateinfo.szName, filename, strlen(filename));
			//printf("szName : %s\n", filestateinfo.szName);
			vecfilelist.push_back(filestateinfo);
			//printf("accept : %s\n", filestateinfo.szName);
		}
	}
	closedir(dp);
}

#endif

static bool
comparebywildcard(rctstr filter, rctstr realname)
{
	int count = 0;

	const tchar* lp1 = filter.c_str();
	const tchar* lp2 = realname.c_str();

	for ( ;*lp1 && *lp2; lp1++,lp2++) {
		if(*lp1 == L'?')
			continue;
		if(*lp1 != *lp2)
			return false;
	}

	return (*lp1 - *lp2) == 0;
}

//-------------------------------------------------------------------
// 하위 디렉토리 검색은 없음
// 파일명에 해당하는 필터의 와일드카드를 기준으로 filter를 한다.
// mid_dir 을 함께 받아서 파일명 세팅을 할 때 mid_dir/filename 으로
//-------------------------------------------------------------------
void CSystemUtil4Linux::FindFileList(vector<FILESTATEINFO> & vecfilelist,	rctstr dir_only,	rctstr mid_dir,	rctstr filter_only,	time_t startTime ,	time_t endTime )
{
	//NEO_TITLE(CSystemUtil4Linux::FindFileList_Opt);

	//printf("\n");
	//printf("dir_only    : %s\n", dir_only.c_str());
	//printf("filter_only : %s\n", filter_only.c_str());
	//printf("mid_dir     : %s\n", mid_dir.c_str());

	DIR *dp;
	struct dirent *entry;
	struct stat64 stat_buf;
	char filename[1024];
	int ret;
	FILESTATEINFO filestateinfo;

	// 1개 파일 대상작업인지 여러파일 대상작업인지 체크
	bool isOneFileFilter = false;
	if (filter_only.find('*') == std::string::npos &&
		filter_only.find('?') == std::string::npos) {
		isOneFileFilter = true;
	}

	// CODE ADD BEGIN : 20140314 sodazim
	// filter에 *는 없고 ?만 있는 지 체크
	bool isWildCard = false;
    if (filter_only.find("?") != std::string::npos &&
        filter_only.find("*") == std::string::npos)
        isWildCard = true;
    else
        isWildCard = false;
    // CODE ADD END

	//----------------------------------//
	// 1개의 파일만 찾는 필터일 경우    //
	//----------------------------------//
	if (isOneFileFilter == true) {
		// full filename : 파일 정보를 구하기 위해서 풀패스가 필요함
		memset(&stat_buf, 0x00, sizeof(struct stat));
		sprintf(filename, "%s/%s", dir_only.c_str(), filter_only.c_str());

		// base_dir 를 제외한 mid_dir + filename
		string mid_filepath;
		if (mid_dir.size() > 0)
			mid_filepath = mid_dir + "/" + filter_only;
		else
			mid_filepath = filter_only;

		if ((ret = stat64(filename, &stat_buf)) < 0) {
			perror("stat() error");
			return;
		}

		if (S_ISREG(stat_buf.st_mode)) {
			//printf("[Regular File] : fullpath = %s filelength = %llu \n", filename, stat_buf.st_size);
			memset(&filestateinfo, 0x00, sizeof(filestateinfo));
			filestateinfo.lastAccess = stat_buf.st_mtime;
			filestateinfo.length     = stat_buf.st_size;
			strcpy(filestateinfo.szName, mid_filepath.c_str());
			//printf("szName : %s\n", filestateinfo.szName);
			vecfilelist.push_back(filestateinfo);
		}
		return;
	}

	//----------------------------------//
	// 여러개의 파일이 존재할 경우      //
	//----------------------------------//
	if ((dp = opendir(dir_only.c_str())) == NULL) {
		printf("\nCannot open directory : %s\n", dir_only.c_str());
		return;
	}

	while ((entry = readdir(dp)) != NULL) {

		//memset(&stat_buf, 0x00, sizeof(struct stat));
		sprintf(filename, "%s/%s", dir_only.c_str(), entry->d_name);
		//printf("filename : %s\n", filename);

		if ((ret = stat64(filename, &stat_buf)) < 0) {
			perror("stat() error");
			return;
		}

		if (S_ISREG(stat_buf.st_mode)) {
			//printf("[Regular File] : fullpath = %s filelength = %llu \n", filename, stat_buf.st_size);
			memset(&filestateinfo, 0x00, sizeof(filestateinfo));
			filestateinfo.lastAccess = stat_buf.st_mtime;
			filestateinfo.length     = stat_buf.st_size;

		    string mid_filepath;

		    if (mid_dir.size() > 0)
				mid_filepath = mid_dir + "/" + entry->d_name;
    		else
        		mid_filepath = entry->d_name;

			strcpy(filestateinfo.szName, mid_filepath.c_str());

			// CODE ADD BEGIN : 20140314
			if (isWildCard == true) {
				if (!comparebywildcard(filter_only, entry->d_name)) {
// 					printf("[NOMATCH] filter_only : %s, entry->d_name = %s\n",
// 							filter_only.c_str(), entry->d_name);
					continue;
				}
			}
			// CODE ADD END

			vecfilelist.push_back(filestateinfo);

/*			printf("accept : %s\n", filestateinfo.szName);*/
		}
	}
	closedir(dp);
}

//------------------------------------------------------------------------------
// FTW_D  : Directory
// FTW_SL : Symbolic Link
//------------------------------------------------------------------------------
//static int CallBack_DirList(const char *fpath, const struct stat *sb, int tflag)
static int CallBack_DirList(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftw)
{

    //FTW_DP,		/* Directory, all subdirs have been visited. */
    //FTW_SLN		/* Symbolic link naming non-existing file.  */
    if(g_depth != -1 && g_depth < ftw->level) {
		return 0;
	}

	if (tflag == FTW_DP || tflag == FTW_SLN) {
		g_vecDirList.push_back(fpath);
	}

	return 0;
}


void CSystemUtil4Linux::FindSubDirectorys(VECSTRING  & vecdirlist,rctstr startpath,int depth)
{

		int flags = 0;

		flags |= FTW_DEPTH;
        g_depth = depth;
		if (nftw(startpath.c_str(), CallBack_DirList, 20,flags) == -1) {
			perror("ftw() for FindSubDirectorys");
		}
		int i = 0;

		FORVECTER (vector<tstring>, g_vecDirList) {
			//printf("[%-6d] : %s\n", i, piter->c_str());
			vecdirlist.push_back(*piter);
			i++;
		}
		g_vecDirList.clear();
}


void CSystemUtil4Linux::FindSubDirectorys_Opt
(
		VECSTRING & vecdirlist,
		rctstr startpath,
		rctstr mid_dir,
		bool skip_subtree
)
{
	//NEO_TITLE(CSystemUtil4Linux::FindSubDirectorys_Opt);
//	printf("\n");

	int i = 0;

	string fulldirpath;

	if (mid_dir.size() > 0)
		fulldirpath = startpath + "/" + mid_dir;
	else
		fulldirpath = startpath;

	//---------------------------------------//
	// SKIP SUBTREE                          //
	//---------------------------------------//
	if (skip_subtree == true) {
		//printf("\n[%-6d] : %s\n", i, startpath.c_str());
		//vecdirlist.push_back(startpath);
//		printf("\n[%-6d] : %s\n", i, fulldirpath.c_str());
		vecdirlist.push_back(fulldirpath);
	}
	//---------------------------------------//
	// NO SKIP SUBTREE                       //
	//---------------------------------------//
	else {
		int flags = 0;

		flags |= FTW_DEPTH;
        g_depth = -1;
		//if (ftw(startpath.c_str(), CallBack_DirList, flags) == -1) {
		if (nftw(fulldirpath.c_str(), CallBack_DirList, 20,flags) == -1) {
			perror("ftw() for FindSubDirectorys_Opt");
		}

		FORVECTER (vector<tstring>, g_vecDirList) {
			printf("[%-6d] : %s\n", i, piter->c_str());
			vecdirlist.push_back(*piter);
			i++;
		}
		g_vecDirList.clear();
	}
}


bool CSystemUtil4Linux::ReadBufferFile
(
		rctstr fileName,
		pvoid buffer,
		neocolib::int64 offset,
		neocolib::uint & readSize)
{
	int sendfd;
	struct stat64 stat_buf;
	off64_t bytes_read;
	off64_t curpos;

	sendfd = open64(fileName.c_str(), O_RDONLY);
	if (sendfd < 0) {
		perror("open64() error");
//		printf("open64() error : %s\n", fileName.c_str());
		return false;
	}

	curpos = lseek64(sendfd, (off64_t)offset, SEEK_SET);
	if (curpos < 0) {
		perror("lseek64() error");
		close(sendfd);
		return false;
	}

	bytes_read = read(sendfd, buffer, readSize);
	if (bytes_read < 0) {
		perror("read() error");
		readSize = bytes_read;
		close(sendfd);
		return false;
	}

// 	printf("[%s] offset = %llu, length = %llu bytes\n",
// 		fileName.c_str(), offset, bytes_read);

	readSize = bytes_read;
	close(sendfd);

	return true;
}


// 큰 파일 테스트 해봐야 함.
neocolib::int64 CSystemUtil4Linux::GetFileSize(rctstr fileName)
{
	int ret;
	struct stat64 stat_buf;

	memset(&stat_buf, 0x00, sizeof(struct stat));

	if ((ret = stat64(fileName.c_str(), &stat_buf)) < 0) {
		perror("stat() error");
//		printf("stat() error : %s\n", fileName.c_str());
		return -1;
	}

	if (S_ISREG(stat_buf.st_mode)) {
		//printf("[Regular File] : fullpath = %s filelength = %llu \n", fileName.c_str(), stat_buf.st_size);
		return (neocolib::int64) stat_buf.st_size;
	}
	return -1;

/*
	//struct stat64 stat_buf;
	int logfd;
	off64_t offset;

	logfd = open64(fileName.c_str(), O_RDONLY);
	if (logfd < 0) {
		printf("open64() error\n");
		return -1;
	}

	offset = lseek64(logfd, (off64_t)0, SEEK_END);
	if (offset < 0) {
		printf("lseek() failed for '%s' (errno=%d)\n", fileName, errno);
		close(logfd);
		return -1;
	}

	close(logfd);
	return (int64) offset;
*/
}


neocolib::int64 CSystemUtil4Linux::ConvStringUint64(rctstr str){

	char *pEnd;

	return strtoull(str.c_str(), &pEnd, 10);
}

neocolib::pvoid CSystemUtil4Linux::CreateThread(neocolib::pvoid pfunction,neocolib::pvoid pparam){
 #ifdef __USEPTHREAD__	
	int thr_id;
	int status;
	pthread_t p_thread;
	thr_id = pthread_create(&p_thread, NULL, (void* (*)(void*)) pfunction,pparam);

	return (pvoid)p_thread;
#endif

}
void CSystemUtil4Linux::CloseThreand(neocolib::pvoid threadhandle){


}
neocolib::uint CSystemUtil4Linux::GetTickCount(){
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);

}
void  CSystemUtil4Linux::WaitTime(neocolib::uint msec){
	usleep(msec*1000);  /* sleep for 100 milliSeconds */

}
bool CSystemUtil4Linux::createDirectoryRecursively( rctstr path ){
    printf("CSystemUtil4Linux::createDirectoryRecursively\r\n");
	       char tmp[256];
        char *p = NULL;
        size_t len;

        snprintf(tmp, sizeof(tmp),"%s",path.c_str());
        printf("%s\r\n",tmp);
        len = strlen(tmp);
        if(tmp[len - 1] == '/')
                tmp[len - 1] = 0;
        for(p = tmp + 1; *p; p++)
                if(*p == '/') {
                        *p = 0;
                        mkdir(tmp, S_IRWXU);
                        *p = '/';
                        printf("%s",p);
                }
        mkdir(tmp, S_IRWXU);
}
void CSystemUtil4Linux::CorrectFullPath(string & path)
{
	char resolved_path[256];
    realpath(path.c_str(), resolved_path);
    path = resolved_path;

}
void CSystemUtil4Linux::SetCurPosDir(string& olddir){

	char the_path[256];
    getcwd(the_path, 255);
    olddir = the_path;


}
bool CSystemUtil4Linux::FileMove(rctstr src,rctstr dst){
	int result= rename( src.c_str() , dst.c_str() );
	if ( result != 0 )
    return false;
    
    return true;
}

void  CSystemUtil4Linux::Test(){
    NEO_TITLE(CSystemUtil4Linux::Test);
    VECSTRING  vecdirlist;
	rctstr startpath = "/home/yeoksam/lsa";
	rctstr mid_dir = "";
	bool skip_subtree = false;

	NEO_MARK(TEST);


   FindSubDirectorys(vecdirlist,startpath,5);



    NEO_MARK(TEST END);

    FORVECTER(VECSTRING,vecdirlist){
        NEO_STR(piter->c_str());
    }
    NEO_MARK(TEST END);

}

#if 0

unsigned int GetTickCount(void)
{
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

unsigned int GetTickCount2(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);

	return (unsigned int)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
#endif
